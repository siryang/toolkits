
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
// 加上这句360会报错。
// 指定目标程序中函数代码存在的代码段。开发驱动时会用到它。
#pragma code_seg("name"/*,"class"*/)
void Test();
void test2();
#pragma code_seg()

/************************************************************************/
// 在内存中建立一个数据段，并定义共享数据。
// 可以定义在dll中，使多个使用到此dll的进程共享。需要给这个变量加上set/get函数，这样进程可共享
// 两个不同的dll中定义相同名称的数据段会出现什么问题？
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
// 声明需要的lib，如果此程序生成的是lib，则字符串会出现在生成的lib中，供链接器搜索。
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
// 编译时使用的字符编码，一般用于编译程序中字符串的编码类型。
// 注意和函数setlocale的不同，后者作用在程序运行时。
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