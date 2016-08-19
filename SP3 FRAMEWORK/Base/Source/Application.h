#ifndef APPLICATION_H
#define APPLICATION_H

#include "Singleton.h"
#include "timer.h"
#include "Sound.h"
#include "Scene.h"
#include "SceneManager.h"

class Application
{
public:
	static Application& GetInstance()
	{
		static Application app;
		return app;
	}
	void Init();
	void Run();
	void Exit();
	static bool IsKeyPressed(unsigned short key);
	bool GetMouseUpdate();

	//Declare variables to store the last and current mouse position
	static double mouse_last_x, mouse_last_y, mouse_current_x, mouse_current_y, mouse_diff_x, mouse_diff_y;
	static double camera_yaw, camera_pitch;

	static int GetWindowWidth();
	static int GetWindowHeight();

	SceneManager *sceneManager;
private:
	Application();
	~Application();

	// Declare the window width and height as constant integer
	const static int m_window_deadzone = 300;
	const static int m_window_width = 800;
	const static int m_window_height = 600;
	//Declare a window object
	StopWatch m_timer;

	double m_dElapsedTime;
	double m_dAccumlatedTime_ThreadOne;
	double m_dAccumlatedTime_ThreadTwo;
	double m_dAccumlatedTime_ThreadThree;
	double m_dAccumlatedTime_ThreadFour;

	// The handler for the scene
	//SceneLevel01 *scene;
	//SceneLevel02 *scene;
	//SceneLevel03 *scene;
	//SceneLevel04 *scene;

	//SceneShadow *scene;
};

#endif