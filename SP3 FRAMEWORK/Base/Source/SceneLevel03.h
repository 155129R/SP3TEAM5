#ifndef SCENELEVEL03_H
#define SCENELEVEL03_H

#include "SceneBase.h"

class SceneLevel03 : public SceneBase
{
public:
	SceneLevel03();
	~SceneLevel03();

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

	void RenderPassGPass();
	void RenderPassMain();
	void RenderWorld();
	void RenderReflection();

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

	//Forest
	Vector3 Tree[400];
	int Tree_Type[400];
	Vector3 Bush[400];

	float Degree;

	AABBObject* Change;
	SpriteAnimation* Water;
};

#endif