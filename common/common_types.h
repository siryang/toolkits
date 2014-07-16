#pragma once

typedef int int32;
typedef unsigned int uint32;
typedef short int16;
typedef unsigned short uint16;
typedef unsigned char uint8;
typedef signed char int8;
typedef long long int64;

// I disable warning 4127 because CQ_ASSERT() will generate it.
#pragma warning (disable: 4127) // warning C4127: conditional expression is constant
#pragma warning (disable: 4996) // warning C4127: conditional expression is constant
// I disable warning 4206 because some .c files are guarded with a macro for specific platform
#pragma warning (disable: 4206)	// warning C4206: nonstandard extension used : translation unit is empty
#pragma warning (error: 4029)	// This warning means that function definition and implementation don't match. It's a very dangerous error.
//#pragma warning (error: 4100)	// unreferenced formal parameter. Some variables may be overwritten by another variable of the same name.

#define CQ_HTONL(a) {a = ((((a) & 0x000000ff) << 24) | (((a) & 0x0000ff00) << 8) | (((a) & 0x00ff0000) >> 8)  | (((a) & 0xff000000) >> 24)); }
#define CQ_HTONS(a) {a = ((((a) & 0x00ff) << 8) | (((a) & 0xff00) >> 8)); }

#define element_of(o) (sizeof(o) / sizeof((o)[0]))

#define ASS_ASSERT(o) {do{\
	if(!(o)){printf("%s, %d"#o, __FILE__, __LINE__); __debugbreak();}\
}while(0)}

template<typename ElementType>
class PointImplement
{
public:
	ElementType x, y;

	PointImplement(){}
	PointImplement(ElementType _x, ElementType _y):x(_x), y(_y)
	{}

	bool operator != (const PointImplement& right) const
	{
		return x != right.x || y != right.y;
	}
};

template<typename ElementType>
class PointImplement3
{
public:
	ElementType x, y, z;

	PointImplement3(){}
	PointImplement3(ElementType _x, ElementType _y, ElementType _z):x(_x), y(_y), z(_z)
	{}

	bool operator != (const PointImplement3& right) const
	{
		return x != right.x || y != right.y || z != right.z;
	}
};

typedef PointImplement<int> Point;
typedef PointImplement<float> PointF;
typedef PointImplement<int> Vector2;

typedef PointImplement3<int> Point3I;
typedef PointImplement3<float> Point3F;
typedef PointImplement<int> Point2I;

template<typename ElementType>
struct RectImplement
{
	ElementType left, right, top, bottom;

	void set(ElementType _left, ElementType _right, ElementType _top, ElementType _bottom)
	{
		left = _left; right = _right; top = _top; bottom = _bottom;
	}

	ElementType width()
	{
		return right - left;
	}

	ElementType height()
	{
		return bottom - top;
	}
};

typedef RectImplement<float> RectF;
typedef RectImplement<int> Rect;

struct Size
{
	int width, height;
};

enum Dir3D
{
	Dir3D_left = 0,
	Dir3D_right, 
	Dir3D_forward,
	Dir3D_backward,
	Dir3D_up, 
	Dir3D_down
};
