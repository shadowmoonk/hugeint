#include "StdAfx.h"
#include "HUGEINT.h"

namespace HUGEINT_H_NAMESPACE{


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//���ź���,��������,ƽ��,��ƽ��,������,��ƽ������,
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
		//���������ʱ���ѡ��,��С������,û���Ż��ı�Ҫ,
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

//���Լ��
bool HUGEINT::isprime(){
	return true;
}

}