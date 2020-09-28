
#include <string>
#include <vector>
#include <stdio.h>
#include <algorithm>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <GL/freeglut.h>
#include <glm/gtc/type_ptr.hpp>


#define AngleToRad(x)  (3.141/180.0)*x
#define WIDTH 800
#define HEIGHT 600
#define NUM_VERTICES 36					//each face has 2 triangles

GLint windowWidth = 800;
GLint windowHeight = 600;
glm::vec4 vertex_data[NUM_VERTICES]; 
glm::vec4 color_data[NUM_VERTICES];


glm::vec4 cube_colors[8] =
{
	glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),
	glm::vec4(0.0f, 0.0f, 1.0f, 1.0f),
	glm::vec4(0.0f, 1.0f, 0.0f, 1.0f),
	glm::vec4(0.0f, 1.0f, 1.0f, 1.0f),
	glm::vec4(1.0f, 0.0f, 0.0f, 1.0f),
	glm::vec4(1.0f, 0.0f, 1.0f, 1.0f),
	glm::vec4(1.0f, 1.0f, 0.0f, 1.0f),
	glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)
};

glm::vec4 cube_vertices[8] =
{
	glm::vec4(-.5f, -.5f, .5f, 1.0f),
	glm::vec4(-.5f, .5f, .5f, 1.0f),
	glm::vec4(.5f, .5f, .5f, 1.0f),
	glm::vec4(.5f, -.5f, .5f, 1.0f),
	glm::vec4(-.5f, -.5f, -.5f, 1.0f),
	glm::vec4(-.5f, .5f, -.5f, 1.0f),
	glm::vec4(.5f, .5f, -.5f, 1.0f),
	glm::vec4(.5f, -.5f, -.5f, 1.0f)
};

static int index = 0;
void quad(int a, int b, int c, int d)
{
	vertex_data[index] = cube_vertices[a];
	color_data[index] = cube_colors[a];
	index++;
	vertex_data[index] = cube_vertices[b];
	color_data[index] = cube_colors[b];
	index++;
	vertex_data[index] = cube_vertices[c];
	color_data[index] = cube_colors[c];
	index++;
	vertex_data[index] = cube_vertices[a];
	color_data[index] = cube_colors[a];
	index++;
	vertex_data[index] = cube_vertices[c];
	color_data[index] = cube_colors[c];
	index++;
	vertex_data[index] = cube_vertices[d];
	color_data[index] = cube_colors[d];
	index++;


	if (index > 35) index = 0;
}




GLuint theProgram;

const std::string strVertexShader(
	"#version 330\n"
	"in vec4 vPosition;\n"
	"in vec4 vColor;\n"
	"out vec4 color;\n"
	"void main()\n"
	"{\n"
	"   gl_Position = vPosition;\n"
	"	color = vColor;\n"
	"}\n"
);


const std::string strFragmentShader(
	"#version 330\n"
	"out vec4 fragColor;\n"
	"in vec4 color;\n"
	"void main()\n"
	"{\n"
	"   fragColor = color;\n"
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

// see cube.png
void colorCube()
{
	quad(0, 3, 2, 1);
	quad(3, 7, 6, 2);
	quad(4, 5, 6, 7);
	quad(4, 0, 1, 5);
	quad(4, 7, 3, 0);
	quad(2, 6, 5, 1);

}



void idle()
{
	glutPostRedisplay();
}

void myMouse(int button, int state, int x, int y)
{

	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{
		exit(0);
	}

}

void myKeyboard(unsigned char key, int x, int y)
{
	if (key == 'q' || key == 'Q')
		exit(0);
}

void init()
{
	// Function callbacks
	glutMouseFunc(myMouse);
	glutKeyboardFunc(myKeyboard);

	// Program specific functions
	colorCube();

	// Other
	InitializeProgram();
	glUseProgram(theProgram);
	
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_data)+ sizeof(color_data), NULL, GL_STATIC_DRAW);

	// Params:		  target	  offset		size			data
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertex_data), vertex_data);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertex_data), sizeof(color_data), color_data);

	GLuint loc = glGetAttribLocation(theProgram, "vPosition");
	glEnableVertexAttribArray(loc);
	glVertexAttribPointer(loc, 4, GL_FLOAT, GL_FALSE, 0, 0);

	GLuint color = glGetAttribLocation(theProgram, "vColor");
	glEnableVertexAttribArray(color);
	glVertexAttribPointer(color, 4, GL_FLOAT, GL_FALSE, 0, (void*)(sizeof(vertex_data)) );
	
	glEnable(GL_DEPTH_TEST);
	//glClearColor(1.0, 1.0, 1.0, 1.0);

	glUseProgram(0);
}

void reshape(GLsizei w, GLsizei h)
{
	windowWidth = w;
	windowHeight = h;
	glViewport(0, 0, windowWidth, windowHeight);
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glDrawArrays(GL_TRIANGLES, 0, NUM_VERTICES);
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