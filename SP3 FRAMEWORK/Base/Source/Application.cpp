#include "Application.h"

//Include GLEW
#include <GL/glew.h>

//Include GLFW
#include <GLFW/glfw3.h>

//Include the standard C++ headers
#include <stdio.h>
#include <stdlib.h>

GLFWwindow* m_window;
const unsigned char FPS = 60; // FPS of this game
const unsigned int frameTime = 1000 / FPS; // time for each frame
double Application::mouse_last_x = 0.0, Application::mouse_last_y = 0.0, 
	   Application::mouse_current_x = 0.0, Application::mouse_current_y = 0.0,
	   Application::mouse_diff_x = 0.0, Application::mouse_diff_y = 0.0;
double Application::camera_yaw = 0.0, Application::camera_pitch = 0.0;
int m_width = 800;
int m_height = 600;

//Define an error callback
static void error_callback(int error, const char* description)
{
	fputs(description, stderr);
	_fgetchar();
}

//Define the key input callback
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

void resize_callback(GLFWwindow* window, int w, int h)
{
	glViewport(0, 0, w, h);
	m_width = w;
	m_height = h;
}

int Application::GetWindowWidth()
{
	return m_width;
}

int Application::GetWindowHeight()
{
	return m_height;
}

bool Application::IsKeyPressed(unsigned short key)
{
    return ((GetAsyncKeyState(key) & 0x8001) != 0);
}
bool Application::IsMousePressed(unsigned short key) //0 - Left, 1 - Right, 2 - Middle
{
	return glfwGetMouseButton(m_window, key) != 0;
}
void Application::ShowCursor()
{
	glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}
void Application::HideCursor()
{
	glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
}
void Application::GetCursorPos(double *xpos, double *ypos)
{
	glfwGetCursorPos(m_window, xpos, ypos);
}
bool Application::GetMouseUpdate()
{
    glfwGetCursorPos(m_window, &mouse_current_x, &mouse_current_y);

	// Calculate the difference in positions
	mouse_diff_x = mouse_current_x - mouse_last_x;
	mouse_diff_y = mouse_current_y - mouse_last_y;

	//Calculate the yaw and pitch
	camera_yaw = (float) mouse_diff_x * 0.0174555555555556f;// * 3.142f / 180.0f;
	camera_pitch = mouse_diff_y * 0.0174555555555556f;// 3.142f / 180.0f );

	// Do a wraparound if the mouse cursor has gone out of the deadzone
	if ((mouse_current_x < m_window_deadzone) || (mouse_current_x > m_window_width-m_window_deadzone))
	{
		mouse_current_x = m_window_width >> 1;
		glfwSetCursorPos(m_window, mouse_current_x, mouse_current_y);
	}
	if ((mouse_current_y < m_window_deadzone) || (mouse_current_y > m_window_height-m_window_deadzone))
	{
		mouse_current_y = m_window_height >> 1;
		glfwSetCursorPos(m_window, mouse_current_x, mouse_current_y);
	}

	// Store the current position as the last position
	mouse_last_x = mouse_current_x;
	mouse_last_y = mouse_current_y;

    return false;
}
void Application::SetMousePosition(double x, double y)
{
	if (x == 0 && y == 0){
		int sizeX = 0;
		int sizeY = 0;

		glfwGetWindowSize(m_window, &sizeX, &sizeY);

		sizeX = (sizeX / 2) - x;
		sizeY = (sizeY / 2) - y;

		glfwSetCursorPos(m_window, sizeX, sizeY);
	}
	else{
		glfwSetCursorPos(m_window, x, y);
	}
}
Application::Application()
{
}

Application::~Application()
{

}

