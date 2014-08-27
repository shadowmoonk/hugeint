#include "stdafx.h"

#include<stdio.h>  
#include<pthread.h>  
#include<Windows.h>  
#pragma comment(lib, "pthreadVC2.lib")  //必须加上这句  


#include <iostream>
#include <string>
#include "threadpoolmanager.h"
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>

#include <vector>

using namespace std;
int seq = 0;
int billNum =0;
int inter = 1;
pthread_mutex_t m_mutex;

unsigned int iFuncTest = 0;
void myFunc(void*arg)
{
	pthread_mutex_lock(&m_mutex);
	seq++;
	if(seq%inter == 0 )
	{
		cout << "fun 1=" << seq << endl;
	}
	if(seq>=1000000000)
	{
		cout << "billion" << endl;
		seq = 0;
		billNum++;
	}
	iFuncTest ++;
//	cout<<iFuncTest ++ <<"---测试任务"<<endl;
	pthread_mutex_unlock(&m_mutex);
	Sleep(15);
}

int _tmain(int argc, _TCHAR* argv[])
{
	if(argc != 6)
	{
		cout << "必须有5个参数 任务执行次数 任务池大小 线程池大小 预创建线程数 输出间隔" << endl;
		cout << "eg: ./test 999999 10000 100 10 20" << endl;
		cout << "上例代表创建一个间隔20个任务输出，任务池大小为10000，线程池大小为100，预创建10个线程，执行任务次数为：999999" << endl;
		//	return 0;
	}
	/*
	double loopSize = atof(argv[1]);
	int taskSize = atoi(argv[2]);
	int threadPoolSize = atoi(argv[3]);
	int preSize = atoi(argv[4]);
	inter = atoi(argv[5]);
	*/
	unsigned int loopSize = 100;
	int taskSize = 8;
	int threadPoolSize = 5;
	int preSize = 3;
	inter = 300;

	pthread_mutex_init(&m_mutex,NULL);
	ThreadPoolManager manager;
//	if(0>manager.Init(taskSize,  threadPoolSize, preSize))
	{
	//	cout << "初始化失败" << endl;
	//	return 0;
	}
	cout << "*******************初始化完成*********************" << endl;
	struct timeval time_beg, time_end;
	memset(&time_beg, 0, sizeof(struct timeval));
	memset(&time_end, 0, sizeof(struct timeval)); 
	//	gettimeofday(&time_beg, NULL);
	int i=0;
	Task * task;
	unsigned int taskID = 0;
	Task_Join * pTask_Jion = NULL;

	time_t iTestTime = clock();
	iFuncTest = 0;
	vector<Task *> vID;
	vID.resize(loopSize);

	unsigned int iDUAN = 10;
	std::list<unsigned int> tempList;
	tempList.clear();	
	for(i = 0; i<loopSize; ++i)
	{
		task = manager.GetIdleTask();
		vID[i] = task;
		task->fun = myFunc;
		task->data = NULL;
		task->state = 0;
		if(i % iDUAN != 0){
			//	task->dependencelist.clear();
			task->dependencelist.push_back(vID[i - 1]->taskID);
		}
		if(i % iDUAN == iDUAN - 1){
			tempList.push_back(vID[i]->taskID);
		}
		task->state = TASK_DEPENDENCE;
		manager.AddTask(vID[i]);
	}


	pTask_Jion = manager.join(tempList);

	//把任务增加进去,
	for(i = 0; i<loopSize; ++i){
	//	manager.AddTask(vID[i]);
		manager.SetTaskFree(vID[i]->taskID);
	}



//	Sleep(1000);

	manager.waittask(pTask_Jion);
	iTestTime = clock() - iTestTime;
	cout<<"多线程花费时间"<<iTestTime<<endl;
//	Sleep(2000);
	cout<<"保证线程不会被优化"<<iFuncTest<<endl;


	iFuncTest = 0;
	iTestTime = clock();
	for(i = 0; i<loopSize; ++i){
		//	manager.AddTask(vID[i]);
		myFunc(NULL);
	}
	iTestTime = clock() - iTestTime;
	cout<<"单线程执行时间"<<iTestTime<<endl;
	//	Sleep(1200);
	//	gettimeofday(&time_end, NULL);
	long total = (time_end.tv_sec - time_beg.tv_sec)*1000000 + (time_end.tv_usec - time_beg.tv_usec);
	cout << "total time =" << total << endl;
	cout << "total num =" << i  << " billion num=" << billNum<< endl;
	cout << __FILE__ << "将关闭所有线程" << endl;
	cout<<"保证线程不会被优化"<<iFuncTest<<endl;
	//pthread_mutex_destroy(&m_mutex);
	//	Sleep(800);
	return 0;
}
