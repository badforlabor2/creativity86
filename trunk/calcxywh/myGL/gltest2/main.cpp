/*
	研究一下矩阵运算和基本的数学方法咯
*/


#include <stdio.h>
#include <stdlib.h>
#include <GL/freeglut.h>

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
GLbyte *gltLoadTGA(const char *szFileName, GLint *iWidth, GLint *iHeight, GLint *iComponents, GLenum *eFormat);
#pragma pack(1)
typedef struct
{
	GLbyte	identsize;              // Size of ID field that follows header (0)
	GLbyte	colorMapType;           // 0 = None, 1 = paletted
	GLbyte	imageType;              // 0 = none, 1 = indexed, 2 = rgb, 3 = grey, +8=rle
	unsigned short	colorMapStart;          // First colour map entry
	unsigned short	colorMapLength;         // Number of colors
	unsigned char 	colorMapBits;   // bits per palette entry
	unsigned short	xstart;                 // image x origin
	unsigned short	ystart;                 // image y origin
	unsigned short	width;                  // width in pixels
	unsigned short	height;                 // height in pixels
	GLbyte	bits;                   // bits per pixel (8 16, 24, 32)
	GLbyte	descriptor;             // image descriptor
} TGAHEADER;
#pragma pack(8)

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

void SetTexture()
{

	int width, height, component;
	GLenum format;
	GLbyte* pbuffer = gltLoadTGA("stone.tga", &width, &height, &component, &format);
	glTexImage2D(GL_TEXTURE_2D, 0, component, width, height, 
		0, format, GL_UNSIGNED_BYTE, pbuffer);
	free(pbuffer);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);	//调用这个之后，就支持了glColor，让贴图颜色和绘制颜色相混合
	glEnable(GL_TEXTURE_2D);
}
void RenderTexture()
{
	//glShadeModel(GL_FLAT);
	glClear(GL_COLOR_BUFFER_BIT);

	glColor3ub(255, 255, 0);
	// glColor3ub(255, 255, 0);
	glBegin(GL_TRIANGLES);

		glNormal3f(0, 0, 1);
		glTexCoord2f(0, 0);
		glVertex3d(0, 50, -50);

		//glColor3ub(0, 255, 0);
		glTexCoord2f(0, 1);
		glVertex3d(-50, 0, -50);

		//glColor3ub(0, 0, 255);
		glTexCoord2f(1, 1);
		glVertex3d(50, 0, -50);
			
	glEnd();
}
static const size_t CONS_TEXTURE_CNT = 2;
GLuint Textures[CONS_TEXTURE_CNT] = {0};
static const char* const Images[CONS_TEXTURE_CNT] = {"stone.tga", "wood.tga"};
void SetTexture2()
{

	glEnable(GL_TEXTURE_2D);
	glGenTextures(CONS_TEXTURE_CNT, Textures);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);	//调用这个之后，就支持了glColor，让贴图颜色和绘制颜色相混合

	for(size_t i=0; i<CONS_TEXTURE_CNT; i++)
	{	
		bool istex = glIsTexture(Textures[i]) > 0;
		glBindTexture(GL_TEXTURE_2D, Textures[i]);

		int width, height, component;
		GLenum format;
		GLbyte* pbuffer = gltLoadTGA(Images[i], &width, &height, &component, &format);
		glTexImage2D(GL_TEXTURE_2D, 0, component, width, height, 
			0, format, GL_UNSIGNED_BYTE, pbuffer);
		free(pbuffer);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		
		// mipmap相关的
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
		//gluBuild2DMipmaps();
	}
}
void RenderTexture2()
{
	//glBindTexture(GL_TEXTURE_2D, Textures[0]);
	glBindTexture(GL_TEXTURE_2D, Textures[1]);
	//glShadeModel(GL_FLAT);
	glClear(GL_COLOR_BUFFER_BIT);

	glColor3ub(255, 255, 0);
	// glColor3ub(255, 255, 0);
	glBegin(GL_TRIANGLES);

	glNormal3f(0, 0, 1);
	glTexCoord2f(0, 0);
	glVertex3d(0, 50, -50);

	//glColor3ub(0, 255, 0);
	glTexCoord2f(0, 1);
	glVertex3d(-50, 0, -50);

	//glColor3ub(0, 0, 255);
	glTexCoord2f(1, 1);
	glVertex3d(50, 0, -50);

	glEnd();
}



