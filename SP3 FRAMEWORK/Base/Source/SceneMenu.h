#ifndef SCENEMENU_H
#define SCENEMENU_H

#include "SceneBase.h"
#include "MeshBuilder.h"

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
	void RenderSkyplane(bool inverted = false);
	void RenderTerrain();
	void RenderEnvironment(bool Light, bool inverted = false);
	void RenderSprite();
	void RenderHUD();
	void RenderLight();

	void UpdateParticle(double dt);
	ParticleObject* GetParticles(void);
	void RenderParticle(ParticleObject* particle);

	void UpdateBullet(double dt);

	void RenderPassGPass();
	void RenderPassMain();
	void RenderWorld();
	void RenderReflection();

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
	Vector3 Tree[400];
	int Tree_Type[400];
	Vector3 Bush[400];
};

#endif