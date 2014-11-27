#pragma once

#include <iostream>

using namespace std;

namespace parent
{
	class Node
	{
	public:
		virtual void show()
		{
			cout << "this is parent node" << endl;
		}

		// warning static virtual function is illegal.
		//static virtual void staticShow() = 0;
		virtual Node* clone() = 0;
	};
}

namespace child
{
	using namespace parent;
	class Node: public parent::Node
	{
	public:
		virtual void show()
		{
			cout << "this is child node" << endl;
		}

		virtual Node* clone()
		{
			Node* clone = new Node();
			// copy;
			*clone = *this;
			return clone;
		}
	};

	//static virtual void staticShow()
	//{
	//	cout << "this is a static virtual function by child" << endl;
	//}
}