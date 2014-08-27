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
大数类HUGEINT头文件,
调用时必须包含这个头文件
*/
//#pragma once
#include "stdafx.h"

#include <iostream>
#include <vector>



#ifndef __HUGEINT_H_
#define __HUGEINT_H_

#include "HUGE_INT_CORE.h"
#include "threadpoolmanager.h"
#include "hugeinttask.h"

//#define __HUGEINT_DEBUG_2MOUDULE_
#define __HUGEINT_DEBUG_2MOUDULE_PRI_

//#define __HUGEINT_STD__

namespace HUGEINT_H_NAMESPACE{


	class HUGE_INT_CORE;

	typedef unsigned int INT;

#ifdef __HUGEINT_STD__
	typedef std::vector< INT> DATA;
#else
	typedef HUGE_INT_CORE DATA;
#endif
	//typedef  DATA::iterator ITERATOR;
	typedef unsigned int SIZE_T;


	
#ifdef __HUGEINT_DEBUG_2MOUDULE_
	enum MOD_DATA{
		iOrder_root = 21,
		iOrder_root_second = 8,
		iOrder_root_third = 2,

		M = 2013265921,
		M_second = 1107296257,
		M_third = 167772161,

		Modular_inversion = 894784856,
	//	Modular_inversion_second = 101861669,
		Modular_inversion_third = 128007980,
	};
#else
	enum MOD_DATA{
		iOrder_root = 170,				//21
		iOrder_root_second = 103,			//8
		iOrder_root_third = 40,			//2

		M = 2013265921,
		M_second = 1811939329,
		M_third = 1711276033,

		Modular_inversion = 671088707,
		Modular_inversion_second = 1811939167,
	//	Modular_inversion_third = 128007980,
	};

#endif

	enum ENUM_DATA{

		ENUM_RADIX = 1000000000,						//进制 1000000000
		ENUM_NEWTON_FORECAST = 5,				//牛顿迭代进行预测的分界点,随便写,
		//{1,18,68,200};optimizations
		ENUM_MUL_OPTIMIZATION_ZERO	= 1,			//代码长度为0,
		ENUM_MUL_OPTIMIZATION_INT	= 2,			//乘数为INT
		ENUM_MUL_OPTIMIZATION_HARD	= 18,			//18
		ENUM_MUL_OPTIMIZATION_KARATSUBA	= 68,		//68
		ENUM_MUL_OPTIMIZATION_TOOM3		= 2,		//100
		ENUM_MUL_OPTIMIZATION_TOOM4		= 2,		//130
		ENUM_MUL_OPTIMIZATION_FFT		= 2,			//可能会利用多线程
		ENUM_MUL_OPTIMIZATION_TOOM3_AND_FFT = 1 << 12,	//这个必须有多线才能较为有效提速,
		ENUM_MUL_OPTIMIZATION_SECTION	= 4,
		
	};

	class HUGEINTDLL_API HUGEINT
	{
	private:
		//------------------------------------------------
		//两个组合
		/*
#ifdef __HUGEINT_DEBUG_2MOUDULE_
		const static INT  iOrder_root = 8,iOrder_root_second = 21,iOrder_root_third = 2;//vOrder_root = 2,vOrder_root_second = 8,vOrder_root_third = 21;
		const static INT M = 1107296257,M_second = 2013265921,M_third = 167772161;
		const static INT Modular_inversion = 435009245,Modular_inversion_second = 101861669,Modular_inversion_third = 128007980;
#else
		const static INT  iOrder_root = 21,iOrder_root_second = 8,iOrder_root_third = 2;//vOrder_root = 21,vOrder_root_second = 8,vOrder_root_third = 2;
		const static INT M = 2013265921,M_second = 1107296257,M_third = 167772161;// M = 2013265921,M_second = 1107296257,M_third = 167772161;
		const static INT Modular_inversion = 1891249807,Modular_inversion_second = 329552456;//,Modular_inversion_third = 128007980;
#endif
		*/


		//--------------------------------------------------
		//3个组合

		//  128007980 							  329552456							  1891249807  逆,	//M < M_second < M_third

		//101861669  435009245		// M = 167772161; M_second = 1107296257;
		//这里的模逆是指CRT组合里使用的Mi,

		bool bSymbol;					//符号
	//	int iExponent;					//指数
		DATA vData;						//真实数据.
		//static const INT iRADIX = 10;
		static INT iRADIX;
	//	static SIZE_T iTEST_TEMP;


		////////////////////////////////////////////////////////////////////////////////
		//汇编优化函数
		unsigned int asm_bsr(unsigned int n);	//逆向扫描指令BSR(Bit Scan Reverse)从左向右扫描，即：从高位向低位扫描。
		unsigned int sm_bsf(unsigned int n);	//正向扫描指令BSF(Bit Scan Forward)从右向左扫描，即：从低位向高位扫描；
		unsigned int asm_mul_mod(unsigned int a,unsigned int b,unsigned int p);			// a*b%p
		unsigned int asm_mul_mod(unsigned int a,unsigned int b,unsigned int p,unsigned int & quotient);