void RenderScene()
{
	glClear(GL_COLOR_BUFFER_BIT);

	//RenderCube1();
	//RenderCube2();
	RenderTexture2();

	glFlush();
}

void SetRenderCondition()
{
	glClearColor(0, 0, 0, 1);


	//glEnable(GL_LIGHTING);
	//float ambientLight[] = {1, 0, 0, 1};
	//glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientLight);

#if light0
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, ambientLight);
	glLightfv(GL_LIGHT0, GL_SPECULAR, ambientLight);
	glEnable(GL_LIGHT0);
#endif
	SetTexture2();

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


GLbyte *gltLoadTGA(const char *szFileName, GLint *iWidth, GLint *iHeight, GLint *iComponents, GLenum *eFormat)
{
	FILE *pFile;			// File pointer
	TGAHEADER tgaHeader;		// TGA file header
	unsigned long lImageSize;		// Size in bytes of image
	short sDepth;			// Pixel depth;
	GLbyte	*pBits = NULL;          // Pointer to bits

	// Default/Failed values
	*iWidth = 0;
	*iHeight = 0;
	*eFormat = GL_BGR_EXT;
	*iComponents = GL_RGB8;

	// Attempt to open the file
	pFile = fopen(szFileName, "rb");
	if(pFile == NULL)
		return NULL;

	// Read in header (binary)
	fread(&tgaHeader, 18/* sizeof(TGAHEADER)*/, 1, pFile);

	// Do byte swap for big vs little endian
#ifdef __APPLE__
	LITTLE_ENDIAN_WORD(&tgaHeader.colorMapStart);
	LITTLE_ENDIAN_WORD(&tgaHeader.colorMapLength);
	LITTLE_ENDIAN_WORD(&tgaHeader.xstart);
	LITTLE_ENDIAN_WORD(&tgaHeader.ystart);
	LITTLE_ENDIAN_WORD(&tgaHeader.width);
	LITTLE_ENDIAN_WORD(&tgaHeader.height);
#endif


	// Get width, height, and depth of texture
	*iWidth = tgaHeader.width;
	*iHeight = tgaHeader.height;
	sDepth = tgaHeader.bits / 8;

	// Put some validity checks here. Very simply, I only understand
	// or care about 8, 24, or 32 bit targa's.
	if(tgaHeader.bits != 8 && tgaHeader.bits != 24 && tgaHeader.bits != 32)
		return NULL;

	// Calculate size of image buffer
	lImageSize = tgaHeader.width * tgaHeader.height * sDepth;

	// Allocate memory and check for success
	pBits = (GLbyte*)malloc(lImageSize * sizeof(GLbyte));
	if(pBits == NULL)
		return NULL;

	// Read in the bits
	// Check for read error. This should catch RLE or other 
	// weird formats that I don't want to recognize
	if(fread(pBits, lImageSize, 1, pFile) != 1)
	{
		free(pBits);
		return NULL;
	}

	// Set OpenGL format expected
	switch(sDepth)
	{
	case 3:     // Most likely case
		*eFormat = GL_BGR_EXT;
		*iComponents = GL_RGB8;
		break;
	case 4:
		*eFormat = GL_BGRA_EXT;
		*iComponents = GL_RGBA8;
		break;
	case 1:
		*eFormat = GL_LUMINANCE;
		*iComponents = GL_LUMINANCE8;
		break;
	};


	// Done with File
	fclose(pFile);

	// Return pointer to image data
	return pBits;
}


