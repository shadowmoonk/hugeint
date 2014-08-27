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
计划预留的基础数学函数
*/
#include "StdAfx.h"
#include "HUGEINT.h"

namespace HUGEINT_H_NAMESPACE{


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//开放函数,大致有幂,平方,开平方,开立方,开平方倒数,
	HUGEINT HUGEINT::radix_pow(const int n)const {
		HUGEINT cTemp(*this);
		if(n > 0){
			cTemp.shift_left(cTemp.vData,n);
		}else{
			cTemp.shift_right(cTemp.vData, - n);
		}
		return cTemp;
	}

	HUGEINT HUGEINT::pow(const SIZE_T n){
		return *this;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

HUGEINT HUGEINT::pow2(){
	if(vData.size() < 150){
		//这个数字暂时随便选的,过小的数字,没有优化的必要,
		HUGEINT cTemp1(*this),cTemp2(*this);
		cTemp1.bSymbol = false;
		cTemp2.bSymbol = false;
		cTemp1 *= cTemp2;
		return cTemp1;
	}
	HUGEINT cTemp(*this);
	cTemp.mul_FFT(cTemp.vData,true);

	return cTemp;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

HUGEINT HUGEINT::reciprocal_precision(SIZE_T n)const{
	std::pair<HUGEINT,HUGEINT>  pairValue;

	if(! n)
	n = vData.size();

	HUGEINT cReciprocal;
	HUGEINT cTemp(*this);
	cTemp.reciprocal_newton(cReciprocal,vData);

	std::pair<HUGEINT,HUGEINT> pairTemp;

	//HUGEINT cTemp;
	cTemp.vData.resize(n + vData.size() + 1);
	cTemp.vData[cTemp.vData.size() - 1] = 1;
	cTemp.div_newton(*this,pairTemp,cReciprocal);
	return pairTemp.first;	
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

HUGEINT HUGEINT::sqrt(){
	return *this;
}

HUGEINT HUGEINT::inverse_sqrt(){
	return *this;
}

HUGEINT HUGEINT::gcd(const HUGEINT & a){
	return *this;
}

HUGEINT HUGEINT::lcm(const HUGEINT & a){
	return *this;
}

//素性检测
bool HUGEINT::isprime(){
	return true;
}

}