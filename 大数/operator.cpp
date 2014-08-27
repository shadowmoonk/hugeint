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
大数运算符重载.
*/

#include "stdafx.h"

#include "hugeint.h"

namespace HUGEINT_H_NAMESPACE{

	////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//运算符
	bool HUGEINT::operator == (const int n) const{
		HUGEINT cTemp(n);
		return *this == cTemp;
	}

	bool HUGEINT::operator != (const int n) const{
		HUGEINT cTemp(n);
		return *this != cTemp;
	}

	bool HUGEINT::operator < (const int n) const{
		HUGEINT cTemp(n);
		return *this < cTemp;
	}

	bool HUGEINT::operator > (const int n) const{
		HUGEINT cTemp(n);
		return cTemp < *this;
	}

	bool HUGEINT::operator <= (const int n) const{
		HUGEINT cTemp(n);
		return *this <= cTemp;
	}

	bool HUGEINT::operator >= (const int n) const{
		HUGEINT cTemp(n);
		return cTemp <= *this;
	}

	////////////////////////////////////////////////////////////////////////////////

	bool HUGEINT::operator == (const HUGEINT & a) const{
		if(bSymbol != a.bSymbol)
			return false;
		return abs_compare_equal(a.vData);
	}

	bool HUGEINT::operator != (const HUGEINT & a) const{
		if(bSymbol != a.bSymbol)
			return true;
		if(vData.size() != a.vData.size())
			return true;
		return abs_compare_noequal(a.vData);
	}

	bool HUGEINT::operator < (const HUGEINT & a) const{
		if(bSymbol != a.bSymbol)
			return bSymbol;
		if(bSymbol){
			//负数
			return a.abs_compare_min(vData);
		}
		//正数
		return abs_compare_min(a.vData);
	}

	bool HUGEINT::operator > (const HUGEINT & a) const{
		return a < *this ;
	}

	bool HUGEINT::operator <= (const HUGEINT & a) const{
		if(bSymbol != a.bSymbol)
			return bSymbol;
		if(bSymbol){
			//负数
			return a.abs_compare_nomax(vData);
		}
		//正数
		return abs_compare_nomax(a.vData);
	}

	bool HUGEINT::operator >= (const HUGEINT & a) const{
		return a <= *this;
	}

	
	////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//四则运算

	HUGEINT& HUGEINT::operator += (const int n){
		HUGEINT cTemp(n);
		*this += cTemp;
		return *this;
	}

	HUGEINT& HUGEINT::operator -= (const int n){
		HUGEINT cTemp(n);
		*this -= cTemp;
		return *this;
	}

	HUGEINT& HUGEINT::operator *= (const int n){
		HUGEINT cTemp(n);
		*this *= cTemp;
		return *this;
	}

	HUGEINT& HUGEINT::operator /= (const int n){
		HUGEINT cTemp(n);
		*this /= cTemp;
		return *this;
	}

	HUGEINT& HUGEINT::operator %= (const int n){
		HUGEINT cTemp(n);
		*this %= cTemp;
		return *this;
	}

	////////////////////////////////////////////////////////////////////////////////

	HUGEINT HUGEINT::operator + (const int n)const{
		HUGEINT cTemp1(*this),cTemp2(n);
		cTemp1 += cTemp2;
		return cTemp1;
	}

	HUGEINT HUGEINT::operator - (const int n)const{
		HUGEINT cTemp1(*this),cTemp2(n);
		cTemp1 -= cTemp2;
		return cTemp1;
	}

	HUGEINT HUGEINT::operator * (const int n)const{
		HUGEINT cTemp1(*this),cTemp2(n);
		cTemp1 *= cTemp2;
		return cTemp1;
	}

	HUGEINT HUGEINT::operator / (const int n)const{
		HUGEINT cTemp1(*this),cTemp2(n);
		cTemp1 /= cTemp2;
		return cTemp1;
	}

