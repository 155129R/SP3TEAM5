#ifndef SCENESHADOW_H
#define SCENESHADOW_H

#include "SceneBase.h"
#include "MeshBuilder.h"

class SceneShadow : public SceneBase
{
public:
	SceneShadow();
	~SceneShadow();

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

	void UpdateParticle(double dt);
	ParticleObject* GetParticles(void);
	void RenderParticle(ParticleObject* particle);

	void RenderPassGPass();
	void RenderPassMain();
	void RenderWorld();

	void UpdatePlayer(double dt);
	void UpdateFearEffect(double dt);

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

	Color Black;

	//Forest
	Vector3 Tree[120];
	int Tree_Type[120];
	Vector3 Bush[200];
};

#endif