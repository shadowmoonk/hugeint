#include "stdafx.h"

#include "taskpool.h"

#include <string.h>

#include <stdio.h>
#include <pthread.h>


TaskPool::TaskPool(const int & poolMaxSize,ThreadPoolManager * threadpoolmanager)
:	// m_poolSize(poolMaxSize)
  m_taskListSize(0) 
, m_bStop(false)
,m_threadpoolmanager(threadpoolmanager)
{
	pthread_mutex_init(&m_lock, NULL);
	pthread_mutex_init(&m_idleMutex, NULL);
	pthread_cond_init(&m_idleCond, NULL);

	vIDlist.clear();
	vIDlist.push_back(1);
	vIDlist.resize(300,0);		//�������������������
	InitPool(poolMaxSize);

	pthread_attr_t attr;
	pthread_attr_init( &attr );
	pthread_attr_setdetachstate( &attr, PTHREAD_CREATE_JOINABLE ); // ���̶߳�������
	pthread_create(&m_idleId, &attr, CheckIdleTask, this); //����������������� 
	pthread_attr_destroy(&attr);
}

TaskPool::~TaskPool()
{
	if(!m_bStop)
	{
		StopPool();
	}
	if(!m_taskList.empty())
	{
		std::list<Task*>::iterator it = m_taskList.begin();
		for(; it != m_taskList.end(); ++it)
		{
			if(*it != NULL)
			{
				delete *it;
				*it = NULL;
			}
		}
		m_taskList.clear();
		m_taskListSize = 0;
	}
	if(!m_idleList.empty())
	{
		std::list<Task*>::iterator it = m_idleList.begin();
		for(; it != m_idleList.end(); ++it)
		{
			if(*it != NULL)
			{
				delete *it;
				*it = NULL;
			}
		}
		m_idleList.clear();
	}


	if(!m_taskjoinList.empty())
	{
		std::list<Task_Join*>::iterator it = m_taskjoinList.begin();
		for(; it != m_taskjoinList.end(); ++it)
		{
			if(*it != NULL)
			{
				delete *it;
				*it = NULL;
			}
		}
		m_taskjoinList.clear();
	}

	pthread_mutex_destroy(&m_lock);
	pthread_mutex_destroy(&m_idleMutex);
	pthread_cond_destroy(&m_idleCond);
}

void TaskPool::InitPool(unsigned int  poolMaxSize){
	LockIdle();

	if(poolMaxSize > 300)
		poolMaxSize = 300;
	while(m_taskListSize < poolMaxSize || m_taskListSize < 50){
		Task * task = new Task;
		task->fun = 0;
		task->data = NULL;
		task->taskID = 0;
		for(unsigned int i = 0;i < vIDlist.size();++ i){
			if(! vIDlist[i]){
				vIDlist[i] = 1;
				task->taskID = i;
				break;
			}
		}
	//	task->taskID =  ++ m_poolSize;
		m_taskListSize ++;
		task->dependencelist.clear();
		task->state = 0;
		task->last_time = time(0);
		m_idleList.push_back(task);
	}	
	UnlockIdle();
}

int TaskPool::GetTaskListSize(){
	return m_taskList.size();
}


void * TaskPool::CheckIdleTask(void * arg)
{
	TaskPool * pool = (TaskPool*)arg;
	while(1)
	{
		pool->LockIdle();
		pool->RemoveIdleTask();
		if(pool->GetStop())
		{
			pool->UnlockIdle();
			break;
		}
		pool->CheckIdleWait();
		pool->UnlockIdle();
	}
	return 0;			//������Ҫ����һ��ֵ
}

void TaskPool::StopPool()
{
	m_bStop = true;
	LockIdle();
	pthread_cond_signal(&m_idleCond); //��ֹ����߳����ڵȴ����������޷��˳�������
	UnlockIdle();
	pthread_join(m_idleId, NULL);
}

