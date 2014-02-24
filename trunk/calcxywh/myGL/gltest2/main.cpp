/*
	研究一下矩阵运算和基本的数学方法咯
*/


#include <stdio.h>
#include <stdlib.h>
#include <GL/freeglut.h>

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

// 执行glOrtho或者glFrustum的目的是改变投影矩阵
// 如果不使用opengl提供的接口，可以自己变换投影矩阵
void OrthoView(float w, float h)
{
	float ratio = 1.0f * w / h;
	if(w < h)
		glOrtho(-100, 100, -100 / ratio, 100/ratio, 100, -100);	// w<h, 所以当(right-left)<(top-bottom)时才会看起来正常
	else
		glOrtho(-100*ratio, 100*ratio, -100, 100, 100, -100);
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
	gluPerspective(FOV, w/h, ZNear, ZFar);
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
	OrthoView(w, h);
	//FrustumView(w, h);
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