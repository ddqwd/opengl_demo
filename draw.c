#include "piglit-util-gl.h"

PIGLIT_GL_TEST_CONFIG_BEGIN

	config.supports_gl_compat_version = 10;

	config.window_width=1200;
	config.window_height = 1130;
	config.window_visual = PIGLIT_GL_VISUAL_RGBA | PIGLIT_GL_VISUAL_DOUBLE;
//	config.khr_no_error_support = PIGLIT_NO_ERRORS;

PIGLIT_GL_TEST_CONFIG_END


const char * vertexShaderSource= 
"#version 450\n"
"layout(location = 0) in vec4 pos;\n"
"void main() {\n"
"gl_Position = pos;\n"
"}\n";


const char*  fs=
"#version 330\n"
"out vec4 outputColor;\n"
"void main()\n"
"{\n"
"    outputColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);\n"
"}";

enum piglit_result 
piglit_display(void)
{

	glDrawArrays(GL_TRIANGLES,0,3);	

	glFlush();
	piglit_present_results();
	return PIGLIT_PASS;
}




enum piglit_result 
piglit_display11(void)
{

	glClear(GL_COLOR_BUFFER_BIT);
//static GLfloat vertices[12] = {225.0, 175.0, 0.0,
//				225.0, 225.0, 0.0,
//				175.0, 175.0, 0.0,
//				175.0, 225.0, 0.0};
static const float vertices[] = {
    0.75f, 0.75f, 0.0f, 1.0f,
    0.75f, -0.75f, 0.0f, 1.0f,
    -0.75f, -0.75f, 0.0f, 1.0f,
};
	GLuint vbo;
//	GLuint vao;
//	glGenVertexArrays(1,&vao);
//	glBindVertexArrays(vao);
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices),vertices, GL_STATIC_DRAW);
	
	glVertexAttribPointer(0,4,GL_FLOAT,GL_FALSE,0, (void*)0);
	glEnableVertexAttribArray(0);
	GLuint vertexShader;

	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader,1, (const GLchar**) &vertexShaderSource, NULL);
	glCompileShader(vertexShader);


	GLint stat;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &stat);

	if (!stat) {
		GLint maxLength =0;
		glGetShaderiv(vertexShader,GL_INFO_LOG_LENGTH,&maxLength);


		GLchar err[2048];

		glGetShaderInfoLog(vertexShader, maxLength,&maxLength, &err[0]);

		printf("err = %s\n", err);
		printf("error compiling vertex shader\n");
		exit(1);
	}

	GLuint fragShader;
	fragShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragShader, 1, (const GLchar**) &fs, NULL);
	glCompileShader(fragShader);

	glGetShaderiv(fragShader, GL_COMPILE_STATUS, &stat);
	if (!stat) {
		GLint maxLen;
		glGetShaderiv(fragShader, GL_INFO_LOG_LENGTH,&maxLen);

		GLchar err[2048];
		glGetShaderInfoLog(fragShader, maxLen, &maxLen, err);
		printf("error compiling fragShader %s \n", err);	
		exit(1);
	}


	GLuint program;
	program = glCreateProgram();

	glAttachShader(program, vertexShader);
	glAttachShader(program, fragShader);
	glLinkProgram(program);

	GLuint success;
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success) {
		printf("error link");	
		exit(1);
	}
	glUseProgram(program);


	glDrawArrays(GL_TRIANGLES,0,3);	

	glFlush();
	piglit_present_results();
	return PIGLIT_PASS;
}



void piglit_init(int argc, char **argv)
{
	
	//piglit_ortho_projection(piglit_width, piglit_height, GL_FALSE);

	glClearColor(0,0,0,0);
	piglit_display11();
	/* nop */
}
