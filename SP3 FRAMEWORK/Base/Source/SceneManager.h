#include "Scene.h"

class SceneManager
{
public:
	SceneManager();
	~SceneManager();

	Scene* m_scene;

	void ChangeScene(short scene);
	void Update(double dt);
	void Render();
	void Exit();
};