#include "SceneLevel03.h"
#include "GL\glew.h"

#include "shader.hpp"
#include "Application.h"
#include "Utility.h"
#include <sstream>

SceneLevel03::SceneLevel03()
{
}

SceneLevel03::~SceneLevel03()
{
}

void SceneLevel03::Init()
{
	sound.playMusic("Sound/level3bgm.mp3");

	Application::HideCursor();

	SceneBase::Init();
	camera.position += Vector3(0, 0, 1800);

	meshList[TERRAIN_LEVEL03] = MeshBuilder::GenerateTerrain("Terrain", "Image//Terrain//Terrain_Level03.raw", m_heightMap_3, level3_Heights);
	meshList[TERRAIN_LEVEL03]->textureArray[0] = LoadTGA("Image//Forest//Grass.tga");
	meshList[TERRAIN_LEVEL03]->textureArray[1] = LoadTGA("Image//Forest//Dead_Leaves.tga");

	//Forest
	meshList[GEO_TREE_1] = MeshBuilder::GenerateQuad("Thin Tree", Color(0, 0, 0), 1.f);
	meshList[GEO_TREE_1]->textureArray[0] = LoadTGA("Image//Forest//Tree_1.tga");
	meshList[GEO_TREE_2] = MeshBuilder::GenerateQuad("Fat Tree", Color(0, 0, 0), 1.f);
	meshList[GEO_TREE_2]->textureArray[0] = LoadTGA("Image//Forest//Tree_2.tga");
	meshList[GEO_TREE_3] = MeshBuilder::GenerateQuad("Dead Tree", Color(0, 0, 0), 1.f);
	meshList[GEO_TREE_3]->textureArray[0] = LoadTGA("Image//Forest//Dead_Tree.tga");
	meshList[GEO_BUSH] = MeshBuilder::GenerateQuad("Water", Color(0, 0, 0), 1.f);
	meshList[GEO_BUSH]->textureArray[0] = LoadTGA("Image//Forest//Bush.tga");
	meshList[GEO_BRIDGE] = MeshBuilder::GenerateOBJ("Bridge", "OBJ//Forest//Bridge.obj");
	meshList[GEO_BRIDGE]->textureArray[0] = LoadTGA("Image//Forest//Bridge.tga");
	meshList[GEO_LOGS] = MeshBuilder::GenerateOBJ("Logs", "OBJ//Forest//Logs.obj");
	meshList[GEO_LOGS]->textureArray[0] = LoadTGA("Image//Forest//Logs.tga");

	meshList[WATER_FLOW] = MeshBuilder::GenerateSpriteAnimation("Water", 4, 4);
	meshList[WATER_FLOW]->textureArray[0] = LoadTGA("Image//Forest//Water_flow.tga");

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	terrainHeight = TERRAINSIZE.y;
	Terrainsize = TERRAINSIZE * 0.5f;
	//Random my random randomly using srand
	srand(time(NULL));

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

	fogColor.Set(0.2f, 0.2f, 0.2f);
	glUniform3fv(m_parameters[U_FOG_COLOR], 1, &fogColor.r);

	Water = dynamic_cast<SpriteAnimation*>(meshList[WATER_FLOW]);
	if (Water)
	{
		Water->m_anim = new Animation();
		Water->m_anim->Set(0, 15, 0, 2.0f, true);
	}

	for (int i = 0; i < 400; ++i)
	{
		Tree[i].Set(Math::RandIntMinMax(-1800, 1800), 0, Math::RandIntMinMax(-1000, 1800));
		Tree_Type[i] = Math::RandIntMinMax(1, 3);
	}
	for (int i = 0; i < 400; ++i)
	{
		Bush[i].Set(Math::RandIntMinMax(-2000, 2000), 0, Math::RandIntMinMax(-1000, 1800));
	}

	SpawnGhost();

	//Loading dialogue
	ReadDialogue("Text//Dialogue_3.txt", Dialogue);

	initSceneObjects();
}

