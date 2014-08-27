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
��ҪΪ��ʾ�������,
ͬʱҲ�ṩһ��test�ӿں���,�ṩ�������Թ���.
*/
#include "StdAfx.h"
#include "HUGEINT.h"

namespace HUGEINT_H_NAMESPACE{


	void HUGEINT::adjust_add(){
		//����ר��Ϊ����ÿλ���ֵС��2*iRADIX ʱ���еĵ���,��Ҫ��Ϊ��ӵ���ʱ�����Ż�.
		//����ֱ��copy adjustδ�Ż�
		INT iSum = 0;
		for(SIZE_T i = 0; i < vData.size();++ i){
			vData[i] += iSum;
			asm_remaind_mod_module(vData[i],iRADIX,iSum);
		}
		if(iSum)
			vData.push_back(iSum);
		SIZE_T i = 0;
		for(;i < vData.size();++ i){
			if(vData[vData.size() - 1 - i])
				break;;
		}
		vData.resize(vData.size() - i);
	}

	void HUGEINT::adjust(){
		INT iSum = 0;
		for(SIZE_T i = 0; i < vData.size();++ i){
			iSum += vData[i];
			vData[i] = iSum % iRADIX;
			iSum /= iRADIX;
		}
		while(iSum){
			vData.push_back(iSum % iRADIX);
			iSum /= iRADIX;
		}
		SIZE_T i = 0;
		for(;i < vData.size();++ i){
			if(vData[vData.size() - 1 - i])
				break;;
		}
		vData.resize(vData.size() - i);
		//����ֵΪ0,����λ���븴λ,
		if(! vData.size())
			bSymbol = false;
	}

	////////////////////////////////////////////////////////////////////////////////
	//���������������<<��
	HUGEINTDLL_API std::ostream & operator << (std::ostream & fOutput,const HUGEINT & a){

		//���ǰ������е���,adjust();
		if(! a.vData.size()){
			fOutput<<"0";
			return fOutput;
		}
		if(a.bSymbol)
			fOutput<<"-";
		SIZE_T iLengthUnit = 0;
		SIZE_T iLength = 0;
		SIZE_T iLengthTop = 0;
		{
			SIZE_T iTemp = a.iRADIX;
			while(iTemp){
				iTemp /= 10;				// ֻ֧��10���� ����������ⲻ��ȡ����,�ػ���ʾ,
				++ iLengthUnit;
			}
			-- iLengthUnit;

			iTemp = a.vData[a.vData.size() - 1];
			while(iTemp){
				iTemp /= 10;	
				++ iLengthTop;
			}

			iLength = iLengthTop + iLengthUnit * (a.vData.size() - 1);
		}

		INT Mode = 1;
		//iLengthTop -1�η��Ͷ���,
		for(SIZE_T i = 1; i < iLengthTop;++ i){
			Mode *= 10;		
		}
		//3Ϊ������ݼ������,
		unsigned int iLengthspace = 9;			//���ڳ���,��Ҫ��ȡ
		SIZE_T iFillSpace = iLength % iLengthspace;
		for(SIZE_T i = 0;i < a.vData.size();++ i){
			INT iTemp = a.vData[a.vData.size() - 1 - i];
			while(Mode){
				fOutput << iTemp / Mode;
				iTemp %= Mode;
				Mode /= 10;
				iFillSpace = (iFillSpace + iLengthspace - 1) % iLengthspace;
				if(! iFillSpace)
					std::cout<<" ";
			}
			Mode = a.iRADIX / 10;
		}

		return fOutput;
	}


	//���������������>>��
	HUGEINTDLL_API std::istream & operator >> (std::istream & fInput,const HUGEINT & a){
		return fInput;
	}


#include <time.h>

	////////////////////////////////////////////////////////////////////////////////
	inline unsigned __int64 GetCycleCount()//����ʱ��ļ�����,���ص��Ǽ���ֵ,���Ǿ���ʱ��ֵ.

	{

		__asm _emit 0x0F

			__asm _emit 0x31

	}
	void HUGEINT::test(const HUGEINT & a,const HUGEINT & b){
		HUGEINT cTemp1;
		HUGEINT cTemp2;
		HUGEINT cTemp3;
		HUGEINT cTemp4;

		__int64 iTime;

		std::cout<<"�°汾"<<std::endl;

		if((a == 0) || (b == 0)){
			std::cout<<"����������0 ,����Ĭ�����ݽ��в���"<<std::endl;
			cTemp3.vData.clear();
			//cTemp3.vData.resize(110113,999999999);
			DATA vTemp(633000 ,9235664);		//161100 9235664
			cTemp3.vData = vTemp;
			cTemp3.adjust();


			cTemp4.vData.clear();
			//cTemp4.vData.resize(51013,666666666);
			vTemp.clear();
			vTemp.resize(371000,55112344);	//101000 55112344
			cTemp4.vData = vTemp;
			cTemp4.adjust();
		}else{
			cTemp3 = a;
			cTemp3.adjust();
			cTemp4 = b;
			cTemp4.adjust();
		}




		std::cout<<cTemp3.size()<<" �� "<<cTemp3.vData[0]<<"  * "<<cTemp4.size()<<" �� "<<cTemp4.vData[0]<<std::endl;
		iTime = GetCycleCount();	
	//	iTEST_TEMP = 0;
		time_t iTime_t = clock();
		//std::cout<<"������ "<<cTemp3<<std::endl;
		//std::cout<<"���� "<<cTemp4<<std::endl;
		cTemp1 = cTemp3 * cTemp4;
		iTime_t = clock() - iTime_t;
		std::cout<<"�˷�����ʱ��"<<iTime_t<<"    "<<std::endl;

		iTime_t = clock();
		cTemp1 = cTemp3 / cTemp4;
		iTime_t = clock() - iTime_t;
		std::cout<<"��������ʱ��"<<iTime_t<<"    "<<std::endl;

		iTime_t = clock();
		cTemp1 = cTemp3 / cTemp4;
		cTemp2 = cTemp3 % cTemp4;

		if(cTemp1 * cTemp4 + cTemp2 == cTemp3){
			std::cout<<"�����ȷ"<<std::endl;
		}else{
			std::cout<<"�������"<<std::endl;
		}	

		if(cTemp1.size() < 100){
			std::cout<<"�� "<<cTemp1<<std::endl;
		}else{
			std::cout<<"�̵ĳ��� "<<cTemp1.size()<<std::endl;
		}
		if(cTemp2.size() < 100){
			std::cout<<"���� "<<cTemp2<<std::endl;
		}else{
			std::cout<<"�����ĳ��� "<<cTemp2.size()<<std::endl;
		}

		iTime_t = clock() - iTime_t;
		std::cout<<"У�黨��ʱ��"<<iTime_t<<"    "<<std::endl;		


		//	cTemp1 = cTemp4.reciprocal_precision();
		//std::cout<<"�� "<<cTemp1<<std::endl;


		//iTime = GetCycleCount() - iTime;
		//	iTime /= 1600000;
		//std::cout<<"����ʱ��"<<iTime_t<<"    "<<std::endl;
		system("pause");

	}

}