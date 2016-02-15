//Jeff Chastine
#include <Windows.h>
#include <GL\glew.h>
#include <GL\freeglut.h>
#include <iostream>

#define BUFFER_OFFSET(i) ((char *) NULL + (i)))
GLuint shaderProgramID;
GLuint vao = 0;
GLuint vbo;
GLuint positionID, colorID;

using namespace std;

#pragma region SHADER_FUNCTIONS
static char* readFile(const char* filename) {
	// Open the file
	FILE* fp = fopen(filename, "r");
	// Move the file pointer to the end of the file and determine the length
	fseek(fp, 0, SEEK_END);
	long file_length = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	char* contents = new char[file_length + 1];
	// zero out memory
	for (int i = 0; i < file_length + 1; i++) {
		contents[i] = 0;
	}
	// Here's the actual read
	fread(contents, 1, file_length, fp);
	// This is how you denote the end of a string in C
	contents[file_length + 1] = '\0';
	fclose(fp);
	return contents;
}

GLuint makeVertexShader(const char* shaderSource) {
	GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShaderID, 1, (const GLchar**)&shaderSource, NULL);
	glCompileShader(vertexShaderID);
	return vertexShaderID;
}

GLuint makeFragmentShader(const char* shaderSource) {
	GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShaderID, 1, (const GLchar**)&shaderSource, NULL);
	glCompileShader(fragmentShaderID);
	return fragmentShaderID;
}

GLuint makeShaderProgram(GLuint vertexShaderID, GLuint fragmentShaderID) {
	GLuint shaderID = glCreateProgram();
	glAttachShader(shaderID, vertexShaderID);
	glAttachShader(shaderID, fragmentShaderID);
	glLinkProgram(shaderID);
	return shaderID;
}
#pragma endregion SHADER_FUNCTIONS

void changeViewPort(int w, int h)
{
	glViewport(0, 0, w, h);
}

void render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glutSwapBuffers();
}

int main(int argc, char* argv[]) {

	// Initialize GLUT
	glutInit(&argc, argv);
	// Set up some memory buffers for our display
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	// Set the window size
	glutInitWindowSize(800, 600);
	// Create the window with the title "Hello,GL"
	glutCreateWindow("Hello, GL");
	// Bind the two functions (above) to respond when necessary
	glutReshapeFunc(changeViewPort);
	glutDisplayFunc(render);

	// Very important!  This initializes the entry points in the OpenGL driver so we can 
	// call all the functions in the API.
	GLenum err = glewInit();
	if (GLEW_OK != err) {
		fprintf(stderr, "GLEW error");
		return 1;
	}

	// Vertices and colors of a triangle
	// Notice, position values are between -1.0f and +1.0f
	GLfloat vertices[] = { -0.5f, -0.5f, 0.0f,			// Lower-left
							0.5f, -0.5f, 0.0f,				// Lower-right
							0.0f, 0.5f, 0.0f };				// Top
	GLfloat colors[] = { 1.0f, 0.0f, 0.0f, 1.0f,
						0.0f, 1.0f, 0.0f, 1.0f,
						0.0f, 0.0f, 1.0f, 1.0f };

	// Make a shader
	char* vertexShaderSource = readFile("vShader.vsh");
	char* fragmentShaderSource = readFile("fShader.fsh");
	GLuint vertShaderID = makeVertexShader(vertexShaderSource);
	GLuint fragShaderID = makeFragmentShader(fragmentShaderSource);
	GLuint shaderProgramID = makeShaderProgram(vertShaderID, fragShaderID);
	glUseProgram(shaderProgramID);
	printf("vertShaderID is %d\n", vertShaderID);
	printf("fragShaderID is %d\n", fragShaderID);
	printf("shaderProgramID is %d\n", shaderProgramID);

	// Create the "remember all"
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	// Create the buffer, but don't load anything yet
	glBufferData(GL_ARRAY_BUFFER, 7 * 3 * sizeof(GLfloat), NULL, GL_STATIC_DRAW);
	// Load the vertex points
	glBufferSubData(GL_ARRAY_BUFFER, 0, 3 * 3 * sizeof(GLfloat), vertices);
	// Load the colors right after that
	glBufferSubData(GL_ARRAY_BUFFER, 3 * 3 * sizeof(GLfloat), 3 * 4 * sizeof(GLfloat), colors);


	// Find the position of the variables in the shader
	positionID = glGetAttribLocation(shaderProgramID, "s_vPosition");
	colorID = glGetAttribLocation(shaderProgramID, "s_vColor");

	glVertexAttribPointer(positionID, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glVertexAttribPointer(colorID, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(3 * 3 * sizeof(GLfloat));
	glUseProgram(shaderProgramID);
	glEnableVertexAttribArray(positionID);
	glEnableVertexAttribArray(colorID);

	// glDeleteProgram(shaderProgramID);
	glutMainLoop(); 
	return 0;
}