#include "SceneManager.h"
#include "SceneShadow.h"
#include "SceneLevel01.h"
#include "SceneLevel02.h"
#include "SceneLevel03.h"
#include "SceneLevel04.h"
#include "SceneMenu.h"

SceneManager::SceneManager() : m_scene(0)
{
}

SceneManager::~SceneManager()
{
}

void SceneManager::ChangeScene(short scene)
{
	if (m_scene)
	{
		m_scene->Exit();
		delete m_scene;
	}
	switch (scene)
	{
	case 0: m_scene = new SceneMenu();
		break;
	case 1: m_scene = new SceneLevel01();
		break;
	case 2: m_scene = new SceneLevel02();
		break;
	case 3: m_scene = new SceneLevel03();
		break;
	case 4: m_scene = new SceneLevel04();
		break;

	default:
		break;
	}

	if (m_scene)
	{
		m_scene->Init();
	}
}

void SceneManager::Update(double dt)
{
	if (m_scene)
	{
		m_scene->Update(dt);
	}
}

void SceneManager::Render()
{
	if (m_scene)
	{
		m_scene->Render();
	}
}

void SceneManager::Exit()
{
	if (m_scene)
	{
		m_scene->Exit();
		delete m_scene;
	}
}