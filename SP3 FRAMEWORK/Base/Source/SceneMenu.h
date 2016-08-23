#ifndef SCENEMENU_H
#define SCENEMENU_H

#include "SceneBase.h"

class SceneMenu : public SceneBase
{
public:
	SceneMenu();
	~SceneMenu();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

	void RenderGround();
	void RenderSkyplane();
	void RenderTerrain();
	void RenderEnvironment(bool Light);
	void RenderSprite();
	void RenderHUD();
	void RenderLight();
	void RenderTombstone(bool Light);
	void RenderFence(bool Light);
	void UpdateParticle(double dt);
	ParticleObject* GetParticles(void);
	void RenderParticle(ParticleObject* particle);

	void RenderPassGPass();
	void RenderPassMain();
	void RenderWorld();

private:
	float rotateAngle;

	//Toggle Axis
	bool Axis;
	float Axis_Wait;

	//Toggle FlashLight
	bool Flashlight = false;
	float Flashlight_Wait = 0.0f;

	//Toggle Fog
	bool FogEffect;
	bool Switch;

	std::vector<Vector3> gravePos;
	std::vector<Vector3> pocongPos;

	float cameraMove;
	bool checkCameraMove;
};

#endif