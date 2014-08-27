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
大数类的最核心文件.
四则运算的核心运算都在这里时间.
*/
#include "StdAfx.h"
#include "HUGEINT.h"

namespace HUGEINT_H_NAMESPACE{


	////////////////////////////////////////////////////////////////////////////////
	//这个才是hugeint真正的运算级内核.

	void HUGEINT::FFT(DATA & vSource,unsigned int iSize,bool inverse_mark,unsigned int Module,unsigned int Root){
#ifdef __HUGEINT_STD__
		DATA & v = vSource;
#else
		INT * v = vSource.getdata();
#endif


		if(inverse_mark)
			for(unsigned int x = 1;x < iSize;x += x){//蝶形变换长度的一半.
				unsigned int root;
				root = pow_mod(Root,33554432 - (33554432)/(x + x),Module);		//33554432这个数字特意不提取出来,特化显示,
				unsigned int temp_root = 1;
				for(unsigned int t = 0; t < x;++ t){//有交叉的蝶形变换的偏移量
					for(unsigned int i = 0; i < iSize;i += x + x){//x + x是一个完整蝶形变换的宽度.
						unsigned int a,b,iTemp;
						SIZE_T p,q;
		//				++ iTEST_TEMP;
						p = i + t;
						q = p + x;
						//a = v[i + t];
						a = v[p];
						//b = v[i + t + x];
						b = v[q];
						//b = asm_mul_mod(v[i + t + x],temp_root,Module);
						__asm{
							mov eax,temp_root
								mul b
								div Module
								mov b,edx	//(b = b * temp_root % Module

								mov eax,a
								add eax,b
								xor edx,edx
								cmp eax,Module
								cmovnb edx,Module
								sub eax,edx
								mov iTemp,eax	//iTemp = (a + b) % Module

								mov eax,a
								add eax,Module
								sub eax,b
								xor edx,edx
								cmp eax,Module
								cmovnb edx,Module
								sub eax,edx
								mov b,eax		//b = (a - b) % Module														
						}				
						//v[i + t] = (a + b) % Module;			//蝶形变换上臂
						//v[i + t] = iTemp;
						v[p] = iTemp;
						//v[i + t + x] = (a - b + Module) % Module;//蝶形变换下臂
						//v[i + t + x] = b;
						v[q] = b;
					}
					//temp_root = (__int64) temp_root * root % M;
					//temp_root = asm_mul_mod(temp_root,root,Module);

					__asm{
						mov eax,temp_root
							mul root
							div Module
							mov temp_root,edx
					}

				}
			}
		else
			for(unsigned int x = iSize / 2;x; x >>= 1){//蝶形变换长度的一半.
				unsigned int root;
				root = pow_mod(Root,(33554432)/(x + x),Module);
				unsigned int temp_root = 1;
				for(unsigned int t = 0; t < x;++ t){//有交叉的蝶形变换的偏移量
					for(unsigned int i = 0; i < iSize;i += x + x){//x + x是一个完整蝶形变换的宽度.
						unsigned int a,b,iTemp;
						SIZE_T p,q;
		//				++ iTEST_TEMP;
						//a = (v[i + t] + v[i + t + x] ) % Module;
						//a = asm_remaind_mod_module(v[i + t] + v[i + t + x],Module);
						//b = ((__int64)v[i + t] - v[i + t + x]  + Module) % Module;
						//b = asm_remaind_mod_module(Module + v[i + t] - v[i + t +x],Module);
						p = i + t;
						q = p + x;
						//a = v[i + t];
						a = v[p];
						//b = v[i + t + x];
						b = v[q];
						__asm{
							mov eax,a
								add eax,b
								xor edx,edx
								cmp eax,Module
								cmovnb edx,Module
								sub eax,edx
								mov iTemp,eax		//iTemp = (a + b) % Module

								mov eax,a
								add eax,Module
								sub eax,b
								xor edx,edx
								cmp eax,Module
								cmovnb edx,Module
								sub eax,edx
								mov b,eax			//b = (a - b) % Module

								mov eax,iTemp
								mov a,eax

								mov eax,temp_root
								mul b
								div Module
								mov b,edx			//b = b * temp % Module
						}

						//v[i + t] = a;			//蝶形变换上臂
						v[p] = a;
						//v[i + t + x] = (__int64) b * temp_root % Module;//蝶形变换下臂
						//v[i + t + x] = asm_mul_mod(b,temp_root,Module);
						//v[i + t + x] = b;
						v[q] = b;
					}
					//temp_root = (__int64) temp_root * root % Module;
					//temp_root = asm_mul_mod(temp_root,root,Module);
					__asm{
						mov eax,temp_root
							mul root
							div Module
							mov temp_root,edx
					}
				}
				//cout<<endl;
			}
			//std::cout<<"输出测试根变更次数"<<iNum<<" ------- 变换长度 "<<v.size()<<std::endl;
			//		HUGEINT cTemp;
			//		cTemp.vData = vSource; std::cout<<"FFT变换 Module"<<Module<<" ----  "<<cTemp<<std::endl;

	}

	////////////-------------------------------------------------------------------------------------------------------------///////////////
	/*
	void HUGEINT::FFT(DATA & vSource,unsigned int iSize,bool inverse_mark,unsigned int Module,unsigned int Root){
	//unsigned int iNum = 0;
	INT * v = vSource.getdata();
	if(inverse_mark)
	for(unsigned int x = 1;x < iSize;x += x){//蝶形变换长度的一半.
	unsigned int root;
	root = pow_mod(Root,16777216 - (16777216)/(x + x),Module);
	unsigned int temp_root = 1;
	for(unsigned int t = 0; t < x;++ t){//有交叉的蝶形变换的偏移量
	for(unsigned int i = 0; i < iSize;i += x + x){//x + x是一个完整蝶形变换的宽度.
	unsigned int a,b;
	a = v[i + t];
	//b = (__int64)v[i + t + x] * temp_root % Module;
	b = asm_mul_mod(v[i + t + x],temp_root,Module);

	//v[i + t] = (a + b) % Module;			//蝶形变换上臂
	v[i + t] = asm_remaind_mod_module(a + b,Module);
	//v[i + t + x] = (a - b + Module) % Module;//蝶形变换下臂
	v[i + t + x] = asm_remaind_mod_module(Module + a - b,Module);
	}
	//temp_root = (__int64) temp_root * root % M;
	temp_root = asm_mul_mod(temp_root,root,Module);
	//++ iNum;
	}
	}
	else
	for(unsigned int x = iSize / 2;x; x >>= 1){//蝶形变换长度的一半.
	unsigned int root;
	root = pow_mod(Root,(16777216)/(x + x),Module);
	unsigned int temp_root = 1;
	for(unsigned int t = 0; t < x;++ t){//有交叉的蝶形变换的偏移量
	for(unsigned int i = 0; i < iSize;i += x + x){//x + x是一个完整蝶形变换的宽度.
	unsigned int a,b;
	//a = (v[i + t] + v[i + t + x] ) % Module;
	a = asm_remaind_mod_module(v[i + t] + v[i + t + x],Module);
	//b = ((__int64)v[i + t] - v[i + t + x]  + Module) % Module;
	b = asm_remaind_mod_module(Module + v[i + t] - v[i + t +x],Module);

	v[i + t] = a;			//蝶形变换上臂
	//v[i + t + x] = (__int64) b * temp_root % Module;//蝶形变换下臂
	v[i + t + x] = asm_mul_mod(b,temp_root,Module);
	}
	//temp_root = (__int64) temp_root * root % Module;
	temp_root = asm_mul_mod(temp_root,root,Module);
	//++ iNum;
	}
	//cout<<endl;
	}
	//std::cout<<"输出测试根变更次数"<<iNum<<" ------- 变换长度 "<<v.size()<<std::endl;
	}
	*/

	void HUGEINT::mul_FFT_module(DATA & vSource,unsigned int Module,unsigned int Root,bool bPow){
		if(bPow){
			//平方优化
			unsigned int iSize = vData.size();
			FFT(vData,vData.size(),false,Module,Root);
			for(unsigned int i = 0;i < iSize;++ i){
				vData[i] = asm_mul_mod(vData[i],vData[i],Module);
				//vData[i] = (__int64)vData[i] * v[i] % M;
			}
			FFT(vData,iSize,true,Module,Root);

			unsigned int x = pow_mod(iSize,Module - 2,Module);
			for(unsigned int i = 0;i < iSize;++ i){
				vData[i] = asm_mul_mod(vData[i],x,Module);
				//vData[i] = (__int64)vData[i] * x % Module;
			}			
		}else{
			unsigned int iSize = vData.size();
			FFT(vData,vData.size(),false,Module,Root);
			FFT(vSource,vSource.size(),false,Module,Root);

			for(unsigned int i = 0;i < iSize;++ i){
				vData[i] = asm_mul_mod(vData[i],vSource[i],Module);
				//vData[i] = (__int64)vData[i] * v[i] % M;
			}
			FFT(vData,iSize,true,Module,Root);

			unsigned int x = pow_mod(iSize,Module - 2,Module);
			for(unsigned int i = 0;i < iSize;++ i){
				vData[i] = asm_mul_mod(vData[i],x,Module);
				//vData[i] = (__int64)vData[i] * x % Module;
			}
		}
	}

