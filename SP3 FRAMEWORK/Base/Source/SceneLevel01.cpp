#include "SceneLevel01.h"
#include "GL\glew.h"

#include "shader.hpp"
#include "MeshBuilder.h"
#include "Application.h"
#include "Utility.h"
#include "LoadTGA.h"
#include <sstream>
#include "LoadHmap.h"
#include "Bullet.h"

SceneLevel01::SceneLevel01()
{
}

SceneLevel01::~SceneLevel01()
{
}

static const Vector3 TERRAINSIZE(4000.0f, 200.0f, 4000.0f);

void SceneLevel01::Init()
{
	SceneBase::Init();

	//Random my random randomly using srand
	srand(time(NULL));

	// Projection matrix : 45� Field of View, 4:3 ratio, display range : 0.1 unit <-> 1000 units
	Mtx44 perspective;
	perspective.SetToPerspective(45.0f, 4.0f / 3.0f, 0.1f, 10000.0f);
	//perspective.SetToOrtho(-80, 80, -60, 60, -1000, 1000);
	projectionStack.LoadMatrix(perspective);

	rotateAngle = 0;

	m_particlesCount = 0;
	MAX_PARTICLE = 1000;
	m_gravity.Set(0, -9.8f, 0);

	//Shadow stuff
	m_lightDepthFBO.Init(4096, 4096);

	bLightEnabled = true;

	Axis = false;
	Axis_Wait = 0.0f;

	FogEffect = false;
	Switch = false;

	weapontype = 1;
}

void SceneLevel01::Update(double dt)
{
	SceneBase::Update(dt);

	UpdateParticle(dt);

	//UpdatePlayer(dt);
	//bullet->UpdateShoot(dt);
	

	camera.Terrain = TERRAINSIZE.y * ReadHeightMap(m_heightMap, camera.position.x / TERRAINSIZE.x, camera.position.z / TERRAINSIZE.z);
	camera.Update(dt);


	if (Flashlight)
	{
		Vector3 view = (camera.target - camera.position).Normalized();
		lights[1].position.Set(camera.position.x, camera.position.y, camera.position.z);
		lights[1].spotDirection.Set(-view.x, -view.y, -view.z);
	}

	//TOGGLE AXIS
	if (Application::IsKeyPressed('X') && Axis_Wait >= 0.5f)
	{
		Axis_Wait = 0.0f;
		if (Axis == false)
		{
			Axis = true;
		}
		else if (Axis == true)
		{
			Axis = false;
		}
	}
	else
	{
		Axis_Wait += dt;
	}

	if (Application::IsKeyPressed('F') && Flashlight_Wait >= 0.5f)
	{
		Flashlight_Wait = 0.0f;
		if (Flashlight == false)
		{
			lights[1].power = 8.0f;
			glUniform1f(m_parameters[U_LIGHT1_POWER], lights[1].power);
			Flashlight = true;
		}
		else if (Flashlight == true)
		{
			lights[1].power = 0.0f;
			glUniform1f(m_parameters[U_LIGHT1_POWER], lights[1].power);
			Flashlight = false;
		}
	}
	else
	{
		Flashlight_Wait += dt;
	}

	//TOGGLE FogEffect
	if (Application::IsKeyPressed('Z'))
	{
		Switch = true;
	}
	if (Switch)
	{
		if (FogEffect == true)
		{
			if (FogAmount > 1500)
			{
				FogAmount -= 3000 * dt;
				glUniform1f(m_parameters[U_FOG_END], FogAmount);
			}
			else
			{
				FogEffect = false;
				Switch = false;
			}
		}
		if (FogEffect == false)
		{
			if (FogAmount < 6000)
			{
				FogAmount += 3000 * dt;
				glUniform1f(m_parameters[U_FOG_END], FogAmount);
			}
			else
			{
				FogEffect = true;
				Switch = false;
			}
		}
	}

	rotateAngle += (float)(1 * dt);


	fps = (float)(1.f / dt);
}

//void SceneLevel01::UpdateBullet(double dt)
//{
//	for (vector<Bullet*>::iterator it = Bullet::bulletList.begin(); it != Bullet::bulletList.end();){
//		if ((*it)->deleteBullet == true){
//			delete *it;
//			it = Bullet::bulletList.erase(it);
//		}
//		else{
//			(*it)->Update(dt);
//			it++;
//		}
//	}
//}

