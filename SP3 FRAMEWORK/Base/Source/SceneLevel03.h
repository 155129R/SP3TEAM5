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

	void initSceneObjects();

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

	//Forest
	Vector3 Tree[300];
	int Tree_Type[300];
	Vector3 Bush[300];

	float Degree;

	AABBObject* Change;
	SpriteAnimation* Water;

	float distance = 0.0f;

	string Dialogue[2];
	float Dialogue_Timer = 1.0f;
	int Dialogue_Selection = 0;
};

#endif