	void HUGEINT::mul_FFT_toom_module(DATA & vDest,DATA & vTemp_dest1,DATA & vTemp_dest2,DATA & vTemp_dest3,
		const DATA & v1,const DATA & v2,const DATA & v3,const DATA & v4,const DATA v5,const DATA & v6){

			if(v1.size() < (ENUM_MUL_OPTIMIZATION_TOOM3_AND_FFT)){

				for(unsigned int i = 0;i < v1.size();++ i){
					vTemp_dest1[i] = asm_mul_mod(v1[i],v2[i],M);
					//vData[i] = (__int64)vData[i] * v[i] % M;
				}	
				FFT(vTemp_dest1,vTemp_dest1.size(),true,M,iOrder_root);
				unsigned int x = pow_mod(v1.size(),M - 2,M);
				for(unsigned int i = 0;i < v1.size();++ i){
					vTemp_dest1[i] = asm_mul_mod(vTemp_dest1[i],x,M);
					//vData[i] = (__int64)vData[i] * x % Module;
				}
				//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

				for(unsigned int i = 0;i < v3.size();++ i){
					vTemp_dest2[i] = asm_mul_mod(v3[i],v4[i],M_second);
					//vData[i] = (__int64)vData[i] * v[i] % M;
				}	
				FFT(vTemp_dest2,vTemp_dest2.size(),true,M_second,iOrder_root_second);
				x = pow_mod(v3.size(),M_second - 2,M_second);
				for(unsigned int i = 0;i < v3.size();++ i){
					vTemp_dest2[i] = asm_mul_mod(vTemp_dest2[i],x,M_second);
					//vData[i] = (__int64)vData[i] * x % Module;
				}
				////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

				for(unsigned int i = 0;i < v5.size();++ i){
					vTemp_dest3[i] = asm_mul_mod(v5[i],v6[i],M_third);
					//vData[i] = (__int64)vData[i] * v[i] % M;
				}	
				FFT(vTemp_dest3,vTemp_dest3.size(),true,M_third,iOrder_root_third);
				x = pow_mod(v5.size(),M_third - 2,M_third);
				for(unsigned int i = 0;i < v5.size();++ i){
					vTemp_dest3[i] = asm_mul_mod(vTemp_dest3[i],x,M_third);
					//vData[i] = (__int64)vData[i] * x % Module;
				}
				////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
				////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
				////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
				////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			}else{

				for(unsigned int i = 0;i < v1.size();++ i){
					vTemp_dest1[i] = asm_mul_mod(v1[i],v2[i],M);
					//vData[i] = (__int64)vData[i] * v[i] % M;
				}	
				for(unsigned int i = 0;i < v3.size();++ i){
					vTemp_dest2[i] = asm_mul_mod(v3[i],v4[i],M_second);
					//vData[i] = (__int64)vData[i] * v[i] % M;
				}	

				for(unsigned int i = 0;i < v5.size();++ i){
					vTemp_dest3[i] = asm_mul_mod(v5[i],v6[i],M_third);
					//vData[i] = (__int64)vData[i] * v[i] % M;
				}	

				//		FFT(vTemp_dest1,vTemp_dest1.size(),true,M,iOrder_root);
				//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
				//		FFT(vTemp_dest2,vTemp_dest2.size(),true,M_second,iOrder_root_second);
				///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
				//		FFT(vTemp_dest3,vTemp_dest3.size(),true,M_third,iOrder_root_third);

				Task * vTaskv1,* vTaskv1_Module2,* vTaskv1_Module3;
				HUGEINTTASK_MULFFT vTaskv1arg,vTaskv1_Module2arg,vTaskv1_Module3arg;


				vTaskv1 = manager.GetIdleTask();				vTaskv1->fun = static_taskthread;
				vTaskv1->data = (void *) & vTaskv1arg;	vTaskv1->state = TASK_DEPENDENCE;

				vTaskv1arg.p = this;
				vTaskv1arg.m_v = & vTemp_dest1;					vTaskv1arg.m_iSize = vTemp_dest1.size();
				vTaskv1arg.m_inverse_mark = true;			vTaskv1arg.m_Module = M;
				vTaskv1arg.m_Root = iOrder_root;

				//FFT(v1_module2[i],v1_module2[i].size(),false,M_second,iOrder_root_second);

				vTaskv1_Module2 = manager.GetIdleTask();				vTaskv1_Module2->fun = static_taskthread;
				vTaskv1_Module2->data = (void *) & vTaskv1_Module2arg;	vTaskv1_Module2->state = TASK_DEPENDENCE;

				vTaskv1_Module2arg.p = this;
				vTaskv1_Module2arg.m_v = & vTemp_dest2;					vTaskv1_Module2arg.m_iSize = vTemp_dest1.size();
				vTaskv1_Module2arg.m_inverse_mark = true;			vTaskv1_Module2arg.m_Module = M_second;
				vTaskv1_Module2arg.m_Root = iOrder_root_second;


				//FFT(v1_module3[i],v1_module3[i].size(),false,M_third,iOrder_root_third);

				vTaskv1_Module3 = manager.GetIdleTask();				vTaskv1_Module3->fun = static_taskthread;
				vTaskv1_Module3->data = (void *) & vTaskv1_Module3arg;	vTaskv1_Module3->state = TASK_DEPENDENCE;

				vTaskv1_Module3arg.p = this;
				vTaskv1_Module3arg.m_v = & vTemp_dest3;					vTaskv1_Module3arg.m_iSize = vTemp_dest3.size();
				vTaskv1_Module3arg.m_inverse_mark = true;			vTaskv1_Module3arg.m_Module = M_third;
				vTaskv1_Module3arg.m_Root = iOrder_root_third;

				std::list<unsigned int> lTempList;
				lTempList.push_back(vTaskv1->taskID);
				lTempList.push_back(vTaskv1_Module2->taskID);
				lTempList.push_back(vTaskv1_Module3->taskID);

				Task_Join * pJoin = manager.join(lTempList);
				manager.AddTask(vTaskv1);	
				manager.AddTask(vTaskv1_Module2);	
				manager.AddTask(vTaskv1_Module3);
				manager.SetTaskFree(vTaskv1->taskID);
				manager.SetTaskFree(vTaskv1_Module2->taskID);
				manager.SetTaskFree(vTaskv1_Module3->taskID);

				manager.waittask(pJoin);


				unsigned int x = pow_mod(v1.size(),M - 2,M);
				for(unsigned int i = 0;i < v1.size();++ i){
					vTemp_dest1[i] = asm_mul_mod(vTemp_dest1[i],x,M);
					//vData[i] = (__int64)vData[i] * x % Module;
				}

				x = pow_mod(v3.size(),M_second - 2,M_second);
				for(unsigned int i = 0;i < v3.size();++ i){
					vTemp_dest2[i] = asm_mul_mod(vTemp_dest2[i],x,M_second);
					//vData[i] = (__int64)vData[i] * x % Module;
				}

				x = pow_mod(v5.size(),M_third - 2,M_third);
				for(unsigned int i = 0;i < v5.size();++ i){
					vTemp_dest3[i] = asm_mul_mod(vTemp_dest3[i],x,M_third);
					//vData[i] = (__int64)vData[i] * x % Module;
				}
			}







			//	FFT(vTemp_dest1,vTemp_dest1.size(),true,M,iOrder_root);

			//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

			//	FFT(vTemp_dest2,vTemp_dest2.size(),true,M_second,iOrder_root_second);

			////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

			//	FFT(vTemp_dest3,vTemp_dest3.size(),true,M_third,iOrder_root_third);




			CRT(vDest,vTemp_dest1,vTemp_dest2,vTemp_dest3);
	}

	static const unsigned int iMinSection9[ ] = { 773 ,773  };
	static const unsigned int iMinSection10[ ] = { 1028 ,1028 ,1283 ,1291  };
	static const unsigned int iMinSection11[ ] = { 1028 ,1028 ,1028 ,1546 ,1546 ,2837  };
	static const unsigned int iMinSection12[ ] = { 773 ,1028 ,1028 ,1028 ,1546 ,1546 ,3092 ,3092  };
	static const unsigned int iMinSection13[ ] = { 1028 ,773 ,1283 ,1801 ,1801 ,1801 ,3347 ,3347 ,3347 ,3347  };
	static const unsigned int iMinSection14[ ] = { 1283 ,1028 ,773 ,1283 ,1801 ,1801 ,1801 ,3602 ,3602 ,3602 ,3602 ,3602  };
	static const unsigned int iMinSection15[ ] = { 1283 ,1283 ,1028 ,773 ,1283 ,2056 ,2056 ,3857 ,3857 ,3857 ,3857 ,3857 ,3857 ,3857  };
	static const unsigned int iMinSection16[ ] = { 769 ,1538 ,1028 ,1028 ,2056 ,2056 ,2056 ,2056 ,4112 ,4112 ,4112 ,4112 ,4112 ,4112 ,4112 ,4112  };
	static const unsigned int iMinSection17[ ] = { 769 ,1538 ,1283 ,1028 ,2311 ,2311 ,2311 ,4367 ,4367 ,4367 ,4367 ,4367 ,4367 ,4367 ,4367  };
	static const unsigned int iMinSection18[ ] = { 769 ,1538 ,1283 ,1028 ,2311 ,2311 ,2311 ,4622 ,4622 ,4622 ,4622 ,4622 ,4622 ,4622  };
	static const unsigned int iMinSection19[ ] = { 1793 ,1283 ,1283 ,1028 ,2566 ,2566 ,4877 ,4877 ,4877 ,4877 ,4877 ,4877 ,4877  };
	static const unsigned int iMinSection20[ ] = { 1793 ,1283 ,1283 ,1028 ,2566 ,2566 ,5132 ,5132 ,5132 ,5132 ,5132 ,5132  };
	static const unsigned int iMinSection21[ ] = { 1793 ,1538 ,1283 ,2821 ,2821 ,5387 ,5387 ,5387 ,5387 ,5387 ,5387  };
	static const unsigned int iMinSection22[ ] = { 769 ,1538 ,1283 ,2821 ,2821 ,5642 ,5642 ,5642 ,5642 ,5642  };
	static const unsigned int iMinSection23[ ] = { 769 ,1538 ,1283 ,3076 ,5897 ,5897 ,5897 ,5897 ,5897  };
	static const unsigned int iMinSection24[ ] = { 769 ,1538 ,1283 ,3076 ,6152 ,6152 ,6152 ,6152  };
	static const unsigned int iMinSection25[ ] = { 1793 ,1283 ,1283 ,6407 ,6407 ,6407 ,6407  };
	static const unsigned int iMinSection26[ ] = { 1793 ,1538 ,3331 ,6662 ,6662 ,6662  };
	static const unsigned int iMinSection27[ ] = { 1793 ,1538 ,1283 ,6917 ,6917  };
	static const unsigned int iMinSection28[ ] = { 1793 ,1538 ,1283 ,7172  };
	static const unsigned int iMinSection29[ ] = { 769 ,1538 ,1283  };
	static const unsigned int iMinSection30[ ] = { 769 ,1538  };
	static const unsigned int iMinSection31[ ] = { 1793  };
	static const unsigned int * iMinSection[] = {iMinSection9,iMinSection10,iMinSection11,iMinSection12,iMinSection13,iMinSection14,
		iMinSection15,iMinSection16,iMinSection17,iMinSection18,iMinSection19,iMinSection20,
		iMinSection21,iMinSection22,iMinSection23,iMinSection24,iMinSection25,iMinSection26,
		iMinSection27,iMinSection28,iMinSection29,iMinSection30,iMinSection31};


	void HUGEINT::mul_FFT_toom(const DATA & vSource,bool bPow){
		SIZE_T iLength_section1,iLength_section2;
		SIZE_T iLength_convert;
		//预留两种变换长度
		{
			unsigned int iSize = 2;
			for(;iSize < vData.size() + vSource.size();iSize += iSize)
				;
			iLength_convert = iSize;
			iLength_section1 = iLength_convert - vSource.size();
			iLength_section2 = vSource.size();


			iSize = 1;
			for(;(1 << iSize) < vData.size() + vSource.size();iSize ++)
				;
			iLength_section1 = ( (vData.size() + (1 << (iSize - 5)) - 1) / (1 << (iSize - 5)) );
			iLength_section2 = ( (vSource.size() + (1 << (iSize - 5)) - 1) / (1 << (iSize - 5)) );

			if(iLength_section1 >= 16){
				iLength_convert = iMinSection[iLength_section1 - 9][iLength_section2 - 1];
			}else{
				iLength_convert = iMinSection[iLength_section1 - 9][iLength_section2 -  (17 - iLength_section1)];
			}
			iLength_section1 = iLength_convert / 256;	iLength_section2 = iLength_convert % 256;
			//段数,
			iLength_convert = (1 << iSize) / (32 / (iLength_section1 + iLength_section2));		
			iLength_section1 = (1 << iSize) / 32 * iLength_section1;
			iLength_section2 = (1 << iSize) / 32 * iLength_section2;
		}
		mul_FFT_toom(vSource,iLength_section1,iLength_section2,bPow);
	}