void SceneLevel01::UpdateParticle(double dt)
{
	if (m_particlesCount < MAX_PARTICLE)
	{
		ParticleObject* particle = GetParticles();
		particle->type = PARTICLEOBJECT_TYPE::P_WATER;
		particle->pos.Set(Math::RandFloatMinMax(-1700.0f, 1700.0f), 1200.0f, Math::RandFloatMinMax(-1700.0f, 1700.0f));
		particle->vel.Set(0, -10, 0);
		particle->scale.Set(10, 10, 10);
		particle->rotateSpeed = Math::RandFloatMinMax(20.0f, 40.0f);
	}
	for (auto it : particleList)
	{
		ParticleObject* particle = (ParticleObject*)it;
		if (particle->active)
		{
			if (particle->type == PARTICLEOBJECT_TYPE::P_WATER)
			{
				particle->vel += m_gravity *(float)dt * 10.0f;
				particle->pos += particle->vel *(float)dt * 10.0f;
			}
			if (particle->pos.y < (ReadHeightMap(m_heightMap, particle->pos.x / TERRAINSIZE.x, particle->pos.z / TERRAINSIZE.z)  * TERRAINSIZE.y) - 50)
			{
				particle->active = false;
				m_particlesCount--;
			}
		}
	}
}
//void SceneLevel01::UpdateWeaponType(double dt)
//{
//	switch (Singleton::getInstance()->player->GetWeaponType())
//	{
//	case 1:
//		weapontype = 1;
//		break;
//	case 2:
//		weapontype = 2;
//		break;
//	default:
//		break;
//	}
//}

ParticleObject* SceneLevel01::GetParticles(void)
{
	for (auto it : particleList)
	{
		ParticleObject* particle = (ParticleObject*)it;
		if (!particle->active)
		{
			particle->active = true;
			m_particlesCount++;
			return particle;
		}
	}
	for (unsigned i = 0; i <= 10; ++i)
	{
		ParticleObject* particle = new ParticleObject(PARTICLEOBJECT_TYPE::P_WATER);
		particleList.push_back(particle);
	}

	ParticleObject* particle = particleList.back();
	particle->active = true;
	m_particlesCount++;

	return particle;
}

void SceneLevel01::RenderGround()
{
	modelStack.PushMatrix();
	modelStack.Rotate(-90, 1, 0, 0);
	modelStack.Translate(0, 0, -20);
	modelStack.Rotate(-90, 0, 0, 1);
	modelStack.Scale(100.0f, 100.0f, 100.0f);

	for (int x = 0; x<10; x++)
	{
		for (int z = 0; z<10; z++)
		{
			modelStack.PushMatrix();
			modelStack.Translate(x - 5.0f, z - 5.0f, 0.0f);
			if (((x * 9 + z) % 2) == 0)
				RenderMesh(meshList[GEO_GRASS_DARKGREEN], false);
			else
				RenderMesh(meshList[GEO_GRASS_LIGHTGREEN], false);
			modelStack.PopMatrix();
		}
	}
	modelStack.PopMatrix();
}

void SceneLevel01::RenderSkyplane()
{
	modelStack.PushMatrix();
	modelStack.Translate(camera.position.x, 1800, camera.position.z);
	modelStack.Rotate(rotateAngle, 0, 1, 0);
	RenderMesh(meshList[SKYPLANE], false);
	modelStack.PopMatrix();
}

void SceneLevel01::RenderTerrain()
{
	modelStack.PushMatrix();
	modelStack.Translate(1450, -50, 0);
	modelStack.Scale(TERRAINSIZE.x, TERRAINSIZE.y, TERRAINSIZE.z);
	RenderMesh(meshList[LEVEL01_TERRAIN], true);
	modelStack.PopMatrix();
}

void SceneLevel01::RenderLevel(bool Light)
{
	modelStack.PushMatrix();
	modelStack.Translate(-689, -180 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), 10);
	modelStack.Scale(1, 1, 1);
	RenderMeshOutlined(meshList[STAIRS], Light);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-550, -50 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), 190);
	modelStack.Scale(1, 1, 1);
	modelStack.Rotate(180, 0, 1, 0);
	RenderMeshOutlined(meshList[STAIRS], Light);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-400, -50 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), 97);
	modelStack.Scale(1, 1.2, 1);
	RenderMeshOutlined(meshList[INDOORGATE], Light);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(600, -50 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), 97);
	modelStack.Scale(1, 1, 1);
	RenderMeshOutlined(meshList[TOILETBOWL], Light);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(400, 20 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), 97);
	modelStack.Scale(1, 1, 1);
	RenderMeshOutlined(meshList[CHANDELIER], Light);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(850, 20 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), 97);
	modelStack.Scale(1, 1, 1);
	RenderMeshOutlined(meshList[CHANDELIER], Light);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(1300, 20 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), 97);
	modelStack.Scale(1, 1, 1);
	RenderMeshOutlined(meshList[CHANDELIER], Light);
	modelStack.PopMatrix();

	//whole level
	modelStack.PushMatrix();
	modelStack.Translate(-150, 0 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), 0);
	modelStack.Scale(20, 20, 20);
	RenderMeshOutlined(meshList[LEVEL01], Light);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(500, 60 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), 100);
	modelStack.Scale(1050, 1, 305);
	modelStack.Rotate(90, 1, 0, 0);
	RenderMesh(meshList[CEILING], Light);
	modelStack.PopMatrix();
}

