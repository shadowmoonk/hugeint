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