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
大数类存储具体数据类HUGE_INT_CORE头文件,

*/

#pragma once

#ifndef __HUGE_INT_CORE_H_
#define __HUGE_INT_CORE_H_

#include <iostream>
#include <vector>

//#ifndef HUGEINTDLL_EXPORTS
//#define HUGEINTDLL_EXPORTS
//#endif

#ifdef HUGEINTDLL_EXPORTS
#define HUGEINTDLL_API __declspec(dllexport)
#else
//#define HUGEINTDLL_API __declspec(dllimport)
#define HUGEINTDLL_API
#endif


namespace HUGEINT_H_NAMESPACE{

	typedef unsigned int INT;
	//typedef std::vector< INT> DATA;
	//typedef  DATA::iterator ITERATOR;
	typedef unsigned int SIZE_T;

	class HUGEINTDLL_API HUGE_INT_CORE
	{
		SIZE_T iLength,iLength_max;
		INT * pData;
		INT * pData_start;
		INT * newdata(INT * * pDest_start,SIZE_T & num,SIZE_T n);
	public:
		INT * getdata()const;
		HUGE_INT_CORE(const SIZE_T n = 0,const INT a = 0);
		HUGE_INT_CORE(const INT * pbegin,const INT * pend);
		HUGE_INT_CORE(const HUGE_INT_CORE & a);
		HUGE_INT_CORE(std::vector<unsigned int>::const_iterator pbegin,std::vector<unsigned int>::const_iterator pend);
		HUGE_INT_CORE(std::vector<int>::const_iterator pbegin,std::vector<int>::const_iterator pend);

		~HUGE_INT_CORE(void);

		SIZE_T size() const;
		void resize(const SIZE_T n = 0,const INT a = 0);
		void push_back(const INT a);
		void push_back (const INT * pBegin,const INT * pEnd);
		INT * pop_back();
		void clear();
		void swap(HUGE_INT_CORE & a);
		INT * begin()const;
		INT * end()const;

		void set_capacity(const SIZE_T n);



		HUGE_INT_CORE & operator = (const HUGE_INT_CORE & a);	
		HUGE_INT_CORE & operator = (const INT n);

		INT & operator [](SIZE_T n)const;
	};
}

#endif