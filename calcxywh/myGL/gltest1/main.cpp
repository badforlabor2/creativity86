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
	matrix *= rota;
	matrix *= trans;

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
	// ����w��200��h��400��������������ã���ô�ῴ���ӽǱ�ѹ����
	glOrtho(-100, 100, -100, 100, 1, -1);
	// ���Ե���һ��ת�����������ӽǿ������������ģ�û��ѹ��
	*/
	float ratio = 1.0f * w / h;
	if(w < h)
		glOrtho(-100, 100, -100 / ratio, 100/ratio, 100, -100);	// w<h, ���Ե�(right-left)<(top-bottom)ʱ�Żῴ��������
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