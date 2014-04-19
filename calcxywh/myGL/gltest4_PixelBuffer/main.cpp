
/*
	���ػ�������Ŀ��������CPUֱ���޸�GPU�������ݣ�������Ҫ�����ڴ��޸���һ��
	�ٸ������ӣ�������ͼʱ����Ҫ��ȡ�ļ����ڴ��У��ķ�CPU����Ҫһ���ڴ�buffer����Ȼ���ٽ��ڴ��е��������ݿ�����GPU��
			���ʹ�����ػ���������ô���԰��ļ���������Ϣֱ�����뵽GPU�С�
				��������File --> MemBuffer --> GPU Memory.   File --> GPU Memory

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
 * ȫ�ֺ���
*/
GLuint gTextureID[1];
GLbyte* gPixels = NULL;
GLuint gBufferID[1];

/*
 * ����������������ں��濩
*/
void SetMaterial();
void RenderTexture1();

void RenderCube1()
{
	// �����壬��ƽ������ת
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

// ִ��glOrtho����glFrustum��Ŀ���Ǹı�ͶӰ����
// �����ʹ��opengl�ṩ�Ľӿڣ������Լ��任ͶӰ����
void OrthoView(float w, float h)
{
	float ratio = 1.0f * w / h;
#if 1
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

void SetMaterial2()
{
	glEnable(GL_TEXTURE_2D);	// ������ͼ��Ⱦ����ֻҪ��render֮ǰִ�о���
	glGenTextures(1, &gTextureID[0]);	// ����һ����ͼID

	GLint width = 0, height = 0, comp = 0;
	GLenum format = 0;

	// ���õ�һ����ͼ������
	gPixels = gltLoadTGA("wood.tga", &width, &height, &comp, &format);
	glBindTexture(GL_TEXTURE_2D, gTextureID[0]);	// �󶨺�����ø���ͼ
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

	// �����ػ�������ֱ�ӽ��ļ���ȡ���Դ���
	glGenBuffers(1, gBufferID);
	glBindBuffer(GL_PIXEL_UNPACK_BUFFER, gBufferID[0]);
#if 0
	glBufferData(GL_PIXEL_UNPACK_BUFFER, width * height*3, gPixels, GL_STREAM_DRAW);
#else
	// ע�⣬���ﲻ����GL_STREAM_DRAW������GL_DYNAMIC_DRAW.GL_STREAM��ֻ��������һ������,GL_DYNAMIC�������޸����ݣ���������glMapBuffer��
	glBufferData(GL_PIXEL_UNPACK_BUFFER, width * height*3, NULL, GL_DYNAMIC_DRAW);
	GLubyte* buff = (GLubyte*)glMapBuffer(GL_PIXEL_UNPACK_BUFFER, GL_READ_WRITE);
	if(comp == GL_RGB8)
		memcpy(buff, gPixels, width*height*3);
	glUnmapBuffer(GL_PIXEL_UNPACK_BUFFER);
#endif
	glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);

	// �Դ�֮��Ŀ���
	glBindBuffer(GL_PIXEL_UNPACK_BUFFER, gBufferID[0]);
	glGenTextures(1, &gTextureID[0]);	// ����һ����ͼID
	glBindTexture(GL_TEXTURE_2D, gTextureID[0]);
	glTexImage2D(GL_TEXTURE_2D, 0, comp, width, height, 0, format, 
			GL_UNSIGNED_BYTE, BUFFER_OFFSET(0));
	free(gPixels);
	glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);

	// ����buffer���ͷ�
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