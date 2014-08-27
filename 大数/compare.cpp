#include "StdAfx.h"
#include "HUGEINT.h"

namespace HUGEINT_H_NAMESPACE{

	////////////////////////////////////////////////////////////////////////////////
	//核心比较函数
	//必须重载== ,!= ,< <=, 为以后有话考虑,还添加了错位比较,
	bool HUGEINT::abs_compare_equal(const DATA & v)const{
		return ! abs_compare_noequal(v);
	}

	bool HUGEINT::abs_compare_noequal(const DATA & v)const {
		if(vData.size() != v.size())
			return true;
		for(SIZE_T i = 0;i < vData.size();++ i){
			if(vData[i] != v[i])
				return true;
		}
		return false;
	}

	bool HUGEINT::abs_compare_min(const DATA & v)const {
		if(vData.size() != v.size()){
			return vData.size() < v.size();
		}
		for(SIZE_T i = 0;i < vData.size();++ i){
			if(vData[vData.size() - 1 - i] != v[vData.size() - 1 - i])
				return vData[vData.size() - 1 - i] < v[vData.size() - 1 - i];
		}
		return false;
	}

	bool HUGEINT::abs_compare_min(const DATA & v,const SIZE_T n)const {
		if(vData.size() != v.size() + n){
			return vData.size() < (v.size() + n);
		}
		for(SIZE_T i = 0;i < v.size();++ i){
			if(vData[vData.size() - 1 - i] != v[v.size() - 1 - i ])
				return vData[vData.size() - 1 - i] < v[v.size() - 1 - i];
		}
		return false;
	}

	bool HUGEINT::abs_compare_nomax(const DATA & v)const {
		if(vData.size() != v.size()){
			return vData.size() < v.size();
		}
		for(SIZE_T i = 0;i < vData.size();++ i){
			if(v[vData.size() - 1 - i] != vData[vData.size() - 1 - i])
				return vData[vData.size() - 1 - i] < v[vData.size() - 1 - i];
		}
		return true;
	}

	bool HUGEINT::abs_compare_nomax(const DATA & v,const SIZE_T n)const{
		if(vData.size() != v.size() + n){
			return vData.size() < v.size() + n;
		}
		for(SIZE_T i = 0;i < v.size();++ i){
			if(v[v.size() - 1 - i] != vData[vData.size() - 1 - i])
				return vData[vData.size() - 1 - i] < v[v.size() - 1 - i];
		}
		return true;
	}

}