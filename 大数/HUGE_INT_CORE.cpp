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
����洢�������ݵ���ʵ��
*/
#include "StdAfx.h"
#include "HUGE_INT_CORE.h"
namespace HUGEINT_H_NAMESPACE{

	const unsigned int HUGE_INT_CORE_MIN_LENGTH = 64;


	INT * HUGE_INT_CORE::newdata(INT * * pDest,SIZE_T & num,SIZE_T n){
		//new��֤ʵ�ʿ����ÿռ䲻С��n,����ռ���ʼָ�뷵����pDest_start,ʵ�ʿ�����ָ�뷵����pTemp
		//Ϊ��֤16�ֽڶ���,
		if(n < HUGE_INT_CORE_MIN_LENGTH)
			n = HUGE_INT_CORE_MIN_LENGTH;
		INT * pTemp;
		* pDest = new INT[n + 3];
		pTemp = (INT *) ((INT) (*pDest) & (~0X0F) );
		if(pTemp < *pDest)
			pTemp += 4;
		num = * pDest + n + 3 - pTemp;
	//	std::cout<<"��������"<<*pDest<<std::endl;
		return pTemp;

	}

	HUGE_INT_CORE::HUGE_INT_CORE(const SIZE_T n,const INT a):iLength(0),iLength_max(0)
	{


		//pData = new INT[iLength_max];
		pData = newdata( & pData_start,iLength_max,n);

		iLength = n;
		for(SIZE_T i = 0; i < iLength;++ i)
			pData[i] = a;
	}

	HUGE_INT_CORE::HUGE_INT_CORE(const INT * pbegin,const INT * pend):iLength(0),iLength_max(0){
		if(pend < pbegin){
			//����Ӧ�ò���һ������,�쳣����
			system("pause");
		}

		SIZE_T n = pend - pbegin;

		pData = newdata(& pData_start,iLength_max,n);
		iLength = n;
		for(SIZE_T i = 0; i < iLength;++ i)
			pData[i] = * pbegin ++;
	}


	HUGE_INT_CORE::HUGE_INT_CORE(std::vector<unsigned int>::const_iterator pbegin,std::vector<unsigned int>::const_iterator pend):iLength(0),iLength_max(0){
		if(pend < pbegin){
			//����Ӧ�ò���һ������,�쳣����
			system("pause");
		}

		SIZE_T n = pend - pbegin;

		pData = newdata(& pData_start,iLength_max,n);
		iLength = n;
		for(SIZE_T i = 0; i < iLength;++ i)
			pData[i] = * pbegin ++;
	}

	HUGE_INT_CORE::HUGE_INT_CORE(std::vector<int>::const_iterator pbegin,std::vector<int>::const_iterator pend):iLength(0),iLength_max(0){
		if(pend < pbegin){
			//����Ӧ�ò���һ������,�쳣����
			system("pause");
		}

		SIZE_T n = pend - pbegin;

		pData = newdata(& pData_start,iLength_max,n);
		iLength = n;
		for(SIZE_T i = 0; i < iLength;++ i)
			pData[i] = * pbegin ++;
	}

	HUGE_INT_CORE::~HUGE_INT_CORE(void)
	{
		if(pData_start)			//Ŀǰ ����������Ŀռ�Ϊ����,�������ʧ��,������ᱻ�쳣�����.
			delete [] pData_start ;
	//	if(pData_start){
	//		std::cout<<"���ɾ������"<<pData_start<<std::endl;
	//	}

		pData_start = 0;
	}

	//////////////////////////////////////////////////////////////////////////////////////////////////////

	INT * HUGE_INT_CORE::getdata() const{
		return pData;
	}

	HUGE_INT_CORE::HUGE_INT_CORE(const HUGE_INT_CORE & a){
		//pData = new INT[iLength_max];
		pData = newdata(& pData_start,iLength_max,a.iLength);
		iLength = a.iLength;
		for(SIZE_T i = 0; i < iLength;++ i)
			pData[i] = a.pData[i];
	}


	SIZE_T HUGE_INT_CORE::size() const{
		return iLength;
	}

