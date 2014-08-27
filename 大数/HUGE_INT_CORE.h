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