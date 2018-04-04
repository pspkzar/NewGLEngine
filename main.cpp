#define GLM_FORCE_RADIANS
#include "gl_core_4_5.h"
#include <IL/il.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/ext.hpp>
#include "GLSLShader.h"
#include <iostream>
#include <sstream>

#include "Scene.h"
#include "Camera.h"

#include <libunwind.h>
#include <cxxabi.h>

char * execCommand;

int getFileAndLine(unw_word_t addr, char *file, size_t flen, int *line) {
	static char buf[256];

	// prepare command to be executed
	// our program need to be passed after the -e parameter
	sprintf(buf, "/usr/bin/addr2line -C -e %s -f -i %lx", execCommand, addr);

	FILE* f = popen(buf, "r");

	if (f == NULL) {
		perror(buf);
		return 0;
	}

	// get function name
	fgets(buf, 256, f);

	// get file and line
	fgets(buf, 256, f);

	if (buf[0] != '?') {
		char *p = buf;

		// file name is until ':'
		while (*p != ':') {
			p++;
		}

		*p++ = 0;
		// after file name follows line number
		strcpy(file, buf);
		sscanf(p, "%d", line);
	} else {
		strcpy(file, "unknown");
		*line = -1;
	}

	pclose(f);
	return 1;
}

void printStack() {

	unw_cursor_t cursor;
	unw_context_t uc;
	unw_word_t ip, off;

	char file[256], name[256];
	int line;
	int status;

	unw_getcontext(&uc);
	unw_init_local(&cursor, &uc);
	while (unw_step(&cursor) > 0) {
		unw_get_reg(&cursor, UNW_REG_IP, &ip);

		unw_get_proc_name(&cursor, name, sizeof(name), &off);
		getFileAndLine((long) ip, file, 256, &line);

		if (line >= 0) {
			char *realname;
			realname = abi::__cxa_demangle(name, 0, 0, &status);

			if (realname) {
				//printf("%s: %s, %d\n", realname, file, line);
				std::cerr << realname << ": " << file << ", " << line << std::endl;
				free(realname);
			} else {
				//printf("%s: %s, %d\n", name, file, line);
				std::cerr << name << ": " << file << ", " << line << std::endl;
			}
		}
	}
}

using namespace std;

const int WIDTH = 1920;
const int HEIGHT = 1080;

GLSLShader shader;
Camera cam;
Scene s;
bool msaa=true;

float ftime = 0.f;
float frame_time = 0.f;

#define SPEED 1.f
#define ANG_SPEED 0.002f

std::string getDebugSeverityString(GLenum severity) {
	switch (severity) {
	case GL_DEBUG_SEVERITY_HIGH:
		return "High";
	case GL_DEBUG_SEVERITY_LOW:
		return "Low";
	case GL_DEBUG_SEVERITY_MEDIUM:
		return "Medium";
	case GL_DEBUG_SEVERITY_NOTIFICATION:
		return "Notification";
	}
	return "";
}

std::string getDebugSourceString(GLenum source) {
	switch (source) {
	case GL_DEBUG_SOURCE_API:
		return "API";
	case GL_DEBUG_SOURCE_APPLICATION:
		return "Application";
	case GL_DEBUG_SOURCE_OTHER:
		return "Othrer";
	case GL_DEBUG_SOURCE_SHADER_COMPILER:
		return "Shader Compiler";
	case GL_DEBUG_SOURCE_THIRD_PARTY:
		return "Third Party";
	case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
		return "Window System";
	}
	return "";
}

std::string getDebugTypeString(GLenum type) {
	switch (type) {
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
		return "Deprecated Behaviour";
	case GL_DEBUG_TYPE_ERROR:
		return "Error";
	case GL_DEBUG_TYPE_MARKER:
		return "Marker";
	case GL_DEBUG_TYPE_OTHER:
		return "Other";
	case GL_DEBUG_TYPE_PERFORMANCE:
		return "Performance";
	case GL_DEBUG_TYPE_POP_GROUP:
		return "Pop Group";
	case GL_DEBUG_TYPE_PORTABILITY:
		return "Portability";
	case GL_DEBUG_TYPE_PUSH_GROUP:
		return "Push Group";
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
		return "Undefined Behaviour";
	}
	return "";
}

void glDebugProcess(GLenum source, GLenum type, GLuint id, GLenum severity,
		GLsizei length, const GLchar *message, const void *userParam) {
	std::cerr << "Debug message:" << std::endl;
	std::cerr << "\tSource:" << getDebugSourceString(source) << std::endl;
	std::cerr << "\tType:" << getDebugTypeString(type) << std::endl;
	std::cerr << "\tId:" << id << std::endl;
	std::cerr << "\tSeverity:" << getDebugSeverityString(severity) << std::endl;
	std::cerr << "\tMessage:" << message << std::endl;
	printStack();
}