void SceneLevel03::initSceneObjects()
{
	AABBObject * Logs = new AABBObject();
	Logs->Object = AABBObject::OBJECT_TYPE::LOGS;
	Logs->active = true;
	Logs->pos.Set(-1150, 100, -1100);
	Logs->scale.Set(4, 5, 5);
	instance->Object_list.push_back(Logs);

	Logs = new AABBObject();
	Logs->Object = AABBObject::OBJECT_TYPE::LOGS;
	Logs->active = true;
	Logs->pos.Set(-1700, 100, -1100);
	Logs->scale.Set(4, 5, 5);
	instance->Object_list.push_back(Logs);

	Logs = new AABBObject();
	Logs->Object = AABBObject::OBJECT_TYPE::LOGS;
	Logs->active = true;
	Logs->pos.Set(-600, 100, -1100);
	Logs->scale.Set(4, 5, 5);
	instance->Object_list.push_back(Logs);

	Logs = new AABBObject();
	Logs->Object = AABBObject::OBJECT_TYPE::LOGS;
	Logs->active = true;
	Logs->pos.Set(1150, 100, -1100);
	Logs->scale.Set(4, 5, 5);
	instance->Object_list.push_back(Logs);

	Logs = new AABBObject();
	Logs->Object = AABBObject::OBJECT_TYPE::LOGS;
	Logs->active = true;
	Logs->pos.Set(1700, 100, -1100);
	Logs->scale.Set(4, 5, 5);
	instance->Object_list.push_back(Logs);

	Logs = new AABBObject();
	Logs->Object = AABBObject::OBJECT_TYPE::LOGS;
	Logs->active = true;
	Logs->pos.Set(600, 100, -1100);
	Logs->scale.Set(4, 5, 5);
	instance->Object_list.push_back(Logs);

	AABBObject * Bridge = new AABBObject();
	Bridge->Object = AABBObject::OBJECT_TYPE::BRIDGE;
	Bridge->active = true;
	Bridge->pos.Set(0, 58, -1420);
	Bridge->scale.Set(5, 5, 5);
	instance->Object_list.push_back(Bridge);

	//Left
	AABBObject * Boundary = new AABBObject();
	Boundary->Object = AABBObject::OBJECT_TYPE::BOUNDARY;
	Boundary->active = true;
	Boundary->pos.Set(-1900, 100, 0);
	Boundary->scale.Set(5, 10, 380);
	instance->Object_list.push_back(Boundary);

	//Right
	Boundary = new AABBObject();
	Boundary->Object = AABBObject::OBJECT_TYPE::BOUNDARY;
	Boundary->active = true;
	Boundary->pos.Set(1900, 100, 0);
	Boundary->scale.Set(5, 10, 380);
	instance->Object_list.push_back(Boundary);

	//Back
	Boundary = new AABBObject();
	Boundary->Object = AABBObject::OBJECT_TYPE::BOUNDARY;
	Boundary->active = true;
	Boundary->pos.Set(0, 100, 1900);
	Boundary->scale.Set(380, 10, 5);
	instance->Object_list.push_back(Boundary);

	//Front
	Boundary = new AABBObject();
	Boundary->Object = AABBObject::OBJECT_TYPE::BOUNDARY;
	Boundary->active = true;
	Boundary->pos.Set(-1100, 100, -1100);
	Boundary->scale.Set(210, 10, 5);
	instance->Object_list.push_back(Boundary);

	//Front
	Boundary = new AABBObject();
	Boundary->Object = AABBObject::OBJECT_TYPE::BOUNDARY;
	Boundary->active = true;
	Boundary->pos.Set(1100, 100, -1100);
	Boundary->scale.Set(210, 10, 5);;
	instance->Object_list.push_back(Boundary);

	//To level 4
	Change = new AABBObject();
	Change->Object = AABBObject::OBJECT_TYPE::BOUNDARY;
	Change->active = true;
	Change->pos.Set(0, 100, -1200);
	Change->scale.Set(20, 25, 15);;
	instance->Object_list.push_back(Change);
}

