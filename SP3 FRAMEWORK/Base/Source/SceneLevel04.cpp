#include "SceneLevel04.h"
#include "GL\glew.h"

#include "shader.hpp"
#include "Application.h"
#include "Utility.h"
#include <sstream>

SceneLevel04::SceneLevel04()
{
}

SceneLevel04::~SceneLevel04()
{
}

static const Vector3 TERRAINSIZE(4000.0f, 800.0f, 4000.0f);
void SceneLevel04::Init()
{
	sound.playMusic("Sound/level4bgm.mp3");

	Application::HideCursor();

	SceneBase::Init();
	meshList[TERRAIN_LEVEL04] = MeshBuilder::GenerateTerrain("Terrain", "Image//Terrain//Terrain_Level04.raw", m_heightMap_4, level4_Heights);
	meshList[TERRAIN_LEVEL04]->textureArray[0] = LoadTGA("Image//Graveyard//level4_ground.tga");

	//Level 4 - Graveyard
	meshList[TOMBSTONE] = MeshBuilder::GenerateOBJ("Tombstone", "OBJ//Graveyard//Tombstone.obj");
	meshList[TOMBSTONE]->textureArray[0] = LoadTGA("Image//Graveyard//Tombstone.tga");
	meshList[FENCE] = MeshBuilder::GenerateOBJ("Fence", "OBJ//Graveyard//wooden_fence.obj");
	meshList[FENCE]->textureArray[0] = LoadTGA("Image//Graveyard//wood_1.tga");
	meshList[DEADTREE] = MeshBuilder::GenerateOBJ("DEADTREE", "OBJ//Graveyard//tree.obj");
	meshList[DEADTREE]->textureArray[0] = LoadTGA("Image//Graveyard//deadtree.tga");
	meshList[GEO_TREE_1] = MeshBuilder::GenerateQuad("Thin Tree", Color(0, 0, 0), 1.f);
	meshList[GEO_TREE_1]->textureArray[0] = LoadTGA("Image//Forest//Tree_1.tga");
	meshList[GEO_TREE_2] = MeshBuilder::GenerateQuad("Fat Tree", Color(0, 0, 0), 1.f);
	meshList[GEO_TREE_2]->textureArray[0] = LoadTGA("Image//Forest//Tree_2.tga");
	meshList[GEO_TREE_3] = MeshBuilder::GenerateQuad("Dead Tree", Color(0, 0, 0), 1.f);
	meshList[GEO_TREE_3]->textureArray[0] = LoadTGA("Image//Forest//Dead_Tree.tga");

	terrainHeight = TERRAINSIZE.y;
	Terrainsize = TERRAINSIZE * 0.5f;
	InitPartitioning();

	camera.Init(Vector3(1, 80, 500), Vector3(0, 200, 1), Vector3(0, 1, 0));
	camera.Reset();
	//Random my random randomly using srand

	// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 1000 units
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

	initSceneObjects();
	////////////

	spatialPartitioning = true;
	nightVision = false;
	lightning = false;

	lights[0].power = 0.8f;
	glUniform1f(m_parameters[U_LIGHT0_POWER], lights[0].power);
	lights[0].color = (0.0f, 0.2f, 0.4f);
	glUniform3fv(m_parameters[U_LIGHT0_COLOR], 1, &lights[0].color.r);
	glUniform1f(m_parameters[U_LIGHT0_POWER], lights[0].power);
	fogColor.Set(0.2f, 0.2f, 0.2f);
	glUniform3fv(m_parameters[U_FOG_COLOR], 1, &fogColor.r);

	lightning = true;
	sound.Init();

	for (int i = 0; i < 128; ++i)
	{
		random[i] = Math::RandIntMinMax(1, 3);
	}
	for (int i = 0; i < 128; ++i)
	{
		randompos_x[i] = Math::RandFloatMinMax(-1600, 1600);
	}
	for (int i = 0; i < 128; ++i)
	{
		randompos_z[i] = Math::RandFloatMinMax(-1600, -2000);
	}

	Singleton::getInstance()->boss->reset();
}

