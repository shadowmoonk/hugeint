#include "StdAfx.h"
#include "hugeinttask.h"
#include "HUGEINT.h"

namespace HUGEINT_H_NAMESPACE{

	void static_taskthread(void* arg){
		HUGEINTTASK * task = (HUGEINTTASK *) arg;
		task->p->TaskThread(task);
	}
}
