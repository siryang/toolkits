#pragma once

#include <iostream>
#include <string>

#include <vector> // 模板特例化vector<bool>
#include <deque>	// dequeue
#include <list>
#include <set> // set multiset
#include <hash_set> // hash_set hash_multiset  namespace: stdext
#include <map> // map multimap
#include <hash_map> // hash_map hash_multimap


//标准非stl容器
//#include <array>
#include <bitset>
#include <valarray>  //valarray slice slice_array gslice
#include <stack> // base dequeue.
#include <queue>  //queue and priority_queue


// iterator
#include <iterator>
#include <utility>
#include <memory> // auto_ptr
//<utility>是一个很小的头文件，它包括了贯穿使用在STL中的几个模板的声明，
//<iterator>中提供了迭代器使用的许多方法
//<memory>的描述则十分的困难，它以不同寻常的方式为容器中的元素分配存储空间，同时也为某些算法执行期间产生的临时对象提供机制,<memory>中的主要部分是模板类allocator，它负责产生所有容器中的默认分配器。


// algorithms 
#include <numeric>
#include <algorithm>
#include <functional>
//<algorithm>是所有STL头文件中最大的一个（尽管它很好理解），它是由一大堆模版函数组成的，可以认为每个函数在很大程度上都是独立的，其中常用到的功能范围涉及到比较、交换、查找、遍历操作、复制、修改、移除、反转、排序、合并等等。
//<numeric>体积很小，只包括几个在序列上面进行简单数学运算的模板函数，包括加法和乘法在序列上的一些操作。
//<functional>中则定义了一些模板类，用以声明函数对象。

using namespace std;
//#include <xstring>
//#include <xtree>
//addition
//array
//forward_list
#include <xtree>
#include <unordered_map>
#include <unordered_set>
#include <tuple>
//unordered_multimap
//unordered_set
//unordered_multiset

//#include <slist>
//#include <rope>

using namespace std;

typedef vector<int> VecInt;
typedef vector<Elem> vecElem;

void vector_member();
void autoptr_test();
void list_test();
void hash_set_test();
void set_test();
void map_test();


