/*
	数学公式、矩阵
*/
#ifndef _MY_MATH_H_
#define _MY_MATH_H_

#include <math.h>

#define PI 3.14159265358979323846

inline float TransAngle(float angle)
{
	angle = angle / 180 * PI;
	return angle;
}

class FMatrix
{
public:
	FMatrix()
	{
		M[0] = 1; M[4] = 0; M[8] = 0;  M[12] = 0;
		M[1] = 0; M[5] = 1; M[9] = 0;  M[13] = 0;
		M[2] = 0; M[6] = 0; M[10] = 1; M[14] = 0;
		M[3] = 0; M[7] = 0; M[11] = 0; M[15] = 1;
	}

	FMatrix operator *(const FMatrix& b) const
	{
		FMatrix ret;
		ret.M[0] = M[0]*b.M[0] + M[4]*b.M[1] + M[8]*b.M[2] + M[12]*b.M[3];

#define MULTI_PLANE(row, col) (M[row] * b.M[0+4*col] + M[row+4*1] * b.M[1+4*col] + M[row+4*2] * b.M[2+4*col] + M[row+4*3] * b.M[3+4*col])
		// check(ret.M[0] == MULTI_PLANE(0, 0));
		ret.M[0] = MULTI_PLANE(0, 0);
		ret.M[1] = MULTI_PLANE(1, 0);
		ret.M[2] = MULTI_PLANE(2, 0);
		ret.M[3] = MULTI_PLANE(3, 0);

		ret.M[4] = MULTI_PLANE(0, 1);
		ret.M[5] = MULTI_PLANE(1, 1);
		ret.M[6] = MULTI_PLANE(2, 1);
		ret.M[7] = MULTI_PLANE(3, 1);

		ret.M[8] = MULTI_PLANE(0, 2);
		ret.M[9] = MULTI_PLANE(1, 2);
		ret.M[10] = MULTI_PLANE(2, 2);
		ret.M[11] = MULTI_PLANE(3, 2);

		ret.M[12] = MULTI_PLANE(0, 3);
		ret.M[13] = MULTI_PLANE(1, 3);
		ret.M[14] = MULTI_PLANE(2, 3);
		ret.M[15] = MULTI_PLANE(3, 3);
#undef MULTI_PLANE

		return ret;
	}

	void operator*=(const FMatrix& b)
	{
		(*this) = operator*(b);
	}


public:
	float M[16];	// 列主序的矩阵
};