void SceneLevel04::Update(double dt)
{
	//std::cout << instance->player->GetFear() << endl;
	//camera.Terrain = TERRAINSIZE.y * ReadHeightMap(m_heightMap_4, camera.position.x / TERRAINSIZE.x, camera.position.z / TERRAINSIZE.z);
	sound.Update(irrklang::vec3df(camera.position.x, camera.position.y, camera.position.z),
		irrklang::vec3df(-camera.view.x, camera.view.y, -camera.view.z));

	camera.Update(dt);

	SceneBase::Update(dt);

	UpdateBoss(dt);

	if (instance->boss->getHP() > instance->boss->getMAX() * 0.5)
	{
		boss_hp_ratio = (instance->boss->getHP() - (instance->boss->getMAX() * 0.5)) / (instance->boss->getMAX() * 0.5);
	}
	else
	{
		boss_hp_ratio = (instance->boss->getHP() / (instance->boss->getMAX() * 0.5));
	}

	UpdateParticle(dt);

	UpdateHitboxes(dt);

	camera.Terrain = getHeightofTerrain(TERRAINSIZE.x, level4_Heights);
	
	if (Flashlight)
	{
		Vector3 view = (camera.target - camera.position).Normalized();
		lights[1].position.Set(camera.position.x, camera.position.y, camera.position.z);
		lights[1].spotDirection.Set(-view.x, -view.y, -view.z);
	}
	//if (Application::IsKeyPressed(VK_RETURN))
	//	lightning = true;

	{
		lightningRand = Math::RandIntMinMax(1, 100);
		if (lightningRand == 25)
		{
			lightning = true;
		}
	}
	if (lightning)
	{
		lights[0].power = 2.f;
		lights[0].color = (0.0f, 0.8f, 0.5f);
		glUniform3fv(m_parameters[U_LIGHT0_COLOR], 1, &lights[0].color.r);
		glUniform1f(m_parameters[U_LIGHT0_POWER], lights[0].power);
		if (instance->player->GetFear() == 1)
		{
			fogColor.Set(1.f, 1.f, 1.f);
			glUniform3fv(m_parameters[U_FOG_COLOR], 1, &fogColor.r);
		}
		lightning = false;
	}
	else
	{
		if (lights[0].power > 0.5f)
			lights[0].power -= 2.f * (float)dt;
		lights[0].color = (0.f, 0.2f, 0.4f);
		glUniform3fv(m_parameters[U_LIGHT0_COLOR], 1, &lights[0].color.r);
		glUniform1f(m_parameters[U_LIGHT0_POWER], lights[0].power);
		if (instance->player->GetFear() == 1)
		{
			fogColor.Set(0.2f, 0.2f, 0.2f);
			glUniform3fv(m_parameters[U_FOG_COLOR], 1, &fogColor.r);
		}
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
		Axis_Wait += (float)dt;
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
		Flashlight_Wait += (float)dt;
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
				FogAmount -= 3000 * (float)dt;
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
				FogAmount += 3000 * (float)dt;
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

	////////////////////////////////////////////////////////
	//	for next time winning condition to go next scene  //
	////////////////////////////////////////////////////////
	if (Application::IsKeyPressed('V'))
	{
		sound.stopMusic();
		Singleton::getInstance()->stateCheck = true;
		Singleton::getInstance()->program_state = Singleton::PROGRAM_GAME1;
	}
	if (Application::IsKeyPressed('B'))
	{
		sound.stopMusic();
		Singleton::getInstance()->stateCheck = true;
		Singleton::getInstance()->program_state = Singleton::PROGRAM_GAME2;
	}
	if (Application::IsKeyPressed('N'))
	{
		sound.stopMusic();
		Singleton::getInstance()->stateCheck = true;
		Singleton::getInstance()->program_state = Singleton::PROGRAM_GAME3;
	}
	if (Application::IsKeyPressed('M'))
	{
		sound.stopMusic();
		Singleton::getInstance()->stateCheck = true;
		Singleton::getInstance()->program_state = Singleton::PROGRAM_GAME4;
	}

	if (Singleton::getInstance()->boss->getHP() <= 0)
	{
		lights[0].power = 1.0f;
		glUniform1f(m_parameters[U_LIGHT0_POWER], lights[0].power);
		lights[0].color = (1.0f,1.0f,1.0f);
		glUniform3fv(m_parameters[U_LIGHT0_COLOR], 1, &lights[0].color.r);

		FogAmount = 6000;
		glUniform1f(m_parameters[U_FOG_END], FogAmount);
	}

	fps = (float)(1.f / dt);
}

void SceneLevel04::UpdateParticle(double dt)
{
	if (m_particlesCount < (int)MAX_PARTICLE)
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
			if (particle->pos.y < (ReadHeightMap(m_heightMap_4, particle->pos.x / TERRAINSIZE.x, particle->pos.z / TERRAINSIZE.z)  * TERRAINSIZE.y) - 50)
			{
				particle->active = false;
				m_particlesCount--;
			}
		}
	}
}

