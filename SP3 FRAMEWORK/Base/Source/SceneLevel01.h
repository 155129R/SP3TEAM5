#ifndef SCENELEVEL01_H
#define SCENELEVEL01_H

#include "SceneBase.h"

class SceneLevel01 : public SceneBase
{
public:
	SceneLevel01();
	~SceneLevel01();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

	void RenderGround();
	void RenderSkyplane();
	void RenderTerrain();
	void RenderCorridor(bool Light);
	void RenderRoom(bool Light);
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
};

#endif