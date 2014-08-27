#include "stdafx.h"

#include "hugeint.h"

namespace HUGEINT_H_NAMESPACE{

	////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//�����
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
			//����
			return a.abs_compare_min(vData);
		}
		//����
		return abs_compare_min(a.vData);
	}

	bool HUGEINT::operator > (const HUGEINT & a) const{
		return a < *this ;
	}

	bool HUGEINT::operator <= (const HUGEINT & a) const{
		if(bSymbol != a.bSymbol)
			return bSymbol;
		if(bSymbol){
			//����
			return a.abs_compare_nomax(vData);
		}
		//����
		return abs_compare_nomax(a.vData);
	}

	bool HUGEINT::operator >= (const HUGEINT & a) const{
		return a <= *this;
	}

	
	////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//��������

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
				//����ɸ������ݽ�С,
				//�����Ż�,���ٸ�ֵ����,ֱ���ڽ�С�ϼ���,
				sub(a.vData,true);
				bSymbol = a.bSymbol;
			}else{
				sub(a.vData);
				//����λ����
			}
			//������Ҫ������ϡ,�ӷ���λ�����ܳ���0,
			adjust();
		}
		return *this;
	}

	HUGEINT& HUGEINT::operator -= (const HUGEINT & a){
		if(bSymbol == a.bSymbol){
			if(abs_compare_min(a.vData)){
				//����ɸ������ݽ�С,
				//�����Ż�,���ٸ�ֵ����,ֱ���ڽ�С�ϼ���,
				sub(a.vData,true);
				bSymbol = ! bSymbol;
			}else{
				sub(a.vData);
				//����λ����
			}
			adjust();
		}else{
			add(a.vData);
		}
		return *this;
	}

	HUGEINT& HUGEINT::operator *= (const HUGEINT & a){
		//����a����;�п���ʹ�ö��,��˱����ж� *this a�Ƿ�����ͬ����
		mul(a.vData);
		bSymbol = (bSymbol != a.bSymbol);
		return *this;
	}

	HUGEINT& HUGEINT::operator /= (const HUGEINT & a){
		//����a����;�п���ʹ�ö��,��˱����ж� *this a�Ƿ�����ͬ����,����a����copyһ��
		//����ȡ�����ͱ�����ͬ��,
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
		//����a����;�п���ʹ�ö��,��˱����ж� *this a�Ƿ�����ͬ����,����a����copyһ��
		//����ȡ�����ͱ�����ͬ��,
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