ParticleObject* SceneLevel04::GetParticles(void)
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

void SceneLevel04::initSceneObjects()
{
	AABBObject* Tree = new AABBObject();
	Tree->Object = AABBObject::OBJECT_TYPE::DEADTREE;
	Tree->active = true;
	Tree->pos.SetZero();
	Tree->pos.y = (ReadHeightMap(m_heightMap_4, Tree->pos.x / TERRAINSIZE.x, Tree->pos.z / TERRAINSIZE.z)  * TERRAINSIZE.y);
	Tree->pos.y -= 50;
	Tree->scale.Set(60, 70, 60);
	instance->Object_list.push_back(Tree);
	int numOfTombstones = 180;
	{
		int yOffset = 80;
		for (int i = 0; i < numOfTombstones * 0.25f; i++)
		{
			AABBObject* Tombstone = new AABBObject();
			Tombstone->Object = AABBObject::OBJECT_TYPE::TOMBSTONE;
			Tombstone->active = true;
			Tombstone->pos.Set(Math::RandFloatMinMax(-Terrainsize.x + 400, 0), 0, Math::RandFloatMinMax(-Terrainsize.z + 400, 0));
			Tombstone->pos.y = (ReadHeightMap(m_heightMap_4, Tombstone->pos.x / TERRAINSIZE.x, Tombstone->pos.z / TERRAINSIZE.z)  * TERRAINSIZE.y) - yOffset;
			Tombstone->scale.Set(10, 10, 10);
			instance->Object_list.push_back(Tombstone);
		}
		for (int i = 0; i < numOfTombstones * 0.25f; i++)
		{
			AABBObject* Tombstone = new AABBObject();
			Tombstone->Object = AABBObject::OBJECT_TYPE::TOMBSTONE;
			Tombstone->active = true;
			Tombstone->pos.Set(Math::RandFloatMinMax(0, Terrainsize.x - 400), 0, Math::RandFloatMinMax(-Terrainsize.z + 400, 0));
			Tombstone->pos.y = (ReadHeightMap(m_heightMap_4, Tombstone->pos.x / TERRAINSIZE.x, Tombstone->pos.z / TERRAINSIZE.z)  * TERRAINSIZE.y) - yOffset;
			Tombstone->scale.Set(10, 10, 10);
			instance->Object_list.push_back(Tombstone);
		}
		for (int i = 0; i < numOfTombstones * 0.25f; i++)
		{
			AABBObject* Tombstone = new AABBObject();
			Tombstone->Object = AABBObject::OBJECT_TYPE::TOMBSTONE;
			Tombstone->active = true;
			Tombstone->pos.Set(Math::RandFloatMinMax(-Terrainsize.x + 400, 0), 0, Math::RandFloatMinMax(0, Terrainsize.z - 400));
			Tombstone->pos.y = (ReadHeightMap(m_heightMap_4, Tombstone->pos.x / TERRAINSIZE.x, Tombstone->pos.z / TERRAINSIZE.z)  * TERRAINSIZE.y) - yOffset;
			Tombstone->scale.Set(10, 10, 10);
			instance->Object_list.push_back(Tombstone);
		}
		for (int i = 0; i < numOfTombstones * 0.25f; i++)
		{
			AABBObject* Tombstone = new AABBObject();
			Tombstone->Object = AABBObject::OBJECT_TYPE::TOMBSTONE;
			Tombstone->active = true;
			Tombstone->pos.Set(Math::RandFloatMinMax(0, Terrainsize.x - 400), 0, Math::RandFloatMinMax(0, Terrainsize.z - 400));
			Tombstone->pos.y = (ReadHeightMap(m_heightMap_4, Tombstone->pos.x / TERRAINSIZE.x, Tombstone->pos.z / TERRAINSIZE.z)  * TERRAINSIZE.y) - yOffset;
			Tombstone->scale.Set(10, 10, 10);
			instance->Object_list.push_back(Tombstone);
		}
	}
	{
		int yOffset = 50;

		for (int i = 0; i < 15; i++)
		{
			AABBObject* Fence = new AABBObject();
			Fence->Object = AABBObject::OBJECT_TYPE::FENCE;
			Fence->active = true;
			Fence->pos.Set(Terrainsize.x - 400, 0, -Terrainsize.z + (600 * i));
			Fence->pos.y = (ReadHeightMap(m_heightMap_4, Fence->pos.x / TERRAINSIZE.x, Fence->pos.z / TERRAINSIZE.z)  * TERRAINSIZE.y) - yOffset;
			Fence->scale.Set(0.6f, 0.6f, 0.6f);
			Fence->angle = 90.f;
			Fence->rotate.Set(0, 1, 0);
			instance->Object_list.push_back(Fence);
		}
		for (int i = 0; i < 15; i++)
		{
			AABBObject* Fence = new AABBObject();
			Fence->Object = AABBObject::OBJECT_TYPE::FENCE;
			Fence->active = true;
			Fence->pos.Set(-Terrainsize.x + 400, 0, -Terrainsize.z + (600 * i));
			Fence->pos.y = (ReadHeightMap(m_heightMap_4, Fence->pos.x / TERRAINSIZE.x, Fence->pos.z / TERRAINSIZE.z)  * TERRAINSIZE.y) - yOffset;
			Fence->angle = 90.f;
			Fence->rotate.Set(0, 1, 0);
			Fence->scale.Set(0.6f, 0.6f, 0.6f);
			instance->Object_list.push_back(Fence);
		}
	}
	AABBObject* Boundary = new AABBObject();
	Boundary->Object = AABBObject::OBJECT_TYPE::BOUNDARY;
	Boundary->active = true;
	Boundary->pos.Set(0, 0, 2000);
	Boundary->scale.Set(350, 40, 15);
	instance->Object_list.push_back(Boundary);

	Boundary = new AABBObject();
	Boundary->Object = AABBObject::OBJECT_TYPE::BOUNDARY;
	Boundary->active = true;
	Boundary->pos.Set(0, 0, -2000);
	Boundary->scale.Set(350, 40, 15);
	instance->Object_list.push_back(Boundary);
}

