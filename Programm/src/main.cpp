#include <iostream>
				      
#define GLEW_STATIC  
#include <GL/glew.h>
#include <GLFW/glfw3.h>

/*---------GLM---------*/
#include <glm/glm.hpp>
#include <glm/ext.hpp>

using namespace glm;
/*-------------------*/

#include "graphics/Shader.h"
#include "graphics/texture.h"
#include "window/Window.h"
#include "window/Events.h"
#include "window/camera.h"
#include "loaders/png_loading.h"


int WIDTH = 1280; //������ ������
int HEIGHT = 720; //������ ������

float vertices[] = { // ������ ��������� ������
	//x    y     z     u     v
   -1.0f,-1.0f, 0.0f, 0.0f, 0.0f,  // NEW
	1.0f,-1.0f, 0.0f, 1.0f, 0.0f,
   -1.0f, 1.0f, 0.0f, 0.0f, 1.0f,

	1.0f,-1.0f, 0.0f, 1.0f, 0.0f,
	1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
   -1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
};

int main() {
	Window::initialize(WIDTH, HEIGHT, "MineCraft C++"); // ������ ������� ����
	Events::initialize(); // ������ ������� �������

	glClearColor(0.6f, 0.62f, 0.65f, 1); // ���� ���� RGBA

	Shader* shader = load_shader("res/frag.glsl", "res/vert.glsl"); // �������� ������������ � ���������� �������
	if(shader == nullptr) {
		std::cerr << "failed to load shader" << std::endl;
		Window::terminate(); //�������� ����
		return 1;
	}

	Texture* texture = load_texture("res/img.png");
	if (texture == nullptr) {
		std::cerr << "failed to load texture" << std::endl;
		delete shader;
		Window::terminate(); //�������� ����
		return 1;
	}





	/*�������� VAO � VBO
		
		VBO - �������� OpenGL, ������� ��������� ��������� � ������ GPU(����������) ������������ ������ (���������� ������, �����, �������)

		VAO - "������" � ������� �������� ���������� � ���, ����� ����� VBO ������������ � ��� ��� ������ ����� ����������������

	*/ 
	GLuint VAO, VBO; 
	
	glGenVertexArrays(1, &VAO); // �������� ������� ��������� ��������
	glGenBuffers(1, &VBO);	// �������� ���������� �������

	glBindVertexArray(VAO); // ����������� VAO ���, ������ �� ����� ���-�� ������ ��� � ���� VAO�
	glBindBuffer(GL_ARRAY_BUFFER, VBO); // ��������� ������ � ������ ���������� ���, ������ �� ����� ���-�� ������ ��� � ���� VBO�
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // ��������� ������ � �������� ������ � ��. ���������: 1) ����� ���������� | 2) ������ ������� � ������ | 3) ������ ������ | 4) 
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat))); // ��������� ������� �� ���� ����� ������ ��� ������� ��������� (Vertex attribute array).
																									     // ���������: 1) ������ ���������� ������� | 
																										// 2) ������ ���������� �������� (� ��������) ���-�� ��������� (3 - ���������� ������)
																									   // 3) ��� ������ | 4) ����� �� ������������� ��������������� ��� ��� ��� ��������������� ��� ��� �� ��������� | 
																									  // 5) ����� ���� ��������� ������, ����� ���� ������ �� ���� ������� � ������
																									 // 6) �� ������� ������� ��������� ������� ������������ ������ � ������
	glEnableVertexAttribArray(0); // �������� ��������� ��������� ������� (vertex attribute array)

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));   ///  NEW
	glEnableVertexAttribArray(1);																			///  NEW

	glBindVertexArray(0); // ����������� VAO �� ����� ����������

	glClearColor(0.6f, 0.62f, 0.65f, 1);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	Camera* camera = new Camera(vec3(0, 0, 1), radians(90.0f));
	mat4 model(1.0f);
	model = translate(model, vec3(0.5f, 0, 0));

	float lastTime = glfwGetTime();
	float delta = 0.0f;

	float camX = 0.0f;
	float camY = 0.0f;

	float speed = 5;

	/*=============�������� ���� ����====================*/
	while (!Window::isShouldClose()) { 

		float currentTime = glfwGetTime();
		delta = currentTime - lastTime;
		lastTime = currentTime;

		//Events::pullEvents();
		if (Events::jpressed(GLFW_KEY_ESCAPE)) {
			Window::setShouldClose(true); 
		}
		if (Events::jclicked(GLFW_MOUSE_BUTTON_1)) {
			glClearColor(0.8f, 0.4f, 0.2f, 1);
		}

		if (Events::jpressed(GLFW_KEY_TAB)) {
			Events::toogleCursor();
		}

		if (Events::pressed(GLFW_KEY_W)) {
			camera->position += camera->front * delta * speed;
		}
		if (Events::pressed(GLFW_KEY_S)) {
			camera->position -= camera->front * delta * speed;
		}
		if (Events::pressed(GLFW_KEY_D)) {
			camera->position += camera->right * delta * speed;
		}
		if (Events::pressed(GLFW_KEY_A)) {
			camera->position -= camera->right * delta * speed;
		}

		if (Events::_cursor_locked) {
			camY += -Events::deltaY / Window::height * 2;
			camX += -Events::deltaX / Window::height * 2;

			if (camY < -radians(89.0f)) {
				camY = -radians(89.0f);
			}
			if (camY > radians(89.0f)) {
				camY = radians(89.0f);
			}

			camera->rotation = mat4(1.0f);
			camera->rotate(camY, camX, 0);
		}

		glClear(GL_COLOR_BUFFER_BIT);	// ��������� ����� ����

		
		
		/*------��������� ������-------*/
		shader->use(); // ������ �������
		shader->uniformMatrix("model", model);
		shader->uniformMatrix("projview", camera->getProjection()*camera->getView());
		texture->bind(); // ����������� ��������
		glBindVertexArray(VAO); // ������ VAO
		glDrawArrays(GL_TRIANGLES, 0, 6); // ��������� ����������, ���������: 1) ��� ������������ (��� ���������) | 2) � ����� ������� ������ �������� | 3) ���-�� ������, ������� ���� ����������
		glBindVertexArray(0); // ����������� VAO �� ����� ����������
		/*----����� ��������� ������--*/

		Window::swapBuffers();
		Events::pullEvents();
	}
	/*=========����� ��������� ����� ����==============*/

	delete shader; // �������� �������� � ������� ������
	delete texture; // �������� ������� � ������� ������
	glDeleteBuffers(1, &VBO); // �������� ������ � ������� ������
	glDeleteVertexArrays(1, &VAO); // �������� ������� � ������� ������
	Window::terminate(); // �������� ����
	return 0;
}