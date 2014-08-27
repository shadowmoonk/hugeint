#include "stdafx.h"

#ifndef THREADPOOL_H
#define THREADPOOL_H
/* purpose @ �̳߳��࣬�����̵߳Ĵ��������٣�ʵ���̳߳�ʱ�Զ��˳�����(��פ��)a
 *          ���̳߳��˳�ʱ����TerminalCheck�̣߳��������̳߳������߳��˳�
 * date    @ 2013.12.23
 * author  @ haibin.wang
 */

#include <list>
#include <string>
#include "taskpool.h"
//ͨ��threadmanager������������Ƚ���
//threadpool��TerminalCheck�̸߳������̳߳������߳��˳�

class ThreadPoolManager;
class ThreadPool;

class Thread
{
public:
    Thread(bool detach, ThreadPool * pool);
    ~Thread();
    pthread_t  m_threadId; //�߳�id
    pthread_mutex_t m_mutex; //������
    pthread_cond_t m_cond; //��������
    pthread_attr_t m_attr; //�߳�����
	Task task; // 
    ThreadPool * m_pool; //�����̳߳�
};

//�̳߳أ����𴴽��̴߳������񣬴�����Ϻ�Ὣ�̼߳��뵽���ж�����,���������
class ThreadPool
{
public:
    ThreadPool(ThreadPoolManager * threadpoolmanager = NULL);
    ~ThreadPool();

    /* pur @ ��ʼ���̳߳�
     * para @ poolMax �̳߳�����߳���
     * para @ poolPre Ԥ�����߳���
     * return @ 0:�ɹ�
     *          -1: parameter error, must poolMax > poolPre >=0
     *          -2: �����߳�ʧ��
    */
    int InitPool(const int & poolMax, const int & poolPre);

    /* pur @ ִ��һ������
     * para @ task ����ָ��
     * return @ 0�������ɹ�����ֵ �������ʧ�ܣ�-1���������߳�ʧ��
    */
    int Run(Task * task);
	
	/* pur @ �����Ƿ�ֹͣ�̳߳ع���
     * para @ bStop trueֹͣ��false��ֹͣ
    */
	void StopPool(bool bStop);
    
public: //�˹��к�����Ҫ���ھ�̬��������
    /* pur @ ��ȡ���̳ص���ͣ״̬
     * return @ 
    */
    bool GetStop();    
	void SaveIdleThread(Thread * thread );
    void LockMutex();
    void UnlockMutex();
    void DecreaseTotalNum();
    void IncreaseTotalNum();
    void RemoveThread(Thread* thread);
    void TerminalCondSignal();
    int TotalThreads();
    void SendSignal();
private:
	/* pur @ �����߳�
     * return @ �ǿ� �ɹ���NULLʧ�ܣ�
    */
	Thread * CreateThread();

    /* pur @ ���̳߳��л�ȡһ��һ���߳���������
     * para @ fun ����ָ��
     * para @ arg ��������
     * return @ 
    */
    void GetThreadRun(Task * task);

	static void * WapperFun(void*);
	static void * TerminalCheck(void*);//ѭ������Ƿ������߳���ֹ�߳�

private:
    int m_poolMax;//�̳߳�����߳���
    int m_idleNum; //�����߳���
    int m_totalNum; //��ǰ�߳����� С������߳���	
	bool m_bStop; //�Ƿ�ֹͣ�̳߳�
	pthread_mutex_t m_mutex; //�߳��б���
	pthread_mutex_t m_runMutex; //run������

    pthread_mutex_t m_terminalMutex; //��ֹ�����̻߳�����
    pthread_cond_t  m_terminalCond; //��ֹ�����߳���������
    pthread_cond_t  m_emptyCond; //�����̲߳�����������
	
    std::list<Thread*> m_threads; // �߳��б�
	ThreadPoolManager * m_threadpoolmanager;	//��¼�����̳߳ع�����
};
#endif
