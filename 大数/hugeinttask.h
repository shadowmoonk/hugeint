#include "stdafx.h"


#include "HUGEINT.h"

#ifndef __HUGEINTTASK_H_
#define __HUGEINTTASK_H_
namespace HUGEINT_H_NAMESPACE{

	enum TASK_TYPE{
		ENUM_TASKBASE,
		ENUM_HUGEINTTASK_MULFFT,
	};

	void static_taskthread(void* arg);


	class HUGEINT;
#ifdef __HUGEINT_STD__
	typedef std::vector< INT> DATA;
#else
	typedef HUGE_INT_CORE DATA;
#endif
	//	void FFT(DATA & v,unsigned int iSize,bool inverse_mark = false,unsigned int Module = M,unsigned int Root = iOrder_root);
	class HUGEINTTASK{
	public:
		TASK_TYPE iType;
		HUGEINT * p;
		HUGEINTTASK():iType(ENUM_TASKBASE){p = 0;};
	};

	class HUGEINTTASK_MULFFT :public HUGEINTTASK{
	public:
		HUGEINTTASK_MULFFT(){iType =ENUM_HUGEINTTASK_MULFFT; p = 0;};
		DATA * m_v;
		unsigned int m_iSize;
		bool m_inverse_mark;
		unsigned int m_Module;
		unsigned int m_Root;
	};
}
#endif