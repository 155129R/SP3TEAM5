#ifndef SCENELEVEL04_H
#define SCENELEVEL04_H

#include "SceneBase.h"

class SceneLevel04 : public SceneBase
{
public:
	SceneLevel04();
	~SceneLevel04();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

	void initSceneObjects();

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

private:
	float rotateAngle;
	int yOffset;

	std::vector<Vector3> gravePos;

	int lightningRand;
	bool lightning;

	int random[128];
	float randompos_x[128];
	float randompos_z[128];

	float boss_hp_ratio = 0.0f;

	string Dialogue[2];
	float Dialogue_Timer = 1.0f;
	float Dialogue_Timer2 = 1.0f;
	int Dialogue_Selection = 0;
	bool boss_bool = false;
};

#endif