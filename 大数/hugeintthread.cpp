/* .
Copyright 2010-2014 ShadowMoonk.

This file is part of the HUGEINT Library.

The HUGEINT Library is free software; you can redistribute it and/or modify
it under the terms of either:

  * the GNU Lesser General Public License as published by the Free
    Software Foundation; either version 3 of the License, or (at your
    option) any later version.

or

  * the GNU General Public License as published by the Free Software
    Foundation; either version 2 of the License, or (at your option) any
    later version.

or both in parallel, as here.

The HUGEINT Library is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received copies of the GNU General Public License and the
GNU Lesser General Public License along with the HUGEINT Library.  If not,
see https://www.gnu.org/licenses/.  */

/*
大数 线程 实际意义上的winproc函数
*/
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


