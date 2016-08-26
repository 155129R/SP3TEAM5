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
	void RenderLevel(bool Light);
	void RenderRoomObjects(bool Light);
	void RenderSprite();
	void RenderHUD();
	void RenderLight();

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

	float timer;
	bool timerstart;

	bool d1;
	bool d2;
	bool d3;
	bool d4;
	bool d5;
	bool d6;
	bool d7;
	bool d8;

	AABBObject* door1;
	AABBObject* door2;
	AABBObject* door3;
	AABBObject* door4;
	AABBObject* door5;
	AABBObject* door6;
	AABBObject* door7;
	AABBObject* door8;

	AABBObject* exitPtr;

	AABBObject* hammerPtr;
};

#endif