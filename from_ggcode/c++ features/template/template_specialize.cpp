class SmartSimpleUnit
{
public:
	virtual void show() = 0;
};

template<typename T1, typename T2>
class SmartSimple
	:public SmartSimpleUnit
{
public:
	void show()
	{
		printf ("original T1, T2\n");
	}
private:
	T1 m_t1;
	T2 m_t2;
};


template<>
class SmartSimple<int ,int>
	:public SmartSimpleUnit
{
public:
	void show()
	{
		printf ("special int, int\n");
	}
private:
	int m_t1;
	int m_t2;
};


template<typename T1>
class SmartSimple<T1, int>
	:public SmartSimpleUnit
{
public:
	void show()
	{
		printf("special T1, int\n");
	}
};

template<> 
void SmartSimple<double, double>::show()
{
	printf ("special function, double , int\n");
}

// error
template<typename T1> 
void SmartSimple<T1, double>::show()
{
	printf ("special function, double , int\n");
}

// 类成员函数可以特例化，但是不支持偏特化。

template<typename M>
class Widget
{
public:
	void show()
	{
		printf ("Widget show\n");
	}
};

template<> 
void Widget<int>::show()
{
	printf ("widget int\n");
}





int main()
{
	SmartSimpleUnit* dd[4];
	dd[0] = new SmartSimple<double, double>;
	dd[1] = new SmartSimple<int, double>;
	dd[2] = new SmartSimple<double, int>;
	dd[3] = new SmartSimple<int, int>;

	for (size_t i = 0; i < 4; i++)
	{
		dd[i]->show();
	}

	system("pause");

	return 0;
}