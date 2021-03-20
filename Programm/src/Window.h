#ifndef WINDOW_WINDOW_H_
#define WINDOW_WINDOW_H_

class GLFWwindow;

class Window {
	GLFWwindow* window;
public:
	static int initialize(int width, int height, const char* title);
	static void terminate();

};

#endif // !WINDOW_WINDOW_H_