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
������HUGEINTͷ�ļ�,
����ʱ����������ͷ�ļ�
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

		ENUM_RADIX = 1000000000,						//���� 1000000000
		ENUM_NEWTON_FORECAST = 5,				//ţ�ٵ�������Ԥ��ķֽ��,���д,
		//{1,18,68,200};optimizations
		ENUM_MUL_OPTIMIZATION_ZERO	= 1,			//���볤��Ϊ0,
		ENUM_MUL_OPTIMIZATION_INT	= 2,			//����ΪINT
		ENUM_MUL_OPTIMIZATION_HARD	= 18,			//18
		ENUM_MUL_OPTIMIZATION_KARATSUBA	= 68,		//68
		ENUM_MUL_OPTIMIZATION_TOOM3		= 2,		//100
		ENUM_MUL_OPTIMIZATION_TOOM4		= 2,		//130
		ENUM_MUL_OPTIMIZATION_FFT		= 2,			//���ܻ����ö��߳�
		ENUM_MUL_OPTIMIZATION_TOOM3_AND_FFT = 1 << 12,	//��������ж��߲��ܽ�Ϊ��Ч����,
		ENUM_MUL_OPTIMIZATION_SECTION	= 4,
		
	};

	class HUGEINTDLL_API HUGEINT
	{
	private:
		//------------------------------------------------
		//�������
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
		//3�����

		//  128007980 							  329552456							  1891249807  ��,	//M < M_second < M_third

		//101861669  435009245		// M = 167772161; M_second = 1107296257;
		//�����ģ����ָCRT�����ʹ�õ�Mi,

		bool bSymbol;					//����
	//	int iExponent;					//ָ��
		DATA vData;						//��ʵ����.
		//static const INT iRADIX = 10;
		static INT iRADIX;
	//	static SIZE_T iTEST_TEMP;


		////////////////////////////////////////////////////////////////////////////////
		//����Ż�����
		unsigned int asm_bsr(unsigned int n);	//����ɨ��ָ��BSR(Bit Scan Reverse)��������ɨ�裬�����Ӹ�λ���λɨ�衣
		unsigned int sm_bsf(unsigned int n);	//����ɨ��ָ��BSF(Bit Scan Forward)��������ɨ�裬�����ӵ�λ���λɨ�裻
		unsigned int asm_mul_mod(unsigned int a,unsigned int b,unsigned int p);			// a*b%p
		unsigned int asm_mul_mod(unsigned int a,unsigned int b,unsigned int p,unsigned int & quotient);

		//���������ǻ���iRADIX = 10^9,�����Ĳ�����,
		// if(a > b) a - b; a < b + b,������ʣ��ϵ����
		unsigned int asm_remaind_mod(unsigned int a);
		//if(a > b),carry = 1,return a - b; a < b + b,�����ڵ���
		unsigned int asm_remaind_mod(unsigned int a,unsigned int & carry);
		unsigned int asm_remaind_mod_module(unsigned int a,unsigned int module);					//������module����Ҫ��Ϊż��,�� module < (1 <<31),
		unsigned int asm_remaind_mod_module(unsigned int a,unsigned int module,unsigned int & carry);

		unsigned int pow_mod(unsigned int a,unsigned int b,unsigned int mod);

		void adjust_add();
		void adjust();
		////////////////////////////////////////////////////////////////////////////////
		//��������Ҫ�ĺ���
		SIZE_T num_end_zero(const DATA & v);
		SIZE_T num_front_zero(const DATA & v);

		void shift_right(DATA & v,const SIZE_T n);
		void shift_left(DATA & v,const SIZE_T n);

		////////////////////////////////////////////////////////////////////////////////
		//���ıȽϺ���
		//��������== ,!= ,< <=, Ϊ�Ժ��л�����,������˴�λ�Ƚ�,
		bool abs_compare_equal(const DATA & v) const;
		bool abs_compare_noequal(const DATA & v) const;
		bool abs_compare_min(const DATA & v) const;
		bool abs_compare_min(const DATA & v,const SIZE_T n) const;
		bool abs_compare_nomax(const DATA & v) const;
		bool abs_compare_nomax(const DATA & v,const SIZE_T n) const;


		////////////////////////////////////////////////////////////////////////////////
		//�����ں�.
		//�˷��ĺ���,FFT,
		void FFT(DATA & v,unsigned int iSize,bool inverse_mark = false,unsigned int Module = M,unsigned int Root = iOrder_root);
		void mul_FFT_module(DATA & vSource,unsigned int Module,unsigned int Root,bool bPow = false);
		void mul_FFT_toom_module(DATA & vDest,DATA & vTemp_dest1,DATA & vTem_dest2,DATA & vTemp_dest3,
								const DATA & v1,const DATA & v2,const DATA & v3,const DATA & v4,const DATA v5,const DATA & v6);
		void mul_FFT_toom(const DATA & vSource,SIZE_T iLength_section1,SIZE_T iLength_section2,bool bPow = false);
		void mul_FFT_toom(const DATA & vSource,bool bPow = false);
		void mul_FFT(const DATA & vSource,bool bPow = false);
		void CRT(DATA & vDest,const DATA & v1,const DATA & v2,const DATA & v3);


		HUGEINT mul_single(const unsigned int n);	//������������ĳ˷�,��Ҫ��֤���������ı�,
		void mul_single(DATA & v,const DATA & vSource,const unsigned int n);
		void mul_hard(const DATA & vSource);			//�����������ĳ˷�,���ս����ֵ�ڱ�������.
		void mul_karatsuba(const DATA & vSource);
		void mul_toom_cook_3(const DATA & vSource);
		void mul_toom_cook_4(const DATA & vSource);		//δ���,����

		void mul_section(const DATA & vSource);


		unsigned int div_3_2_int(unsigned int d,unsigned int e,unsigned int a ,unsigned int b,unsigned int c);
		void div_single(const unsigned int n,std::pair<HUGEINT,HUGEINT> & pairTemp);
		void div_hard(const HUGEINT & a,std::pair<HUGEINT,HUGEINT> & pairTemp);
		void div_newton(const HUGEINT & a,std::pair<HUGEINT,HUGEINT> & pairTemp);
		void div_newton(const HUGEINT & a,std::pair<HUGEINT,HUGEINT> & pairTemp,const HUGEINT & cReciprocal);
		void reciprocal_newton(HUGEINT & a,const DATA & v,SIZE_T n = 0);						//�����Ż�����, ʵ�ʾ��Ƚ�����֤��С��  n / 2�ľ���,
		//ʵ�ʾ�ȷ��Ч���ַ��ص�Ϊ���س��ȵ�һ��,

		bool forecast_quotient_newton(HUGEINT & cQuotient,const HUGEINT & a,const HUGEINT & cReciprocal);

		void add(const DATA & v);
		void sub(const DATA & v,bool bMin = false);
		void add(const DATA & v,const unsigned int n);//���е�ƫ�������Ǳ�ʾ�����ƫ����,
		void sub_shift(const DATA & v,const unsigned int n = 0,bool bMin = false);

	public:
		void mul_order(HUGEINT & a);				//Ϊ������С�ڳ�������,����,�����������豣��ʱ,�����Ż����� ���� copy�ļ���,
		//һ��ֻ�ʺ����ڲ�ʹ��.���ṩ����ӿ�.
	private:
		void mul(const DATA & v);
		void div(const DATA & v,std::pair<HUGEINT,HUGEINT> & pairTemp);
		void mod(const DATA & v,std::pair<HUGEINT,HUGEINT> & pairTemp);

		/////////////////////////////////////////////////////////////////////////////////////////////////////////
		//mod���������躯��,
		//������ڿ���Ч������,�ƺ��Ѿ�û��Ҫ֧�� std::vector<unsigned int> 
		//���Ҳ����ټ�����Խ������

		bool abs_compare_min_mod(INT * pDest,INT * pSource,SIZE_T n);
		bool abs_compare_min_reduction_down(INT * pDest,SIZE_T n);
		void mul_mod(INT * pDest,INT * pSource,SIZE_T n);
		INT add_mod(INT * pDest,INT * pSource_up,INT * pSource_down,SIZE_T n); 
		INT add_mod(INT * pDest,INT a,SIZE_T n);
		INT sub_mod(INT * pDest,INT * pSource_up,INT * pSource_down,SIZE_T n);
		INT reduction_up(INT * pDest,INT * pSource_up,INT * pSource_down,SIZE_T n);		//��־λ����,��λ
		INT reduction_down(INT * pDest,INT * pSource_up,INT * pSource_down,SIZE_T n);		////��־λ����,��λ
		void normalized(INT * pDest,INT * pSource_up,INT * pSource_down,SIZE_T n);
	public:
			void TaskThread(void*); //�������߳�
	private:
		ThreadPoolManager manager;
	public:
		HUGEINT(const int n = 0);
		HUGEINT(const DATA & v);
#ifndef __HUGEINT_STD__
		HUGEINT::HUGEINT(const std::vector< INT> & v);		//����ʹ���Զ�������,�����ṩvector<unsigned int> ��Ϊ�����������,����������ݽ��й淶��
#endif
		HUGEINT(const HUGEINT & a);
		SIZE_T size();
		~HUGEINT(void);

		HUGEINT & operator = (const int n);
		HUGEINT & operator = (const HUGEINT & a);
		void swap(HUGEINT & a);

		INT operator [] (SIZE_T n)const;		//���￼�����ݹ淶����,���� []���㲢�����������ṩ�޸ĵ�����,

		////////////////////////////////////////////////////////////////////////////////
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//�����
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
		//��������

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
		HUGEINTDLL_API friend std::ostream & operator << (std::ostream & fOutput,const HUGEINT & a);			//���������������<<��
		HUGEINTDLL_API friend std::istream & operator >> (std::istream & fInput,const HUGEINT & a);			//���������������>>��

		////////////////////////////////////////////////////////////////////////////////
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//���ź���,��������,ƽ��,��ƽ��,������,��ƽ������,

		HUGEINT radix_pow(const int n) const;

		HUGEINT pow(const SIZE_T n);
		HUGEINT pow2();

		HUGEINT reciprocal_precision(SIZE_T n = 0)const;

		HUGEINT sqrt();
		HUGEINT inverse_sqrt();

		HUGEINT gcd(const HUGEINT & a);
		HUGEINT lcm(const HUGEINT & a);
		bool isprime();			//���Լ��


		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void HUGEINT::test(const HUGEINT & a = 0,const HUGEINT & b = 0);
	};

}
#endif