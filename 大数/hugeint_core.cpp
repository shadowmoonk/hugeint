#include "StdAfx.h"
#include "HUGEINT.h"

namespace HUGEINT_H_NAMESPACE{

	////////////////////////////////////////////////////////////////////////////////
	//汇编优化函数

	unsigned int HUGEINT::asm_bsr(unsigned int n){
		__asm
		{
			bsr eax,n
		}
	}

	unsigned int asm_bsf(unsigned int n){
		__asm
		{
			bsf eax,n
		}
	}

	// a*b%p
	unsigned int HUGEINT::asm_mul_mod(unsigned int a,unsigned int b,unsigned int p){
		__asm
		{
			mov   eax,   a
				mov   edx,   b
				mul   edx
				div   p
				mov eax,edx
		}
		//	输出   eax商   ed余数
	}

	unsigned int HUGEINT::asm_mul_mod(unsigned int a,unsigned int b,unsigned int p,unsigned int & quotient){
		__asm
		{
			mov   eax,   a
				mov   edx,   b
				mul   edx
				div   p
				mov ecx,dword ptr[quotient]
			mov dword ptr[ecx],eax	// 原为直接给quotient赋值
				mov eax,edx
		}
		//	输出   eax商   ed余数
	}


	// if(a > b) b - a, a < b + b,两倍内剩余系调整,//这个优化是基于iRADIX = 10^9的,其他的不适用,
	unsigned int HUGEINT::asm_remaind_mod(unsigned int a){
		_asm
		{
			xor  eax,eax
				mov  edx,1000000000
				cmp  a,edx
				cmovnb eax,edx
				sub  a,eax
				mov eax,a
		}	

	}

	//if(a > b),carry = 1,return a - b; a < b + b,两倍内调整
	//这个优化是基于iRADIX = 10^9的,其他的不适用,
	unsigned int HUGEINT::asm_remaind_mod(unsigned int a,unsigned int & carry){
		_asm
		{

			xor  eax,eax
				mov  edx,1000000000
				cmp  a,edx
				cmovnb eax,edx
				sub  a,eax
				shr  eax,29
				mov edx,dword ptr[carry]
			mov dword ptr[edx],eax
				mov eax,a
		}
	}

	unsigned int HUGEINT::asm_remaind_mod_module(unsigned int a,unsigned int module){
		//return a % module;
		_asm
		{
			xor  eax,eax
				mov  edx,module
				cmp  a,edx
				cmovnb eax,edx
				sub  a,eax
				mov eax,a
		}
	}

	unsigned int HUGEINT::asm_remaind_mod_module(unsigned int a,unsigned int module,unsigned int & carry){
		_asm
		{
			xor  eax,eax
				mov  edx,module
				cmp  a,edx
				cmovnb eax,edx
				sub  a,eax			//这里用eax表示是否有进位,0没进位,module为进位.
				mov edx,module		//mov edx,module - 1
				dec edx
				sub edx,eax
				shr  edx,31
				mov eax,dword ptr[carry]
			mov dword ptr[eax],edx
				mov eax,a
		}
	}

	//a^b mod mod
	unsigned int HUGEINT::pow_mod(unsigned int a,unsigned int b,unsigned int mod){
		if(b < 1)
			return 1;
		a %= mod;
		unsigned int iX = 1;
		for(unsigned int i = b,iTemp = a;i;i >>= 1){
			if(i & 1){
				iX = asm_mul_mod(iX,iTemp,mod);
				//iX = ((unsigned __int64) iX * iTemp) % mod;
			}
			iTemp = asm_mul_mod(iTemp,iTemp,mod);
			//iTemp = ((unsigned __int64) iTemp * iTemp) % mod;
		}
		return iX;
	}

	////////////////////////////////////////////////////////////////////////////////
	//调整所需要的函数
	SIZE_T HUGEINT::num_end_zero(const DATA & v){
		SIZE_T num;
		for(num = 0;num < v.size();++ num){
			if(v[num])
				break;
		}
		return num;
	}

	SIZE_T HUGEINT::num_front_zero(const DATA & v){
		SIZE_T num;
		for(num = 0;num < v.size();++ num){
			if(v[v.size() - 1 - num])
				break;
		}
		return num;
	}

	void HUGEINT::shift_right(DATA & v,const SIZE_T n){
		if(! n)
			return;

		if(v.size() <= n){
			v.clear();
			return;
		}
		for(unsigned int i = 0;i + n < v.size();++ i){
			v[i] = v[i + n];
		}
		v.resize(v.size() - n);
	}

	void HUGEINT::shift_left(DATA & v,const SIZE_T n){
		if(! n)
			return;
		//如果没有进行偏移,输入数据错误.
		if(33554432 < v.size() + n){
				//这里应该产生一个断言,异常处理
				//_DEBUG_ERROR("vector subscript out of range");
		//		_SCL_SECURE_OUT_OF_RANGE
			return;
		//数据异常,返回不作处理
		}

		v.resize(v.size() + n);
		for(unsigned int i = 0;i < v.size() - n;++ i){
			v[v.size() - 1 - i] = v[v.size() - 1 - i - n];
		}
		for(unsigned int i = 0;i < n;++ i){
			v[i] = 0;
		}
	}

}