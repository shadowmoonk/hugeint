#include "StdAfx.h"
#include "HUGEINT.h"

#include "hugeinttask.h"

namespace HUGEINT_H_NAMESPACE{
//任务处理线程
	//	FFT(DATA & vSource,unsigned int iSize,bool inverse_mark,unsigned int Module,unsigned int Root)
	void HUGEINT::TaskThread(void * arg){
		HUGEINTTASK * task = (HUGEINTTASK *) arg;
		do{
			if(task->iType == ENUM_HUGEINTTASK_MULFFT){
				HUGEINTTASK_MULFFT * pt = (HUGEINTTASK_MULFFT *)task;
				pt->p->FFT(* pt->m_v,pt->m_iSize,pt->m_inverse_mark,pt->m_Module,pt->m_Root);
				break;
			}

			break;
		}while(0);
	}
}