void SceneLevel04::RenderGround()
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

void SceneLevel04::RenderSkyplane()
{
	modelStack.PushMatrix();
	modelStack.Translate(camera.position.x, 1800, camera.position.z);
	modelStack.Rotate(rotateAngle, 0, 1, 0);
	RenderMesh(meshList[SKYPLANE], false);
	modelStack.PopMatrix();
}

void SceneLevel04::RenderTerrain()
{
	modelStack.PushMatrix();
	modelStack.Translate(0, -50, 0);
	modelStack.Scale(TERRAINSIZE.x, TERRAINSIZE.y, TERRAINSIZE.z);
	RenderMesh(meshList[TERRAIN_LEVEL04], true);
	modelStack.PopMatrix();
}

void SceneLevel04::RenderEnvironment(bool Light)
{
	RenderObjects(ShowHitbox);
	RenderEnemies(ShowHitbox);
	yOffset = 50;
	modelStack.PushMatrix();
	modelStack.Translate(1100, (ReadHeightMap(m_heightMap_4, 1100 / TERRAINSIZE.x, 1970 / TERRAINSIZE.z)  * TERRAINSIZE.y) - yOffset, 1970);
	modelStack.Scale(0.6f, 0.6f, 0.6f);
	RenderMeshOutlined(meshList[FENCE], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(230, (ReadHeightMap(m_heightMap_4, 250 / TERRAINSIZE.x, 1970 / TERRAINSIZE.z)  * TERRAINSIZE.y) - yOffset, 1970);
	modelStack.Scale(0.6f, 0.6f, 0.6f);
	RenderMeshOutlined(meshList[FENCE], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-640, (ReadHeightMap(m_heightMap_4, -640 / TERRAINSIZE.x, 1970 / TERRAINSIZE.z)  * TERRAINSIZE.y) - yOffset, 1970);
	modelStack.Scale(0.6f, 0.6f, 0.6f);
	RenderMeshOutlined(meshList[FENCE], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-1510, (ReadHeightMap(m_heightMap_4, -640 / TERRAINSIZE.x, 1970 / TERRAINSIZE.z)  * TERRAINSIZE.y) - yOffset, 1970);
	modelStack.Scale(0.6f, 0.6f, 0.6f);
	RenderMeshOutlined(meshList[FENCE], true);
	modelStack.PopMatrix();

	for (int i = 0; i < 128; ++i)
	{
		Degree = Math::RadianToDegree(atan2(-(randompos_z[i] - instance->player->getPosition().z), randompos_x[i] - instance->player->getPosition().x));
		switch (random[i])
		{
		case 1:
		{
			modelStack.PushMatrix();
			modelStack.Translate(randompos_x[i], 120 + TERRAINSIZE.y * ReadHeightMap(m_heightMap_4, randompos_x[i] / TERRAINSIZE.x, randompos_z[i] / TERRAINSIZE.z), randompos_z[i]);
			modelStack.Rotate(Degree - 90, 0, 1, 0);
			modelStack.Scale(250, 400, 250);
			RenderMeshOutlined(meshList[GEO_TREE_1], false);
			modelStack.PopMatrix();
			break;
		}
		case 2:
		{
			modelStack.PushMatrix();
			modelStack.Translate(randompos_x[i], 120 + TERRAINSIZE.y * ReadHeightMap(m_heightMap_4, randompos_x[i] / TERRAINSIZE.x, randompos_z[i] / TERRAINSIZE.z), randompos_z[i]);
			modelStack.Rotate(Degree - 90, 0, 1, 0);
			modelStack.Scale(400, 400, 400);
			RenderMeshOutlined(meshList[GEO_TREE_2], false);
			modelStack.PopMatrix();
			break;
		}
		case 3:
		{
			modelStack.PushMatrix();
			modelStack.Translate(randompos_x[i], 120 + TERRAINSIZE.y * ReadHeightMap(m_heightMap_4, randompos_x[i] / TERRAINSIZE.x, randompos_z[i] / TERRAINSIZE.z), randompos_z[i]);
			modelStack.Rotate(Degree - 90, 0, 1, 0);
			modelStack.Scale(150, 400, 150);
			RenderMeshOutlined(meshList[GEO_TREE_3], false);
			modelStack.PopMatrix();
			break;
		}
		}
	}
}

void SceneLevel04::RenderHUD()
{
	RenderRadar();

	if (instance->boss->getHP() > instance->boss->getMAX() * 0.5)
	{
		RenderImageOnScreen(meshList[GEO_BOSS_HP], Vector3(boss_hp_ratio * 50, 20, 1), Vector3(40, 55, 0), Vector3(0, 0, 0));
		RenderImageOnScreen(meshList[GEO_HP], Vector3((float)instance->boss->getMAX() / 4, 20, 1), Vector3(40, 55, 0), Vector3(0, 0, 0));
	}
	else
	{
		RenderImageOnScreen(meshList[GEO_HP], Vector3(boss_hp_ratio * 50, 20, 1), Vector3(40, 55, 0), Vector3(0, 0, 0));
	}
}

void SceneLevel04::RenderSprite()
{
}

void SceneLevel04::RenderParticle(ParticleObject* particle)
{
	//switch (particle->type)
	//{

	//default:
	//	break;
	//}
}

void SceneLevel04::RenderLight()
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

void SceneLevel04::RenderWorld()
{
	RenderSkyplane();
	RenderTerrain();
	RenderEnvironment(true);
	RenderBullets(false);
}

void SceneLevel04::RenderPassGPass()
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

void SceneLevel04::RenderPassMain()
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

	
	RenderInventory();
	RenderGUI();
	RenderWeapons(true);
	RenderBullets(false);
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

	glUniform1f(m_parameters[U_FOG_ENABLE], 1);
	RenderWorld();
	RenderBoss(false);
	glUniform1f(m_parameters[U_FOG_ENABLE], 0);

	if (!Singleton::getInstance()->stateCheck)
	{
		RenderHUD();
	}

	//bLightEnabled = false;

	//On screen text
	std::ostringstream ss;

	if (showText)
	{
		ss.precision(5);
		ss << "FPS: " << fps;
	    RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 2, 3);

		{
			std::ostringstream ss;
			ss.precision(5);
			ss << "Partition: " << getPartition(camera.position);
			RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 2, 8);
		}
		{
			std::ostringstream ss;
			ss.precision(5);
			ss << "Position: " << camera.position;
			RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 2, 15);
		}
	}
	switch (weaponType)
	{
	case 1:
		ss.str("");
		ss.precision(5);
		ss << pistolAmmo << "/" << maxPistolAmmo << "         " << "MAG:" << pistolMag;
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 1.5f, 3, 7);
		break;
	case 2:
		ss.str("");
		ss.precision(5);
		ss << rifleAmmo << "/" << maxRifleAmmo << "         " << "MAG:" << rifleMag;
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 1.5f, 3, 7);
		break;
	case 3:

		break;
	}
	SceneBase::Render();
}

void SceneLevel04::Render()
{
	//PRE RENDER PASS
	RenderPassGPass();

	//MAIN RENDER PASS
	RenderPassMain();
}

void SceneLevel04::Exit()
{
	delete level4_Heights;
	SceneBase::Exit();
}