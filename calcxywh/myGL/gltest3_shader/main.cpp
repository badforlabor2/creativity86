/*
	学习shader，听说这个很神奇- -
*/


/*
	基本函数：
	需要包含glee.h，扩展功能
	函数：
	glCreateShader();
	glDeleteShader();
	glShaderSource();
	glCompileShader();
	glGetShaderiv();
	glGetShaderInfoLog();
	glAttachShader();
	glDetachShader();
	glLinkProgram();
	glValidateProgram();
	glUseProgram();
	关键字：
		void
		int
		bool
		float
		bvec2, bvec3, bvec4. bool vector
		ivec2, .... int vector
		vec3, ....  float vector
		max2, max3, max4
		max2x3, max2x4, max3x2, max3x4, max4x2, max4x3. 4x3的矩阵
		sampler1D, sampler2D, sampler3D 一个特殊目的的常量，用于引用特定的1D(2D, 3D)纹理.
		samplerCube
		sampler1DShadow, sampler2DShadow. 一个特殊目的的常量，用于引用一个带阴影比较的深度纹理常量
		const, attribute, uniform, varying, centroid, invariant, in, out, inout

		内置顶点着色器变量
		gl_Color, gl_SecondaryColor, gl_Normal, gl_Vertex, gl_MultiTexCoord, ...

		内置片段着色器变量
		gl_Color, gl_SecondaryColor, ...


*/
#include <stdio.h>
#include <stdlib.h>
#include "../util/GLee.h"
#include <GL/freeglut.h>

bool GetValidateError(GLuint program);
GLchar *LoadShaderText(const char *fileName);

void Alert(const char* msg)
{
	::OutputDebugStringA("[labor] error:");
	::OutputDebugStringA(msg);
	::OutputDebugStringA("\n");
}

#define CHECK_RETURN(exp) if(!(exp)) { Alert(#exp); return; }
#define CHECK_RETURNV(exp, retv) if(!(exp)) { Alert(#exp); return retv; }

void RenderCube1()
{
	// 立方体，先平移在旋转
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glColor3f(1.f, 1.f, 0);
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

	static bool bvalidate = false;
	if(!bvalidate)
	{
		bvalidate = true;
		GLint program = 0;
		glGetIntegerv(GL_CURRENT_PROGRAM, &program);
		glValidateProgram(program);
		if(GetValidateError(program))
		{
			return;
		}
	}

	RenderCube1();
	glFlush();
}
void OpenShader();
void SetRenderCondition()
{
	glClearColor(0, 0, 0, 1);
	glShadeModel(GL_SMOOTH);

	// 构建shader
	OpenShader();
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

bool GetShaderError(GLuint shader)
{
	int result = 1;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
	if(!result)
	{
		char msg[1024] = {0};
		glGetShaderInfoLog(shader, 1023, NULL, msg);
		Alert(msg);
		return true;
	}
	return false;
}
bool GetProgramError(GLuint program)
{
	int result = 0;
	glGetProgramiv(program, GL_LINK_STATUS, &result);
	if(!result)
	{
		char msg[1024] = {0};
		glGetProgramInfoLog(program, 1023, NULL, msg);
		Alert(msg);
		return true;
	}
	return false;
}
bool GetValidateError(GLuint program)
{
	int result = 0;
	glGetProgramiv(program, GL_VALIDATE_STATUS, &result);
	if(!result)
	{
		char msg[1024] = {0};
		glGetProgramInfoLog(program, 1023, NULL, msg);
		Alert(msg);
		return true;
	}
	return false;
}

void OpenShader()
{
	GLuint vertexShader;
	GLuint pixelShader;
	GLuint program;

	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	pixelShader = glCreateShader(GL_FRAGMENT_SHADER);

	CHECK_RETURN(vertexShader != 0);
	CHECK_RETURN(pixelShader != 0);

	GLchar vsString[] = 
		"void main(void)\n"
		"{\n"
		"    // This is our Hello World vertex shader\n"
		"    // notice how comments are preceded by '//'\n"
		"\n"
		"    // normal MVP transform\n"
		"    vec4 clipCoord = gl_ModelViewProjectionMatrix * gl_Vertex;\n"
		"    gl_Position = clipCoord;\n"
		"\n"
		"    // Copy the primary color\n"
		"    gl_FrontColor = gl_Color;\n"
		"\n"
		"    // Calculate NDC\n"
		"    vec4 ndc = vec4(clipCoord.xyz, 0) / clipCoord.w;\n"
		"\n"
		"    // Map from [-1,1] to [0,1] before outputting\n"
		"    gl_FrontSecondaryColor = (ndc * 0.5) + 0.5;\n"
		"}\n";

	GLchar fsString[] = 
		"uniform float flickerFactor;\n"
		"\n"
		"void main(void)\n"
		"{\n"
		"    // Mix primary and secondary colors, 50/50\n"
		"    vec4 temp = mix(gl_Color, vec4(vec3(gl_SecondaryColor), 1.0), 0.5);\n"
		"\n"
		"    // Multiply by flicker factor\n"
		"    gl_FragColor = temp * flickerFactor;\n"
		"}\n";

	//const char* pVsShader = vsString;
	const char* pVsShader = LoadShaderText("fog.vs");
	//const char* pFsShader = fsString;
	const char* pFsShader = LoadShaderText("grayscale.fs");
	glShaderSource(vertexShader, 1, &pVsShader, NULL);
	glShaderSource(pixelShader, 1, &pFsShader, NULL);

	// 编译shader
	glCompileShader(vertexShader);
	if(GetShaderError(vertexShader))
	{
		return;
	}

	glCompileShader(pixelShader);
	if(GetShaderError(pixelShader))
	{
		return;
	}

	// 把shader绑定到对象上
	program = glCreateProgram();

	//if(true)
	//{
	//	glAttachShader(program, vertexShader);
	//}
	//else
	//{
	//	glDetachShader(program, vertexShader);
	//}

	if(true)
	{
		glAttachShader(program, pixelShader);
	}
	else
	{
		glDetachShader(program, pixelShader);
	}

	// 连接对象
	glLinkProgram(program);
	if(GetProgramError(program))
	{
		return;
	}
	// 检查连接是否正常
	glValidateProgram(program);
	if(GetValidateError(program))
	{
		return;
	}

	// 使用shader，相当于glEnable
	glUseProgram(program);

}


// Load shader from disk into a null-terminated string
GLchar *LoadShaderText(const char *fileName)
{
	GLchar *shaderText = NULL;
	GLint shaderLength = 0;
	FILE *fp;

	fp = fopen(fileName, "r");
	if (fp != NULL)
	{
		while (fgetc(fp) != EOF)
		{
			shaderLength++;
		}
		rewind(fp);
		shaderText = (GLchar *)malloc(shaderLength+1);
		if (shaderText != NULL)
		{
			fread(shaderText, 1, shaderLength, fp);
		}
		shaderText[shaderLength] = '\0';
		fclose(fp);
	}

	return shaderText;
}
