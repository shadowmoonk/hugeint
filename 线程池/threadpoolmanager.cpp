#include "stdafx.h"
#include "threadpoolmanager.h"
#include "threadpool.h"
#include "taskpool.h"

#include <errno.h>
#include <string.h>

#pragma comment(lib, "pthreadVC2.lib")  //必须加上这句  
//#pragma comment(lib, "pthreaddll.lib")  //必须加上这句  
//pthreaddll.dll
/*#include <string.h>
#include <sys/time.h>
#include <stdio.h>*/
//   struct timeval time_beg, time_end;
ThreadPoolManager::ThreadPoolManager(const int &tastPoolSize ,const int &threadPoolMax  ,	const int &threadPoolPre )
: m_threadPool(NULL)
, m_taskPool(NULL)
, m_bStop(false)
{
	pthread_mutex_init(&m_mutex_task,NULL);
	pthread_cond_init(&m_cond_task, NULL);

	Init(tastPoolSize,threadPoolMax,threadPoolPre);

	/* memset(&time_beg, 0, sizeof(struct timeval));
	memset(&time_end, 0, sizeof(struct timeval)); 
	gettimeofday(&time_beg, NULL);*/
}

ThreadPoolManager::~ThreadPoolManager()
{
	StopAll();
	if(NULL != m_threadPool)
	{
		delete m_threadPool;
		m_threadPool = NULL;
	}
	if(NULL != m_taskPool)
	{
		delete m_taskPool;
		m_taskPool = NULL;
	}

	pthread_cond_destroy( &m_cond_task);
	pthread_mutex_destroy( &m_mutex_task );

	/*gettimeofday(&time_end, NULL);
	long total = (time_end.tv_sec - time_beg.tv_sec)*1000000 + (time_end.tv_usec - time_beg.tv_usec);
	printf("manager total time = %d\n", total);
	gettimeofday(&time_beg, NULL);*/
}

int ThreadPoolManager::Init(
							const int &tastPoolSize,
							const int &threadPoolMax,
							const int &threadPoolPre)
{
	if(m_threadPool == NULL)
	m_threadPool = new ThreadPool(this);
	if(NULL == m_threadPool)
	{
		return -1;
	}

	if(m_taskPool == NULL)
	m_taskPool = new TaskPool(tastPoolSize,this);
	if(NULL == m_taskPool)
	{
		return -2;
	}

	if(0>m_threadPool->InitPool(threadPoolMax, threadPoolPre))
	{
		return -3;
	}
	//启动线程池
	//启动任务池
	//启动任务获取线程，从任务池中不断拿任务到线程池中
	pthread_attr_t attr;
	pthread_attr_init( &attr );
	pthread_attr_setdetachstate( &attr, PTHREAD_CREATE_JOINABLE );
	pthread_create(&m_taskThreadId, &attr, TaskThread, this); //创建获取任务进程
	pthread_attr_destroy(&attr);
	return 0;
}

void ThreadPoolManager::StopAll()
{
	m_bStop = true;
	LockTask();
	pthread_cond_signal(&m_cond_task);
	UnlockTask();
	pthread_join(m_taskThreadId, NULL);
	//等待当前所有任务执行完毕
	m_taskPool->StopPool();
	m_threadPool->StopPool(true); // 停止线程池工作
}

void ThreadPoolManager::LockTask()
{
	pthread_mutex_lock(&m_mutex_task);
}

void ThreadPoolManager::UnlockTask()
{
	pthread_mutex_unlock(&m_mutex_task);
}

#include <iostream>
#include <Windows.h>
void* ThreadPoolManager::TaskThread(void* arg)
{
	ThreadPoolManager * manager = (ThreadPoolManager*)arg;
	while(1)
	{
		manager->LockTask(); //防止任务没有执行完毕发送了停止信号
		while(1) //将任务队列中的任务执行完再退出
		{
		//	printf("NEW -------------- \n");
			Task * task = manager->GetTaskPool()->GetTask();
			if(NULL == task)
			{
			//	if(! manager->GetTaskPool()->GetTaskListSize())
				break;
			//	else
			//		Sleep(30);
			}
			else
			{
		//		printf("运行一个新任务\n");
				manager->GetThreadPool()->Run(task);
		//		manager->GetTaskPool()->SaveActiveTask(task);
				//    manager->GetTaskPool()->SaveIdleTask(task);
			}
		}

		if(manager->GetStop())
		{
			manager->UnlockTask();
			break;
		}
		manager->TaskCondWait(); //等待有任务的时候执行
		manager->UnlockTask();
	}
	return 0;
}

ThreadPool * ThreadPoolManager::GetThreadPool()
{
	return m_threadPool;
}

TaskPool * ThreadPoolManager::GetTaskPool()
{
	return m_taskPool;
}


#include <Windows.h>

int ThreadPoolManager::AddTask(Task * task){

	if(0 == task->fun)
	{
		return 0;
	}
	if(!m_bStop)
	{   
		int iRet =  m_taskPool->AddTask(task);

		if(iRet == 0 && (0 == pthread_mutex_trylock(&m_mutex_task)) ) 
		{
			pthread_cond_signal(&m_cond_task);
			UnlockTask();
		}
		return iRet;
	}
	else
	{
		return -3;
	}
}
int ThreadPoolManager::SetTaskFree(unsigned int taskID){
	return m_taskPool->SetTaskFree(taskID);
}

Task* ThreadPoolManager::GetIdleTask(){
	return m_taskPool->GetIdleTask();
}

void ThreadPoolManager::UpdataTask(unsigned int taskID,unsigned int state){
	if(m_taskPool != NULL)
		m_taskPool->UpdataTask(taskID,state);
}	

Task_Join * ThreadPoolManager::join(std::list<unsigned int> & vTask_Join){
	return m_taskPool->join(vTask_Join);
}

void ThreadPoolManager::waittask(Task_Join * task_join){
	m_taskPool->waittask(task_join);
}

bool ThreadPoolManager::GetStop()
{
	return m_bStop;
}

void ThreadPoolManager::TaskCondWait()
{
	struct timespec to;
	memset(&to, 0, sizeof to);
	to.tv_sec = time(0) + 60;
	to.tv_nsec = 0;

	pthread_cond_timedwait( &m_cond_task, &m_mutex_task, &to); //60秒超时
}

void ThreadPoolManager::taskwake(){
	pthread_cond_signal(&m_cond_task);
}