	void HUGEINT::mul_FFT_toom(const DATA & vSource,SIZE_T iLength_section1,SIZE_T iLength_section2,bool bPow ){	

		SIZE_T iLength_convert = iLength_section1 + iLength_section2;

		std::vector<DATA> v1,v2;
		{
			SIZE_T iTemp_num = (vData.size() + iLength_section1 - 1) / iLength_section1;
			v1.resize(iTemp_num);
			for(SIZE_T i = 0; i < v1.size();++ i){
				v1[i].set_capacity(iLength_convert);
			}
			for(SIZE_T i = 0; i < v1.size() - 1;++ i){
				v1[i].push_back(vData.getdata() + i * iLength_section1,vData.getdata() + (i + 1) * iLength_section1);
			}
			v1[v1.size() - 1].push_back(vData.getdata() + (v1.size() - 1) * iLength_section1,vData.end());
			for(SIZE_T i = 0; i < v1.size();++ i){
				v1[i].resize(iLength_convert);
			}
		}
		{
			SIZE_T iTemp_num = (vSource.size() + iLength_section2 - 1)/ iLength_section2;
			v2.resize(iTemp_num);
			for(SIZE_T i = 0; i < v2.size();++ i){
				v2[i].set_capacity(iLength_convert);
			}
			for(SIZE_T i = 0; i < v2.size() - 1;++ i){
				v2[i].push_back(vSource.getdata() + i * iLength_section2,vSource.getdata() + (i + 1) * iLength_section2);
			}
			v2[v2.size() - 1].push_back(vSource.getdata() + (v2.size() - 1) * iLength_section2,vSource.end());
			for(SIZE_T i = 0; i < v2.size();++ i){
				v2[i].resize(iLength_convert);
			}
		}

		std::vector<DATA> v1_module2(v1),v1_module3(v1),v2_module2(v2),v2_module3(v2);

		HUGEINT cTemp;
		if(iLength_convert < ENUM_MUL_OPTIMIZATION_TOOM3_AND_FFT){
			for(SIZE_T i = 0; i < v1.size(); ++ i){
				FFT(v1[i],v1[i].size(),false,M,iOrder_root);	//cTemp.vData =  v1[i]; std::cout<<"Module"<<M<<" ----  "<<cTemp<<std::endl;
				FFT(v1_module2[i],v1_module2[i].size(),false,M_second,iOrder_root_second);	//cTemp.vData = v1_module2[i]; std::cout<<"Module"<<M_second<<" ----  "<<cTemp<<std::endl;
				FFT(v1_module3[i],v1_module3[i].size(),false,M_third,iOrder_root_third);	//	cTemp.vData = v1_module2[i]; std::cout<<"Module"<<M_third<<" ----  "<<cTemp<<std::endl;
			}	


			for(SIZE_T i = 0; i < v2.size(); ++ i){
				FFT(v2[i],v2[i].size(),false,M,iOrder_root);	//cTemp.vData =  v2[i]; std::cout<<"Module"<<M<<" ----  "<<cTemp<<std::endl;
				FFT(v2_module2[i],v2_module2[i].size(),false,M_second,iOrder_root_second);	//cTemp.vData =  v2_module2[i]; std::cout<<"Module"<<M_second<<" ----  "<<cTemp<<std::endl;
				FFT(v2_module3[i],v2_module3[i].size(),false,M_third,iOrder_root_third);	//cTemp.vData =  v2_module3[i]; std::cout<<"Module"<<M_third<<" ----  "<<cTemp<<std::endl;
			}
		}else{
			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			std::vector<Task *> vTaskv1,vTaskv1_Module2,vTaskv1_Module3;
			std::vector<HUGEINTTASK_MULFFT> vTaskv1arg,vTaskv1_Module2arg,vTaskv1_Module3arg;
			vTaskv1.resize(v1.size());		vTaskv1_Module2.resize(v1.size());		vTaskv1_Module3.resize(v1.size());
			vTaskv1arg.resize(v1.size());	vTaskv1_Module2arg.resize(v1.size());	vTaskv1_Module3arg.resize(v1.size());
			//FFT(v1[i],v1[i].size(),false,M,iOrder_root);
			for(SIZE_T i = 0; i < vTaskv1.size(); ++ i){
				vTaskv1[i] = manager.GetIdleTask();				vTaskv1[i]->fun = static_taskthread;
				vTaskv1[i]->data = (void *) & vTaskv1arg[i];	vTaskv1[i]->state = TASK_DEPENDENCE;

				vTaskv1arg[i].p = this;
				vTaskv1arg[i].m_v = & v1[i];					vTaskv1arg[i].m_iSize = v1[i].size();
				vTaskv1arg[i].m_inverse_mark = false;			vTaskv1arg[i].m_Module = M;
				vTaskv1arg[i].m_Root = iOrder_root;
			}
			//FFT(v1_module2[i],v1_module2[i].size(),false,M_second,iOrder_root_second);
			for(SIZE_T i = 0; i < vTaskv1.size(); ++ i){
				vTaskv1_Module2[i] = manager.GetIdleTask();				vTaskv1_Module2[i]->fun = static_taskthread;
				vTaskv1_Module2[i]->data = (void *) & vTaskv1_Module2arg[i];	vTaskv1_Module2[i]->state = TASK_DEPENDENCE;

				vTaskv1_Module2arg[i].p = this;
				vTaskv1_Module2arg[i].m_v = & v1_module2[i];					vTaskv1_Module2arg[i].m_iSize = v1_module2[i].size();
				vTaskv1_Module2arg[i].m_inverse_mark = false;			vTaskv1_Module2arg[i].m_Module = M_second;
				vTaskv1_Module2arg[i].m_Root = iOrder_root_second;
			}

			//FFT(v1_module3[i],v1_module3[i].size(),false,M_third,iOrder_root_third);
			for(SIZE_T i = 0; i < vTaskv1.size(); ++ i){
				vTaskv1_Module3[i] = manager.GetIdleTask();				vTaskv1_Module3[i]->fun = static_taskthread;
				vTaskv1_Module3[i]->data = (void *) & vTaskv1_Module3arg[i];	vTaskv1_Module3[i]->state = TASK_DEPENDENCE;

				vTaskv1_Module3arg[i].p = this;
				vTaskv1_Module3arg[i].m_v = & v1_module3[i];					vTaskv1_Module3arg[i].m_iSize = v1_module3[i].size();
				vTaskv1_Module3arg[i].m_inverse_mark = false;			vTaskv1_Module3arg[i].m_Module = M_third;
				vTaskv1_Module3arg[i].m_Root = iOrder_root_third;
			}




			///////////////////////////////////////////////////////////////////////////////////////////////////////////////
			///////////////////////////////////////////////////////////////////////////////////////////////////////////////

			std::vector<Task *> vTaskv2,vTaskv2_Module2,vTaskv2_Module3;
			std::vector<HUGEINTTASK_MULFFT> vTaskv2arg,vTaskv2_Module2arg,vTaskv2_Module3arg;
			vTaskv2.resize(v2.size());		vTaskv2_Module2.resize(v2.size());		vTaskv2_Module3.resize(v2.size());
			vTaskv2arg.resize(v2.size());	vTaskv2_Module2arg.resize(v2.size());	vTaskv2_Module3arg.resize(v2.size());

			//FFT(v2[i],v2[i].size(),false,M,iOrder_root);
			for(SIZE_T i = 0; i < vTaskv2.size(); ++ i){
				vTaskv2[i] = manager.GetIdleTask();				vTaskv2[i]->fun = static_taskthread;
				vTaskv2[i]->data = (void *) & vTaskv2arg[i];	vTaskv2[i]->state = TASK_DEPENDENCE;

				vTaskv2arg[i].p = this;
				vTaskv2arg[i].m_v = & v2[i];					vTaskv2arg[i].m_iSize = v2[i].size();
				vTaskv2arg[i].m_inverse_mark = false;			vTaskv2arg[i].m_Module = M;
				vTaskv2arg[i].m_Root = iOrder_root;
			}
			//FFT(v2_module2[i],v2_module2[i].size(),false,M_second,iOrder_root_second);
			for(SIZE_T i = 0; i < vTaskv2.size(); ++ i){
				vTaskv2_Module2[i] = manager.GetIdleTask();				vTaskv2_Module2[i]->fun = static_taskthread;
				vTaskv2_Module2[i]->data = (void *) & vTaskv2_Module2arg[i];	vTaskv2_Module2[i]->state = TASK_DEPENDENCE;

				vTaskv2_Module2arg[i].p = this;
				vTaskv2_Module2arg[i].m_v = & v2_module2[i];					vTaskv2_Module2arg[i].m_iSize = v2_module2[i].size();
				vTaskv2_Module2arg[i].m_inverse_mark = false;			vTaskv2_Module2arg[i].m_Module = M_second;
				vTaskv2_Module2arg[i].m_Root = iOrder_root_second;
			}
			//FFT(v2_module3[i],v2_module3[i].size(),false,M_third,iOrder_root_third);
			for(SIZE_T i = 0; i < vTaskv2.size(); ++ i){
				vTaskv2_Module3[i] = manager.GetIdleTask();				vTaskv2_Module3[i]->fun = static_taskthread;
				vTaskv2_Module3[i]->data = (void *) & vTaskv2_Module3arg[i];	vTaskv2_Module3[i]->state = TASK_DEPENDENCE;

				vTaskv2_Module3arg[i].p = this;
				vTaskv2_Module3arg[i].m_v = & v2_module3[i];					vTaskv2_Module3arg[i].m_iSize = v2_module3[i].size();
				vTaskv2_Module3arg[i].m_inverse_mark = false;			vTaskv2_Module3arg[i].m_Module = M_third;
				vTaskv2_Module3arg[i].m_Root = iOrder_root_third;
			}

			std::list<unsigned int> lTempList;
			for(unsigned int i = 0; i < vTaskv1.size();++ i){
				lTempList.push_back(vTaskv1[i]->taskID);
				lTempList.push_back(vTaskv1_Module2[i]->taskID);
				lTempList.push_back(vTaskv1_Module3[i]->taskID);
			}
			for(unsigned int i = 0; i < vTaskv2.size();++ i){
				lTempList.push_back(vTaskv2[i]->taskID);
				lTempList.push_back(vTaskv2_Module2[i]->taskID);
				lTempList.push_back(vTaskv2_Module3[i]->taskID);
			}

			Task_Join * pJoin = manager.join(lTempList);
			for(unsigned int i = 0; i < vTaskv1.size();++ i){	//vTaskv1,vTaskv1_Module2,vTaskv1_Module3;
				manager.AddTask(vTaskv1[i]);	
				manager.AddTask(vTaskv1_Module2[i]);	
				manager.AddTask(vTaskv1_Module3[i]);
				manager.SetTaskFree(vTaskv1[i]->taskID);
				manager.SetTaskFree(vTaskv1_Module2[i]->taskID);
				manager.SetTaskFree(vTaskv1_Module3[i]->taskID);
			}
			for(unsigned int i = 0; i < vTaskv2.size();++ i){
				manager.AddTask(vTaskv2[i]);	
				manager.AddTask(vTaskv2_Module2[i]);	
				manager.AddTask(vTaskv2_Module3[i]);
				manager.SetTaskFree(vTaskv2[i]->taskID);
				manager.SetTaskFree(vTaskv2_Module2[i]->taskID);
				manager.SetTaskFree(vTaskv2_Module3[i]->taskID);
			}

			//	Sleep(5000);
			manager.waittask(pJoin);


			/*
			for(SIZE_T i = 0; i < v1.size(); ++ i){
			cTemp.vData =  v1[i]; std::cout<<"Module"<<M<<" ----  "<<cTemp<<std::endl;
			cTemp.vData = v1_module2[i]; std::cout<<"Module"<<M_second<<" ----  "<<cTemp<<std::endl;
			cTemp.vData = v1_module3[i]; std::cout<<"Module"<<M_third<<" ----  "<<cTemp<<std::endl;
			}

			for(SIZE_T i = 0; i < v2.size(); ++ i){
			cTemp.vData =  v2[i]; std::cout<<"Module"<<M<<" ----  "<<cTemp<<std::endl;
			cTemp.vData =  v2_module2[i]; std::cout<<"Module"<<M_second<<" ----  "<<cTemp<<std::endl;
			cTemp.vData =  v2_module3[i]; std::cout<<"Module"<<M_third<<" ----  "<<cTemp<<std::endl;
			}
			*/
		}
				



		vData.clear();
		for(SIZE_T i = 0;i < v1.size();++ i){
			for(SIZE_T t = 0; t < v2.size();++ t){
				DATA vDest(iLength_convert),vTemp1(iLength_convert),vTemp2(iLength_convert),vTemp3(iLength_convert);
				mul_FFT_toom_module(vDest,vTemp1,vTemp2,vTemp3,v1[i],v2[t],v1_module2[i],v2_module2[t],v1_module3[i],v2_module3[t]);
				add(vDest,i * iLength_section1 + t * iLength_section2);
			}
		}


		adjust();
	}

