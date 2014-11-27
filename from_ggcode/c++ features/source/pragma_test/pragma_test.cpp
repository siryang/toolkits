
// http://msdn.microsoft.com/zh-cn/library/d9x1s805

// 1.compile once only .

#define TESTVALUE 4
#if (TESTVALUE < 3)
int = 2
#else if(TESTVALUE >= 3)
int = 3
#endif

#pragma once

/************************************************************************/
// 2.display in compile info window.
#pragma message("Hello World")
#pragma message("Complie")

/************************************************************************/
//#pragme intrinsic()

/************************************************************************/
// 3.appoint code section place.
// �������360�ᱨ��
// ָ��Ŀ������к���������ڵĴ���Ρ���������ʱ���õ�����
#pragma code_seg("name"/*,"class"*/)
void Test();
void test2();
#pragma code_seg()

/************************************************************************/
// ���ڴ��н���һ�����ݶΣ������干�����ݡ�
// ���Զ�����dll�У�ʹ���ʹ�õ���dll�Ľ��̹�����Ҫ�������������set/get�������������̿ɹ���
// ������ͬ��dll�ж�����ͬ���Ƶ����ݶλ����ʲô���⣿
#pragma data_seg(".GLOBAL_DATA")
int g_shareData = 0;
#pragma data_seg()

void setShareData(int value){
	g_shareData = value;
}

int getShareData(){
	return g_shareData;
}

/************************************************************************/
#pragma warning(push)
#pragma	warning(disable: 4507; once: 4508; error:4509)
#pragma warning(pop)

#pragma warning(push, 3)
#pragma	warning(disable: 4507; once: 4508; error:4509)
#pragma warning(pop)

/************************************************************************/
// put program comments to destination file.
//#pragma comment(compiler)
//#pragma comment(exestr, "the comment pre_defined")
// ������Ҫ��lib������˳������ɵ���lib�����ַ�������������ɵ�lib�У���������������
//#pragma comment(lib, "libNeeded")


/************************************************************************/
// define in pre_complie header, declare pre_complie stop here
#pragma hdrstop

/************************************************************************/
// struct aligning
#pragma pack(1)
#pragma pack()

/************************************************************************/
// put code to section
extern "C"{
	void main();
};
#pragma alloc_text("name", main)

/************************************************************************/
//#pragma init_seg()
//#pragma optimize()
//#pragma auto_inline()
//#pragma component()
//#pragma pointer

/************************************************************************/
// ����ʱʹ�õ��ַ����룬һ�����ڱ���������ַ����ı������͡�
// ע��ͺ���setlocale�Ĳ�ͬ�����������ڳ�������ʱ��
#pragma setlocale("chinese")


#if !defined(__TYPE_POSITION_DEFINED)
#define __TYPE_POSITION_DEFINED
typedef int Position;
#endif

#if defined(_Position)
#pragma message("Position defined")
#else
#pragma message("Position not define")
#endif
//#pragma push(Position)



void Test(){}

void test2(){}

// pragma_directive_function.cpp
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
// A function has known behavior. 
// The compiler may call the function and not replace the function call with inline instructions, if it will result in better performance.
#pragma intrinsic(memset, strlen, printf)
// do not use strlen intrinsic
#pragma function(strlen)

void main()
{
	printf("Hello world\n");
}