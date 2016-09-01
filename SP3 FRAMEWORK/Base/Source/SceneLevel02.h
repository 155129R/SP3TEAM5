#ifndef SCENELEVEL02_H
#define SCENELEVEL02_H

#include "SceneBase.h"
#include "Sound.h"

class SceneLevel02 : public SceneBase
{
public:
	SceneLevel02();
	~SceneLevel02();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

	void initSceneObjects();

	void RenderGround();
	void RenderSkyplane();
	void RenderTerrain();
	void RenderEnvironment(bool Light);
	void RenderOthers(bool Light);
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

	float soundTimer;

	bool openGate;
	int rotateGate;

	bool questToNextScene;
	int distanceLeft;

	AABBObject* keyPtr;
	AABBObject* gatePtr;

	string Dialogue[4];
	float Dialogue_Timer = 1.0f;
	float Dialogue_Timer2 = 1.0f;
	int Dialogue_Selection = 0;
	bool key_bool = false;
};

#endif