void SceneLevel01::RenderRoomObjects(bool Light)
{

	//[Left] first room (front doors)
	modelStack.PushMatrix();
	modelStack.Translate(400, -50 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), -203);
	modelStack.Scale(4, 4, 4);
	RenderMeshOutlined(meshList[DOOR], Light);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(400, -50 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), -212);
	modelStack.Scale(4, 4, 4);
	RenderMeshOutlined(meshList[DOOR], Light);
	modelStack.PopMatrix();

	//[Left] second room (front doors)
	modelStack.PushMatrix();
	modelStack.Translate(1300, -50 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), -203);
	modelStack.Scale(4, 4, 4);
	RenderMeshOutlined(meshList[DOOR], Light);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(1300, -50 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), -212);
	modelStack.Scale(4, 4, 4);
	RenderMeshOutlined(meshList[DOOR], Light);
	modelStack.PopMatrix();

	//[Right] first room (front doors)
	modelStack.PushMatrix();
	modelStack.Translate(400, -50 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), 404);
	modelStack.Scale(4, 4, 4);
	RenderMeshOutlined(meshList[DOOR], Light);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(400, -50 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), 413);
	modelStack.Scale(4, 4, 4);
	RenderMeshOutlined(meshList[DOOR], Light);
	modelStack.PopMatrix();

	//[Right] second room (front doors)
	modelStack.PushMatrix();
	modelStack.Translate(1300, -50 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), 404);
	modelStack.Scale(4, 4, 4);
	RenderMeshOutlined(meshList[DOOR], Light);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(1300, -50 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), 413);
	modelStack.Scale(4, 4, 4);
	RenderMeshOutlined(meshList[DOOR], Light);
	modelStack.PopMatrix();

	//[Left] first room
	modelStack.PushMatrix();
	modelStack.Translate(463, -50 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), -656);
	modelStack.Scale(4, 3.5, 4);
	modelStack.Rotate(-75, 0, 1, 0);
	RenderMeshOutlined(meshList[DOOR], Light);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(500, -50 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), -390);
	modelStack.Scale(4, 3.5, 4);
	modelStack.Rotate(105, 0, 1, 0);
	RenderMeshOutlined(meshList[DOOR], Light);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(590, -50 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), -600);
	modelStack.Scale(1, 1, 1);
	RenderMesh(meshList[TABLE], Light);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(590, -50 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), -610);
	modelStack.Scale(1, 1, 1);
	RenderMeshOutlined(meshList[CHAIR], Light);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(590, -50 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), -590);
	modelStack.Scale(1, 1, 1);
	modelStack.Rotate(180, 0, 1, 0);
	RenderMeshOutlined(meshList[CHAIR], Light);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(120, -50 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), -420);
	modelStack.Scale(1.2, 1, 1);
	RenderMeshOutlined(meshList[BLOCKAGE], Light);
	modelStack.PopMatrix();

	//[Left] Second room
	modelStack.PushMatrix();
	modelStack.Translate(1195, -50 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), -659);
	modelStack.Scale(4, 3.5, 4);
	modelStack.Rotate(-75, 0, 1, 0);
	RenderMeshOutlined(meshList[DOOR], Light);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(1220, -50 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), -659);
	modelStack.Scale(4, 3.5, 4);
	modelStack.Rotate(-105, 0, 1, 0);
	RenderMeshOutlined(meshList[DOOR], Light);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(1093, -50 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), -720);
	modelStack.Scale(1, 1, 1);
	RenderMesh(meshList[TABLE], Light);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(1090, -50 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), -730);
	modelStack.Scale(1, 1, 1);
	RenderMeshOutlined(meshList[CHAIR], Light);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(1090, -50 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), -710);
	modelStack.Scale(1, 1, 1);
	modelStack.Rotate(180, 0, 1, 0);
	RenderMeshOutlined(meshList[CHAIR], Light);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(1410, -50 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), -485);
	modelStack.Scale(1.2, 1, 1);
	modelStack.Rotate(90, 0, 1, 0);
	RenderMeshOutlined(meshList[BLOCKAGE], Light);
	modelStack.PopMatrix();


	//[Right] first room
	modelStack.PushMatrix();
	modelStack.Translate(410, -50 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), 720);
	modelStack.Scale(4, 3.5, 4);
	modelStack.Rotate(-75, 0, 1, 0);
	RenderMeshOutlined(meshList[DOOR], Light);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(500, -50 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), -390);
	modelStack.Scale(4, 3.5, 4);
	modelStack.Rotate(105, 0, 1, 0);
	RenderMeshOutlined(meshList[DOOR], Light);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(500, -50 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), 640);
	modelStack.Scale(1, 1, 1);
	RenderMesh(meshList[TABLE], Light);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(500, -50 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), 630);
	modelStack.Scale(1, 1, 1);
	RenderMeshOutlined(meshList[CHAIR], Light);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(500, -50 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), 650);
	modelStack.Scale(1, 1, 1);
	modelStack.Rotate(180, 0, 1, 0);
	RenderMeshOutlined(meshList[CHAIR], Light);
	modelStack.PopMatrix();


	//[Right] second room
	modelStack.PushMatrix();
	modelStack.Translate(1190, -50 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), 830);
	modelStack.Scale(4, 3.5, 4);
	modelStack.Rotate(-15, 0, 1, 0);
	RenderMeshOutlined(meshList[DOOR], Light);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(1400, -50 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), 865);
	modelStack.Scale(4, 3.5, 4);
	modelStack.Rotate(105, 0, 1, 0);
	RenderMeshOutlined(meshList[DOOR], Light);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(1480, -50 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), 835);
	modelStack.Scale(4, 3.5, 4);
	modelStack.Rotate(195, 0, 1, 0);
	RenderMeshOutlined(meshList[DOOR], Light);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(1400, -50 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), 640);
	modelStack.Scale(1, 1, 1);
	RenderMesh(meshList[TABLE], Light);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(1400, -50 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), 630);
	modelStack.Scale(1, 1, 1);
	RenderMeshOutlined(meshList[CHAIR], Light);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(1400, -50 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), 650);
	modelStack.Scale(1, 1, 1);
	modelStack.Rotate(180, 0, 1, 0);
	RenderMeshOutlined(meshList[CHAIR], Light);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(1020, -50 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), 630);
	modelStack.Scale(1.2, 1, 1);
	RenderMeshOutlined(meshList[BLOCKAGE], Light);
	modelStack.PopMatrix();
	//weapon
	/*modelStack.PushMatrix();
	modelStack.Translate(401, 0 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), -500);
	modelStack.Scale(1, 1, 1);
	RenderMesh(meshList[PISTOL], Light);
	modelStack.PopMatrix();*/

}