	void HUGEINT::mul_FFT(const DATA & vSource,bool bPow){

		if( vData.size() + vSource.size() > 4096)
			return mul_FFT_toom(vSource,bPow);

		unsigned int iSize = 2;
		for(;iSize < vData.size() + vSource.size();iSize += iSize)
			;
		vData.resize(iSize,0);
		//vSource.resize(iSize,0);
		DATA v(vSource);
		v.resize(iSize,0);
		//std::cout<<"FFT时的长度"<<vData.size()<<std::endl;

		DATA v1(vData),v2(vData),v3(v);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//主要在这里分发任务FFT,另一个位置在mul位置,.
		mul_FFT_module(v,M,iOrder_root,bPow);

		vData.swap(v1);			//保存M下的结果,并传入待处理数据,
		v = v3;
		mul_FFT_module(v,M_second,iOrder_root_second,bPow);

		vData.swap(v2);
#ifndef __HUGEINT_DEBUG_2MOUDULE_
		v = v3;
		//	for(SIZE_T i = 0; i < vData.size();++ i){
		//		vData[i] %= M_third;
		//		v[i] %= M_third;
		//	}
		//修正数据错误,当iRadix > M_third时需要进行先把数据变换到剩余域上,
		//否则优化时的 mod操作将出错.

		mul_FFT_module(v,M_third,iOrder_root_third,bPow);

		vData.swap(v3);			//只是统一3个变换结构后的名字
#endif

		CRT(vData,v1,v2,v3);

		/*
		__int64 lTemp = 0;
		for(unsigned int i = 0;i < vData.size();++ i){
		#ifdef __HUGEINT_DEBUG_2MOUDULE_

		#ifndef __HUGEINT_DEBUG_2MOUDULE_PRI_
		v1[i] = asm_mul_mod(v1[i],Modular_inversion,M);
		v2[i] = asm_mul_mod(v2[i],Modular_inversion_second,M_second);
		v3[i] = asm_mul_mod(v3[i],Modular_inversion_third,M_third);


		if((__int64)v1[i] * M_second + (__int64)v2[i] * M >= (__int64)M * M_second)
		lTemp += (__int64)v1[i] * M_second + (__int64)v2[i] * M - (__int64)M * M_second;
		else
		lTemp += (__int64)v1[i] * M_second + (__int64)v2[i] * M;
		vData[i] = lTemp % iRADIX;
		lTemp /= iRADIX;
		#else

		//unsigned int iTest = (M + v1[i] - v2[i]) % M;
		unsigned int iTest = asm_remaind_mod_module(M + v1[i] - v2[i],M);
		//iTest = ((__int64)iTest * Modular_inversion) % M;
		iTest = asm_mul_mod(iTest,Modular_inversion,M);
		lTemp += v2[i] + (__int64)iTest * M_second;

		vData[i] = lTemp % iRADIX;
		lTemp /= iRADIX;
		#endif


		#else

		//v1[i] = (M + v1[i] - v3[i])% M;	v2[i] = (M_second + v2[i] - v3[i]) % M_second;	//首先消掉M3iM1M2项,
		v1[i] = asm_remaind_mod_module(M + v1[i] - v3[i],M);v2[i] = asm_remaind_mod_module(M_second + v2[i] - v3[i],M_second);
		v1[i] = asm_mul_mod(v1[i],Modular_inversion,M);
		v2[i] = asm_mul_mod(v2[i],Modular_inversion_second,M_second);
		//v1[i] += ((__int64)v2[i] * M) / M_second;
		//v1[i] %= M;
		//v2[i] = ((__int64)v2[i] * M)  % M_second;
		unsigned int iQuotient;
		v2[i] = asm_mul_mod(v2[i],M,M_second,iQuotient);
		v1[i] = asm_remaind_mod_module(v1[i] + iQuotient,M);
		//v1[i] = (v1[i] + iQuotient)% M;


		//1946936316 34333687 101442501 2013265921
		//aM2M3 + bM3 + c
		//到此为止,剩余系表示法的数据已经完整,

		INT iTemp1,iTemp2;
		__int64 la,lb;
		//	la = ((__int64) v1[i] * M_second) / iRADIX;lb = ((__int64)v1[i] * M_second) % iRADIX;
		lb = asm_mul_mod(v1[i],M_second,iRADIX,iTemp1);la = iTemp1;
		la *= M_third;
		//	la += (lb * M_third) /iRADIX; lb = (lb * M_third) % iRADIX;
		iTemp2 = (INT) lb;lb = asm_mul_mod(iTemp2,M_third,iRADIX,iTemp1);la += iTemp1;
		//	la += ((__int64)v2[i] *M_third) / iRADIX;lb += ((__int64)v2[i] *M_third) % iRADIX;
		lb += asm_mul_mod(v2[i],M_third,iRADIX,iTemp1);la += iTemp1;
		lTemp += v3[i] + lb;

		la += lTemp / iRADIX;
		vData[i] = lTemp % iRADIX;
		lTemp = la;		
		#endif

		}

		//SIZE_T i = 0,iNum = vData.size();
		//while((i < iNum) && (! vData[iNum - 1 - i]))
		//	++ i;
		//vData.resize(iNum - i);
		SIZE_T iNum = num_front_zero(vData);
		vData.resize(vData.size() - iNum);
		//adjust();
		*/
	}

	void HUGEINT::CRT(DATA & vDest,const DATA & v1,const DATA & v2,const DATA & v3){
		vDest.resize(v1.size());
		__int64 lTemp = 0;
		for(unsigned int i = 0;i < v1.size();++ i){
			//v1[i] = (M + v1[i] - v3[i])% M;	v2[i] = (M_second + v2[i] - v3[i]) % M_second;	//首先消掉M3iM1M2项,
			INT iTemp_v1,iTemp_v2;
			iTemp_v1 = asm_remaind_mod_module(M + v1[i] - v3[i],M);iTemp_v2 = asm_remaind_mod_module(M_second + v2[i] - v3[i],M_second);
			iTemp_v1 = asm_mul_mod(iTemp_v1,Modular_inversion,M);
			iTemp_v2 = asm_mul_mod(iTemp_v2,Modular_inversion_second,M_second);
			//v1[i] += ((__int64)v2[i] * M) / M_second;
			//v1[i] %= M;
			//v2[i] = ((__int64)v2[i] * M)  % M_second;
			unsigned int iQuotient;
			iTemp_v2 = asm_mul_mod(iTemp_v2,M,M_second,iQuotient);
			iTemp_v1 = asm_remaind_mod_module(iTemp_v1 + iQuotient,M);
			//v1[i] = (v1[i] + iQuotient)% M;

			//1946936316 34333687 101442501 2013265921
			//aM2M3 + bM3 + c
			//到此为止,剩余系表示法的数据已经完整,

			INT iTemp1,iTemp2;
			__int64 la,lb;
			//	la = ((__int64) v1[i] * M_second) / iRADIX;lb = ((__int64)v1[i] * M_second) % iRADIX;
			lb = asm_mul_mod(iTemp_v1,M_second,iRADIX,iTemp1);la = iTemp1;
			la *= M_third;
			//	la += (lb * M_third) /iRADIX; lb = (lb * M_third) % iRADIX;
			iTemp2 = (INT) lb;lb = asm_mul_mod(iTemp2,M_third,iRADIX,iTemp1);la += iTemp1;
			//	la += ((__int64)v2[i] *M_third) / iRADIX;lb += ((__int64)v2[i] *M_third) % iRADIX;
			lb += asm_mul_mod(iTemp_v2,M_third,iRADIX,iTemp1);la += iTemp1;
			lTemp += v3[i] + lb;

			la += lTemp / iRADIX;
			vDest[i] = lTemp % iRADIX;
			lTemp = la;		

		}

		SIZE_T iNum = num_front_zero(vDest);
		vDest.resize(vDest.size() - iNum);
	}


	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	HUGEINT HUGEINT::mul_single(const unsigned int n){
		//这里有个要求 n < iBASE;
		HUGEINT cTemp;
		cTemp.vData.resize(vData.size() + 1);
		mul_single(cTemp.vData,vData,n);
		return cTemp;
	}

	void HUGEINT::mul_single(DATA & v,const DATA & vSource,const unsigned int n){
		//这里有个要求 n < iBASE;
		if(n == 0){
			v.clear();
			return ;
		}
		v.resize(vSource.size() + 1);//预留空间
		//v.resize(vSource.size());

		__int64 iTemp = 0;
		//HUGEINT cTemp(vData);//为了不改变 *this的值,这里创建了一个局部变量.最后则返回这个局部变量.
		for(unsigned int i = 0;i < vSource.size();++ i){
			iTemp += (__int64)vSource[i] * n;			
			v[i] = iTemp % iRADIX;
			iTemp /= iRADIX;
		}

		if(iTemp)
			v[v.size() - 1] = (INT)iTemp;
		else
			v.resize(v.size() - 1);		
	}

	void HUGEINT::mul_hard(const DATA & vSource){
		//硬乘法,
		HUGEINT cTemp(0);
		for(unsigned int i = 0;i < vSource.size();++ i){
			cTemp.add(mul_single(vSource[i]).vData,i);
		}
		vData.swap(cTemp.vData);
		return;
	}

	void HUGEINT::mul_karatsuba(const DATA & vSource){
		/*
		ab * cd = 
		+ ac
		+ ac + bd + (a - b)*(d - c )		//正常的模式是(a - b) * (c - d)才是对称形式
		+  bd
		3(n/2)(n/2)  4加
		*/
		//SIZE_T n = (vData.size())/ 2;
		SIZE_T n;
		HUGEINT a,b,c,d;
		n = vData.size() / 2;

		//{DATA vTemp(vData.begin() + n,vData.end());	a.vData.swap(vTemp);a.adjust();}
		//{DATA vTemp(vData.begin(),vData.begin() + n);b.vData.swap(vTemp);b.adjust();}
		{
			{DATA vTemp(vData.begin() + n,vData.end());	a.vData.swap(vTemp);a.adjust();}
			{DATA vTemp(vData.begin(),vData.begin() + n);b.vData.swap(vTemp);b.adjust();}
		}

		if(vSource.size() >= n)
		{
			{DATA vTemp(vSource.begin() + n,vSource.end());	c.vData.swap(vTemp);c.adjust();}
			{DATA vTemp(vSource.begin(),vSource.begin() + n);d.vData.swap(vTemp);d.adjust();}
		}else{
			{DATA vTemp(vSource.begin(),vSource.end());	d.vData.swap(vTemp);d.adjust();}
		}


		HUGEINT e = a * c;
		HUGEINT f = b * d;

		//HUGEINT t = (a - b) * (d - c) + e + f;
		HUGEINT cTemp = a - b;
		cTemp = d - c;

		HUGEINT t = (a - b) * (d - c);
		t += e + f;
		f.add(e.vData,n + n);
		f.add(t.vData,n);
		//bSymbol = f.bSymbol; //无需赋值,在后面运算符级会自动增加上去
		vData.swap(f.vData);	//结果放到本对象位置,
		adjust();
		return ;
	}

