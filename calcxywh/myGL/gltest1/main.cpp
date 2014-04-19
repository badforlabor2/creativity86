/*
	�о�һ�¾�������ͻ�������ѧ������
*/


#include <stdio.h>
#include <stdlib.h>
#include <GL/freeglut.h>

#include "mymath.h"

/*
	RenderCube1��RenderCube2�൱����ԭ�����ͼ�Σ�Ȼ�������ƶ�����ת�Ȳ�����
*/

void RenderCube11()
{
	// �����壬��ƽ������ת
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glColor3f(1.f, 0, 0);
	glTranslatef(0, 60, -200);
	glRotatef(30, 0, 0, 1);
	glutWireCube(25);
}
void RenderCube1()
{
	// �����壬��ƽ������ת
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glColor3f(1.f, 0, 0);
	glTranslatef(0, 60, 0);
	glRotatef(30, 0, 0, 1);
	glutWireCube(25);
}
void RenderCube2()
{
	// ͬ���������壬����ת����ƽ��
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
	// ʹ�þ�����������������壬��ƽ�ƣ�����ת
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
	// ʹ�þ�����������������壬����ת����ƽ��
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
		glMultMatrixf������:
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
		������������е�һ����ƽ�ƺ���ת��Ȼ���ٻ�����Щ�㣡
		����ÿһ���㶼ִ�о���任
	*/
}

void RenderCube6()
{
	// �����壬��ƽ������ת
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
	// �����壬��ƽ������ת
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

// ִ��glOrtho����glFrustum��Ŀ���Ǹı�ͶӰ����
// �����ʹ��opengl�ṩ�Ľӿڣ������Լ��任ͶӰ����
void OrthoView(float w, float h)
{
	float ratio = 1.0f * w / h;
#if 0
	if(w < h)
		glOrtho(-100, 100, -100 / ratio, 100/ratio, 0, -200);	// w<h, ���Ե�(right-left)<(top-bottom)ʱ�Żῴ��������
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
		znear ��ʾz���ϣ���׼�㵽ԭ��ľ��룬���znear==10, ��ô˵����znear�Ļ�׼����(0,0,-10)
		gluPerspective�Ĳ����ǣ�fov, w/h, znear, zfar.
		glFrustum�Ĳ����� ���ü���Ŀ��*0.5������ֵ�����������ü���ĸ߶�*0.5������ֵ��������znear, zfar
			��glOrtho�Ĳ������ơ�ֻ����һ���Ǵ�ԭ�㿴Z�᷽��(glFrustum)��һ���Ǵ�ԭ�㿴����(glOrtho)
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
		glFrustum(-near_size, near_size, -near_size / ratio, near_size/ratio, ZNear, ZFar);	// w<h, ���Ե�(right-left)<(top-bottom)ʱ�Żῴ��������
	else
		glFrustum(-near_size*ratio, near_size*ratio, -near_size, near_size, ZNear, ZFar);
#endif
}
void OnWindowSized(GLsizei w, GLsizei h)
{
	if(w == 0 || h == 0)
		return;

	glViewport(0, 0, w, h);

	// ִ��glOrtho����glFrustum��Ŀ���Ǹı�ͶӰ����
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//OrthoView(w, h);
	FrustumView(w, h);
	/* 
	// ����w��200��h��400��������������ã���ô�ῴ���ӽǱ�ѹ����
	glOrtho(-100, 100, -100, 100, 1, -1);
	// ���Ե���һ��ת�����������ӽǿ������������ģ�û��ѹ��
	*/
	//float ratio = 1.0f * w / h;
	//if(w < h)
	//	glOrtho(-100, 100, -100 / ratio, 100/ratio, 100, -100);	// w<h, ���Ե�(right-left)<(top-bottom)ʱ�Żῴ��������
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
