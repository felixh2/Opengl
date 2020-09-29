
#include <string>
#include <vector>
#include <stdio.h>
#include <algorithm>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <GL/freeglut.h>
#include <glm/gtc/type_ptr.hpp>
#include"Shader.h"
#include "ResourceManager.h"

#define AngleToRad(x)  (3.141/180.0)*x
#define WIDTH 800
#define HEIGHT 600
#define NUM_VERTICES 36					//each face has 2 triangles

GLint windowWidth = 800;
GLint windowHeight = 600;
glm::vec4 vertex_data[NUM_VERTICES]; 
glm::vec4 color_data[NUM_VERTICES];
Shader shader;
float thetas[3];

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
	glm::vec4(-1.0f, -1.0f, 1.0f, 1.0f),
	glm::vec4(-1.0f, 1.0f, 1.0f, 1.0f),
	glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
	glm::vec4(1.0f, -1.0f, 1.0f, 1.0f),
	glm::vec4(-1.0f, -1.0f, -1.0f, 1.0f),
	glm::vec4(-1.0f, 1.0f, -1.0f, 1.0f),
	glm::vec4(1.0f, 1.0f, -1.0f, 1.0f),
	glm::vec4(1.0f, -1.0f, -1.0f, 1.0f)
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

	// rotate about z:
	if (key== 'z')
		thetas[2] = thetas[2] + 10.f;
	if (key == 'x')
		thetas[2] = thetas[2] - 10.f;

	// rotate around x
	if (key == 'w')
		thetas[0] = thetas[0] + 10.f;
	if (key == 's')
		thetas[0] = thetas[0] - 10.f;

	// rotate around y
	if (key == 'd')
		thetas[1] = thetas[1] + 10.f;
	if (key == 'a')
		thetas[1] = thetas[1] - 10.f;

	glutPostRedisplay();
}

void InitializeProgram()
{
	shader = ResourceManager::LoadShader("Shaders/vertexShader.vs", "Shaders/fragmentShader.vs", nullptr, "Shader");
}


glm::mat4 rotateX(float theta)
{
	glm::mat4 rotationMatrix = glm::mat4(1.0);
	rotationMatrix[1] = glm::vec4(0.0, cos(glm::radians(theta)), sin(glm::radians(theta)), 0.0);
	rotationMatrix[2] = glm::vec4(0.0, -sin(glm::radians(theta)), cos(glm::radians(theta)), 0.0);
	return rotationMatrix;
}

glm::mat4 rotateZ(float theta)
{
	glm::mat4 rotationMatrix = glm::mat4(1.0);
	rotationMatrix[0] = glm::vec4(cos(glm::radians(theta)), sin(glm::radians(theta)), 0.0, 0.0);
	rotationMatrix[1] = glm::vec4(-sin(glm::radians(theta)), cos(glm::radians(theta)), 0.0, 0.0);
	return rotationMatrix;
}

glm::mat4 rotateY(float theta)
{
	glm::mat4 rotationMatrix = glm::mat4(1.0);
	rotationMatrix[0] = glm::vec4(cos(glm::radians(theta)),0.0, -sin(glm::radians(theta)), 0.0);
	rotationMatrix[2] = glm::vec4(sin(glm::radians(theta)), 0.0, cos(glm::radians(theta)), 0.0);
	return rotationMatrix;
}

void init()
{
	// Function callbacks
	glutMouseFunc(myMouse);
	glutKeyboardFunc(myKeyboard);

	// Program specific functions
	colorCube();
	glm::mat4 scaleMat = glm::mat4(0.2);
	scaleMat[3][3] = 1.0f;




	// Other
	InitializeProgram();
	shader.Use();						
	

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

	GLuint loc = glGetAttribLocation(shader.Use(), "vPosition");
	glEnableVertexAttribArray(loc);
	glVertexAttribPointer(loc, 4, GL_FLOAT, GL_FALSE, 0, 0);

	GLuint color = glGetAttribLocation(shader.Use(), "vColor");
	glEnableVertexAttribArray(color);
	glVertexAttribPointer(color, 4, GL_FLOAT, GL_FALSE, 0, (void*)(sizeof(vertex_data)) );
	
	GLuint scaleMatLoc = glGetUniformLocation(shader.Use(), "scale");
	glUniformMatrix4fv(scaleMatLoc, 1, GL_FALSE, glm::value_ptr(scaleMat) );
	//glVertexAttribPointer(scaleMatLoc,1,GL_mat4)

	glEnable(GL_DEPTH_TEST);
	glClearColor(.0, 1.0, 1.0, 1.0);

	//glUseProgram(0);
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

	GLuint rotXMatLoc = glGetUniformLocation(shader.Use(), "rotationX");
	glUniformMatrix4fv(rotXMatLoc, 1, GL_FALSE, glm::value_ptr(rotateX(thetas[0])));

	GLuint rotYMatLoc = glGetUniformLocation(shader.Use(), "rotationY");
	glUniformMatrix4fv(rotYMatLoc, 1, GL_FALSE, glm::value_ptr(rotateY(thetas[1])));

	GLuint rotZMatLoc = glGetUniformLocation(shader.Use(), "rotationZ");
	glUniformMatrix4fv(rotZMatLoc, 1, GL_FALSE, glm::value_ptr(rotateZ(thetas[2])));

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