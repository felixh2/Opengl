
#include <string>
#include <vector>
#include <stdio.h>
#include <algorithm>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <GL/freeglut.h>

void display();

#define WIDTH 800
#define HEIGHT 600
#define NUM_VERTICES 3

GLint windowWidth = 800;
GLint windowHeight = 600;

const int num_divisions = 2;
const int num_vertices = pow(3, num_divisions+1);
glm::vec2 vertices[NUM_VERTICES];

#define AngleToRad(x)  (3.141/180.0)*x

GLuint vbo;
GLuint vao;
GLuint theProgram;

const std::string strVertexShader(
	"#version 330\n"
	"layout(location = 0) in vec4 position;\n"
	"void main()\n"
	"{\n"
	"   gl_Position = position;\n"
	"}\n"
);

const std::string strFragmentShader(
	"#version 330\n"
	"out vec4 outputColor;\n"
	"void main()\n"
	"{\n"
	"   outputColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);\n"
	"}\n"
);

GLuint CreateShader(GLenum eShaderType, const std::string &strShaderFile)
{
	GLuint shader = glCreateShader(eShaderType);
	const char *strFileData = strShaderFile.c_str();
	glShaderSource(shader, 1, &strFileData, NULL);

	glCompileShader(shader);

	GLint status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE)
	{
		GLint infoLogLength;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);

		GLchar *strInfoLog = new GLchar[infoLogLength + 1];
		glGetShaderInfoLog(shader, infoLogLength, NULL, strInfoLog);

		const char *strShaderType = NULL;
		switch (eShaderType)
		{
		case GL_VERTEX_SHADER: strShaderType = "vertex"; break;
		case GL_GEOMETRY_SHADER: strShaderType = "geometry"; break;
		case GL_FRAGMENT_SHADER: strShaderType = "fragment"; break;
		}

		fprintf(stderr, "Compile failure in %s shader:\n%s\n", strShaderType, strInfoLog);
		delete[] strInfoLog;
	}

	return shader;
}

GLuint CreateProgram(const std::vector<GLuint> &shaderList)
{
	GLuint program = glCreateProgram();

	for (size_t iLoop = 0; iLoop < shaderList.size(); iLoop++)
		glAttachShader(program, shaderList[iLoop]);

	glLinkProgram(program);

	GLint status;
	glGetProgramiv(program, GL_LINK_STATUS, &status);
	if (status == GL_FALSE)
	{
		GLint infoLogLength;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLength);

		GLchar *strInfoLog = new GLchar[infoLogLength + 1];
		glGetProgramInfoLog(program, infoLogLength, NULL, strInfoLog);
		fprintf(stderr, "Linker failure: %s\n", strInfoLog);
		delete[] strInfoLog;
	}

	for (size_t iLoop = 0; iLoop < shaderList.size(); iLoop++)
		glDetachShader(program, shaderList[iLoop]);

	return program;
}

void InitializeProgram()
{
	std::vector<GLuint> shaderList;

	shaderList.push_back(CreateShader(GL_VERTEX_SHADER, strVertexShader));
	shaderList.push_back(CreateShader(GL_FRAGMENT_SHADER, strFragmentShader));

	theProgram = CreateProgram(shaderList);

	std::for_each(shaderList.begin(), shaderList.end(), glDeleteShader);
}

void copyVertexData()
{
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
}

void InitializeVertexBuffer()
{
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
}

void triangle(glm::vec2 a, glm::vec2 b, glm::vec2 c)
{
	static int index = 0;
	vertices[index++] = a;
	vertices[index++] = b;
	vertices[index++] = c;
	
}

void divide_triangle(glm::vec2 a, glm::vec2 b, glm::vec2 c, int k)
{
	if (k > 0)
	{
		glm::vec2 ab = glm::vec2((a + b)*0.5f);
		glm::vec2 ac = glm::vec2((a + c)*0.5f);
		glm::vec2 bc = glm::vec2((b + c)*0.5f);

		divide_triangle(a, ab, ac, k - 1);
		divide_triangle(b, ab, bc, k - 1);
		divide_triangle(c, ac, bc, k - 1);
	}
	else
	{
		triangle(a, b, c);
	}
}

void idle()
{
	glutPostRedisplay();
}

static int index = 0;
void myMouse(int button, int state, int x, int y)
{
	
	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{
		exit(0);
	}

	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		glutIdleFunc(NULL);		
		// we need to convert from the mouse coordinate system (which has the origin on the top left corner) 
		// to clip coordinate system
		vertices[index++] = glm::vec2(((float)x / (WIDTH / 2) - 1.0), ((float)(HEIGHT - y) / (HEIGHT / 2) - 1.0));		
	}

	if (index == 3)
	{
		index = 0;		
		glutIdleFunc(idle);
	}
}

void myKeyboard(unsigned char key, int x, int y)
{
	if (key == 'q' || key == 'Q')
		exit(0);
}

void init()
{
	//divide_triangle(glm::vec2(0.0f, 1.0f), glm::vec2(-1.0f, -1.0f), glm::vec2(1.0f, -1.0f), num_divisions);
	//triangle(glm::vec2(0.0f, 1.0f), glm::vec2(-1.0f, -1.0f), glm::vec2(1.0f, -1.0f));

	InitializeProgram();
	InitializeVertexBuffer();


	glutMouseFunc(myMouse);
	glutKeyboardFunc(myKeyboard);

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

}

void reshape(GLsizei w, GLsizei h)
{
	windowWidth = w;
	windowHeight = h;
	glViewport(0, 0, windowWidth, windowHeight);
}

void rotateTriangle()
{
	float angle =0.1f;

		for (int i = 0; i < 3; i++)
		{
			float x = vertices[i].x*cos(AngleToRad(angle)) - vertices[i].y*sin(AngleToRad(angle));
			float y = vertices[i].x*sin(AngleToRad(angle)) + vertices[i].y*cos(AngleToRad(angle));

			vertices[i].x = x;
			vertices[i].y = y;
		}
	
	

}

void display()
{

	glClear(GL_COLOR_BUFFER_BIT);
	glUseProgram(theProgram);

	rotateTriangle();
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	copyVertexData();
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

	glDrawArrays(GL_TRIANGLES, 0, NUM_VERTICES);

	glDisableVertexAttribArray(0);
	glUseProgram(0);
	
	//glFlush();
	glutSwapBuffers();

}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(WIDTH, HEIGHT);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Simple OpenGL example");
	glewInit();
	glutDisplayFunc(display);
	init();
	glutMainLoop();

}