void SceneLevel03::Update(double dt)
{
	camera.Update(dt);

	SceneBase::Update(dt);

	UpdateParticle(dt);

	if (Water)
	{
		Water->Update(dt);
		Water->m_anim->animActive = true;
	}

	UpdateEnemy(dt);

	//Dialogues
	if (Dialogue_Selection != 1 &&
		Application::IsKeyPressed('E') &&
		Dialogue_Timer <= 0.0f)
	{
		Dialogue_Timer = 1.0f;
		Dialogue_Selection++;
	}
	else
	{
		Dialogue_Timer -= (float)dt;
	}
	if (Dialogue_Selection == 1 &&
		Application::IsKeyPressed('E') &&
		Dialogue_Timer <= 0.0f)
	{
		Dialogue_Timer = 1.0f;
		Dialogues = false;
	}

	//camera.Terrain = TERRAINSIZE.y * ReadHeightMap(m_heightMap, camera.position.x / TERRAINSIZE.x, camera.position.z / TERRAINSIZE.z);
	camera.Terrain = getHeightofTerrain(TERRAINSIZE.x, level3_Heights);

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

	////////////////////////////////////////////////////////
	//	for next time winning condition to go next scene  //
	////////////////////////////////////////////////////////
	float distance = (Change->pos - Singleton::getInstance()->player->getPosition()).Length();
	if (Application::IsKeyPressed('E') && distance <= 200)
	{
		sound.stopMusic();
		Singleton::getInstance()->stateCheck = true;
		Singleton::getInstance()->program_state = Singleton::PROGRAM_GAME4;
		instance->gotClear = true;
	}

	fps = (float)(1.f / dt);
}

void SceneLevel03::UpdateParticle(double dt)
{
}

ParticleObject* SceneLevel03::GetParticles(void)
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

void SceneLevel03::RenderGround()
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

void SceneLevel03::RenderSkyplane(bool inverted)
{
	modelStack.PushMatrix();
	modelStack.Translate(camera.position.x, 1800, camera.position.z);
	modelStack.Rotate(rotateAngle, 0, 1, 0);
	if (inverted)
	{
		modelStack.Rotate(180, 1, 0, 0);
	}
	RenderMesh(meshList[SKYPLANE], false);
	modelStack.PopMatrix();
}

void SceneLevel03::RenderTerrain()
{
	modelStack.PushMatrix();
	modelStack.Translate(0, -50, 0);
	modelStack.Scale(TERRAINSIZE.x, TERRAINSIZE.y, TERRAINSIZE.z);
	RenderMesh(meshList[TERRAIN], true);
	modelStack.PopMatrix();
}