		//以下两个是基于iRADIX = 10^9,其他的不适用,
		// if(a > b) a - b; a < b + b,两倍内剩余系调整
		unsigned int asm_remaind_mod(unsigned int a);
		//if(a > b),carry = 1,return a - b; a < b + b,两倍内调整
		unsigned int asm_remaind_mod(unsigned int a,unsigned int & carry);
		unsigned int asm_remaind_mod_module(unsigned int a,unsigned int module);					//这两个module必须要求为偶数,且 module < (1 <<31),
		unsigned int asm_remaind_mod_module(unsigned int a,unsigned int module,unsigned int & carry);

		unsigned int pow_mod(unsigned int a,unsigned int b,unsigned int mod);

		void adjust_add();
		void adjust();
		////////////////////////////////////////////////////////////////////////////////
		//调整所需要的函数
		SIZE_T num_end_zero(const DATA & v);
		SIZE_T num_front_zero(const DATA & v);

		void shift_right(DATA & v,const SIZE_T n);
		void shift_left(DATA & v,const SIZE_T n);

		////////////////////////////////////////////////////////////////////////////////
		//核心比较函数
		//必须重载== ,!= ,< <=, 为以后有话考虑,还添加了错位比较,
		bool abs_compare_equal(const DATA & v) const;
		bool abs_compare_noequal(const DATA & v) const;
		bool abs_compare_min(const DATA & v) const;
		bool abs_compare_min(const DATA & v,const SIZE_T n) const;
		bool abs_compare_nomax(const DATA & v) const;
		bool abs_compare_nomax(const DATA & v,const SIZE_T n) const;


		////////////////////////////////////////////////////////////////////////////////
		//运算内核.
		//乘法的核心,FFT,
		void FFT(DATA & v,unsigned int iSize,bool inverse_mark = false,unsigned int Module = M,unsigned int Root = iOrder_root);
		void mul_FFT_module(DATA & vSource,unsigned int Module,unsigned int Root,bool bPow = false);
		void mul_FFT_toom_module(DATA & vDest,DATA & vTemp_dest1,DATA & vTem_dest2,DATA & vTemp_dest3,
								const DATA & v1,const DATA & v2,const DATA & v3,const DATA & v4,const DATA v5,const DATA & v6);
		void mul_FFT_toom(const DATA & vSource,SIZE_T iLength_section1,SIZE_T iLength_section2,bool bPow = false);
		void mul_FFT_toom(const DATA & vSource,bool bPow = false);
		void mul_FFT(const DATA & vSource,bool bPow = false);
		void CRT(DATA & vDest,const DATA & v1,const DATA & v2,const DATA & v3);


		HUGEINT mul_single(const unsigned int n);	//这个不是完整的乘法,需要保证被乘数不改变,
		void mul_single(DATA & v,const DATA & vSource,const unsigned int n);
		void mul_hard(const DATA & vSource);			//以下是完整的乘法,最终结果赋值在本对象内.
		void mul_karatsuba(const DATA & vSource);
		void mul_toom_cook_3(const DATA & vSource);
		void mul_toom_cook_4(const DATA & vSource);		//未完成,勿用

		void mul_section(const DATA & vSource);


		unsigned int div_3_2_int(unsigned int d,unsigned int e,unsigned int a ,unsigned int b,unsigned int c);
		void div_single(const unsigned int n,std::pair<HUGEINT,HUGEINT> & pairTemp);
		void div_hard(const HUGEINT & a,std::pair<HUGEINT,HUGEINT> & pairTemp);
		void div_newton(const HUGEINT & a,std::pair<HUGEINT,HUGEINT> & pairTemp);
		void div_newton(const HUGEINT & a,std::pair<HUGEINT,HUGEINT> & pairTemp,const HUGEINT & cReciprocal);
		void reciprocal_newton(HUGEINT & a,const DATA & v,SIZE_T n = 0);						//基于优化考虑, 实际精度仅仅保证不小于  n / 2的精度,
		//实际精确有效数字返回的为返回长度的一般,

		bool forecast_quotient_newton(HUGEINT & cQuotient,const HUGEINT & a,const HUGEINT & cReciprocal);

		void add(const DATA & v);
		void sub(const DATA & v,bool bMin = false);
		void add(const DATA & v,const unsigned int n);//所有的偏移量都是表示后面的偏移量,
		void sub_shift(const DATA & v,const unsigned int n = 0,bool bMin = false);

	public:
		void mul_order(HUGEINT & a);				//为被乘数小于乘数长度,而且,乘数可以无需保留时,可以优化减少 乘数 copy的几率,
		//一般只适合类内部使用.但提供对外接口.
	private:
		void mul(const DATA & v);
		void div(const DATA & v,std::pair<HUGEINT,HUGEINT> & pairTemp);
		void mod(const DATA & v,std::pair<HUGEINT,HUGEINT> & pairTemp);

