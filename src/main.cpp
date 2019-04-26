#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <opencv2/opencv.hpp>
#include <LearnGL.h>

#include<iostream>

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}
int main() {
	
#if 1
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	
	glViewport(0, 0, 800, 600);

	//¶ÁÈ¡Í¼Æ¬
	cv::Mat img_1 = cv::imread(IMG_PATH"img1.jpg");
	cv::Mat img_2 = cv::imread(IMG_PATH"img2.jpg");
	if (img_1.channels() == 3) {
		cv::cvtColor(img_1, img_1, cv::COLOR_BGR2RGBA);
		cv::cvtColor(img_2, img_2, cv::COLOR_BGR2RGBA);
	}
	else if (img_1.channels() == 4) {
		cv::cvtColor(img_1, img_1, cv::COLOR_BGRA2RGBA);
		cv::cvtColor(img_2, img_2, cv::COLOR_BGRA2RGBA);
	}
	//cv::imshow("img_1", img_1);
	//cv::imshow("img_2", img_2);
	//cv::waitKey(0);

	LearnGL* learnGL = new LearnGL();
	learnGL->Init(img_1.data, img_1.cols, img_1.rows, img_1.channels(),
				img_2.data, img_2.cols, img_2.rows, img_2.channels());


	while (!glfwWindowShouldClose(window))
	{
		// input
		// -----
		processInput(window);

		// render
		// ------
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//main
		learnGL->LearnGL_Main(img_1.cols, img_1.rows);

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();
#endif 
	return 0;
}