void OnInit(){
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_MULTISAMPLE);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glClearColor(0,0,0,0);
	Texture::InitDefaultTex();
	string scene_file = "/home/cesar/scenes/sponza_fixed/sponza.obj";

    s.LoadSceneFile(scene_file);

	shader.LoadFromFile(GL_VERTEX_SHADER, "../shaders/shader.vert");
	shader.LoadFromFile(GL_FRAGMENT_SHADER, "../shaders/shader.frag");
	shader.CreateAndLinkProgram();

	cout<<"Initialization successfull"<<endl;
}

void OnShutdown() {
	shader.DeleteShaderProgram();
	
	cout<<"Shutdown successfull"<<endl;
}
void OnResize(int w, int h) {
	glViewport (0, 0, (GLsizei) w, (GLsizei) h);
	cam.setProjectionFrustum(glm::pi<float>()/ 4, float(w) / float(h), 10, 5000);
}

void OnRender() {
	if(msaa){
		glEnable(GL_MULTISAMPLE);
	}
	else{
		glDisable(GL_MULTISAMPLE);
	}
	float new_time = float(glutGet(GLUT_ELAPSED_TIME));
	frame_time = new_time - ftime;
	ftime=new_time;
	
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	glm::mat4 pvm = cam.getProjectionMatrix() * cam.getViewMatrix();
	glm::vec3 eye = cam.getPosition();
	shader.getUniform("PVM",VERTEX_SHADER).Set(pvm);
	glm::mat4 V = cam.getViewMatrix();
	shader.getUniform("V",VERTEX_SHADER).Set(V);
	shader.getUniform("normalM",VERTEX_SHADER).Set(glm::transpose(glm::inverse(glm::mat3(V))));

	shader.getUniform("lightDir", FRAGMENT_SHADER).Set(glm::normalize(glm::vec4(1,1,1,0)));
	shader.getUniform("V",FRAGMENT_SHADER).Set(V);

	s.Draw(shader);

	static int framen = 0;
	static float measure_time = 0.f;
	if (measure_time>1000.f){
		cout << 1000.f*framen/measure_time << endl;
		framen = 0;
		measure_time = 0;
	}
	else{
		framen++;
		measure_time += frame_time;
	}
	glutSwapBuffers();
}

void OnKeyboard(unsigned char key, int x, int y){
	glm::vec3 pos = cam.getPosition();
	glm::vec3 up = cam.getUp();
	glm::vec3 ldir = cam.getLookDirection();

	glm::vec3 right = glm::normalize(glm::cross(ldir, up));
	glm::vec3 tup = glm::normalize(glm::cross(right, ldir));

	switch (key){
	case 'w':
		pos += ldir * frame_time * SPEED;
		break;
	case 's':
		pos -= ldir * frame_time * SPEED;
		break;
	case 'i':
		ldir += tup*frame_time*ANG_SPEED;
		break;
	case 'k':
		ldir -= tup*frame_time*ANG_SPEED;
		break;
	case 'j':
		ldir -= right*frame_time*ANG_SPEED;
		break;
	case 'l':
		ldir += right*frame_time*ANG_SPEED;
		break;
	case 'm':
		msaa=!msaa;
	}
	ldir = glm::normalize(ldir);
	cam.update(pos, up, ldir);
}

int main(int argc, char **argv){
	execCommand = argv[0];
	cout << "Initializing glut" << endl;
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA | GLUT_MULTISAMPLE);
	glutInitContextVersion(4,5);
	glutInitContextFlags(GLUT_CORE_PROFILE);
	glutInitContextProfile(GLUT_FORWARD_COMPATIBLE);
	glutInitWindowSize(WIDTH, HEIGHT);
	glutCreateWindow("Getting started with OpenGL 4.5");

	ogl_LoadFunctions();
	glDebugMessageCallback(glDebugProcess, NULL);
	ilInit();

	cout<<"\tVendor: "<<glGetString (GL_VENDOR)<<endl;
	cout<<"\tRenderer: "<<glGetString (GL_RENDERER)<<endl;
	cout<<"\tVersion: "<<glGetString (GL_VERSION)<<endl;
	cout<<"\tGLSL:"<<glGetString(GL_SHADING_LANGUAGE_VERSION)<<endl;

	OnInit();
	glutCloseFunc(OnShutdown);
	glutDisplayFunc(OnRender);
	glutIdleFunc(OnRender);
	glutReshapeFunc(OnResize);
	glutKeyboardFunc(OnKeyboard);

	glutMainLoop();
	return 0;
}