		/////////////////////////////////////////////////////////////////////////////////////////////////////////
		//mod计算卷积所需函数,
		//这里基于考虑效率问题,似乎已经没必要支持 std::vector<unsigned int> 
		//而且不会再检查访问越界问题

		bool abs_compare_min_mod(INT * pDest,INT * pSource,SIZE_T n);
		bool abs_compare_min_reduction_down(INT * pDest,SIZE_T n);
		void mul_mod(INT * pDest,INT * pSource,SIZE_T n);
		INT add_mod(INT * pDest,INT * pSource_up,INT * pSource_down,SIZE_T n); 
		INT add_mod(INT * pDest,INT a,SIZE_T n);
		INT sub_mod(INT * pDest,INT * pSource_up,INT * pSource_down,SIZE_T n);
		INT reduction_up(INT * pDest,INT * pSource_up,INT * pSource_down,SIZE_T n);		//标志位返回,借位
		INT reduction_down(INT * pDest,INT * pSource_up,INT * pSource_down,SIZE_T n);		////标志位返回,进位
		void normalized(INT * pDest,INT * pSource_up,INT * pSource_down,SIZE_T n);
	public:
			void TaskThread(void*); //任务处理线程
	private:
		ThreadPoolManager manager;
	public:
		HUGEINT(const int n = 0);
		HUGEINT(const DATA & v);
#ifndef __HUGEINT_STD__
		HUGEINT::HUGEINT(const std::vector< INT> & v);		//由于使用自定义类型,额外提供vector<unsigned int> 作为构造参数传入,并在其后数据进行规范化
#endif
		HUGEINT(const HUGEINT & a);
		SIZE_T size();
		~HUGEINT(void);

		HUGEINT & operator = (const int n);
		HUGEINT & operator = (const HUGEINT & a);
		void swap(HUGEINT & a);

		INT operator [] (SIZE_T n)const;		//这里考虑数据规范问题,这里 []运算并不返回引用提供修改的能力,

		////////////////////////////////////////////////////////////////////////////////
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//运算符
		bool operator == (const int n) const;
		bool operator != (const int n) const;
		bool operator < (const int n) const;
		bool operator > (const int n) const;
		bool operator <= (const int n) const;
		bool operator >= (const int n) const;

		bool operator == (const HUGEINT & a) const;
		bool operator != (const HUGEINT & a) const;
		bool operator <	(const HUGEINT & a) const;
		bool operator > (const HUGEINT & a) const;
		bool operator <= (const HUGEINT & a) const;
		bool operator >= (const HUGEINT & a) const;


		////////////////////////////////////////////////////////////////////////////////
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//四则运算

		HUGEINT & operator += (const int n);
		HUGEINT & operator -= (const int n);
		HUGEINT & operator *= (const int n);
		HUGEINT & operator /= (const int n);
		HUGEINT & operator %= (const int n);

		HUGEINT operator + (const int n)const;
		HUGEINT operator - (const int n)const;
		HUGEINT operator * (const int n)const;
		HUGEINT operator / (const int n)const;
		HUGEINT operator % (const int n)const;

		HUGEINT & operator += (const HUGEINT & a);
		HUGEINT & operator -= (const HUGEINT & a);
		HUGEINT & operator *= (const HUGEINT & a);
		HUGEINT & operator /= (const HUGEINT & a);
		HUGEINT & operator %= (const HUGEINT & a);

		HUGEINT operator + (const HUGEINT & a) const;
		HUGEINT operator - (const HUGEINT & a) const;
		HUGEINT operator * (const HUGEINT & a) const;
		HUGEINT operator / (const HUGEINT & a) const;
		HUGEINT operator % (const HUGEINT & a) const;

		////////////////////////////////////////////////////////////////////////////////
		HUGEINTDLL_API friend std::ostream & operator << (std::ostream & fOutput,const HUGEINT & a);			//声明重载运算符“<<”
		HUGEINTDLL_API friend std::istream & operator >> (std::istream & fInput,const HUGEINT & a);			//声明重载运算符“>>”

		////////////////////////////////////////////////////////////////////////////////
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//开放函数,大致有幂,平方,开平方,开立方,开平方倒数,

		HUGEINT radix_pow(const int n) const;

		HUGEINT pow(const SIZE_T n);
		HUGEINT pow2();

		HUGEINT reciprocal_precision(SIZE_T n = 0)const;

		HUGEINT sqrt();
		HUGEINT inverse_sqrt();

		HUGEINT gcd(const HUGEINT & a);
		HUGEINT lcm(const HUGEINT & a);
		bool isprime();			//素性检测


		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void HUGEINT::test(const HUGEINT & a = 0,const HUGEINT & b = 0);
	};

}
#endif