	void HUGE_INT_CORE::resize(const SIZE_T n,const INT a){
		if(n <= iLength_max){
			//���ԭ�ռ��㹻
			for(SIZE_T i = iLength; i < n;++ i)
				pData[i] = a;
			iLength = n;

		}else{

			INT * pTemp = 0;
			INT * pTemp_dest = 0;


			//pTemp = new INT[iLength_max];
			pTemp = newdata(& pTemp_dest,iLength_max,n);

			for(SIZE_T i = 0; i < iLength;++ i)
				pTemp[i] = pData[i];

			for(SIZE_T i = iLength; i < n;++ i){
				pTemp[i] = a;
			}

			iLength = n;

			//if(pData_start)
			delete [] pData_start ;
			pData = pTemp;
			pData_start = pTemp_dest;
		}
	}

	void HUGE_INT_CORE::push_back(const INT a){
		if(iLength < iLength_max){
			//������㹻�ռ�
			pData[iLength ++] = a;
		}else{
			INT * pTemp = 0;
			INT * pTemp_dest = 0;

			//pTemp = new INT[iLength_max];
			//�ռ䲻��,ֱ�ӷŴ�һ���ռ�?
			pTemp = newdata(& pTemp_dest,iLength_max,iLength_max + iLength_max);

			for(SIZE_T i = 0; i < iLength;++ i){
				pTemp[i] = pData[i];
			}

			pTemp[iLength ++ ] = a;
			//if(pData_start)		//������Ϊ��,
			delete [] pData_start ;

			pData = pTemp;
			pData_start = pTemp_dest;
		}
	}

	void HUGE_INT_CORE::push_back (const INT * pBegin,const INT * pEnd){
		if(pEnd <= pBegin){
			//�Ƿ�����,
			return;
		}
		if(iLength_max - iLength < (SIZE_T)(pEnd - pBegin)){
			set_capacity(iLength + pEnd - pBegin);
		}
		for(;pBegin != pEnd;){
			*(pData + iLength ++) = * pBegin ++;
		}

	}

	INT * HUGE_INT_CORE::pop_back(){
		if(iLength == 0){
			//����Ӧ�ò���һ������,�쳣����
			system("pause");
			return 0;
		}	
		iLength -= 1;
		return pData + iLength - 1;

	}

	void HUGE_INT_CORE:: clear(){
		iLength = 0;
	}

	void HUGE_INT_CORE::swap(HUGE_INT_CORE & a){
		INT * pTemp = pData;
		pData = a.pData;
		a.pData = pTemp;

		pTemp = pData_start;
		pData_start = a.pData_start;
		a.pData_start = pTemp;

		SIZE_T iTemp;
		iTemp = iLength;
		iLength = a.iLength;
		a.iLength = iTemp;

		iTemp = iLength_max;
		iLength_max = a.iLength_max;
		a.iLength_max = iTemp;

	}

	INT * HUGE_INT_CORE::begin()const{
		return  pData;
	}

	INT * HUGE_INT_CORE::end()const{
		return  pData + iLength;
	}


	void HUGE_INT_CORE::set_capacity(const SIZE_T n){
		if(iLength_max <= n){
			INT * pTemp = 0;
			INT * pTemp_dest = 0;
			iLength_max = n;

			//pTemp = new INT[iLength_max];
			pTemp = newdata(& pTemp_dest,iLength_max,n);
			for(SIZE_T i = 0; i < iLength;++ i){
				pTemp[i] = pData[i];
			}
			iLength_max = n;
			//	if(pData_start)
			delete [] pData_start ;
			pData = pTemp;
			pData_start = pTemp_dest;
		}
	}

	HUGE_INT_CORE & HUGE_INT_CORE::operator = (const HUGE_INT_CORE & a){
		if(a.iLength <= iLength_max){
			iLength = a.iLength;
			for(SIZE_T i = 0; i < iLength;++ i){
				pData[i] = a.pData[i];
			}
		}else{
			//if(pData_start)
			delete [] pData_start ;

			//pData = new INT[iLength_max];
			pData = newdata(& pData_start,iLength_max,a.iLength);
			for(SIZE_T i = 0; i < a.iLength;++ i){
				pData[i] = a.pData[i];
			}
			iLength = a.iLength;
		}

		return * this;
	}

	HUGE_INT_CORE & HUGE_INT_CORE::operator =(const INT a){
		//������뱣֤
		pData[0] = a;
		iLength = 1;
		return *this;
	}



	INT & HUGE_INT_CORE::operator [](SIZE_T n)const{

		if(iLength <= n){
			//����Ӧ�ò���һ������,�쳣����
			std::cout<<"����Խ��"<<std::endl;
			//		system("pause");
		}		
		return pData[n];
	}
}