void Application::Init()
{
	//Set the error callback
	glfwSetErrorCallback(error_callback);

	//Initialize GLFW
	if (!glfwInit())
	{
		exit(EXIT_FAILURE);
	}

	//Set the GLFW window creation hints - these are optional
	glfwWindowHint(GLFW_SAMPLES, 4); //Request 4x antialiasing
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); //Request a specific OpenGL version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); //Request a specific OpenGL version
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //We don't want the old OpenGL 


	//Create a window and create its OpenGL context
	m_window = glfwCreateWindow(m_window_width, m_window_height, "SP3", NULL, NULL);

	//If the window couldn't be created
	if (!m_window)
	{
		fprintf( stderr, "Failed to open GLFW window.\n" );
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	//This function makes the context of the specified window current on the calling thread. 
	glfwMakeContextCurrent(m_window);

	//Sets the key callback
	//glfwSetKeyCallback(m_window, key_callback);
	glfwSetWindowSizeCallback(m_window, resize_callback);

	glewExperimental = true; // Needed for core profile
	//Initialize GLEW
	GLenum err = glewInit();

	//If GLEW hasn't initialized
	if (err != GLEW_OK) 
	{
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
		//return -1;
	}

	//Real time loop stuff
	m_dElapsedTime = 0.0;
	m_dAccumlatedTime_ThreadOne = 0.0;
	m_dAccumlatedTime_ThreadTwo = 0.0;
	m_dAccumlatedTime_ThreadThree = 0.0;
	m_dAccumlatedTime_ThreadFour = 0.0;

	// Initialise scene manager
	sceneManager = new SceneManager();
}

void Application::Run()
{
	//start 1st scene here
	sceneManager->ChangeScene(0);
	//sceneManager->ChangeScene(1);
	//sceneManager->ChangeScene(2);
	//sceneManager->ChangeScene(3);
	//sceneManager->ChangeScene(4);

	m_timer.startTimer();    // Start timer to calculate how long it takes to render this frame
	while (!glfwWindowShouldClose(m_window) && !IsKeyPressed(VK_ESCAPE))
	{
		////////////////////////////
		//    change scene here   //
		////////////////////////////
		if (Singleton::getInstance()->stateCheck)
		{
			Singleton::getInstance()->stateCheck = false;

			if (Singleton::getInstance()->program_state == Singleton::PROGRAM_GAME1)
			{
				sceneManager->ChangeScene(1);
			}
			if (Singleton::getInstance()->program_state == Singleton::PROGRAM_GAME2)
			{
				sceneManager->ChangeScene(2);
			}
			if (Singleton::getInstance()->program_state == Singleton::PROGRAM_GAME3)
			{
				sceneManager->ChangeScene(3);
			}
			if (Singleton::getInstance()->program_state == Singleton::PROGRAM_GAME4)
			{
				sceneManager->ChangeScene(4);
			}
		}

		//Get the elasped time
		m_dElapsedTime = m_timer.getElapsedTime();
		m_dAccumlatedTime_ThreadOne += m_dElapsedTime;
		m_dAccumlatedTime_ThreadTwo += m_dElapsedTime;
		m_dAccumlatedTime_ThreadThree += m_dElapsedTime;
		m_dAccumlatedTime_ThreadFour += m_dElapsedTime;

		//First thread (Scene update)
		if (m_dAccumlatedTime_ThreadOne > 0.016) //60 times every second
		{
			if (Singleton::getInstance()->program_state != Singleton::PROGRAM_MENU && 
				Singleton::getInstance()->showInventory == false)
				GetMouseUpdate();
			sceneManager->Update(m_dElapsedTime);
			m_dAccumlatedTime_ThreadOne = 0.0;
		}

		//Second thread (Keyboard update)
		if (m_dAccumlatedTime_ThreadTwo > 0.001)
		{
			//GetKeyboardUpdate();
			m_dAccumlatedTime_ThreadTwo = 0.0;
		}

		//Third thread (Player update)
		if (m_dAccumlatedTime_ThreadThree > 0.016)
		{
			m_dAccumlatedTime_ThreadThree = 0.0;
		}

		//Fourth thread
		if (m_dAccumlatedTime_ThreadFour > 0.032) //30 times every second
		{
			m_dAccumlatedTime_ThreadFour = 0.0;
		}

		sceneManager->Render();
		//Swap buffers
		glfwSwapBuffers(m_window);
		//Get and organize events, like keyboard and mouse input, window resizing, etc...
		glfwPollEvents();
        m_timer.waitUntil(frameTime);       // Frame rate limiter. Limits each frame to a specified time in ms.   

	} 
	//Check if the ESC key had been pressed or if the window had been closed
	sceneManager->Exit();
	delete sceneManager;
}

void Application::Exit()
{
	//Close OpenGL window and terminate GLFW
	glfwDestroyWindow(m_window);
	//Finalize and clean up GLFW
	glfwTerminate();
}
