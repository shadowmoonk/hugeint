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
大数类构造函数和赋值函数
*/

#include "StdAfx.h"
#include "HUGEINT.h"

namespace HUGEINT_H_NAMESPACE{

	INT HUGEINT::iRADIX = ENUM_RADIX;
//	SIZE_T HUGEINT::iTEST_TEMP = 0;

	HUGEINT::HUGEINT(const int n){
		if(n < 0){
			bSymbol = true;
			vData.push_back(-n);
		}else{
			bSymbol = false;
			vData.push_back(n);
		}
		//iExponent = 0;
		adjust();
	}
	HUGEINT::HUGEINT(const DATA & v){
		bSymbol = false;
		vData = v;
		//iExponent = 0;
		adjust();
	}
#ifndef __HUGEINT_STD__
	HUGEINT::HUGEINT(const std::vector< INT> & v){
		bSymbol = 0;
		vData.set_capacity(v.size());
		for(SIZE_T i = 0; i < v.size();++ i)
			vData.push_back(v[i]);
		adjust();
	}
#endif
	HUGEINT::HUGEINT(const HUGEINT & a){
		bSymbol = a.bSymbol;
		vData = a.vData;
		//iExponent = a.iExponent;
	}

	SIZE_T HUGEINT::size(){
		return vData.size();
	}

	HUGEINT::~HUGEINT(void){

	}


	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	HUGEINT & HUGEINT::operator = (const int n){
		vData.clear();
		if(n < 0){
			bSymbol = true;
			vData.push_back( - n);
		}else{
			bSymbol = false;
			vData.push_back(n);
		}	
		adjust();
		//iExponent = 0;
		return * this;
	}

	HUGEINT & HUGEINT::operator = (const HUGEINT & a){
		bSymbol = a.bSymbol;
		vData = a.vData;
		//iExponent = a.iExponent;
		return *this;
	}

	void HUGEINT::swap(HUGEINT & a){
		bool bTempsymbol = bSymbol;
		bSymbol = a.bSymbol;
		vData.swap(a.vData);
	}

	INT HUGEINT::operator [] (SIZE_T n)const{
		if(n >= vData.size()){
			//正常应该是抛出异常
			return -1;
		}
		return vData[n];
	}
}