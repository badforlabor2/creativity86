/*
	研究一下矩阵运算和基本的数学方法咯
*/


#include <stdio.h>
#include <stdlib.h>
#include <GL/freeglut.h>

#include "mymath.h"

/*
	RenderCube1和RenderCube2相当于在原点绘制图形，然后再做移动、旋转等操作。
*/

void RenderCube11()
{
	// 立方体，先平移在旋转
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glColor3f(1.f, 0, 0);
	glTranslatef(0, 60, -200);
	glRotatef(30, 0, 0, 1);
	glutWireCube(25);
}
void RenderCube1()
{
	// 立方体，先平移在旋转
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glColor3f(1.f, 0, 0);
	glTranslatef(0, 60, 0);
	glRotatef(30, 0, 0, 1);
	glutWireCube(25);
}
void RenderCube2()
{
	// 同样的立方体，先旋转，在平移
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glColor3f(0, 1, 0);
	glRotatef(30, 0, 0, 1);
	glTranslatef(0, 60, 0);
	glScalef(0.5, 1, 1);
	glutWireCube(25);
}

void RenderCube3()
{
	// 使用矩阵操作，绘制立方体，先平移，在旋转
	FMatrix matrix;
	FTranslationMatrix trans(0, 60, 0);
	FRotateMatrix rota(25, 0, 0, 1);
	matrix *= trans;
	matrix *= rota;

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(matrix.M);
	glColor3f(0, 0, 1);
	glutWireCube(25);
}
void RenderCube4()
{
	// 使用矩阵操作，绘制立方体，先旋转，再平移
	FMatrix matrix;
	FTranslationMatrix trans(0, 60, 0);
	FRotateMatrix rota(30, 0, 0, 1);
	//FScaleMatrix scale(0.5, 1, 1);
	FScaleMatrix scale(1, 0.5);
	matrix *= rota;
	matrix *= trans;
	matrix *= scale;

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(matrix.M);
	/*
		glMultMatrixf的例子:
		glLoadIdentity();
		glMultMatrixf(rota.M);
		glMultMatrixf(trans.M);
	*/ 
	glColor3f(0, 1, 1);
	glutWireCube(25);
}


void RenderCube5()
{
	/*
		对立方体的所有点一次做平移和旋转，然后再绘制这些点！
		即对每一个点都执行矩阵变换
	*/
}

void RenderCube6()
{
	// 立方体，先平移在旋转
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glColor3f(1.f, 0, 0);
	glTranslatef(0, 60, 0);
	glTranslatef(0, 0, -100);
	glRotatef(30, 0, 0, 1);
	glutWireCube(25);
}
void RenderCube7()
{
	// 立方体，先平移在旋转
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glColor3f(1.f, 0, 0);
	glTranslatef(0, 60, 0);
	glRotatef(30, 0, 0, 1);
	glutWireCube(25);
}
void RenderScene()
{
	glClear(GL_COLOR_BUFFER_BIT);

	RenderCube11();
	//RenderCube1();
	//RenderCube3();

	//RenderCube2();
	//RenderCube4();

	//RenderCube6();

	glFlush();
}

void SetRenderCondition()
{
	glClearColor(0, 0, 0, 1);
}

// 执行glOrtho或者glFrustum的目的是改变投影矩阵
// 如果不使用opengl提供的接口，可以自己变换投影矩阵
void OrthoView(float w, float h)
{
	float ratio = 1.0f * w / h;
#if 0
	if(w < h)
		glOrtho(-100, 100, -100 / ratio, 100/ratio, 0, -200);	// w<h, 所以当(right-left)<(top-bottom)时才会看起来正常
	else
		glOrtho(-100*ratio, 100*ratio, -100, 100, 0, -200);
#else
	if(w < h)
	{
		FOrthoMatrix ortho(-100, 100, 100/ratio, -100/ratio, 0, -200);
		glMultMatrixf(ortho.M);
	}
	else
	{
		FOrthoMatrix ortho(-100*ratio, 100*ratio, 100, -100, 0, -200);
		glMultMatrixf(ortho.M);
	}
#endif
}
void FrustumView(float w, float h)
{
	/*
		znear 表示z轴上，基准点到原点的距离，如果znear==10, 那么说明是znear的基准点是(0,0,-10)
		gluPerspective的参数是：fov, w/h, znear, zfar.
		glFrustum的参数是 近裁剪面的宽度*0.5（正负值两个），近裁剪面的高度*0.5（正负值两个），znear, zfar
			和glOrtho的参数类似。只不过一个是从原点看Z轴方向(glFrustum)，一个是从原点看四周(glOrtho)
	*/

	const static float FOV = 60;
	const static float ZNear = 1;
	const static float ZFar = 400;
#if 1
#if 0
	gluPerspective(FOV, w/h, ZNear, ZFar);
#else

#if 0
	float near_size = 0;
	near_size = tan(TransAngle(FOV/2)) * ZNear;
	float ratio = 1.0f * w / h;
	FPerspectiveMatrix perspective(-near_size*ratio, near_size*ratio, -near_size, near_size, ZNear, ZFar);
	//FPerspectiveMatrix perspective(-near_size, near_size, -near_size / ratio, near_size/ratio, ZNear, ZFar);
	glMultMatrixf(perspective.M);
#else
	FPerspectiveMatrix perspective(FOV, w/h, ZNear, ZFar);
	glMultMatrixf(perspective.M);
#endif
#endif
#else
	float near_size = 0;
	near_size = tan(TransAngle(FOV)/2) * ZNear;
	float ratio = 1.0f * w / h;
	if(w < h)
		glFrustum(-near_size, near_size, -near_size / ratio, near_size/ratio, ZNear, ZFar);	// w<h, 所以当(right-left)<(top-bottom)时才会看起来正常
	else
		glFrustum(-near_size*ratio, near_size*ratio, -near_size, near_size, ZNear, ZFar);
#endif
}
void OnWindowSized(GLsizei w, GLsizei h)
{
	if(w == 0 || h == 0)
		return;

	glViewport(0, 0, w, h);

	// 执行glOrtho或者glFrustum的目的是改变投影矩阵
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//OrthoView(w, h);
	FrustumView(w, h);
	/* 
	// 加入w是200，h是400，如果做如下设置，那么会看到视角被压瘪了
	glOrtho(-100, 100, -100, 100, 1, -1);
	// 所以得做一下转化，才能让视角看起来是正常的（没有压瘪）
	*/
	//float ratio = 1.0f * w / h;
	//if(w < h)
	//	glOrtho(-100, 100, -100 / ratio, 100/ratio, 100, -100);	// w<h, 所以当(right-left)<(top-bottom)时才会看起来正常
	//else
	//	glOrtho(-100*ratio, 100*ratio, -100, 100, 100, -100);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}
int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE|GLUT_RGBA);
	//glutInitWindowPosition( 100, 100 );
	glutInitWindowSize(800, 600);
	glutCreateWindow("gltest1");

	glutDisplayFunc(RenderScene);
	glutReshapeFunc(OnWindowSized);
	
	SetRenderCondition();

	glutMainLoop();



	return 0;
}