#include <iostream>
#include "threadpool.h"
#include "threadpoolmanager.h"
void TaskPool::UpdataTask(unsigned int taskID,unsigned int state){

	Task * task = NULL;
	pthread_mutex_lock(&m_lock);
	std::list<Task*>::iterator p = m_activeList.begin();
	while(p != m_activeList.end()){
		if(taskID == (* p)->taskID){
				(*p)->state &= ~state;
			//��Ϊ���һ��������,�����0,����Ȼ����ذ�,��ʹ������ִ�����,��Ȼ�к��������Ҫʹ�ô�ID������,
			if((*p)->state != 0){
				pthread_mutex_unlock(&m_lock);
				return;
			}
			task = *p;
			m_activeList.erase(p);
			break;
		}
		++ p;
	}
	pthread_mutex_unlock(&m_lock);




	if(task == NULL){
		//����Ƕ����,
		return;
	}



	task->fun = 0;
	task->data = NULL;
	//����������������Ķ������̵߳�Ӱ��.,
		pthread_mutex_lock(&m_lock);
		p = m_activeList.begin();
		while(p != m_activeList.end()){
			for(std::list<unsigned int>::iterator p2 = (*p)->dependencelist.begin();p2 != (*p)->dependencelist.end();++ p2){
				if(*p2 == task->taskID){
					(*p)->dependencelist.erase(p2);
					break;
				}
			}
			++ p;
		}


		p = m_taskList.begin();
		while(p != m_taskList.end()){
			for(std::list<unsigned int>::iterator p2 = (*p)->dependencelist.begin();p2 != (*p)->dependencelist.end();++ p2){
				if(*p2 == task->taskID){
					(*p)->dependencelist.erase(p2);
					break;
				}
				if(! (*p)->dependencelist.size()){
					m_threadpoolmanager->LockTask();
					m_threadpoolmanager->taskwake();
					m_threadpoolmanager->UnlockTask();
				}
			}
			++ p;
		}

		pthread_mutex_unlock(&m_lock);
		SaveIdleTask(task);

		pthread_mutex_lock(&m_lock);
		std::list<Task_Join*>::iterator pJoin = m_taskjoinList.begin();
		for(;pJoin != m_taskjoinList.end();){
			for(std::list<unsigned int>::iterator pTaskID = (*pJoin)->vTask_Join.begin();pTaskID != (*pJoin)->vTask_Join.end();){
				if( *pTaskID == taskID){
					pTaskID = (*pJoin)->vTask_Join.erase(pTaskID);
					continue;
				}
				++ pTaskID;
			}
			if(! (*pJoin)->vTask_Join.size()){
				pthread_mutex_lock(& (*pJoin)->mutex_task);
				(*pJoin)->bActive = false;
				pthread_cond_signal(& (*pJoin)->cond_task);
				pthread_mutex_unlock(& (*pJoin)->mutex_task);
			}
			++ pJoin;
		}
		pthread_mutex_unlock(&m_lock);
}

Task_Join * TaskPool::join(std::list<unsigned int> & vtask_join){
	Task_Join * task_join = new Task_Join;
	task_join->bActive = true;
	task_join->vTask_Join = vtask_join;
	pthread_mutex_init(&task_join->mutex_task,NULL);
	pthread_cond_init(&task_join->cond_task, NULL);

	pthread_mutex_lock(&m_lock);
	m_taskjoinList.push_back(task_join);
	pthread_mutex_unlock(&m_lock);

	return task_join;
}

#include <iostream>
void TaskPool::waittask(Task_Join * task_join){
	//�����ֹ�ڵȴ�ǰ,������Ѿ������,������λ,ǿ�ƴ���һ��,
	if(task_join->bActive){
	//	std::cout<<"�ȴ�ͬ��------����"<<std::endl;
	//	std::cout<<"������� "<<m_activeList.size() + m_taskList.size()<<std::endl;
	//	if(m_activeList.size() + this->m_taskList.size() > 1){
		//	system("pause");

	//	}
		pthread_mutex_lock(& task_join->mutex_task);
		pthread_cond_signal(& task_join->cond_task);
		pthread_mutex_unlock(& task_join->mutex_task);

		pthread_mutex_lock(& task_join->mutex_task);
		while(task_join->bActive){
			pthread_cond_wait(& task_join->cond_task,& task_join->mutex_task);
		}
		pthread_mutex_unlock(& task_join->mutex_task);
	}else{
	//	std::cout<<"�ȴ�ͬ���Ѿ����-----����"<<std::endl;
	}

	
	pthread_mutex_lock(&m_lock);
	std::list<Task_Join*>::iterator pJoin = m_taskjoinList.begin();
	for(;pJoin != m_taskjoinList.end();++ pJoin){
		if((*pJoin) == task_join){
			pthread_cond_destroy( &task_join->cond_task);
			pthread_mutex_destroy( &task_join->mutex_task);
			delete (*pJoin);
			pJoin = m_taskjoinList.erase(pJoin);
			break;;
		}
	}
	pthread_mutex_unlock(&m_lock);
}

bool TaskPool::GetStop()
{
	return m_bStop;
}

void TaskPool::CheckIdleWait()
{
	struct timespec timeout;
	memset(&timeout, 0, sizeof(timeout));
	timeout.tv_sec = time(0) + CHECK_IDLE_TASK_INTERVAL;
	timeout.tv_nsec = 0;
	pthread_cond_timedwait(&m_idleCond, &m_idleMutex, &timeout);
}

