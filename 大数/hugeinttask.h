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
大数多线程任务映射类.
主要通过 基类HUGEINTTASK进行多态散发任务.
*/
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