	HUGEINT HUGEINT::operator % (const int n)const{
		HUGEINT cTemp1(*this),cTemp2(n);
		cTemp1 %= cTemp2;
		return cTemp1;
	}

	////////////////////////////////////////////////////////////////////////////////

	HUGEINT& HUGEINT::operator += (const HUGEINT & a){
		if(bSymbol == a.bSymbol){
			this->add(a.vData);
		}else{
			if(abs_compare_min(a.vData)){
				//如果可更改数据较小,
				//可以优化,减少赋值处理,直接在较小上计算,
				sub(a.vData,true);
				bSymbol = a.bSymbol;
			}else{
				sub(a.vData);
				//符号位不变
			}
			//减法需要调整依稀,加法高位不可能出现0,
			adjust();
		}
		return *this;
	}

	HUGEINT& HUGEINT::operator -= (const HUGEINT & a){
		if(bSymbol == a.bSymbol){
			if(abs_compare_min(a.vData)){
				//如果可更改数据较小,
				//可以优化,减少赋值处理,直接在较小上计算,
				sub(a.vData,true);
				bSymbol = ! bSymbol;
			}else{
				sub(a.vData);
				//符号位不变
			}
			adjust();
		}else{
			add(a.vData);
		}
		return *this;
	}

	HUGEINT& HUGEINT::operator *= (const HUGEINT & a){
		//由于a对象途中可能使用多次,因此必须判断 *this a是否是相同对象
		mul(a.vData);
		bSymbol = (bSymbol != a.bSymbol);
		return *this;
	}

	HUGEINT& HUGEINT::operator /= (const HUGEINT & a){
		//由于a对象途中可能使用多次,因此必须判断 *this a是否是相同对象,或者a另外copy一份
		//这里取余数和被除数同号,
		std::pair<HUGEINT,HUGEINT> pairTemp;
		//HUGEINT cTemp(a);
		bool bMark = bSymbol;
		//bSymbol = false;
		//cTemp.bSymbol = false;
		div(a.vData,pairTemp);
		pairTemp.first.bSymbol = (bMark  != a.bSymbol);
		pairTemp.second.bSymbol = bMark;
		vData.swap(pairTemp.first.vData);
		bSymbol = pairTemp.first.bSymbol;
		pairTemp.second.adjust();
		adjust();
		return *this;
	}

	HUGEINT& HUGEINT::operator %= (const HUGEINT & a){
		//由于a对象途中可能使用多次,因此必须判断 *this a是否是相同对象,或者a另外copy一份
		//这里取余数和被除数同号,
		std::pair<HUGEINT,HUGEINT> pairTemp;
		//HUGEINT cTemp(a);
		bool bMark = bSymbol;
		//bSymbol = false;
		//cTemp.bSymbol = false;
		div(a.vData,pairTemp);
		pairTemp.first.bSymbol = (bMark  != a.bSymbol);
		pairTemp.second.bSymbol = bMark;
		vData.swap(pairTemp.second.vData);
		bSymbol = pairTemp.second.bSymbol;
		pairTemp.first.adjust();
		adjust();
		return *this;
	}

	////////////////////////////////////////////////////////////////////////////////

	HUGEINT HUGEINT::operator + (const HUGEINT & a)const{
		HUGEINT cTemp(*this);
		cTemp += a;
		return cTemp;
	}

	HUGEINT HUGEINT::operator - (const HUGEINT & a)const{
		HUGEINT cTemp(*this);
		cTemp -= a;
		return cTemp;
	}

	HUGEINT HUGEINT::operator * (const HUGEINT & a)const{
		HUGEINT cTemp(*this);
		cTemp *= a;
		return cTemp;
	}

	HUGEINT HUGEINT::operator / (const HUGEINT & a)const{
		HUGEINT cTemp(*this);
		cTemp /= a;
		return cTemp;
	}

	HUGEINT HUGEINT::operator % (const HUGEINT & a)const{
		HUGEINT cTemp(*this);
		cTemp %= a;
		return cTemp;
	}

}