	void HUGEINT::mul_toom_cook_3(const DATA & vSource){
		/*
		5 (n/3)(n/3)  17加  3乘  2除,
		*/
		SIZE_T n = (vData.size()) / 3;
		HUGEINT x0,x1,x2;
		{
			{DATA vTemp(vData.begin() + n + n,vData.end());	x0.vData.swap(vTemp);x0.adjust();}
			{DATA vTemp(vData.begin() + n,vData.begin() + n + n);x1.vData.swap(vTemp);x1.adjust();}
			{DATA vTemp(vData.begin(),vData.begin() + n);x2.vData.swap(vTemp);x2.adjust();}
		}
		//HUGEINT x0(vData.begin() + n + n,vData.end()),x1(vData.begin() + n,vData.begin() + n + n),x2(vData.begin(),vData.begin() + n);
		HUGEINT y0,y1,y2;
		if(vSource.size() >= n + n)
		{
			{DATA vTemp(vSource.begin() + n + n,vSource.end());	y0.vData.swap(vTemp);y0.adjust();}
			{DATA vTemp(vSource.begin() + n,vSource.begin() + n + n);y1.vData.swap(vTemp);y1.adjust();}
			{DATA vTemp(vSource.begin(),vSource.begin() + n);y2.vData.swap(vTemp);y2.adjust();}
		}else{
			if(vSource.size() >= n){
				{DATA vTemp(vSource.begin() + n,vSource.end());y1.vData.swap(vTemp);y1.adjust();}
				{DATA vTemp(vSource.begin(),vSource.begin() + n);y2.vData.swap(vTemp);y2.adjust();}	
			}else{
				{DATA vTemp(vSource.begin(),vSource.end());	y2.vData.swap(vTemp);y2.adjust();}	
			}
		}

		//std::cout<<" x0,x1,x2"<<std::endl<<x0<<"	"<<x1<<"	"<<x2<<std::endl;
		//std::cout<<" y0,y1,y2"<<std::endl<<y0<<"	"<<y1<<"	"<<y2<<std::endl;
		HUGEINT c0,c1,c2,c3,c4;
		//-2,-1,0,1,2,   x0x1x2 * y0y1y2 =  p0p1p2p3p4    c0c1c2c3c4
		c0 = (x2 - x1 - x1 + x0 * 4) * (y2 - y1 - y1 + y0 * 4);
		c1 = (x2 - x1 + x0) * (y2 - y1 + y0);
		//c2 = x2 * y2;
		c3 = (x2 + x1 + x0) * (y2 + y1 + y0);
		//c4 = (x2 + x1 + x1 + x0 * 4) * (y2 + y1 + y1 + y0 * 4);
		/*
		16p0	-	8p1		+	4p2		-	2p3		+	p4	= c0
		p0		-	p1		+	p2		-	p3		+	p4	= c1
		p4	= c2
		p0		+	p1		+	p2		+	p3		+	p4	= c3
		16p0	+	8p1		+	4p2		+	2p3		+	p4	= c4

		p4 = x2y2;
		p0 = x0y0;
		p2 = (c1 + c3)/2 - p4 - p0;
		6p1 = (14p0 + 2p2 - p4)/ 6 - c0 + 2c1
		*/
		//std::cout<<" c0,c1,c2,c3,c4"<<std::endl<<c0<<"	"<<c1<<"	"<<c2<<"	"<<c3<<"	"<<c4<<std::endl;
		HUGEINT p0,p1,p2,p3,p4;
		p4 =  x2 * y2;
		p0 = x0 * y0;
		p2 = (c1 + c3) / 2 - p4 - p0;

		c3 -= p0 + p2 + p4;
		c0 -= p0 * 16 + p2 * 4 + p4;
		/*
		-8p1 - 2p3 = c0
		p1 +  p3 = c3
		*/
		p1 = (c0 + c3 * 2) / 6;
		p3 = c3 + p1;

		*this = p4;
		add(p3.vData,n);	
		add(p2.vData,n * 2);
		add(p1.vData,n * 3);//由于这里的p1比如是负数,因此无需判断符号,
		add(p0.vData,n * 4);

		adjust();
		return ;
	}

	void HUGEINT::mul_toom_cook_4(const DATA & vSource){
		return;
		/*
		7 * (n / 4)(n / 4) + 33(n/4 加法) 31(n/4 * 1)单乘,4次除法.
		*/
		SIZE_T n = (vData.size()) / 4;
		//HUGEINT x0(vData.begin() + n * 3,vData.end()),x1(vData.begin() + n * 2,vData.begin() + n * 3),x2(vData.begin() + n,vData.begin() + n * 2),x3(vData.begin(),vData.begin() + n);
		HUGEINT x0,x1,x2,x3;
		{DATA vTemp(vSource.begin() + n * 3,vSource.end());	x0.vData.swap(vTemp);x0.adjust();}
		{DATA vTemp(vSource.begin() + n * 2,vSource.begin() + n * 3);x1.vData.swap(vTemp);x1.adjust();}
		{DATA vTemp(vSource.begin() + n,vSource.begin() + n * 2);x2.vData.swap(vTemp);x2.adjust();}
		{DATA vTemp(vSource.begin() ,vSource.begin() + n );x3.vData.swap(vTemp);x3.adjust();}
		HUGEINT y0,y1,y2,y3;
		if(vSource.size() >= n * 3)
		{
			{DATA vTemp(vSource.begin() + n * 3,vSource.end());	y0.vData.swap(vTemp);y0.adjust();}
			{DATA vTemp(vSource.begin() + n * 2,vSource.begin() + n * 3);y1.vData.swap(vTemp);y1.adjust();}
			{DATA vTemp(vSource.begin() + n,vSource.begin() + n * 2);y2.vData.swap(vTemp);y2.adjust();}
			{DATA vTemp(vSource.begin() ,vSource.begin() + n );y3.vData.swap(vTemp);y3.adjust();}
		}else{
			if(vSource.size() >= n * 2){
				{DATA vTemp(vSource.begin() + n * 2,vSource.end());y1.vData.swap(vTemp);y1.adjust();}
				{DATA vTemp(vSource.begin() + n,vSource.begin() + n * 2);y2.vData.swap(vTemp);y2.adjust();}
				{DATA vTemp(vSource.begin() ,vSource.begin() + n );y3.vData.swap(vTemp);y3.adjust();}
			}else{
				if(vSource.size() >= n){
					{DATA vTemp(vSource.begin() + n,vSource.end());y2.vData.swap(vTemp);y2.adjust();}
					{DATA vTemp(vSource.begin() ,vSource.begin() + n );y3.vData.swap(vTemp);y3.adjust();}
				}else{
					{DATA vTemp(vSource.begin() ,vSource.end());y3.vData.swap(vTemp);y3.adjust();}
				}
			}
		}
		HUGEINT c0,c1,c2,c3,c4,c5,c6;

		//-1,-1/2,0,1/2,1,2,
		c0 = (x1 - x0 - x2 + x3) * (y1 - y0 - y2 + y3);
		c1 = (x1 * 2 - x0 - x2 * 4 + x3 * 8) * (y1 * 2 - y0 - y2 * 4 + y3 * 8);//这里乘了64
		//c2 = x3 * y3;
		c3 = (x1 * 2 + x0 + x2 * 4 + x3 * 8) * (y1 * 2 + y0 + y2 * 4 + y3 * 8);//这里乘了64
		c4 = (x1 + x0 + x2 + x3) * (y1 + y0 + y2 + y3);
		c5 = (x1 * 4 + x0 * 8 + x2 * 2 + x3) * (y1 * 4 + y0 * 8 + y2 * 2 + y3);
		//c6 = x0 * y0;
		/*
		1		  2		  3		  5		   2		   3		   4
		0,161,4 441 20 234 
		p0	-	 p1	+	 p2	-	 p3	+	  p4	-	  p5	+	  p6	=	c0
		p0	-	2p1	+	4p2	-	8p3	+	16p4	-	32p5	+	64p6	=	c1
		p6	=	c2
		p0	+	2p1	+	4p2	+	8p3	+	16p4	+	32p5	+	64p6	=	c3
		p0	+	 p1	+	 p2	+	 p3	+	  p4	+	  p5	+	  p6	=	c4
		64p0	+  32p1	+  16p2	+	8p3	+	 4p4	+	 2p5	+	  p6	=	c5


		1001 * 1234
		*/
		HUGEINT p0,p1,p2,p3,p4,p5,p6;
		p6 = x3 * y3;
		p0 = x0 * y0;


		c0	-=	p0	+	p6;
		c1	-=	p0	+	p6 * 64;
		c3	-=	p0	+	p6 * 64;
		c4	-=	p0	+	p6;
		c5	-=	p0*64	+	p6;

		c0	+=	c4;
		c1	+=	c3;
		//2p2	+	2p4	= c0
		//8p2	+	32p4 = c1;
		p4 = (c1 - c0 * 4) / 24;
		p2 = (c0 - p4 * 2) / 2;

		c3 -= p2 * 4 + p4 * 16;
		c4 -= p2 + p4;
		c5 -= p2 * 16 + p4 * 4;
		//2p1	+	8p3		+	32p5	=	c3
		//p1	+	 p3		+	  p5	=	c4
		//32p1	+	8p3		+	 2p5	=	c5
		p3 = (c4 * 34 - c3 - c5) / 18;
		c3 -= p3 * 8;
		c5 -= p3 * 8;
		p1 = (c5 * 16 - c3) / 510;

		p5 = c4 - p1 - p3;

		*this = p0;
		shift_left(vData,n * 6);

		*this += p6;
		if(! p5.bSymbol)	add(p5.vData,n);		else	add(p5.vData,n);
		if(! p4.bSymbol)	add(p4.vData,n * 2);	else	add(p4.vData,n * 2);
		if(! p3.bSymbol)	add(p3.vData,n * 3);	else	add(p3.vData,n * 3);
		if(! p2.bSymbol)	add(p2.vData,n * 4);	else	add(p2.vData,n * 4);
		if(! p1.bSymbol)	add(p1.vData,n * 5);	else	add(p1.vData,n * 5);
		//add_shift(c4.vData,n * 4);
		//return *this;
		return;
	}

