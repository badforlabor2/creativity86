/*
	帧缓冲区，有点像RenderTarget？
*/




#include <stdio.h>
#include <stdlib.h>
#include "../util/GLee.h"
#include <GL/freeglut.h>

#include "myutil.h"

#define USE_FBO 0

void RenderScene();
void SetRenderCondition();

int gWindowWidth = 800;
int gWindowHeight = 600;


void FrustumView(float w, float h)
{

	const static float FOV = 60;
	const static float ZNear = 1;
	const static float ZFar = 400;

	gluPerspective(FOV, w/h, ZNear, ZFar);
}

void OnWindowSized(GLsizei w, GLsizei h)
{
	if(w == 0 || h == 0)
		return;

	gWindowWidth = w;
	gWindowHeight = h;

	glViewport(0, 0, w, h);

	// 执行glOrtho或者glFrustum的目的是改变投影矩阵
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//OrthoView(w, h);
	FrustumView(w, h);

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

void RenderModels();
void RenderTexture1();
void SetMaterial();
void GenerateTex();

void RenderScene()
{
#if USE_FBO
	GenerateTex();
#else
#endif
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT);
	//RenderCube1();
	RenderTexture1();

#if USE_FBO
#else
	RenderModels();
#endif
	glFlush();
}
void SetRenderCondition()
{
	SetMaterial();
	glClearColor(0, 0, 0, 1);
	//glShadeModel(GL_SMOOTH);
}
void RenderModels()
{
	glColor3f(0.0f, 1.0f, 0.0f);
	glPushMatrix();
	glRotatef(30, 1.0f, 0.0f, 0.0f);
	glTranslatef(0.0f, 0.0f, -100.0f);
	glutSolidSphere(25.0f, 50, 50);
	glPopMatrix();


	glColor3f(1.0f, 0.0f, 1.0f);
	glPushMatrix();
	glRotatef(20, 1.0f, 0.0f, 0.0f);
	glTranslatef(0.0f, -50.0f, -150.0f);
	glutSolidTorus(8.0f, 16.0f, 50, 50);
	glPopMatrix();

}

GLuint gTextureID = 0;
GLuint gFrameBuffer = 0;
GLuint gRenderBufferID = 0;
//float gFrameWidth=500, gFrameHeight = 500;

void RenderTexture1()
{
	glEnable(GL_TEXTURE_2D);

	glColor3f(1.0f, 1.0f, 1.0f);
	glBindTexture(GL_TEXTURE_2D, gTextureID);
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
void SetMaterial()
{
	glEnable(GL_TEXTURE_2D);
#if USE_FBO
	glGenFramebuffersEXT(1, &gFrameBuffer);

	// 生成一个Texture，用来绑定到FrameBuffer上
	glGenTextures(1, &gTextureID);
	glBindTexture(GL_TEXTURE_2D, gTextureID);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);  
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);  
	//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);  
	//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);  
	glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE); // automatic mipmap  
	// 这里的width和height一定不能是零，要不就会判定该texture无效的
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, gWindowWidth, gWindowHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	// 绑定到framebuffer
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, gFrameBuffer);
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, gTextureID, 0);

	//glGenRenderbuffersEXT(1, &gRenderBufferID);
	//glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, gRenderBufferID);
	//glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT, gWindowWidth, gWindowHeight);
	//glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, 0);

	//glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, gRenderBufferID);

	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
#else
	GLint width = 0, height = 0, comp = 0;
	GLenum format = 0;

	GLbyte* gPixels = gltLoadTGA("../resource/wood.tga", &width, &height, &comp, &format);

	// 用像素缓冲区，直接将文件读取到显存中
	glGenTextures(1, &gTextureID);	// 生成一个贴图ID
	glBindTexture(GL_TEXTURE_2D, gTextureID);
	glTexImage2D(GL_TEXTURE_2D, 0, comp, width, height, 0, format, 
		GL_UNSIGNED_BYTE, gPixels);
	free(gPixels);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
#endif
}
void GenerateTex()
{
	// 将model渲染到该texture上
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, gFrameBuffer);
	glClearColor(1, 1, 1, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	RenderModels();
	GLenum fboStatus = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
	if (fboStatus != GL_FRAMEBUFFER_COMPLETE_EXT)
	{
		fprintf(stderr, "FBO Error!\n");
	}
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

	// 生成mipmap
	glBindTexture(GL_TEXTURE_2D, gTextureID);
	glGenerateMipmapEXT(GL_TEXTURE_2D);
}