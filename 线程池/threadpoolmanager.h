#include "stdafx.h"

#ifndef THREADPOOLMANAGER_H
#define THREADPOOLMANAGER_H
/* purpose @
 *      �������̣�
 *          �����̳߳غ�����أ��Ƚ������������أ�Ȼ����TaskThread�����������н�����ȡ�����뵽�̳߳���
 *      �������ܣ�
 *          1�������߳̿�����ҵ��æ��ʱ���Զ��˳����ֳ�ʱ�䲻ʹ�õ��߳�
 *          2������ؿ�����ҵ��æ��ʱ���Զ��ͷų�ʱ�䲻ʹ�õ���Դ����ͨ��commondef.h�޸ģ�
 *          3����������ʱ�������������������񣬵����������������ִ����Ϻ���˳���س�����������İ�ȫ�˳���
 *      �߳���Դ��
 *          �����Ԥ�����κδ����̵߳Ļ���ThreadPoolֻ�е��������ʱ���ʵ�ʴ�����Ҫ���̣߳�����̴߳�����Ϊ�û�ָ��
 *          ��manager���ٵ�ʱ��manager�ᴴ��һ�������������ִ����ϵļ���̣߳�ֻ�е���������ִ����Ϻ�manager������
 *          �߳������Ϊ��1��TaskPool�߳� + 1��manager��������߳� + ThreadPool����߳��� + 1��manager�˳�����߳� + 1�̳߳������߳��˳�����߳�
 *          �߳���С��Ϊ��1��TaskPool��������������Դ���ټ���߳� + 1��manager������������߳�
 *      ʹ�÷�����
 *          ThreadPoolManager manager;
 *          manager.Init(100000, 50, 5);//��ʼ��һ�������Ϊ10000���̳߳�����߳���50��Ԥ����5���̵߳Ĺ�����
 *          manager.run(fun, data); //���ִ������manager�У�funΪ����ָ�룬dataΪfun��Ҫ����Ĳ�����data����ΪNULL
 *
 * date    @ 2013.12.23
 * author  @ haibin.wang
 *
 *  ��ϸ�������ƿ����޸�commondef.h�е���ر���ֵ
 */

#include <list>

#include <pthread.h>
#include "taskpool.h"
typedef void (*task_fun)(void *);

class ThreadPool;
class Thread;
struct Task;
class TaskPool;

#define TASK_DEPENDENCE 0X0800
#define TASK_ACTIVE		0X0001

//#ifndef HUGEINTDLL_EXPORTS
//#define HUGEINTDLL_EXPORTS
//#endif

#ifdef HUGEINTDLL_EXPORTS
#define HUGEINTDLL_API __declspec(dllexport)
#else
//#define HUGEINTDLL_API __declspec(dllimport)
#define HUGEINTDLL_API
#endif

//HUGEINTDLL_API int PTW32_CDECL pthread_mutex_init (pthread_mutex_t * mutex,
//												   const pthread_mutexattr_t * attr);



class HUGEINTDLL_API ThreadPoolManager
{
public:
	ThreadPoolManager(const int &tastPoolSize = 300,const int &threadPoolMax = 8 ,	const int &threadPoolPre = 4);
    ~ThreadPoolManager();

    /* pur @ ��ʼ���̳߳�������أ�threadPoolMax > threadPoolPre > threadPoolMin >= 0
     * para @ tastPoolSize ����ش�С
     * para @ threadPoolMax �̳߳�����߳���
     * para @ threadPoolPre Ԥ�����߳���
     * return @ 0����ʼ���ɹ������� ��ʼ��ʧ��
     *          -1�������̳߳�ʧ��
     *          -2�����������ʧ��
     *          -3���̳߳س�ʼ��ʧ��
    */
    int Init(const int &tastPoolSize,
            const int &threadPoolMax,
            const int &threadPoolPre);

    /* pur @ ִ��һ������
     * para @ fun ��Ҫִ�еĺ���ָ��
     * para @ arg fun��Ҫ�Ĳ�����Ĭ��ΪNULL
     * return @ 0 �������ɹ������� �������ʧ��
     *          -1:�������
     *          -2:�����newʧ��
     *          -3:manager�Ѿ�����ֹͣ�źţ����ٽ���������
    */
 //   int Run(task_fun fun,void* arg=NULL);
	int AddTask(Task * task);
	int SetTaskFree(unsigned int taskID);
	 Task* GetIdleTask();

	void UpdataTask(unsigned int taskID,unsigned int state = TASK_ACTIVE);	
	Task_Join * join(std::list<unsigned int> & vtask_join);
	void waittask(Task_Join * task_join);

	void taskwake();
public: //����public������Ҫ���ھ�̬��������
    bool GetStop();
    void TaskCondWait();
    TaskPool * GetTaskPool();
    ThreadPool * GetThreadPool();
    void LockTask();
    void UnlockTask();
    void LockFull();

private:
	static void * TaskThread(void*); //�������߳�
	void StopAll();
    
private:
    ThreadPool *m_threadPool; //�̳߳�
    TaskPool * m_taskPool; //�����
    bool m_bStop; // �Ƿ���ֹ������
    
    pthread_t m_taskThreadId; // TaskThread�߳�id
	pthread_mutex_t m_mutex_task;
    pthread_cond_t m_cond_task;
};
#endif
