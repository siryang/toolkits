#include "design_pattern_stdafx.h"
#include <memory>

// 可以使用 class B : public virtual Base{}; 继承virtual Base类，这样，class B也是virtual class.
class FlyBehavior
{
public:
	virtual void fly() = 0;
	~FlyBehavior()
	{
		std::cout << "delete FlyBehavior" << std::endl;
	}
};
typedef std::auto_ptr<FlyBehavior> FlyBehaviorPointer;

class EnableFlyBehavior
	: public FlyBehavior
{
public:
	void fly()
	{
		std::cout << "I can fly!" << std::endl;
	}
};

class DisableFlyBehavior
	: public FlyBehavior
{
public:
	void fly()
	{
		std::cout << "I can't fly!" << std::endl;
	}
};

class Animinal
{
public:

	// 委托（delegate）
	void fly()
	{
		m_flyBehavior->fly();
	}

	void setFlyBehavior(FlyBehavior* flyBehavior)
	{
		m_flyBehavior = FlyBehaviorPointer(flyBehavior);
	}

	virtual void showSelf() = 0;
protected:
	FlyBehaviorPointer m_flyBehavior;
};

class Brid
	:public Animinal
{
public:
	Brid()
	{
		FlyBehavior* flyBehavior = new EnableFlyBehavior;
		m_flyBehavior = FlyBehaviorPointer(flyBehavior);
	}

	void showSelf()
	{
		std::cout << "I am brid" << std::endl;
	}
};

class Pig
	:public Animinal
{
public:
	Pig()
	{
		m_flyBehavior = FlyBehaviorPointer(new DisableFlyBehavior);
	}

	void showSelf()
	{
		std::cout << "I am pig" << std::endl;
	}
};
/***********************Test*************************************************/
enum ProductType
{
	P1,
	P2
};

class Product
{
public:
	Product()
	{
		//prepareA();
		//prepareB();
	}
	virtual void prepareA() = 0;
	virtual void prepareB() = 0;
};

class PrepareA1
{
public:
	virtual void prepareA()
	{
		std::cout << "i am prepare A1!" << std::endl;
	}
};

class PrepareB1
{
public:
	virtual void prepareB()
	{
		std::cout << "i am prepare B1" << std::endl;
	}
};
//
//// ?? 为什么这种组合方式不能实例化父类虚函数
//class Product2
//	:public Product, public  PrepareA1, public  PrepareB1
//{
//public:
//	void prepareA()
//	{
//		PrepareA1::prepareA();
//	}
//
//	void prepareB()
//	{
//		PrepareB1::prepareB();
//	}
//};

class Product1
	:public  Product
{
private:
	PrepareA1 a;
	PrepareB1 b;
public:
	void prepareA()
	{
		a.prepareA();
	}

	void prepareB()
	{
		b.prepareB();
	}
};

class AbstractFactory
{
public:
	Product* creator(ProductType type)
	{
		Product* p;
		if (type == P1)
		{
			 p = new Product1;	
		}
		else if (type == P2)
		{
			//p = new Product2;
		}
		return p;
	}
};
/************************************************************************/
#include<vector>
using namespace std;

struct P
{
	int i;
	P(int i)
	{
		this->i = i;
	}
};

void testSimple()
{

	AbstractFactory factory;
	Product* q = factory.creator(P1);
	Brid brid;
	brid.showSelf();
	brid.fly();

	Pig p;
	p.showSelf(); 
	p.fly();

	std::cout << "here is a flying pig " << std::endl;
	p.setFlyBehavior(new EnableFlyBehavior);
	p.showSelf();
	p.fly();

	getchar();
}