void SceneLevel01::RenderHUD()
{
}

void SceneLevel01::RenderSprite()
{
}

void SceneLevel01::RenderParticle(ParticleObject* particle)
{
	switch (particle->type)
	{

	default:
		break;
	}
}

void SceneLevel01::RenderLight()
{
	if (lights[0].type == Light::LIGHT_DIRECTIONAL)
	{
		Vector3 lightDir(lights[0].position.x, lights[0].position.y, lights[0].position.z);
		Vector3 lightDirection_cameraspace = viewStack.Top() * lightDir;
		glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, &lightDirection_cameraspace.x);
	}
	else if (lights[0].type == Light::LIGHT_SPOT)
	{
		Position lightPosition_cameraspace = viewStack.Top() * lights[0].position;
		glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, &lightPosition_cameraspace.x);
		Vector3 spotDirection_cameraspace = viewStack.Top() * lights[0].spotDirection;
		glUniform3fv(m_parameters[U_LIGHT0_SPOTDIRECTION], 1, &spotDirection_cameraspace.x);
	}
	else
	{
		Position lightPosition_cameraspace = viewStack.Top() * lights[0].position;
		glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, &lightPosition_cameraspace.x);
	}
	if (lights[1].type == Light::LIGHT_DIRECTIONAL)
	{
		Vector3 lightDir(lights[1].position.x, lights[1].position.y, lights[1].position.z);
		Vector3 lightDirection_cameraspace = viewStack.Top() * lightDir;
		glUniform3fv(m_parameters[U_LIGHT1_POSITION], 1, &lightDirection_cameraspace.x);
	}
	else if (lights[1].type == Light::LIGHT_SPOT)
	{
		Position lightPosition_cameraspace = viewStack.Top() * lights[1].position;
		glUniform3fv(m_parameters[U_LIGHT1_POSITION], 1, &lightPosition_cameraspace.x);
		Vector3 spotDirection_cameraspace = viewStack.Top() * lights[1].spotDirection;
		glUniform3fv(m_parameters[U_LIGHT1_SPOTDIRECTION], 1, &spotDirection_cameraspace.x);
	}
	else
	{
		Position lightPosition_cameraspace = viewStack.Top() * lights[1].position;
		glUniform3fv(m_parameters[U_LIGHT1_POSITION], 1, &lightPosition_cameraspace.x);
	}
}

