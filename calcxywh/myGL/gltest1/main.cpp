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
	matrix *= rota;
	matrix *= trans;

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
void RenderScene()
{
	glClear(GL_COLOR_BUFFER_BIT);

	//RenderCube1();
	RenderCube2();
	//RenderCube3();
	RenderCube4();

	glFlush();
}

void SetRenderCondition()
{
	glClearColor(0, 0, 0, 1);
}
void OnWindowSized(GLsizei w, GLsizei h)
{
	if(w == 0 || h == 0)
		return;

	glViewport(0, 0, w, h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	/* 
	// 加入w是200，h是400，如果做如下设置，那么会看到视角被压瘪了
	glOrtho(-100, 100, -100, 100, 1, -1);
	// 所以得做一下转化，才能让视角看起来是正常的（没有压瘪）
	*/
	float ratio = 1.0f * w / h;
	if(w < h)
		glOrtho(-100, 100, -100 / ratio, 100/ratio, 100, -100);	// w<h, 所以当(right-left)<(top-bottom)时才会看起来正常
	else
		glOrtho(-100*ratio, 100*ratio, -100, 100, 100, -100);
	//glMatrixMode(GL_MODELVIEW);
	//glLoadIdentity();
}
int main(int argc, char** argv)
{
	float a;
	a = sin(PI/2);
	a = tan(PI/4);


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