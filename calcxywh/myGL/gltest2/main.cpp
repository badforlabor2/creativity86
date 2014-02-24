/*
	�о�һ�¾�������ͻ�������ѧ������
*/


#include <stdio.h>
#include <stdlib.h>
#include <GL/freeglut.h>

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
	glShadeModel(GL_FLAT);

	float gray[] = {0.5f, 0.75f, 0.75f, 1};
#if 1
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, gray);
#else
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	glColor4fv(gray);
#endif
	glBegin(GL_TRIANGLES);
		glNormal3f(0, 0, 1);
		//glColor3ub(255, 0, 0);
		glVertex3d(0, 50, -50);

		//glColor3ub(0, 255, 0);
		glVertex3d(-50, 0, -50);

		//glColor3ub(0, 0, 255);
		glVertex3d(50, 0, -50);
	glEnd();
}

void RenderTexture()
{

}

void RenderScene()
{
	glClear(GL_COLOR_BUFFER_BIT);

	//RenderCube1();
	//RenderCube2();
	RenderTexture();

	glFlush();
}

void SetRenderCondition()
{
	glClearColor(0, 0, 0, 1);


	glEnable(GL_LIGHTING);
	float ambientLight[] = {1, 0, 0, 1};
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientLight);

#if light0
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, ambientLight);
	glLightfv(GL_LIGHT0, GL_SPECULAR, ambientLight);
	glEnable(GL_LIGHT0);
#endif

	//gltLoadTGA();

}

// ִ��glOrtho����glFrustum��Ŀ���Ǹı�ͶӰ����
// �����ʹ��opengl�ṩ�Ľӿڣ������Լ��任ͶӰ����
void OrthoView(float w, float h)
{
	float ratio = 1.0f * w / h;
	if(w < h)
		glOrtho(-100, 100, -100 / ratio, 100/ratio, 100, -100);	// w<h, ���Ե�(right-left)<(top-bottom)ʱ�Żῴ��������
	else
		glOrtho(-100*ratio, 100*ratio, -100, 100, 100, -100);
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
	gluPerspective(FOV, w/h, ZNear, ZFar);
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
	OrthoView(w, h);
	//FrustumView(w, h);
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
	//glMatrixMode(GL_MODELVIEW);
	//glLoadIdentity();
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