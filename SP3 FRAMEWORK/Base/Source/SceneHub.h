#ifndef SCENEHUB_H
#define SCENEHUB_H

#include "SceneBase.h"
#include "Sound.h"

class SceneHub : public SceneBase
{
public:
	SceneHub();
	~SceneHub();

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
	void RenderShop();

	void UpdateParticle(double dt);
	ParticleObject* GetParticles(void);
	void RenderParticle(ParticleObject* particle);

	void RenderPassGPass();
	void RenderPassMain();
	void RenderWorld();

private:
	float rotateAngle;
	float shopkeeperAngle;
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

	bool showBuy;
	bool showSell;
};

#endif