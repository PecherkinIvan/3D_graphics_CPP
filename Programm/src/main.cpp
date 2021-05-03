#include <iostream>
				      
#define GLEW_STATIC  
#include <GL/glew.h>
#include <GLFW/glfw3.h>

/*---------GLM---------*/
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace glm;
/*-------------------*/

#include "graphics/Shader.h"
#include "graphics/texture.h"
#include "graphics/Mesh.h"
#include "graphics/voxel_renderer.h"
#include "window/Window.h"
#include "window/Events.h"
#include "window/camera.h"
#include "loaders/png_loading.h"
#include "voxels/voxel.h"
#include "voxels/Chunk.h"
#include "voxels/Chunks.h"

int WIDTH = 1280; //Ширина экрана
int HEIGHT = 720; //Высота экрана

float vertices[] = {
	// x    y
   -0.01f,-0.01f,
	0.01f, 0.01f,

   -0.01f, 0.01f,
	0.01f,-0.01f,
};

int attrs[] = {
		2,  0 //null terminator
};

int main() {
	Window::initialize(WIDTH, HEIGHT, "MineCraft C++"); // Запуск системы окна
	Events::initialize(); // Запуск системы ивентов

	glClearColor(0.6f, 0.62f, 0.65f, 1); // Цвет фона RGBA

	Shader* shader = load_shader("res/frag.glsl", "res/vert.glsl"); // Загрузка фрагментного и вершинного шейдера
	if(shader == nullptr) {
		std::cerr << "failed to load shader" << std::endl;
		Window::terminate(); //Закрытие окна
		return 1;
	}

	Shader* crosshairShader = load_shader("res/crosshairvert.glsl", "res/crosshairfrag.glsl");
	if (crosshairShader == nullptr) {
		std::cerr << "failed to load crosshair shader" << std::endl;
		Window::terminate();
		return 1;
	}

	Texture* texture = load_texture("res/block.png");
	if (texture == nullptr) {
		std::cerr << "failed to load texture" << std::endl;
		delete shader;
		Window::terminate(); //Закрытие окна
		return 1;
	}

	Chunks* chunks = new  Chunks(8, 1, 8);
	Mesh** meshes = new Mesh * [chunks->volume];
	for (size_t i = 0; i < chunks->volume; i++)
		meshes[i] = nullptr;
	VoxelRenderer renderer(1024 * 1024 * 8);
	

	

	glClearColor(0.6f, 0.62f, 0.65f, 1);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);	

	Mesh* crosshair = new Mesh(vertices, 4, attrs);
	Camera* camera = new Camera(vec3(0, 0, 1), radians(90.0f));

	float lastTime = glfwGetTime();
	float delta = 0.0f;

	float camX = 0.0f;
	float camY = 0.0f;

	float speed = 5;

	/*=============Основной цикл игры====================*/
	while (!Window::isShouldClose()) { 

		float currentTime = glfwGetTime();
		delta = currentTime - lastTime;
		lastTime = currentTime;

		//Events::pullEvents();
		if (Events::jpressed(GLFW_KEY_ESCAPE)) {
			Window::setShouldClose(true); 
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


		{
			vec3 end;
			vec3 norm;
			vec3 iend;
			voxel* vox = chunks->rayCast(camera->position, camera->front, 10.0f, end, norm, iend);
			if (vox != nullptr) {
				if (Events::jclicked(GLFW_MOUSE_BUTTON_1)) {
					chunks->set((int)iend.x, (int)iend.y, (int)iend.z, 0);
				}
				if (Events::jclicked(GLFW_MOUSE_BUTTON_2)) {
					chunks->set((int)(iend.x) + (int)(norm.x), (int)(iend.y) + (int)(norm.y), (int)(iend.z) + (int)(norm.z), 1);
				}
			}
		}


		Chunk* closes[27];
		for (size_t i = 0; i < chunks->volume; i++) {
			Chunk* chunk = chunks->chunks[i];
			if (!chunk->modified)
				continue;
			chunk->modified = false;
			if (meshes[i] != nullptr)
				delete meshes[i];

			for (int i = 0; i < 27; i++)
				closes[i] = nullptr;
			for (size_t j = 0; j < chunks->volume; j++) {
				Chunk* other = chunks->chunks[j];

				int ox = other->x - chunk->x;
				int oy = other->y - chunk->y;
				int oz = other->z - chunk->z;

				if (abs(ox) > 1 || abs(oy) > 1 || abs(oz) > 1)
					continue;

				ox += 1;
				oy += 1;
				oz += 1;
				closes[(oy * 3 + oz) * 3 + ox] = other;
			}
			Mesh* mesh = renderer.render(chunk, (const Chunk**)closes, true);
			meshes[i] = mesh;
		}

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Изменение цвета фона

		/*------Отрисовка вершин-------*/
		shader->use(); // Запуск шейдера		
		shader->uniformMatrix("projview", camera->getProjection()*camera->getView());
		texture->bind(); // Превязываем текстуру
		mat4 model(1.0f);
		for (size_t i = 0; i < chunks->volume; i++){
			Chunk* chunk = chunks->chunks[i];
			Mesh* mesh = meshes[i];
			model = glm::translate(mat4(1.0f), vec3(chunk->x*CHUNK_W+0.5f, chunk->y*CHUNK_H+0.5f, chunk->z*CHUNK_D+0.5f));
			shader->uniformMatrix("model", model);
			mesh->draw(GL_TRIANGLES);
		}

		crosshairShader->use();
		crosshair->draw(GL_LINES);
		
		Window::swapBuffers();
		Events::pullEvents();
	}
	/*=========Конец основного цикла игры==============*/

	delete shader; // Удаление шейдеров и очистка памяти
	delete texture; // Удаление текстур и очистка памяти
	delete chunks;
	delete crosshair;
	delete crosshairShader;


	Window::terminate(); // Закрытие окна
	return 0;
}