#include "gtest/gtest.h"

class Child;
class Parent
{
private:
	Child* m_v3;
};

class Child: public Parent
{
public:
};

TEST(Macro, basic)
{
	EXPECT_TRUE(true);
}