void SceneLevel03::RenderEnvironment(bool Light, bool inverted)
{
	playerPartition = getPartition(camera.position);
	if (!inverted)
	{
		for (int i = 0; i < 400; ++i)
		{
			posPartition = getPartition(Tree[i]);
			if (renderCheck(playerPartition, posPartition))
			{
				Degree = Math::RadianToDegree(atan2(-(Tree[i].z - instance->player->getPosition().z), Tree[i].x - instance->player->getPosition().x));
				switch (Tree_Type[i])
				{
				case 1:
				{
					modelStack.PushMatrix();
					modelStack.Translate(Tree[i].x, 120 + TERRAINSIZE.y * ReadHeightMap(m_heightMap_3, Tree[i].x / TERRAINSIZE.x, Tree[i].z / TERRAINSIZE.z), Tree[i].z);
					modelStack.Rotate(Degree - 90, 0, 1, 0);
					modelStack.Scale(250, 400, 250);
					RenderMeshOutlined(meshList[GEO_TREE_1], false);
					modelStack.PopMatrix();
					break;
				}
				case 2:
				{
					modelStack.PushMatrix();
					modelStack.Translate(Tree[i].x, 100 + TERRAINSIZE.y * ReadHeightMap(m_heightMap_3, Tree[i].x / TERRAINSIZE.x, Tree[i].z / TERRAINSIZE.z), Tree[i].z);
					modelStack.Rotate(Degree - 90, 0, 1, 0);
					modelStack.Scale(400, 400, 400);
					RenderMeshOutlined(meshList[GEO_TREE_2], false);
					modelStack.PopMatrix();
					break;
				}
				case 3:
				{
					modelStack.PushMatrix();
					modelStack.Translate(Tree[i].x, 120 + TERRAINSIZE.y * ReadHeightMap(m_heightMap_3, Tree[i].x / TERRAINSIZE.x, Tree[i].z / TERRAINSIZE.z), Tree[i].z);
					modelStack.Rotate(Degree - 90, 0, 1, 0);
					modelStack.Scale(150, 400, 150);
					RenderMeshOutlined(meshList[GEO_TREE_3], false);
					modelStack.PopMatrix();
					break;
				}
				}
			}
		}

		for (int i = 0; i < 400; ++i)
		{
			posPartition = getPartition(Tree[i]);
			if (renderCheck(playerPartition, posPartition))
			{
				Degree = Math::RadianToDegree(atan2(-(Bush[i].z - instance->player->getPosition().z), Bush[i].x - instance->player->getPosition().x));
				modelStack.PushMatrix();
				modelStack.Translate(Bush[i].x, -50 + TERRAINSIZE.y * ReadHeightMap(m_heightMap_3, Bush[i].x / TERRAINSIZE.x, Bush[i].z / TERRAINSIZE.z), Bush[i].z);
				modelStack.Rotate(Degree - 90, 0, 1, 0);
				modelStack.Scale(100, 100, 100);
				RenderMeshOutlined(meshList[GEO_BUSH], false);
				modelStack.PopMatrix();
			}
		}
		for (int i = 0; i < 60; ++i)
		{
			if (renderCheck(playerPartition, posPartition))
			{
				Degree = Math::RadianToDegree(atan2(-((1950 - 50 * i) - instance->player->getPosition().z), -1900 - instance->player->getPosition().x));
				switch (Tree_Type[i])
				{
					case 1:
					{
						modelStack.PushMatrix();
						modelStack.Translate(-1900, 120 + TERRAINSIZE.y * ReadHeightMap(m_heightMap_3, -1950 / TERRAINSIZE.x, (1950 - 20 * i) / TERRAINSIZE.z), 1950 - 50 * i);
						modelStack.Rotate(Degree -90, 0, 1, 0);
						modelStack.Scale(250, 400, 250);
						RenderMeshOutlined(meshList[GEO_TREE_1], false);
						modelStack.PopMatrix();
						break;
					}
					case 2:
					{
						modelStack.PushMatrix();
						modelStack.Translate(-1900, 100 + TERRAINSIZE.y * ReadHeightMap(m_heightMap_3, -1950 / TERRAINSIZE.x, (1950 - 20 * i) / TERRAINSIZE.z), 1950 - 50 * i);
						modelStack.Rotate(Degree - 90, 0, 1, 0);
						modelStack.Scale(400, 400, 400);
						RenderMeshOutlined(meshList[GEO_TREE_2], false);
						modelStack.PopMatrix();
						break;
					}
					case 3:
					{
						modelStack.PushMatrix();
						modelStack.Translate(-1900, 120 + TERRAINSIZE.y * ReadHeightMap(m_heightMap_3, -1950 / TERRAINSIZE.x, (1950 - 20 * i) / TERRAINSIZE.z), 1950 - 50 * i);
						modelStack.Rotate(Degree - 90, 0, 1, 0);
						modelStack.Scale(150, 400, 150);
						RenderMeshOutlined(meshList[GEO_TREE_3], false);
						modelStack.PopMatrix();
						break;
					}
				}
			}	
		}
		for (int i = 0; i < 60; ++i)
		{
			if (renderCheck(playerPartition, posPartition))
			{
				Degree = Math::RadianToDegree(atan2(-((1950 - 50 * i) - instance->player->getPosition().z), 1900 - instance->player->getPosition().x));
				switch (Tree_Type[i])
				{
				case 1:
				{
					modelStack.PushMatrix();
					modelStack.Translate(1900, 120 + TERRAINSIZE.y * ReadHeightMap(m_heightMap_3, -1950 / TERRAINSIZE.x, (1950 - 20 * i) / TERRAINSIZE.z), 1950 - 50 * i);
					modelStack.Rotate(Degree - 90, 0, 1, 0);
					modelStack.Scale(250, 400, 250);
					RenderMeshOutlined(meshList[GEO_TREE_1], false);
					modelStack.PopMatrix();
					break;
				}
				case 2:
				{
					modelStack.PushMatrix();
					modelStack.Translate(1900, 100 + TERRAINSIZE.y * ReadHeightMap(m_heightMap_3, -1950 / TERRAINSIZE.x, (1950 - 20 * i) / TERRAINSIZE.z), 1950 - 50 * i);
					modelStack.Rotate(Degree - 90, 0, 1, 0);
					modelStack.Scale(400, 400, 400);
					RenderMeshOutlined(meshList[GEO_TREE_2], false);
					modelStack.PopMatrix();
					break;
				}
				case 3:
				{
					modelStack.PushMatrix();
					modelStack.Translate(1900, 120 + TERRAINSIZE.y * ReadHeightMap(m_heightMap_3, -1950 / TERRAINSIZE.x, (1950 - 20 * i) / TERRAINSIZE.z), 1950 - 50 * i);
					modelStack.Rotate(Degree - 90, 0, 1, 0);
					modelStack.Scale(150, 400, 150);
					RenderMeshOutlined(meshList[GEO_TREE_3], false);
					modelStack.PopMatrix();
					break;
				}
				}
			}
		}
		for (int i = 0; i < 78; ++i)
		{
			if (renderCheck(playerPartition, posPartition))
			{
				Degree = Math::RadianToDegree(atan2(-(1900 -instance->player->getPosition().z), (1950 - 50 * i) - instance->player->getPosition().x));
				switch (Tree_Type[i])
				{
				case 1:
				{
					modelStack.PushMatrix();
					modelStack.Translate((1950 - 50 * i), 120 + TERRAINSIZE.y * ReadHeightMap(m_heightMap_3, (1950 - 50 * i) / TERRAINSIZE.x, 1900 / TERRAINSIZE.z), 1900);
					modelStack.Rotate(Degree - 90, 0, 1, 0);
					modelStack.Scale(250, 400, 250);
					RenderMeshOutlined(meshList[GEO_TREE_1], false);
					modelStack.PopMatrix();
					break;
				}
				case 2:
				{
					modelStack.PushMatrix();
					modelStack.Translate((1950 - 50 * i), 100 + TERRAINSIZE.y * ReadHeightMap(m_heightMap_3, (1950 - 50 * i) / TERRAINSIZE.x, 1900 / TERRAINSIZE.z), 1900);
					modelStack.Rotate(Degree - 90, 0, 1, 0);
					modelStack.Scale(400, 400, 400);
					RenderMeshOutlined(meshList[GEO_TREE_2], false);
					modelStack.PopMatrix();
					break;
				}
				case 3:
				{
					modelStack.PushMatrix();
					modelStack.Translate((1950 - 50 * i), 120 + TERRAINSIZE.y * ReadHeightMap(m_heightMap_3, (1950 - 50 * i) / TERRAINSIZE.x, 1900 / TERRAINSIZE.z), 1900);
					modelStack.Rotate(Degree - 90, 0, 1, 0);
					modelStack.Scale(150, 400, 150);
					RenderMeshOutlined(meshList[GEO_TREE_3], false);
					modelStack.PopMatrix();
					break;
				}
				}
			}
		}
	}


	if (inverted)
	{
		modelStack.PushMatrix();
		modelStack.Translate(0, 40, -1420);
		modelStack.Rotate(180, 1, 0, 0);
		modelStack.Scale(5, 5, 5);
		RenderMesh(meshList[GEO_BRIDGE], false);
		modelStack.PopMatrix();
	}
}

