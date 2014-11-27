#pragma once

#include <iostream>
#include <string>

#include <vector> // ģ��������vector<bool>
#include <deque>	// dequeue
#include <list>
#include <set> // set multiset
#include <hash_set> // hash_set hash_multiset  namespace: stdext
#include <map> // map multimap
#include <hash_map> // hash_map hash_multimap


//��׼��stl����
//#include <array>
#include <bitset>
#include <valarray>  //valarray slice slice_array gslice
#include <stack> // base dequeue.
#include <queue>  //queue and priority_queue


// iterator
#include <iterator>
#include <utility>
#include <memory> // auto_ptr
//<utility>��һ����С��ͷ�ļ����������˹ᴩʹ����STL�еļ���ģ���������
//<iterator>���ṩ�˵�����ʹ�õ���෽��
//<memory>��������ʮ�ֵ����ѣ����Բ�ͬѰ���ķ�ʽΪ�����е�Ԫ�ط���洢�ռ䣬ͬʱҲΪĳЩ�㷨ִ���ڼ��������ʱ�����ṩ����,<memory>�е���Ҫ������ģ����allocator��������������������е�Ĭ�Ϸ�������


// algorithms 
#include <numeric>
#include <algorithm>
#include <functional>
//<algorithm>������STLͷ�ļ�������һ�����������ܺ���⣩��������һ���ģ�溯����ɵģ�������Ϊÿ�������ںܴ�̶��϶��Ƕ����ģ����г��õ��Ĺ��ܷ�Χ�漰���Ƚϡ����������ҡ��������������ơ��޸ġ��Ƴ�����ת�����򡢺ϲ��ȵȡ�
//<numeric>�����С��ֻ��������������������м���ѧ�����ģ�庯���������ӷ��ͳ˷��������ϵ�һЩ������
//<functional>��������һЩģ���࣬����������������

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


