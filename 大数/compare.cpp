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
大数类比较函数.
*/
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