	void HUGEINT::mul_section(const DATA & vSource){
		SIZE_T iLength_section1,iLength_section2;
		SIZE_T iLength_convert;
		float fSum = 0;
		iLength_convert = 1;
		SIZE_T ilength = 0;
		while(iLength_convert <= vSource.size()){
			iLength_convert += iLength_convert;
			++ ilength;
		}
		SIZE_T iTempLength_section1,iTempLength_section2;
		iLength_section2 = vSource.size();
		iLength_section1 = iLength_convert - iLength_section2;
		fSum = (1 + 2 *  (vData.size() + iLength_section1 - 1) / (float)iLength_section1) * iLength_convert * ilength;

		iTempLength_section2 = (vSource.size() + 1) / 2;
		iTempLength_section1 = (vData.size() + iLength_convert - iTempLength_section2 - 1) / (iLength_convert - iTempLength_section2);

		if((2 + iLength_section1 * 3) * (float)iLength_convert * iLength_convert < fSum){
			iLength_section2 = iTempLength_section2;
			iLength_section1 = iLength_convert - iLength_section2;
		}

		++ ilength;
		iLength_convert += iLength_convert;
		if(1 + 2 * (vData.size() + vSource.size() - 1) / (iLength_convert - vSource.size())  < fSum){
			iLength_section2 = vSource.size();
			iLength_section1 = iLength_convert - iLength_section2;
		}

		iTempLength_section2 = (vSource.size() + 1) / 2;
		iTempLength_section1 = (vData.size() + iLength_convert - iTempLength_section2 - 1) / (iLength_convert - iTempLength_section2);

		if((2 + iLength_section1 * 3) * (float)iLength_convert * iLength_convert < fSum){
			iLength_section2 = iTempLength_section2;
			iLength_section1 = iLength_convert - iLength_section2;
		}
		
		mul_FFT_toom(vSource,iLength_section1,iLength_section2);

	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	unsigned int HUGEINT::div_3_2_int(unsigned int d,unsigned int e,unsigned int a ,unsigned int b,unsigned int c){
		//这个不是真正的除法,是估算.
		//而且有个前提,结果一定是一位的.
		if(c == 0){
			return ((__int64)b * iRADIX + a) / ((__int64)e * iRADIX + d + 1);
		}

		SIZE_T iQuotient = 0;
		//处理高位数据移位,
		if(e < (unsigned int)(1 << (asm_bsr(iRADIX) / 2))){
			//如果除数高位很小.
			__int64 iDivisor = e * iRADIX + d + 1;
			unsigned int iValue =unsigned int (((__int64)iRADIX * iRADIX) / iDivisor);
			unsigned int iDivisor_Down = ((__int64)iRADIX * iRADIX) % iDivisor % iRADIX;
			unsigned int iDiviser_Up = (unsigned int)((((__int64)iRADIX * iRADIX) % iDivisor) / iRADIX);
			while(c){
				iQuotient += iValue * c;
				__int64 iDown =  (__int64)iDivisor_Down * c + (__int64)a;
				__int64 iUp = (__int64)iDiviser_Up * c + (__int64)b;

				iUp += iDown / iRADIX;
				a = iDown % iRADIX;
				b = iUp % iRADIX;
				c =INT (iUp /iRADIX);
			}
			iQuotient +=  SIZE_T((a + b * iRADIX) / iDivisor);
			//iDividend = (a + b * iBASE) % iDivisor;
		}else{
			HUGEINT cTemp(0),cTemp1(0);
			cTemp.vData.push_back(d);
			cTemp.vData.push_back(e);
			cTemp1.vData.push_back(a);
			cTemp1.vData.push_back(b);
			cTemp1.vData.push_back(c);
			{
				unsigned int iValue = (b + c * iRADIX) / (e + 1);
				//如果e较大.则直接考虑除e商,
				iQuotient += iValue;
				cTemp1 -= cTemp * iValue;
			}
			if(cTemp1.vData.size() >= 3){
				unsigned int iValue = (cTemp1.vData[1] + cTemp1.vData[2] * iRADIX) / (e + 1);
				//如果e较大.则直接考虑除e商,
				iQuotient += iValue;
				cTemp1 -= cTemp * iValue;
			}
			while(cTemp <= cTemp1){
				cTemp1 -= cTemp;
				++ iQuotient;
			}
		}
		return iQuotient;
	}

	void HUGEINT::div_single(const unsigned int n,std::pair<HUGEINT,HUGEINT> & pairTemp){
		//这里必须要求 n < iBASE,否则可能出错
		if(! vData.size()){
			//正常需要异常处理,除0
			pairTemp.first.vData.clear();
			pairTemp.first.bSymbol = false;
			pairTemp.second.vData.clear();
			pairTemp.second.bSymbol = false;
			return ;
		}
		HUGEINT cTemp(0);
		pairTemp.first.vData.resize(vData.size(),0);
		pairTemp.second.vData.clear();
		__int64 iTemp = 0;
		for(unsigned int i = 0;i < vData.size();++ i){
			iTemp = (__int64)vData[vData.size() - 1 - i] + iTemp * iRADIX;
			pairTemp.first.vData[pairTemp.first.vData.size() - 1 - i] = INT(iTemp / n);
			iTemp = iTemp % n;
		}
		pairTemp.first.adjust();
		pairTemp.second.vData.push_back((unsigned int)iTemp);
		return ;
	}

	void HUGEINT::div_hard(const HUGEINT & a,std::pair<HUGEINT,HUGEINT> & pairTemp){
		//这里默认了a.vdata.size >= 2 ,且被除数大于除数,
		{
			HUGEINT cTemp;
			cTemp.vData = vData;
			//	std::cout<<"硬除 被除数 "<<cTemp<<std::endl;
			cTemp.vData = a.vData;
			//	std::cout<<"硬除 除数 " <<cTemp<<std::endl;
		}
		//	std::cout<<"硬除法商位数 "<<vData.size() - a.vData.size()<<std::endl;

		SIZE_T iBitlength = asm_bsr(iRADIX);
		INT iQuotient = iRADIX / (a.vData[a.vData.size() - 1] + 1);
		iBitlength /= 2;
		//pairTemp.second.vData.clear();
		//	pairTemp.second.vData.resize(vData.size() - a.vData.size() + 1);

		//if(a.vData[a.vData.size() - 1] < (1 << iBitlength));
		pairTemp.first = 0;
		pairTemp.second = *this;
		pairTemp.first.vData.resize(vData.size() - a.vData.size() + 1);

		//	std::cout<<"当前商 "<<pairTemp.first<<std::endl;
		//	std::cout<<"当前余数 "<<pairTemp.second <<std::endl;
		while(! abs_compare_min(a.vData)){
			HUGEINT cTemp;
			do 
			{
				//							std::cout<<"当前被除数 "<<*this<<std::endl;
				//							std::cout<<"当前除数 "<<a.vData<<std::endl;
				if(vData[vData.size() - 1] < a.vData[a.vData.size() - 1]){
					if(a.vData[a.vData.size() - 1] < (unsigned int)(1 << iBitlength)){
						//如果除数高位过小,
						INT iTemp = iQuotient * vData[vData.size() - 1];
						pairTemp.first.vData[vData.size() - a.vData.size() - 1] += iTemp;
						mul_single(cTemp.vData,a.vData,iTemp);
						sub_shift(cTemp.vData,vData.size() - a.vData.size() - 1,false);
					}else{
						//如果除数高位较大,
						INT iTemp = ((__int64)vData[vData.size() - 1] * iRADIX ) / (a.vData[a.vData.size() - 1] + 1);
						pairTemp.first.vData[vData.size() - a.vData.size() - 1] += iTemp;
						mul_single(cTemp.vData,a.vData,iTemp);
						sub_shift(cTemp.vData,vData.size() - a.vData.size() - 1,false);
					}
					break;
				}
				if(abs_compare_min(a.vData,vData.size() - a.vData.size())){
					//如果最高位相同,但仍然小于
					INT iTemp = ((__int64)vData[vData.size() - 1] * iRADIX ) / (a.vData[a.vData.size() - 1] + 1);
					pairTemp.first.vData[vData.size() - a.vData.size() - 1] += iTemp;
					mul_single(cTemp.vData,a.vData,iTemp);
					sub_shift(cTemp.vData,vData.size() - a.vData.size() - 1,false);
					break;
				}
				{
					INT iTemp = vData[vData.size() - 1] / (a.vData[a.vData.size() - 1] + 1);
					if(! iTemp)
						iTemp = 1;
					pairTemp.first.vData[vData.size() - a.vData.size()] += iTemp;
					mul_single(cTemp.vData,a.vData,iTemp);
					sub_shift(cTemp.vData,vData.size() - a.vData.size(),false);
					break;
				}

			} while (1);
		}
		pairTemp.first.adjust();
		pairTemp.second.vData.swap(vData);
		//	std::cout<<"硬除法 商 "<<pairTemp.first<<std::endl;
		//	std::cout<<"硬除法 余数"<<pairTemp.second<<std::endl;
	}


	void HUGEINT::div_newton(const HUGEINT & a,std::pair<HUGEINT,HUGEINT> & pairValue){
		//xn+1 = 2xn - vx^2n = xn(2 - vxn);
		HUGEINT cReciprocal;									//倒数
		SIZE_T iPrecision(vData.size() - a.vData.size());		//精度 
		//	std::cout<<"需要计算的精度 "<<iPrecision<<std::endl;

		if(iPrecision / a.vData.size() > 7){
			iPrecision /= 4;
		}

		reciprocal_newton(cReciprocal,a.vData,iPrecision);
		shift_right(cReciprocal.vData,cReciprocal.vData.size() / 2);
		//		std::cout<<"倒数迭代数据"<<cReciprocal<<std::endl;
		HUGEINT cTemp(a);
		//cReciprocal = a.reciprocal_precision();
		//返回值精度 只保证不小于vData.size() - a.vData.size(),有可能超过这个精度
		div_newton(a,pairValue,cReciprocal);

	}

	void HUGEINT::div_newton(const HUGEINT & a,std::pair<HUGEINT,HUGEINT> & pairTemp,const HUGEINT & cReciprocal){
		//	std::cout<<"进入牛顿迭代时的初始化数据"<<*this<<std::endl;
		//	std::cout<<pairTemp.first<<std::endl;
		//	std::cout<<pairTemp.second<<std::endl;


		while(1){
			//	std::cout<<"测试中途数据 "<<pairTemp.first * a + *this<<std::endl;
			std::cout<<"测试迭代剩余数据长度"<<vData.size()<<std::endl;
			HUGEINT cQuotient;
			if(vData.size() < a.vData.size() + ENUM_NEWTON_FORECAST){
				//预测计算,当长度过短时,直接调用硬除法得到结果						//存在常数,需要提取
				//std::cout<<"测试是否是指向相同的数据"<<vData.getdata()<<"  "<<a.vData.getdata()<<std::endl;
				//std::cout<<a<<std::endl;
				HUGEINT cTemp(*this);
				if(*this < a){
					pairTemp.first = 0;
					pairTemp.second = vData;
				}else{
					div_hard(a,pairTemp);
				}
				//			std::cout<<"硬除实际合并数据"<<cTemp<<std::endl;
				//			std::cout<<pairTemp.first * a + pairTemp.second<<std::endl<<std::endl;
				break;
			}
			if(forecast_quotient_newton(cQuotient,a,cReciprocal)){
				//		std::cout<<"当前预测过大了"<<*this<<std::endl;

				HUGEINT cTemp(*this);
				div_newton(a,pairTemp,cReciprocal);
				//			std::cout<<"计算偏大时显示进入时数据"<<cTemp<<std::endl;
				//			std::cout<<"计算结果后"<<pairTemp.first * a + pairTemp.second<<std::endl;
				//还是把偏大的数据记录下来,之后进行修正,
				pairTemp.first = cQuotient - pairTemp.first;
				pairTemp.first -= 1;
				pairTemp.second = a - pairTemp.second;

				//std::cout<<"计算偏大时合并数据"<<pairTemp.first * a + pairTemp.second<<std::endl;
				break;
			}else{
				//预测值偏小,
				HUGEINT cTemp (*this);
				//std::cout<<"计算值偏小时数据"<<cTemp<<std::endl;
				div_newton(a,pairTemp,cReciprocal);
				pairTemp.first += cQuotient;
				//		std::cout<<"计算值偏小时数据"<<cTemp<<std::endl;
				//		std::cout<<"计算结果后"<<pairTemp.first * a + pairTemp.second<<std::endl;
				break;
			}
		}
		HUGEINT cTemp(*this);
		//	std::cout<<"最终------------------结果后"<<pairTemp.first * a + pairTemp.second<<std::endl;
	}


	void HUGEINT::reciprocal_newton(HUGEINT & cQuotient,const DATA & v,SIZE_T n){
		//牛顿迭代里面有可能有大量的 a = 2^k  + 1,b = 2^k;这类数据可能可以进行优化计算,
		//需要使用牛顿迭代时,最短的 v的长度肯定是已经比较长的了.
		//返回数据是,cQuotient 只保证 不短于n
		//xn + 1 = xn(2 - vxn); = xn + (xn - v*xn*xn)
		//xn (2 - v * xn);
		//误差为 - adx^2
		if(n == 0)
			n = v.size();

		//		std::cout<<"需要迭代精度"<<n<<std::endl;
		cQuotient = v[v.size() - 1];
		HUGEINT cIteration_factor;
		{
			SIZE_T i = 2;
			while(i < n)
				i += i;
#ifndef __HUGEINT_STD__
			cIteration_factor.vData.set_capacity(n);
#endif
		}
		//		std::cout<<"输入迭代数据"<<HUGEINT(v)<<std::endl;

		{
			HUGEINT cTemp1,cTemp2;
			cTemp1.vData.push_back(0);
			cTemp1.vData.push_back(0);
			cTemp1.vData.push_back(0);
			cTemp1.vData.push_back(0);
			cTemp1.vData.push_back(1);
			cTemp2.vData.push_back(v[v.size() - 2]);
			cTemp2.vData.push_back(v[v.size() - 1]);
			cTemp2 += 1;
			//		std::cout<<cTemp1<<std::endl;
			//		std::cout<<cTemp2<<std::endl;
			cQuotient = cTemp1 / cTemp2;
			//		std::cout<<cQuotient<<std::endl;
			//记录放大系数
		}
		cIteration_factor.vData.push_back(v[v.size() - 2]);
		cIteration_factor.vData.push_back(v[v.size() - 1]);


		int iNum = cQuotient.vData.size() - 2;

		for(SIZE_T i = 2; 1 ; i += i){
			//		std::cout<<std::endl;
			//		std::cout<<"有效位 "<<i<<" -------- "<<cIteration_factor.vData.size()<<std::endl;
			//		std::cout<<"x "<<cQuotient<<std::endl;
			//		std::cout<<"v "<<cIteration_factor<<std::endl;

			HUGEINT cTemp(cQuotient);
			//		std::cout<<"a "<<cIteration_factor<<std::endl;
			//cTemp *= cIteration_factor;		//对应偏移的应该是i + i - 1
			if(cTemp.vData.size() == i + 1){
				//如果是2^k   +1长度,截取最后一位,保证和刚好是2^k,
				HUGEINT cTemp1(cIteration_factor);
				cTemp1 *= cTemp.vData[cTemp.vData.size() - 1];
				cTemp.vData.pop_back();
				cTemp *= cIteration_factor;
				cTemp.add(cTemp1.vData,i);
			}
			//		std::cout<<"ax "<<cTemp<<std::endl;
			//	std::cout<<"x*v "<<cTemp<<" 实际数据需要偏移"<<i<<std::endl;	//对应偏移的应该是i + i - 1
			//这个接近1,看如何处理,

			//	std::cout<<"测试2   2 - xv "<<cTemp.vData.size()<<" - "<<cTemp<<std::endl;
			//	HUGEINT cTemp2(2);
			//	shift_left(cTemp2.vData,i + i - 1);
			//	cTemp2 -= cTemp;
			//	std::cout<<"测试1   2 - xv "<<cTemp2.vData.size()<<" - "<<cTemp2<<std::endl;
			//	cQuotient *= cTemp2;

			//		std::cout<<"当前精度"<<cIteration_factor.vData.size()<<"  "<<cTemp.vData.size()<<"  "<<i<<std::endl;
			if(cTemp.vData.size() > i + i){
				for(SIZE_T t = 0; t < cTemp.vData.size();++ t)
					cTemp.vData[t] = iRADIX - 1 - cTemp.vData[t];
				cTemp += 1;
				cTemp.vData.pop_back();
			}else{
				//		std::cout<<"没超过1"<<std::endl;
				for(SIZE_T t = 0; t < cTemp.vData.size();++ t)
					cTemp.vData[t] = iRADIX - 1 - cTemp.vData[t];
				cTemp.vData.push_back(1);
				cTemp += 1;
			}

			//		std::cout<<"2 - ax "<<cTemp<<std::endl;

			//	std::cout<<"cQuotient "<<cQuotient<<"  "<<cTemp<<std::endl;
			cQuotient *= cTemp;
			/*
			if(cQuotient.vData.size() + cTemp.vData.size() > i + i){
			INT iTemp = cQuotient.vData.size() + cTemp.vData.size() - i - i;
			DATA vTemp1(cTemp.vData.end() - iTemp,cTemp.vData.end());
			HUGEINT cTemp1;
			cTemp1.vData.swap(vTemp1);


			cTemp.vData.resize(cTemp.vData.size() - iTemp);
			cTemp.adjust();

			HUGEINT cTemp2(cQuotient);
			cQuotient *= cTemp;
			cTemp2 *= cTemp1;

			cQuotient.add(cTemp2.vData,cTemp.vData.size());
			}
			*/
			//		std::cout<<"x(2 - ax) "<<cQuotient<<std::endl<<std::endl;
			//	std::cout<<"cQuotient "<<cQuotient<<std::endl;

			shift_right(cQuotient.vData,i);

			//	std::cout<<"迭代结果精度长度 "<<cQuotient.vData.size()<<std::endl;
			//	if(cQuotient.vData.size() > i){
			//		shift_right(cQuotient.vData,cQuotient.vData.size() - i);
			//		std::cout<<"抛弃无效精度的数据"<<std::endl;
			//	}

			//这里控制迭代返回精度,正常来说,应该是 i + i < n / 2,但是由于需要多2次乘法,
			//一般来讲,以后进行多计算1次n * n次乘法,就大致得到迭代精度了.但收敛速度减半,
			//
			//可能没继续计算的必要,
			if(i + i < n ){
				cIteration_factor.vData.resize(i + i);
				if(i + i < v.size()){
					for(SIZE_T t = 0; t < i + i;++ t){
						cIteration_factor.vData[cIteration_factor.vData.size() - 1 - t] = v[v.size() - 1 - t];
					}
				}else{
					//如果输入数据长度不足
					SIZE_T t;
					for(t = 0; t < v.size();++ t){
						cIteration_factor.vData[cIteration_factor.vData.size() - 1 - t] = v[v.size() - 1 - t];
					}
					for(;t < i + i;++ t){
						cIteration_factor.vData[cIteration_factor.vData.size() - 1 - t] = 0;
					}
				}
			}else{
				break;
			}
		}

		//	std::cout<<"有效位 迭代最终精度 "<<" -------- "<<cQuotient.vData.size()<<std::endl;
		//		std::cout<<"有效位 "<<cQuotient.vData.size()<<" -------- "<<cIteration_factor.vData.size()<<std::endl;
		//			std::cout<<"x "<<cQuotient<<std::endl;
	//	if(cQuotient.vData.size() < n)
	//		shift_left(cQuotient.vData,n - cQuotient.vData.size());
		std::cout<<std::endl<<"迭代返回精度 "<<cQuotient.vData.size()<<std::endl;
		return ;

		/*
		SIZE_T iScaling_Factor_Quotient,iScaling_Factor_Multiplier;
		HUGEINT Quotient;
		HUGEINT cMultiplier;
		SIZE_T iQuotient_Size;
		cMultiplier = v[v.size() - 1] * iRADIX + v[v.size() - 2];
		Quotient = ((__int64)iRADIX * iRADIX) / (v[v.size() - 1] * iRADIX + v[v.size() - 2]);
		iScaling_Factor_Multiplier = cMultiplier.vData.size();
		iScaling_Factor_Quotient = Quotient.vData.size();
		iQuotient_Size = 2;
		//xn(2 - vxn)
		for(;iQuotient_Size <= n + n;){
		HUGEINT cTemp = cMultiplier * Quotient;
		HUGEINT cValue = Quotient;
		if(cTemp.vData[cTemp.vData.size() - 1] == 1){
		//已经超出1
		cValue.shift_left(cValue.vData,cTemp.vData.size() -1);
		cTemp.vData.pop_back();
		cTemp.adjust();
		cTemp *= Quotient;
		cValue -= cTemp;
		}else{
		cValue.shift_left(cValue.vData,cTemp.vData.size());
		//cTemp.complement(cTemp.vData);
		for(unsigned int i = 0;i < cTemp.vData.size();++ i){
		cTemp.vData[i] = iRADIX - 1 - cTemp.vData[i];
		}
		cTemp += 1;
		cTemp *= Quotient;
		cValue += cTemp;
		}

		if(cValue.vData.size() - 1 >= (cMultiplier.vData.size() + Quotient.vData.size()))
		if(cValue.vData[cValue.vData.size() - 1 - (cMultiplier.vData.size() + Quotient.vData.size())] >= iRADIX / 2){
		cValue.shift_right(cValue.vData,cValue.vData.size() - iQuotient_Size);
		cValue += 1;
		}else{
		cValue.shift_right(cValue.vData,cValue.vData.size() - iQuotient_Size);
		}

		Quotient = cValue;		
		iQuotient_Size += iQuotient_Size;
		cMultiplier.vData.clear();
		for(unsigned int i = iQuotient_Size < v.size() ? iQuotient_Size : v.size();i;-- i){
		cMultiplier.vData.push_back(v[v.size() - i]);
		}
		}
		return Quotient;
		*/

	}

	bool HUGEINT::forecast_quotient_newton(HUGEINT & cQuotient,const HUGEINT & a,const HUGEINT & cReciprocal){
		//预测商,利用已知的迭代,定位可能的商,
		SIZE_T iPrecision = (vData.size() - a.vData.size());
		//	std::cout<<"预测商精度 "<<iPrecision<<std::endl<<std::endl;
		HUGEINT cTempreciprocal;
		if(cReciprocal.vData.size() >= iPrecision)
		{
			DATA vTemp (cReciprocal.vData.begin() + cReciprocal.vData.size () - iPrecision, cReciprocal.vData.end());
			cTempreciprocal.vData.swap(vTemp);
		}else{
			cTempreciprocal.vData = cReciprocal.vData;
		}
		//	std::cout<<"牛顿迭代精度控制的结果 "<<cReciprocal<<std::endl;
		//std::cout<<cReciprocal * a<<std::endl;

		//HUGEINT cQuotient;
		if(vData.size() >= iPrecision)
		{
			DATA vTemp (vData.begin() + vData.size () - iPrecision, vData.end());
			cQuotient.vData.swap(vTemp);
		}else{
			cQuotient.vData = vData;
		}

		//	std::cout<<cQuotient<<std::endl;
		//	std::cout<<cTempreciprocal<<std::endl;
		cQuotient *= cTempreciprocal;			//cReciprocal 实际是 iRADIX^a.vData.size() + cReciprocal.vData.size()
		if(cQuotient.vData.size() >= iPrecision)
		shift_right(cQuotient.vData,cQuotient.vData.size() - iPrecision);
		//	std::cout<<"预测商 "<<cQuotient<<std::endl;

		//计算商,
		//std::cout<<"预测商"<<cQuotient<<std::endl;

		HUGEINT cTemp(cQuotient);
		cTemp *= a;
		//如果数据不产生进位,那么实际需要偏移1位, 比如111 * 1111实际不是7位数据,而是6位数据,预算的时候需要使用便宜.


		INT iTemp = vData[vData.size() - 1] > cTemp.vData[cTemp.vData.size() - 1] ?
			vData[vData.size() - 1] - cTemp.vData[cTemp.vData.size() - 1] : cTemp.vData[cTemp.vData.size() - 1] - vData[vData.size() - 1];

		if(iTemp <= 1){
			shift_right(cQuotient.vData,vData.size() - cTemp.vData.size());
			shift_right(cTemp.vData,vData.size() - cTemp.vData.size());
		}else{
			shift_right(cQuotient.vData,vData.size() - cTemp.vData.size() + 1);
			shift_right(cTemp.vData,vData.size() - cTemp.vData.size() + 1);
		}

		if(*this >= cTemp){
			//预测值偏小
			*this -= cTemp;
			return false;
		}

		//预测值偏大
		//cTemp -= *this;
		*this = cTemp - *this;
		return true;

	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	void HUGEINT::add(const DATA & v){
		INT iCarry = 0;
		SIZE_T i = 0;
		if(v.size() < vData.size()){

			for(i = 0;i < v.size();++ i){
				vData[i] += v[i] + iCarry;
				vData[i] = asm_remaind_mod_module(vData[i],iRADIX,iCarry);
			}
			while(iCarry &&( i < vData.size())){
				vData[i] += iCarry;
				vData[i] = asm_remaind_mod_module(vData[i],iRADIX,iCarry);
				i ++;
			}
			if(iCarry)
				vData.push_back(iCarry);
		}else{
			unsigned int iLength = vData.size();
			vData.resize(v.size());
			for(i = 0;i < iLength;++ i){
				vData[i] += v[i] + iCarry;
				vData[i] = asm_remaind_mod_module(vData[i],iRADIX,iCarry);
			}
			while(iCarry &&( i < v.size())){
				vData[i] = v[i] + iCarry;
				vData[i] = asm_remaind_mod_module(vData[i],iRADIX,iCarry);
				i ++;
			}
			if(iCarry)
				vData.push_back(iCarry);
			while(i < v.size()){
				vData[i] = v[i];
				i ++;
			}		
		}
		return;
	}

	void HUGEINT::sub(const DATA & v,bool bMin){
		//进入这里说明vData >= v,
		if(! bMin){
			//如果v的数据较小,bMin,在sub之前必须判断正确,
			INT iCarry = 0;
			SIZE_T i;
			for(i = 0;i < v.size();++ i){
				vData[i] += iRADIX - iCarry - v[i];
				vData[i] = asm_remaind_mod_module(vData[i],iRADIX,iCarry);
				iCarry = (~iCarry) &  0X01;
			}
			while(iCarry){
				vData[i] += iRADIX - iCarry;
				vData[i] = asm_remaind_mod_module(vData[i],iRADIX,iCarry);
				iCarry = (~iCarry) & 0X01;
				++ i;
			}
			adjust();
			return;
		}

		SIZE_T iLength = vData.size();
		vData.resize(v.size());

		INT iCarry = 0;
		SIZE_T i;
		for(i = 0;i < iLength;++ i){
			vData[i] = iRADIX + v[i]- iCarry - vData[i];
			vData[i] = asm_remaind_mod_module(vData[i],iRADIX,iCarry);
			iCarry = (~iCarry) &  0X01;

		}
		while(iCarry){
			vData[i] = iRADIX + v[i] - iCarry;
			vData[i] = asm_remaind_mod_module(vData[i],iRADIX,iCarry);
			iCarry = (~iCarry) & 0X01;
			++ i;
		}
		while(i < vData.size()){
			vData[i] = v[i];
			++ i;
		}
		adjust();
		return;

	}

	//所有的偏移量都是表示后面的偏移量,
	void HUGEINT::add(const DATA & v,const unsigned int n){
		INT iCarry = 0;
		SIZE_T i = 0;
		if(v.size() + n< vData.size()){

			for(i = 0;i < v.size();++ i){
				vData[i + n] += v[i] + iCarry;
				vData[i + n] = asm_remaind_mod_module(vData[i + n],iRADIX,iCarry);
			}
			while(iCarry &&( i + n < vData.size())){
				vData[i + n] += iCarry;
				vData[i + n] = asm_remaind_mod_module(vData[i + n],iRADIX,iCarry);
				i ++;
			}
			if(iCarry)
				vData.push_back(iCarry);
		}else{
			unsigned int iLength = vData.size();
			vData.resize(v.size() + n);
			for(i = n;i < iLength;++ i){
				vData[i] += v[i - n] + iCarry;
				vData[i] = asm_remaind_mod_module(vData[i],iRADIX,iCarry);
			}
			i -= n;
			while(iCarry &&( i < v.size())){
				vData[i + n] = v[i] + iCarry;
				vData[i + n] = asm_remaind_mod_module(vData[i + n],iRADIX,iCarry);
				i ++;
			}
			if(iCarry)
				vData.push_back(iCarry);
			while(i < v.size()){
				vData[i + n] = v[i];
				i ++;
			}		
		}
		return;
	}

	void HUGEINT::sub_shift(const DATA & v,const unsigned int n,bool bMin){
		//这个输入数据确保一定能够不产生借位溢出,
		//进入这里说明vData >= v,
		if(! bMin){
			//如果v的数据较小,bMin,在sub之前必须判断正确,
			INT iCarry = 0;
			SIZE_T i;
			for(i = 0;i < v.size();++ i){
				vData[i + n] += iRADIX - iCarry - v[i];
				vData[i + n] = asm_remaind_mod_module(vData[i + n],iRADIX,iCarry);
				iCarry = (~iCarry) &  0X01;
			}
			while(iCarry){
				vData[i + n] += iRADIX - iCarry;
				vData[i + n] = asm_remaind_mod_module(vData[i + n],iRADIX,iCarry);
				iCarry = (~iCarry) &  0X01;
				++ i;
			}
			//由于本身就是被减数,剩余数值无需从新赋值,
			adjust();
			return;
		}

		DATA vTemp(v);
		shift_left(vTemp,n);
		vData.swap(vTemp);
		sub(vTemp);

		return;	
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	void HUGEINT::mul_order(HUGEINT & a){
		if(vData.size() < a.vData.size())
			swap(a);
		return mul(a.vData);
	}
	void HUGEINT::mul(const DATA & v){
		//存在常数,需要提取
		//const static  SIZE_T iarCritical[] = {1,18,68,200};
		//		std::cout<<*this<<std::endl;
		//		std::cout<<HUGEINT(v)<<std::endl;
		if( (! vData.size()) || (! v.size())){
			*this = 0;
			return ;
		}
		if(vData.size() < v.size()){
			DATA vTemp (v);
			vData.swap(vTemp);
			mul(vTemp);
			return;
		}				
		//	std::cout<<"运算乘法"<<vData.size()<<"   "<<v.size()<<std::endl;


		do 
		{
			if(v.size() < ENUM_MUL_OPTIMIZATION_ZERO){
				vData.clear();
				break;
			}
			if(v.size() < ENUM_MUL_OPTIMIZATION_HARD){
				mul_hard(v);
				//mul_FFT(v);
				break;
			}
			if(v.size() < ENUM_MUL_OPTIMIZATION_KARATSUBA){
				mul_karatsuba(v);
				//	mul_FFT(v);
				break;
			}
			if(v.size() < ENUM_MUL_OPTIMIZATION_TOOM3){
				//mul_toom_cook_3(v);
				mul_FFT(v);
				break;
			}
			if(v.size() < ENUM_MUL_OPTIMIZATION_TOOM4){
				//mul_toom_cook_3(v);
				mul_FFT(v);
				break;
			}

			if(vData.size() / v.size() >= ENUM_MUL_OPTIMIZATION_SECTION){
				mul_section(v);
				break;
			}
			mul_FFT(v);

			break;
		} while (1);
		//	std::cout<<*this<<std::endl;
		return;
	}

	void HUGEINT::div(const DATA & v,std::pair<HUGEINT,HUGEINT> & pairValue){
		//xn+1 = 2xn - vx^2n = xn(2 - vxn);
		//first 为商,second 为余数
		if(! v.size()){
			//这里应该抛出异常.
			pairValue.first = 0;
			pairValue.second = 0;
			return ;
		}
		if(vData.size() < v.size()){
			//如果数据被除数远小于除数.
			pairValue.first = 0;
			pairValue.second = *this;
			return;
		}
		HUGEINT cTemp;
		HUGEINT cRemaind;
		SIZE_T iMantissa = num_end_zero(v);
		if(iMantissa)
		{
			//除数末尾有0,
			DATA vTemp(vData.begin(),vData.begin() + iMantissa);
			cRemaind.vData.swap(vTemp);
			shift_right(vData,iMantissa);
			//	shift_right(v,iMantissa);
			DATA vTempdata(v);
			shift_right(vTempdata,iMantissa);
			div(vTempdata,pairValue);
			//商不会变更,余数需要把截断数据不上,
			shift_left(pairValue.second.vData,iMantissa);
			for(SIZE_T i = 0; i < iMantissa;++ i){
				pairValue.second.vData[i] = cRemaind.vData[i];
			}			
			return;
		}
		//如果被除数末尾有0,未知有什么办法优化,
		//

		//cTemp.vData.swap(v);
		HUGEINT cTemp1;
		if(v.size() == 1){
			//std::cout<<"被除数"<<*this<<std::endl;
			//std::cout<<"除数"<<cTemp<<std::endl;
			div_single(v[0],pairValue);
			//std::cout<<"商"<<pairValue.first<<std::endl;
			//std::cout<<"余数"<<pairValue.second<<std::endl;
		}else{
			//if((cTemp.vData.size() < 10) || (vData.size() - cTemp.vData.size() < 10))
			if(vData.size() - v.size() < 5)		//存在常数,需要提取
				div_hard(v,pairValue);
			else
				div_newton(v,pairValue);
		}
		return ;
	}

	void HUGEINT::mod(const DATA & v,std::pair<HUGEINT,HUGEINT> & pairValue){
		//由于除法实际已经使用div已经足够,没有单独编写的必要.
		div(v,pairValue);
		return;
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////
	//模乘的核心
	void HUGEINT::mul_mod(INT * pDest,INT * pSource,SIZE_T n){
		INT iCarry = 0;
		if(n == 2){
			//最小模块,
			pDest[1] = asm_remaind_mod_module(pDest[0] + pDest[1],iRADIX - 1);
			pSource[1] = asm_remaind_mod_module(pSource[0] + pSource[1],iRADIX - 1);
			pDest[0] = asm_mul_mod(pDest[0],pSource[0],iRADIX);
			pDest[1] = asm_mul_mod(pDest[1],pSource[1],iRADIX - 1);
			if(pDest[1] < pDest[0]){
				pDest[1] = iRADIX - 1 + pDest[1] - pDest[0];
			}else{
				pDest[1] -= pDest[0];
			}
		}else{
			//
			iCarry = add_mod(pDest + n / 2,pDest,pDest + n / 2,n / 2);
			if(iCarry)
				add_mod(pDest + n / 2,iCarry,n / 2);
			if(abs_compare_min_reduction_down(pDest + n /2,n / 2)){
				for(SIZE_T i = 0; i < n / 2; ++ i)
					pDest[n / 2 + i] = 0;
			}
			iCarry = add_mod(pSource + n / 2,pSource,pSource + n / 2,n / 2);
			if(iCarry)
				add_mod(pSource + n / 2,iCarry,n / 2);
			if(abs_compare_min_reduction_down(pSource + n /2,n / 2)){
				for(SIZE_T i = 0; i < n / 2; ++ i)
					pSource[n / 2 + i] = 0;
			}
		}
	}

	bool HUGEINT::abs_compare_min_mod(INT * pDest,INT * pSource,SIZE_T n){
		INT * pTemp = pDest;
		pDest += n - 1;pSource += n - 1;
		for(;pTemp <= pDest ;pDest --,pSource --){
			if( * pDest != * pSource){
				return * pDest < * pSource;
			}
		}
		return false;
	}

	bool HUGEINT::abs_compare_min_reduction_down(INT * pDest,SIZE_T n){
		INT * pTemp = pDest;
		pDest += n - 1;
		for(;pTemp <= pDest ;pDest --){
			if( * pDest != iRADIX - 1){
				return * pDest < iRADIX - 1;
			}
		}
		return false;
	}




	INT HUGEINT::add_mod(INT * pDest,INT * pSource_up,INT * pSource_down,SIZE_T n){
		INT iCarry = 0;
		for(SIZE_T i = 0;i < n ;++ i){
			pDest[i] = pSource_up[i] + iCarry + pSource_down[i];
			pDest[i] = asm_remaind_mod_module(vData[i],iRADIX,iCarry);
		}
		return iCarry;
	}

	INT HUGEINT::add_mod(INT * pDest,INT a,SIZE_T iCarry){
		for(SIZE_T i = 0;iCarry;++ i){
			pDest[i] = pDest[i] + iCarry + pDest[i];
			pDest[i] = asm_remaind_mod_module(pDest[i],iRADIX,iCarry);
		}
		return iCarry;
	}


	INT HUGEINT::sub_mod(INT * pDest,INT * pSource_up,INT * pSource_down,SIZE_T n){
		INT iCarry = 0;
		for(SIZE_T i = 0;i < n ;++ i){
			pDest[i] = iRADIX + pSource_down[i] - iCarry - pSource_up[i];
			pDest[i] = asm_remaind_mod_module(pDest[i],iRADIX,iCarry);
			iCarry = (~iCarry) &  0X01;
		}
		return iCarry;
	}


	INT HUGEINT::reduction_up(INT * pDest,INT * pSource_up,INT * pSource_down,SIZE_T n){
		//由于采用特殊的 MOD, iRADIX ,iRADIX - 1,
		//低位无需处理,
		return 0;
	}

	INT HUGEINT::reduction_down(INT * pDest,INT * pSource_up,INT * pSource_down,SIZE_T n){
		INT iCarry = add_mod(pSource_down,pSource_up,pSource_down,n);
		if(iCarry)
			add_mod(pSource_down,iCarry,n);
		if(abs_compare_min_reduction_down(pSource_down,n)){
			for(SIZE_T i = 0; i < n;++ i)
				pSource_down[i] = 0;
		}		
		return iCarry;
	}

	void HUGEINT::normalized(INT * pDest,INT * pSource_up,INT * pSource_down,SIZE_T n){
		bool bMark = (pSource_up[0] ^ pSource_down[0]) & 0x01;

	}

}