void SceneLevel03::RenderHUD()
{
	//RenderImageOnScreen(meshList[GEO_STAMINA], Vector3(100, 2, 1), Vector3(50 - (100 - instance->player->GetStamina() / 3), 1, 0), Vector3(0, 0, 0));

	RenderRadar();
}

void SceneLevel03::RenderSprite()
{
}

void SceneLevel03::RenderParticle(ParticleObject* particle)
{
	switch (particle->type)
	{

	default:
		break;
	}
}

void SceneLevel03::RenderLight()
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

void SceneLevel03::RenderWorld()
{
	RenderSkyplane();
	RenderTerrain();
	RenderEnvironment(false);
}

void SceneLevel03::RenderReflection()
{
	glDisable(GL_CULL_FACE);

	glDepthMask(GL_FALSE); // Don't write to depth buffer
	glEnable(GL_STENCIL_TEST);

	// Reflection surface
	glStencilFunc(GL_ALWAYS, 1, 0xFF); // Set any stencil to 1
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	glStencilMask(0xFF); // Write to stencil buffer
	glClear(GL_STENCIL_BUFFER_BIT); // Clear stencil buffer (0 by default)


	modelStack.PushMatrix();
	modelStack.Translate(1500, 100, -1440);
	modelStack.Rotate(-90, 1, 0, 0);
	modelStack.Rotate(-90, 0, 0, 1);
	modelStack.Scale(800,1000, 1);
	RenderMesh(meshList[WATER_FLOW], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-1500, 100, -1440);
	modelStack.Rotate(-90, 1, 0, 0);
	modelStack.Rotate(-90, 0, 0, 1);
	modelStack.Scale(800, 1000, 1);
	RenderMesh(meshList[WATER_FLOW], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(500, 100, -1440);
	modelStack.Rotate(-90, 1, 0, 0);
	modelStack.Rotate(-90, 0, 0, 1);
	modelStack.Scale(800, 1000, 1);
	RenderMesh(meshList[WATER_FLOW], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-500, 100, -1440);
	modelStack.Rotate(-90, 1, 0, 0);
	modelStack.Rotate(-90, 0, 0, 1);
	modelStack.Scale(800, 1000, 1);
	RenderMesh(meshList[WATER_FLOW], false);
	modelStack.PopMatrix();

	// Reflection
	glStencilFunc(GL_EQUAL, 1, 0xFF); // Pass test if stencil value is 1
	glStencilMask(0x00); // Don't write anything to stencil buffer
	glDepthMask(GL_TRUE); // Write to depth buffer

	RenderEnvironment(false, true);

	glDisable(GL_STENCIL_TEST);

	glEnable(GL_CULL_FACE);

	modelStack.PushMatrix();
	modelStack.Translate(1500, 100, -1440);
	modelStack.Rotate(-90, 1, 0, 0);
	modelStack.Rotate(-90, 0, 0, 1);
	modelStack.Scale(800, 1000, 1);
	RenderMesh(meshList[WATER_FLOW], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-1500, 100, -1440);
	modelStack.Rotate(-90, 1, 0, 0);
	modelStack.Rotate(-90, 0, 0, 1);
	modelStack.Scale(800, 1000, 1);
	RenderMesh(meshList[WATER_FLOW], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(500, 100, -1440);
	modelStack.Rotate(-90, 1, 0, 0);
	modelStack.Rotate(-90, 0, 0, 1);
	modelStack.Scale(800, 1000, 1);
	RenderMesh(meshList[WATER_FLOW], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-500, 100, -1440);
	modelStack.Rotate(-90, 1, 0, 0);
	modelStack.Rotate(-90, 0, 0, 1);
	modelStack.Scale(800, 1000, 1);
	RenderMesh(meshList[WATER_FLOW], false);
	modelStack.PopMatrix();
}

void SceneLevel03::RenderPassGPass()
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

void SceneLevel03::RenderPassMain()
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

	RenderReflection();

	RenderObjects(ShowHitbox);
	RenderEnemies(ShowHitbox);
	glUniform1f(m_parameters[U_FOG_ENABLE], 0);

	if (!Singleton::getInstance()->stateCheck)
	{
		RenderHUD();
	}

	
	RenderWeapons(false);
	RenderBullets(false);

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
		ss << instance->pistolAmmo << "/" << instance->maxPistolAmmo << "         " << "MAG:" << instance->pistolMag;
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 1.5f, 3, 7);
		break;
	case 2:
		ss.str("");
		ss.precision(5);
		ss << instance->rifleAmmo << "/" << instance->maxRifleAmmo << "         " << "MAG:" << instance->rifleMag;
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 1.5f, 3, 7);
		break;
	case 3:

		break;
	}

	ss.str(""); ss.precision(5); ss << instance->player->getHealthPack();
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 2.5f, 6, 10);

	//Dialogues
	if (Dialogues)
	{
		RenderImageOnScreen(meshList[GEO_TEXT_BOX], Vector3(80, 30, 1), Vector3(30, 20, 0), Vector3(0, 0, 0));

		ss.str("");
		ss << Dialogue[Dialogue_Selection];
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0.8f, 0.8f, 0.8f), 2.5, 2, 21);

		ss.str("");
		ss << "Press E to continue...";
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0.5f, 0.8f, 0.5f), 2.5, 2, 19);
	}

	SceneBase::Render();
}

void SceneLevel03::Render()
{
	//PRE RENDER PASS
	RenderPassGPass();

	//MAIN RENDER PASS
	RenderPassMain();
}

void SceneLevel03::Exit()
{
	delete level3_Heights;
	this->SceneBase::Exit();
}