int TaskPool::RemoveIdleTask()
{
	int iRet = 0;
	std::list<Task*>::iterator it,next;
	time_t curTime = time(0);
	//��֤
	if(m_taskListSize > 50)
	for(it = m_idleList.begin(); it != m_idleList.end(); )
	{
		if(difftime(curTime,((*it)->last_time)) >= TASK_DESTROY_INTERVAL)
		{
			iRet++;
			vIDlist[(*it)->taskID] = 0;
	//		std::cout<<(*it)->taskID<<std::endl;
			delete *it;
			*it = NULL;
			it = m_idleList.erase(it);
			m_taskListSize --;
			if(m_taskListSize > 50)
				continue;
			else
				break;
		}
		else
		{
			break;    
		}
		++ it;
	}
	unsigned int iNum = 0;
	std::vector<unsigned int>::reverse_iterator p = vIDlist.rbegin();
	while(p != vIDlist.rend()){
		if( *p)
			break;
		++ iNum;
		++ p;
	}
	vIDlist.resize(vIDlist.size() - iNum);

	return 0;			//������Ҫ����һ��ֵ
}


int TaskPool::AddTask(Task * task)
{
	int iRet = 0;
	if(0 != task->fun)
	{
		pthread_mutex_lock(&m_lock);
		m_taskList.push_back(task);
		++m_taskListSize;
		pthread_mutex_unlock(&m_lock);
	}
	return iRet;
}

#include <iostream>
int TaskPool::SetTaskFree(unsigned int taskID){
	pthread_mutex_lock(&m_lock);
	std::list<Task*>::iterator p = m_taskList.begin();
	for(;p != m_taskList.end();++ p){
		if((*p)->taskID == taskID){
			(*p)->state &= ~TASK_DEPENDENCE;
			break;
		}
	}
	if(p == m_taskList.end()){
		p = m_activeList.begin();
		for(;p != m_activeList.end();++ p){
			if((*p)->taskID == taskID){
				if((*p)->state){
				//	(*p)->state &= ~TASK_DEPENDENCE;
					pthread_mutex_unlock(&m_lock);
					UpdataTask(taskID,TASK_DEPENDENCE);
					return 0;
				}
				break;
			}
		}
	}
	pthread_mutex_unlock(&m_lock);
	return 0;
}


Task* TaskPool::GetTask()
{
	Task *task = NULL;
	pthread_mutex_lock(&m_lock);
	std::list<Task*>::iterator p = m_taskList.begin();
	for(;p != m_taskList.end();++ p){
		if((*p)->dependencelist.size() == 0){
			(*p)->state |= TASK_ACTIVE;
			task = *p;
			m_taskList.erase(p);
			break;
		}
	}
	if(task != NULL)
	m_activeList.push_back(task);
	/*
	if(!m_taskList.empty())
	{
		task =  m_taskList.front();
		m_taskList.pop_front();
		--m_taskListSize;
		task->state += 1;
	}
	*/
	pthread_mutex_unlock(&m_lock);
	return task;
}

void TaskPool::LockIdle()
{
	pthread_mutex_lock(&m_idleMutex);
}

void TaskPool::UnlockIdle()
{
	pthread_mutex_unlock(&m_idleMutex);
}

#include <Windows.h>
//���ｨ������������,���ϰ���ɵ����񷵻ص� �����б�.
Task * TaskPool::GetIdleTask()
{
		Task * task = NULL;
		while(1){
			LockIdle();
			if(!m_idleList.empty())
			{
				task = m_idleList.front();
				m_idleList.pop_front();
				UnlockIdle();
				break;
			}
			if(vIDlist.size() < 50){
				vIDlist.resize(50);
			}
			while(m_taskListSize + 1 < vIDlist.size()){
				vIDlist.resize(vIDlist.size() + vIDlist.size() / 2);
			}
				task = new Task;
				task->fun = 0;
				task->data = NULL;
				task->taskID = 0;
				for(unsigned int i = 0; i < vIDlist.size();++ i){
					if(! vIDlist[i]){
						vIDlist[i] = 1;
						task->taskID = i;
						break;
					}
				}
			//	task->taskID = ++ m_poolSize;
				m_taskListSize ++;
				task->dependencelist.clear();
				task->state = 0;
				task->last_time = time(0);
				UnlockIdle();
				break;	

	//		UnlockIdle();				
	//		Sleep(30);
		}
	//	std::cout<<task->taskID<<std::endl;
	return task;
}


void TaskPool::SaveIdleTask(Task*task)
{
	if(NULL != task)
	{
		task->fun = 0;
		task->data = NULL;
		task->last_time = time(0);
		task->dependencelist.clear();
		LockIdle();
		m_idleList.push_front(task);
		UnlockIdle();
	}
}

