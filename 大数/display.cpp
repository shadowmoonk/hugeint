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
主要为显示输出服务,
同时也提供一个test接口函数,提供基本测试功能.
*/
#include "StdAfx.h"
#include "HUGEINT.h"

namespace HUGEINT_H_NAMESPACE{


	void HUGEINT::adjust_add(){
		//这里专门为调整每位最大值小于2*iRADIX 时进行的调整,主要是为相加调整时进行优化.
		//这里直接copy adjust未优化
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
		//绝对值为0,符号位必须复位,
		if(! vData.size())
			bSymbol = false;
	}

	////////////////////////////////////////////////////////////////////////////////
	//声明重载运算符“<<”
	HUGEINTDLL_API std::ostream & operator << (std::ostream & fOutput,const HUGEINT & a){

		//输出前必须进行调整,adjust();
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
				iTemp /= 10;				// 只支持10进制 这个数字特意不提取出来,特化显示,
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
		//iLengthTop -1次方就对了,
		for(SIZE_T i = 1; i < iLengthTop;++ i){
			Mode *= 10;		
		}
		//3为输出数据间隔长度,
		unsigned int iLengthspace = 9;			//存在常数,需要提取
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


	//声明重载运算符“>>”
	HUGEINTDLL_API std::istream & operator >> (std::istream & fInput,const HUGEINT & a){
		return fInput;
	}


#include <time.h>

	////////////////////////////////////////////////////////////////////////////////
	inline unsigned __int64 GetCycleCount()//测量时间的计数器,返回的是计数值,不是绝对时间值.

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

		std::cout<<"新版本"<<std::endl;

		if((a == 0) || (b == 0)){
			std::cout<<"输入数据有0 ,调用默认数据进行测试"<<std::endl;
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




		std::cout<<cTemp3.size()<<" 个 "<<cTemp3.vData[0]<<"  * "<<cTemp4.size()<<" 个 "<<cTemp4.vData[0]<<std::endl;
		iTime = GetCycleCount();	
	//	iTEST_TEMP = 0;
		time_t iTime_t = clock();
		//std::cout<<"被除数 "<<cTemp3<<std::endl;
		//std::cout<<"除数 "<<cTemp4<<std::endl;
		cTemp1 = cTemp3 * cTemp4;
		iTime_t = clock() - iTime_t;
		std::cout<<"乘法花费时间"<<iTime_t<<"    "<<std::endl;

		iTime_t = clock();
		cTemp1 = cTemp3 / cTemp4;
		iTime_t = clock() - iTime_t;
		std::cout<<"除法花费时间"<<iTime_t<<"    "<<std::endl;

		iTime_t = clock();
		cTemp1 = cTemp3 / cTemp4;
		cTemp2 = cTemp3 % cTemp4;

		if(cTemp1 * cTemp4 + cTemp2 == cTemp3){
			std::cout<<"结果正确"<<std::endl;
		}else{
			std::cout<<"结果错误"<<std::endl;
		}	

		if(cTemp1.size() < 100){
			std::cout<<"商 "<<cTemp1<<std::endl;
		}else{
			std::cout<<"商的长度 "<<cTemp1.size()<<std::endl;
		}
		if(cTemp2.size() < 100){
			std::cout<<"余数 "<<cTemp2<<std::endl;
		}else{
			std::cout<<"余数的长度 "<<cTemp2.size()<<std::endl;
		}

		iTime_t = clock() - iTime_t;
		std::cout<<"校验花费时间"<<iTime_t<<"    "<<std::endl;		


		//	cTemp1 = cTemp4.reciprocal_precision();
		//std::cout<<"商 "<<cTemp1<<std::endl;


		//iTime = GetCycleCount() - iTime;
		//	iTime /= 1600000;
		//std::cout<<"花费时间"<<iTime_t<<"    "<<std::endl;
		system("pause");

	}

}