void SceneLevel01::RenderWorld()
{
	//glUniform1f(m_parameters[U_FOG_ENABLE], 1);
	//RenderBullets(true);
	RenderSkyplane();
	RenderTerrain();
	RenderLevel(false);
	RenderRoomObjects(false);
	RenderBullets(true);
	//RenderSprite();
	//glUniform1f(m_parameters[U_FOG_ENABLE], 0);
}

void SceneLevel01::RenderPassGPass()
{
	m_renderPass = RENDER_PASS_PRE;

	m_lightDepthFBO.BindForWriting();

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glClear(GL_DEPTH_BUFFER_BIT);

	glUseProgram(m_gPassShaderID);

	//these matrices define shadows from light position/direction
	if (lights[0].type == Light::LIGHT_DIRECTIONAL)
		m_lightDepthProj.SetToOrtho(-2000, 2000, -2000, 2000, -3000, 3000);
	else
		m_lightDepthProj.SetToPerspective(90, 1.f, 0.1, 20);

	m_lightDepthView.SetToLookAt(lights[0].position.x, lights[0].position.y, lights[0].position.z, 0, 0, 0, 0, 1, 0);

	RenderWorld();
}

void SceneLevel01::RenderPassMain()
{
	m_renderPass = RENDER_PASS_MAIN;

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, Application::GetWindowWidth(), Application::GetWindowHeight());

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(m_programID);

	//pass light depth texture
	m_lightDepthFBO.BindForReading(GL_TEXTURE8);
	glUniform1i(m_parameters[U_SHADOW_MAP], 8);

	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	Mtx44 perspective;
	perspective.SetToPerspective(45.0f, 4.0f / 3.0f, 0.1f, 10000.0f);
	projectionStack.LoadMatrix(perspective);

	// Camera matrix
	viewStack.LoadIdentity();
	viewStack.LookAt(
		camera.position.x, camera.position.y, camera.position.z,
		camera.target.x, camera.target.y, camera.target.z,
		camera.up.x, camera.up.y, camera.up.z
		);
	// Model matrix : an identity matrix (model will be at the origin)
	modelStack.LoadIdentity();

	if (lights[0].type == Light::LIGHT_DIRECTIONAL)
	{
		Vector3 lightDir(lights[0].position.x, lights[0].position.y, lights[0].position.z);
		Vector3 lightDirection_cameraspace = viewStack.Top() * lightDir;
		glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, &lightDirection_cameraspace.x);
	}

	if (lights[1].type == Light::LIGHT_SPOT)
	{
		Position lightPosition_cameraspace = viewStack.Top() * lights[1].position;
		glUniform3fv(m_parameters[U_LIGHT1_POSITION], 1, &lightPosition_cameraspace.x);
		Vector3 spotDirection_cameraspace = viewStack.Top() * lights[1].spotDirection;
		glUniform3fv(m_parameters[U_LIGHT1_SPOTDIRECTION], 1, &spotDirection_cameraspace.x);
	}


	//render shapes
	if (Axis == true)
	{
		modelStack.PushMatrix();
		modelStack.Scale(1000, 1000, 1000);
		RenderMesh(meshList[GEO_AXES], false);
		modelStack.PopMatrix();
	}

	//Render objects
	RenderLight();

	//Depth quad
	//viewStack.PushMatrix();
	//viewStack.LoadIdentity();
	//modelStack.PushMatrix();
	//modelStack.Translate(-0.4, -0.25, -1);
	//modelStack.Scale(0.25, 0.25, 0.25);
	//RenderMesh(meshList[GEO_LIGHT_DEPTH_QUAD], false);
	//modelStack.PopMatrix();
	//viewStack.PopMatrix();

	// Render the crosshair
	RenderMeshIn2D(meshList[GEO_CROSSHAIR], false, 2.0f);

	RenderWorld();

	//On screen text
	std::ostringstream ss;
	ss.precision(5);
	ss << "FPS: " << fps;
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 2, 3);

}

void SceneLevel01::Render()
{
	//PRE RENDER PASS
	RenderPassGPass();

	//MAIN RENDER PASS
	RenderPassMain();
}

void SceneLevel01::Exit()
{
	SceneBase::Exit();
}