// 旋转矩阵
class FRotateMatrix : public FMatrix
{
	/*
		x轴
			1, 0, 0
			0, cosA, sinA
			0, -SinA, cosA
		y轴
			cosA, 0, -sinA
			0, 1, 0
			sinA, 0, cosA
		z轴
			cosA, sinA, 0
			-sinA, cosA, 0
			0, 0, 1
	*/
public:
	// angle --> [0, 360]
	FRotateMatrix(float angle, float x, float y, float z)
	{
		float mag, s, c;
		float xx, yy, zz, xy, yz, zx, xs, ys, zs, one_c;

		angle = angle / 180 * PI;
		s = float(sin(angle));
		c = float(cos(angle));

		mag = float(sqrt( x*x + y*y + z*z ));

		// Identity matrix
		if (mag == 0.0f) {
			return;
		}

		// Rotation matrix is normalized
		x /= mag;
		y /= mag;
		z /= mag;

#define mrc(row,col)  M[col*4+row]
//#define mrc(row,col)  M[row*4+col]

		xx = x * x;
		yy = y * y;
		zz = z * z;
		xy = x * y;
		yz = y * z;
		zx = z * x;
		xs = x * s;
		ys = y * s;
		zs = z * s;
		one_c = 1.0f - c;

		mrc(0,0) = (one_c * xx) + c;
		mrc(0,1) = (one_c * xy) - zs;
		mrc(0,2) = (one_c * zx) + ys;
		mrc(0,3) = 0.0f;

		mrc(1,0) = (one_c * xy) + zs;
		mrc(1,1) = (one_c * yy) + c;
		mrc(1,2) = (one_c * yz) - xs;
		mrc(1,3) = 0.0f;

		mrc(2,0) = (one_c * zx) - ys;
		mrc(2,1) = (one_c * yz) + xs;
		mrc(2,2) = (one_c * zz) + c;
		mrc(2,3) = 0.0f;

		mrc(3,0) = 0.0f;
		mrc(3,1) = 0.0f;
		mrc(3,2) = 0.0f;
		mrc(3,3) = 1.0f;

#undef M
	}
	FRotateMatrix(float pitch, float yaw, float roll)
	{
		//M[0] = 1; M[4] = 0; M[8] = 0;  M[12] = 0;
		//M[1] = 0; M[5] = 1; M[9] = 0;  M[13] = 0;
		//M[2] = 0; M[6] = 0; M[10] = 1; M[14] = 0;
		//M[3] = 0; M[7] = 0; M[11] = 0; M[15] = 1;
	}
};
// 平移矩阵
class FTranslationMatrix : public FMatrix
{
public:
	FTranslationMatrix(float x, float y, float z)
	{
		M[12] = x; M[13] = y; M[14] = z; M[15] = 1;
	}
};
// 缩放矩阵
class FScaleMatrix : public FMatrix
{
public:
	FScaleMatrix(float xscale=1, float yscale=1, float zscale=1)
	{
		M[0] = xscale;
						M[5] = yscale;
										M[10] = zscale;
	}
};
// 投影矩阵
/*
	2D的：
		x轴：	|1 0|
				|0 0|
		y轴：	|0 1|
				|0 0|
	3D的
		xy平面	|1 0 0|
				|0 1 0|
				|0 0 0|
		yz平面	|0 0 0|
				|0 1 0|
				|0 0 1|
		xz平面	|1 0 0|
				|0 0 0|
				|0 0 1|
	向任意直线或者平面投影（直线和平面都是经过原点的）
*/
class FOrthoMatrixXY : public FMatrix
{
public:
	FOrthoMatrixXY()
	{
		M[0] = 1;
		M[5] = 1;
		M[10] = 0;
	}
};
// 正交投影
class FOrthoMatrix : public FMatrix
{
public:
	FOrthoMatrix(float nNear, float nFar, float width, float height)
	{
	
	}
	FOrthoMatrix(float l, float r, float t, float b, float n, float f)
	{
		// 先投影到CCV上
		// 再进行透视除法？
		// 如何投影到CCV上？先做Z轴投影，再做X、Y轴投影
		/*
			Z轴投影：
				|1 0 0 0|
				|0 1 0 0|
				|0 0 2/(n-f) (n+f)/(n-f)|
				|0 0 0 1|
			X轴投影：
				|2/(r-l) 0 0 -((r+l)/(r-l))|
				|0 1 0 0|
				|0 0 1 0|
				|0 0 0 1|
			Y轴投影：
				|1 0 0 0|
				|0 2/(t-b) 0 -((t+b)/(t-b))|
				|0 0 1 0|
				|0 0 0 1|		
		*/
		M[0] = 2/(r-l); M[4] = 0;		M[8] = 0;			M[12] = -((r+l)/(r-l));
		M[1] = 0;		M[5] = 2/(t-b);	M[9] = 0;			M[13] = -((t+b)/(t-b));
		M[2] = 0;		M[6] = 0;		M[10] = 2/(n-f);	M[14] = +(n+f)/(n-f);
		M[3] = 0;		M[7] = 0;		M[11] = 0;			M[15] = 1;
	}
};

class FPerspectiveMatrix : public FMatrix
{
public:
	FPerspectiveMatrix(float l, float r, float b, float t, float n, float f)
	{
		M[0] = 2*n/(r-l);		M[4] = 0;				M[8] = 0;					M[12] = 0;
		M[1] = 0;				M[5] = 2*n/(t-b);		M[9] = 0;					M[13] = 0;
		M[2] = 0;				M[6] = 0;				M[10] = -1*(f+n)/(f-n);		M[14] = -2*f*n/(f-n);
		M[3] = 0;				M[7] = 0;				M[11] = -1;		M[15] = 0;
	}
	FPerspectiveMatrix(float fovy, float aspect, float n, float f)
	{
		// 这种特殊，是对称的，即r+l=0. b+t=0
		fovy = TransAngle(fovy/2);
		M[0] = 1/tan(fovy)/aspect;	M[4] = 0;			M[8] = 0;					M[12] = 0;
		M[1] = 0;				M[5] = 1/tan(fovy);		M[9] = 0;					M[13] = 0;
		M[2] = 0;				M[6] = 0;				M[10] = -1*(f+n)/(f-n);		M[14] = -2*f*n/(f-n);
		M[3] = 0;				M[7] = 0;				M[11] = -1;					M[15] = 0;
	}

};


#endif