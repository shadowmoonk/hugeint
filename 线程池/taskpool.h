#include "stdafx.h"

#ifndef TASKPOOL_H
#define TASKPOOL_H
/* purpose @ ����أ���Ҫ�ǻ����ⲿ�߲�������������manager�����������
 *          ����ؿ��Զ����ٳ�ʱ����е�Task����
 *          ��ͨ��CHECK_IDLE_TASK_INTERVAL���ü��idle���н�����ѵ�ȴ�ʱ��
 *          TASK_DESTROY_INTERVAL ����Task����ʱ�䣬�������ʱ��ֵ���ᱻCheckIdleTask�߳�����
 * date    @ 2013.12.23
 * author  @ haibin.wang
 */

#include <list>
#include <vector>
#include <pthread.h>
#include "commondef.h"

#define TASK_DEPENDENCE 0X0800
#define TASK_ACTIVE		0X0001


class ThreadPoolManager;
class Thread;
//���е��û�����Ϊһ��task��
typedef void (*task_fun)(void *);
struct Task
{	
	unsigned int taskID;
	unsigned int state;	//0:û������,��ɺ�������ٻ�Ͷ������������б��Թ�ʹ��,1,����������ɾ��

    task_fun fun; //��������
    void* data; //����������
    time_t last_time; //������ж��е�ʱ�䣬�����Զ�����

	std::list<unsigned int> dependencelist;
};

struct Task_Join{
	pthread_mutex_t mutex_task;
	pthread_cond_t cond_task;
	bool bActive;
	std::list<unsigned int> vTask_Join;
};

//����أ����������Ͷ�ݵ�������У������̸߳�������Ͷ�ݸ��̳߳�
class TaskPool
{
public:
	/* pur @ ��ʼ������أ���������ؿ��ж����Զ������߳�
     * para @ maxSize ���������������0
    */ 
    TaskPool(const int & poolMaxSize,ThreadPoolManager * threadpoolmanager = NULL);
    ~TaskPool();

	void InitPool(unsigned int  poolMaxSize);

    /* pur @ �������������е�β��
     * para @ task�� ��������
     * return @ 0 ��ӳɹ������� ���ʧ��
    */    
    int AddTask(Task * task);
	int SetTaskFree(unsigned int taskID);
	
	
    /* pur @ �������б��ͷ��ȡһ������
     * return @  ����б����������򷵻�һ��Taskָ�룬���򷵻�һ��NULL
    */    
    Task* GetTask();

    /* pur @ ����������񵽿��ж�����
     * para @ task �ѱ�����ִ�е�����
     * return @ 
	 */
	void SaveIdleTask(Task*task);

    Task* GetIdleTask();
//	void SaveFinalTask(Task*task);
	
    void StopPool();

	void UpdataTask(unsigned int taskID,unsigned int state = TASK_ACTIVE);
	Task_Join * join(std::list<unsigned int> & vtask_join);
	void waittask(Task_Join * task_join);
public:
    void LockIdle();
    void UnlockIdle();
    void CheckIdleWait();
    int RemoveIdleTask();
    bool GetStop();
private:
    static void * CheckIdleTask(void *);
    /* pur @ ��ȡ���е�task
     * para @ 
     * para @ 
     * return @ NULL˵��û�п��еģ������m_idleList�л�ȡһ��
    */

public:
    int GetTaskListSize();
private:
//    int m_poolSize; //����ش�С
    unsigned int m_taskListSize; // ͳ��taskList�Ĵ�С����Ϊ��List�Ĵ�С�������������������ʱ����
    bool m_bStop; //�Ƿ�ֹͣ
    std::list<Task*> m_taskList;//���д����������б�
	std::list<Task*> m_activeList;//�������е�����
    std::list<Task*> m_idleList;//���п��������б�
//	std::list<Task*> m_FinalList;	//�Ѿ���������߳�
    pthread_mutex_t m_lock; //�������б���м�������֤ÿ��ֻ��ȡһ������
    pthread_mutex_t m_idleMutex; //�������������
    pthread_cond_t m_idleCond; //���ж��еȴ�����
    pthread_t m_idleId;

	std::vector<unsigned int> vIDlist;

	std::list<Task_Join*> m_taskjoinList;
	ThreadPoolManager * m_threadpoolmanager;	//��¼�����̳߳ع�����
};
#endif
