
/*
	像素缓冲区，目的是允许CPU直接修改GPU像素数据，而不需要经过内存修改这一步
	举个简单例子：绘制贴图时，需要读取文件到内存中（耗费CPU，需要一块内存buffer），然后再将内存中的像素数据拷贝到GPU中
			如果使用像素缓冲区，那么可以把文件的像素信息直接吸入到GPU中。
				流程区别：File --> MemBuffer --> GPU Memory.   File --> GPU Memory

				glGenBuffers();
				glBindBuffer();
				glBufferData();
				glMapBuffer();
				glUnmapBuffer();
				glBufferSubData();
				glDeleteBuffer();
				GL_PIXEL_UNPACK_BUFFER;		// glDrawPixels, glTexImage2D.
				GL_PIXEL_PACK_BUFFER;		// glReadPixels, glGetTexImage.
				GL_PIXEL_UNPACK_BUFFER_BINDING;
				GL_PIXEL_PACK_BUFFER_BINDING;


*/

#include <stdio.h>
#include <stdlib.h>
#include "../util/GLee.h"
#include <GL/freeglut.h>

#include "myutil.h"


/*
 * 全局函数
*/
GLuint gTextureID[1];
GLbyte* gPixels = NULL;
GLuint gBufferID[1];

/*
 * 函数声明，定义放在后面咯
*/
void SetMaterial();
void RenderTexture1();

void RenderCube1()
{
	// 立方体，先平移在旋转
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glColor3f(1.f, 0, 0);
	glTranslatef(0, 60, 0);
	glTranslatef(0, 0, -160);
	glRotatef(30, 0, 0, 1);
	glutWireCube(25);
	glColor3f(1.f, 1.f, 1);
	glutSolidCube(24.5);
	
}

void RenderScene()
{
	glClear(GL_COLOR_BUFFER_BIT);

	//RenderCube1();
	RenderTexture1();
	glFlush();
}

void SetRenderCondition()
{
	SetMaterial();
	glClearColor(0, 0, 0, 1);
	glShadeModel(GL_SMOOTH);
}

// 执行glOrtho或者glFrustum的目的是改变投影矩阵
// 如果不使用opengl提供的接口，可以自己变换投影矩阵
void OrthoView(float w, float h)
{
	float ratio = 1.0f * w / h;
#if 1
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
#if 1
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

void SetMaterial2()
{
	glEnable(GL_TEXTURE_2D);	// 开启贴图渲染咯，只要在render之前执行就行
	glGenTextures(1, &gTextureID[0]);	// 生成一个贴图ID

	GLint width = 0, height = 0, comp = 0;
	GLenum format = 0;

	// 设置第一个贴图的属性
	gPixels = gltLoadTGA("wood.tga", &width, &height, &comp, &format);
	glBindTexture(GL_TEXTURE_2D, gTextureID[0]);	// 绑定后好设置该贴图
	glTexImage2D(GL_TEXTURE_2D, 0, comp, width, height, 0, format, GL_UNSIGNED_BYTE, gPixels);
	free(gPixels);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}
void SetMaterial()
{
	glEnable(GL_TEXTURE_2D);

	GLint width = 0, height = 0, comp = 0;
	GLenum format = 0;

	gPixels = gltLoadTGA("wood.tga", &width, &height, &comp, &format);

	// 用像素缓冲区，直接将文件读取到显存中
	glGenBuffers(1, gBufferID);
	glBindBuffer(GL_PIXEL_UNPACK_BUFFER, gBufferID[0]);
#if 0
	glBufferData(GL_PIXEL_UNPACK_BUFFER, width * height*3, gPixels, GL_STREAM_DRAW);
#else
	// 注意，这里不能用GL_STREAM_DRAW，得用GL_DYNAMIC_DRAW.GL_STREAM是只允许设置一次数据,GL_DYNAMIC允许多次修改数据，即允许多次glMapBuffer。
	glBufferData(GL_PIXEL_UNPACK_BUFFER, width * height*3, NULL, GL_DYNAMIC_DRAW);
	GLubyte* buff = (GLubyte*)glMapBuffer(GL_PIXEL_UNPACK_BUFFER, GL_READ_WRITE);
	if(comp == GL_RGB8)
		memcpy(buff, gPixels, width*height*3);
	glUnmapBuffer(GL_PIXEL_UNPACK_BUFFER);
#endif
	glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);

	// 显存之间的拷贝
	glBindBuffer(GL_PIXEL_UNPACK_BUFFER, gBufferID[0]);
	glGenTextures(1, &gTextureID[0]);	// 生成一个贴图ID
	glBindTexture(GL_TEXTURE_2D, gTextureID[0]);
	glTexImage2D(GL_TEXTURE_2D, 0, comp, width, height, 0, format, 
			GL_UNSIGNED_BYTE, BUFFER_OFFSET(0));
	free(gPixels);
	glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);

	// 用完buffer后释放
	glDeleteBuffers(1, gBufferID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

}

void RenderTexture1()
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, gTextureID[0]);

	glBegin(GL_TRIANGLES);
		glNormal3f(0, 0, 1);
		glTexCoord2f(0, 0);
		glVertex3d(0, 50, -150);

		glNormal3f(0, 0, 1);
		glTexCoord2f(0, 1);
		glVertex3d(-50, 0, -150);

		glNormal3f(0, 0, 1);
		glTexCoord2f(1, 1);
		glVertex3d(50, 0, -150);
	glEnd();
}