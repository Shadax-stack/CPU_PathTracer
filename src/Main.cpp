#include <glfw/glfw3.h>
#include <stdio.h>
#include <time.h>
#include <memory>

#include "Texture.h"
#include "Color.h"
#include "PresentUtil.h"
#include "TimeUtil.h"

#include "Scene.h"
#include "Renderer.h"
#include <thread>

//#define FULLSCREEN

uint32_t Width = 640;
uint32_t Height = 480;

Camera* ViewCamera;

glm::vec2 LastCursorPosition;
const float Sensitivity = 0.001f;

void MouseCallback(GLFWwindow* Window, double X, double Y) {
	glm::vec2 CurrentCursorPosition = glm::vec2(X, Y);

	glm::vec2 DeltaPosition = LastCursorPosition -  CurrentCursorPosition;
	DeltaPosition.y = -DeltaPosition.y;

	DeltaPosition *= Sensitivity;

	ViewCamera->AddRotation(glm::vec3(DeltaPosition, 0.0f));

	LastCursorPosition = CurrentCursorPosition;
}

GLFWwindow* Window;
Renderer Renderer;
float DeltaTime = 0.0f;

void RendererThread(void) {
	while (!glfwWindowShouldClose(Window)) {

	}
}

int main() {

	printf("Hello world!\n");

	// Use fixed function stuff since that is the simplest 
	glfwInitHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwInitHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	glfwInit();

	GLFWmonitor* Monitor = nullptr;

#ifdef FULLSCREEN
	Monitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* VideoMode = glfwGetVideoMode(Monitor);
	Width = VideoMode->width;
	Height = VideoMode->height;
#endif

	Window = glfwCreateWindow(Width, Height, "CPU Path Tracer", Monitor, nullptr);

	glfwMakeContextCurrent(Window);
	InitOpenGL();


	Renderer.Init(Width, Height);
	ViewCamera = Renderer.GetCamera();
	ViewCamera->SetPosition(glm::vec3(3.0f, 0.0f, -3.0f));
	ViewCamera->AddRotation(glm::vec3(glm::radians(45.0f), 0.0f, 0.0f));

	LastCursorPosition = glm::vec2(Width, Height) / 2.0f;
	glfwSetInputMode(Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(Window, MouseCallback);

	Mesh Object;

	Object.LoadMesh("res/Suzanne.obj");

	Renderer.AddMesh(&Object);
	
	std::thread RendererThread(RendererThread);

	while (!glfwWindowShouldClose(Window)) {
		Timer Tmr;

		Tmr.Begin();

		ViewCamera->Update();
		if (glfwGetKey(Window, GLFW_KEY_W)) {
			ViewCamera->Move(DeltaTime);
		}
		else if (glfwGetKey(Window, GLFW_KEY_S)) {
			ViewCamera->Move(-DeltaTime);
		}

		Renderer.Begin(); 

		Renderer.UpdateScreen();

		Renderer.End();

		Tmr.End();
		DeltaTime = (float)Tmr.Delta;
		Tmr.DebugTime();

		glfwPollEvents();
		glfwSwapBuffers(Window);

	}

	RendererThread.join();

	Renderer.Free();

	glfwDestroyWindow(Window);

	return 0;
}





/*
void ProcessImageForCurrentThread(size_t YStart) {
	// DbgTmr Tmr;

	Ray ViewRay;
	ViewRay.Direction = glm::vec3(0.0f, 0.0f, -1.0f);

	glm::vec2 ViewportSize;// = glm::vec2(BackBuffer.GetWidth(), BackBuffer.GetHeight());

	size_t YEnd = YStart + RowsPerThread;
	for (size_t Y = YStart; Y < YEnd; Y++) {
		for (size_t X = 0; X < BackBuffer.GetWidth(); X++) {

			HitInfo Hit;

			glm::vec2 Position = glm::vec2(X, Y) / ViewportSize;
			Position *= 2.0f;
			Position -= 1.0f;

			ViewRay.Origin = glm::vec3(Position, 0.0f);

			if (World.Intersect(ViewRay, Hit)) {
				BackBuffer.Write(X, Y, Hit.ComputeInterpolatedVertex().Normal);
			}
		}
	}
}
*/