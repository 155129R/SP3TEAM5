#include "SceneLevel02.h"
#include "GL\glew.h"

#include "shader.hpp"
#include "Application.h"
#include "Utility.h"
#include <sstream>

SceneLevel02::SceneLevel02()
{
}

SceneLevel02::~SceneLevel02()
{
}

static const Vector3 TERRAINSIZE(1400.f, 200.0f, 1400.f);

void SceneLevel02::Init()
{
	sound.playMusic("Sound/level2bgm.mp3");

	Application::HideCursor();
	
	SceneBase::Init();
	lights[0].position.Set(0, 500, 0);

	meshList[FOUNTAIN] = MeshBuilder::GenerateOBJ("fountain", "OBJ//Outdoor//fountain.obj");
	meshList[FOUNTAIN]->textureArray[0] = LoadTGA("Image//Outdoor//fountain.tga");

	meshList[FOUNTAIN_WATER1] = MeshBuilder::GenerateSphere("sphere", Color(0.2f, 0.7f, 1), 18, 36, 1.f);
	meshList[FOUNTAIN_WATER1]->textureArray[0] = LoadTGA("Image//Outdoor//water.tga");

	meshList[FLOOR] = MeshBuilder::GenerateQuad2("floor", Color(0, 0, 0), 10, 10, TexCoord(10, 10));
	meshList[FLOOR]->textureArray[0] = LoadTGA("Image//Outdoor//floor.tga");

	meshList[HOUSE1] = MeshBuilder::GenerateOBJ("house", "OBJ//Outdoor//house.obj");
	meshList[HOUSE1]->textureArray[0] = LoadTGA("Image//Outdoor//houseTex.tga");

	meshList[HOUSE2] = MeshBuilder::GenerateOBJ("house", "OBJ//Outdoor//house.obj");
	meshList[HOUSE2]->textureArray[0] = LoadTGA("Image//Outdoor//houseTex2.tga");

	meshList[HOUSE3] = MeshBuilder::GenerateOBJ("house", "OBJ//Outdoor//house2.obj");
	meshList[HOUSE3]->textureArray[0] = LoadTGA("Image//Outdoor//house3.tga");

	meshList[METAL_FENCE] = MeshBuilder::GenerateOBJ("house", "OBJ//Outdoor//metalFence.obj");
	//meshList[METAL_FENCE]->textureArray[0] = LoadTGA("Image//metalFence.tga");
	meshList[METAL_FENCE]->textureArray[0] = LoadTGA("Image//Outdoor//rust.tga");

	meshList[METAL_GATE] = MeshBuilder::GenerateOBJ("house", "OBJ//Outdoor//gate.obj");
	//meshList[METAL_GATE]->textureArray[0] = LoadTGA("Image//metalFence.tga");
	meshList[METAL_GATE]->textureArray[0] = LoadTGA("Image//Outdoor//rust.tga");

	meshList[HEDGE] = MeshBuilder::GenerateOBJ("house", "OBJ//Outdoor//hedge.obj");
	meshList[HEDGE]->textureArray[0] = LoadTGA("Image//Outdoor//hedge.tga");

	meshList[BENCHES] = MeshBuilder::GenerateOBJ("house", "OBJ//Outdoor//bench.obj");
	meshList[BENCHES]->textureArray[0] = LoadTGA("Image//Outdoor//bench.tga");

	meshList[POT] = MeshBuilder::GenerateOBJ("pot", "OBJ//Outdoor//pot.obj");
	meshList[POT]->textureArray[0] = LoadTGA("Image//Outdoor//pot.tga");

	meshList[COCONUT_TREE] = MeshBuilder::GenerateQuad("Water", Color(0, 0, 0), 1.f);
	meshList[COCONUT_TREE]->textureArray[0] = LoadTGA("Image//Outdoor//coconutTree.tga");

	meshList[QUEST_EXIT] = MeshBuilder::GenerateQuad("Water", Color(0, 0, 0), 1.f);
	meshList[QUEST_EXIT]->textureArray[0] = LoadTGA("Image//Outdoor//questGate.tga");

	meshList[GEO_TREE_1] = MeshBuilder::GenerateQuad("Thin Tree", Color(0, 0, 0), 1.f);
	meshList[GEO_TREE_1]->textureArray[0] = LoadTGA("Image//Forest//Tree_1.tga");
	meshList[GEO_BUSH] = MeshBuilder::GenerateQuad("Water", Color(0, 0, 0), 1.f);
	meshList[GEO_BUSH]->textureArray[0] = LoadTGA("Image//Forest//Bush.tga");

	//harambe
	meshList[PAINTING] = MeshBuilder::GenerateQuad("PAINTING", Color(0, 0, 0), 1.f);
	meshList[PAINTING]->textureArray[0] = LoadTGA("Image//Indoor//paintingHarambe.tga");
	//starry night
	meshList[PAINTING2] = MeshBuilder::GenerateQuad("PAINTING2", Color(0, 0, 0), 1.f);
	meshList[PAINTING2]->textureArray[0] = LoadTGA("Image//Indoor//painting1.tga");

	camera.Init(Vector3(27, 5, -444), Vector3(0, 5, 1), Vector3(0, 1, 0));

	//camera.Init(Vector3(-1190, 20, 335), Vector3(0, 5, 1), Vector3(0, 1, 0));

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
	soundTimer = 0;

	openGate = false;
	rotateGate = 90;

	questToNextScene = false;
	distanceLeft = 0;

	fountainsfx->setDefault3DSoundMinDistance(100.f);
	fountainsfx->setDefault3DSoundMaxDistance(5000.f);
	fountainsfx->setSoundVolume(1.f);

	gatesfx->setDefault3DSoundMinDistance(100.f);
	gatesfx->setDefault3DSoundMaxDistance(5000.f);
	gatesfx->setSoundVolume(1.f);

	fountainsfx->play3D("Sound/fountain.mp3", irrklang::vec3df(0, 0, 0),true);

	//sound.playSoundEffect3D("Sound/fountain.mp3", irrklang::vec3df(0, 0, 0), true);

	initSceneObjects();

	SpawnGhost();

	//Loading dialogue
	ReadDialogue("Text//Dialogue_2.txt", Dialogue);

	lights[0].position.Set(130, 150, 100);
	lights[0].power = 2.f;

	InitPartitioning();

    spatialPartitioning = false;

}

void SceneLevel02::initSceneObjects()
{
	AABBObject * key = new AABBObject();
	key->Object = AABBObject::OBJECT_TYPE::KEY;
	key->active = true;
	key->pos.Set(1000, -35 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), 0);
	key->scale.Set(10, 10, 10);
	keyPtr = key;
	instance->Object_list.push_back(key);

	//Fountain
	AABBObject * Fountain = new AABBObject();
	Fountain->Object = AABBObject::OBJECT_TYPE::FOUNTAIN;
	Fountain->active = true;
	Fountain->pos.Set(0, -48 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), 0);
	Fountain->scale.Set(2, 2, 2);
	instance->Object_list.push_back(Fountain);

	//Hedges
	AABBObject * Hedge = new AABBObject();
	Hedge->Object = AABBObject::OBJECT_TYPE::HEDGE;
	Hedge->active = true;
	Hedge->pos.Set(200, -30 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), 100);
	Hedge->scale.Set(20, 20, 40);
	instance->Object_list.push_back(Hedge);

	Hedge = new AABBObject();
	Hedge->Object = AABBObject::OBJECT_TYPE::HEDGE;
	Hedge->active = true;
	Hedge->pos.Set(200, -30 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), -100);
	Hedge->scale.Set(20, 20, 40);
	instance->Object_list.push_back(Hedge);

	Hedge = new AABBObject();
	Hedge->Object = AABBObject::OBJECT_TYPE::HEDGE;
	Hedge->active = true;
	Hedge->pos.Set(-200, -30 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), 100);
	Hedge->scale.Set(20, 20, 40);
	instance->Object_list.push_back(Hedge);

	Hedge = new AABBObject();
	Hedge->Object = AABBObject::OBJECT_TYPE::HEDGE;
	Hedge->active = true;
	Hedge->pos.Set(-200, -30 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), -100);
	Hedge->scale.Set(20, 20, 40);
	instance->Object_list.push_back(Hedge);

	//Benches
	AABBObject * Bench = new AABBObject();
	Bench->Object = AABBObject::OBJECT_TYPE::BENCH;
	Bench->active = true;
	Bench->angle = -90;
	Bench->rotate.Set(0, 1, 0);
	Bench->pos.Set(200, -50 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), 0);
	Bench->scale.Set(5, 5, 5);
	instance->Object_list.push_back(Bench);

	Bench = new AABBObject();
	Bench->Object = AABBObject::OBJECT_TYPE::BENCH;
	Bench->active = true;
	Bench->angle = 90;
	Bench->rotate.Set(0, 1, 0);
	Bench->pos.Set(-200, -50 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), 0);
	Bench->scale.Set(5, 5, 5);
	instance->Object_list.push_back(Bench);

	//Houses
	for (int i = 0; i < 4; i++)
	{
		AABBObject * House = new AABBObject();
		House->active = true;
		House->angle = -90;
		House->rotate.Set(0, 1, 0);
		House->pos.Set(i * 450 - 750, -50 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), -700);
		House->scale.Set(200, 200, 200);

		if ((i % 2) == 0)
			House->Object = AABBObject::OBJECT_TYPE::HOUSE1;
		else
			House->Object = AABBObject::OBJECT_TYPE::HOUSE2;

		instance->Object_list.push_back(House);
	}


	//Pots
	Vector3 treePos1;
	Vector3 treePos2;
	Vector3 treePos3;
	Vector3 treePos4;

	treePos1.Set(-200, -60 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), -200);
	treePos2.Set(200, -60 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), -200);
	treePos3.Set(-200, -60 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), 200);
	treePos4.Set(200, -60 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), 200);

	AABBObject * Pot = new AABBObject();
	Pot->Object = AABBObject::OBJECT_TYPE::POT;
	Pot->active = true;
	Pot->pos = treePos1;
	Pot->scale.Set(20, 10, 20);
	instance->Object_list.push_back(Pot);

	Pot = new AABBObject();
	Pot->Object = AABBObject::OBJECT_TYPE::POT;
	Pot->active = true;
	Pot->pos = treePos2;
	Pot->scale.Set(20, 10, 20);
	instance->Object_list.push_back(Pot);

	Pot = new AABBObject();
	Pot->Object = AABBObject::OBJECT_TYPE::POT;
	Pot->active = true;
	Pot->pos = treePos3;
	Pot->scale.Set(20, 10, 20);
	instance->Object_list.push_back(Pot);

	Pot = new AABBObject();
	Pot->Object = AABBObject::OBJECT_TYPE::POT;
	Pot->active = true;
	Pot->pos = treePos4;
	Pot->scale.Set(20, 10, 20);
	instance->Object_list.push_back(Pot);

		//Brown building left
		AABBObject* wall = new AABBObject();
		wall->Object = AABBObject::OBJECT_TYPE::BOUNDARY;
		wall->active = true;
		wall->pos.Set(995,20,-235);
		wall->scale.Set(60, 20, 5);
		instance->Object_list.push_back(wall);
		//Brown building right
		wall = new AABBObject();
		wall->Object = AABBObject::OBJECT_TYPE::BOUNDARY;
		wall->active = true;
		wall->pos.Set(995, 20, 235);
		wall->scale.Set(60, 20, 5);
		instance->Object_list.push_back(wall);
		//Brown building back
		wall = new AABBObject();
		wall->Object = AABBObject::OBJECT_TYPE::BOUNDARY;
		wall->active = true;
		wall->pos.Set(1255, 20, 0);
		wall->scale.Set(5, 20, 60);
		instance->Object_list.push_back(wall);
		//Brown building front left
		wall = new AABBObject();
		wall->Object = AABBObject::OBJECT_TYPE::BOUNDARY;
		wall->active = true;
		wall->pos.Set(740, 20, -155);
		wall->scale.Set(5, 20, 23);
		instance->Object_list.push_back(wall);
		//Brown building front right
		wall = new AABBObject();
		wall->Object = AABBObject::OBJECT_TYPE::BOUNDARY;
		wall->active = true;
		wall->pos.Set(740, 20, 155);
		wall->scale.Set(5, 20, 23);
		instance->Object_list.push_back(wall);

		//back fence
		wall = new AABBObject();
		wall->Object = AABBObject::OBJECT_TYPE::BOUNDARY;
		wall->active = true;
		wall->pos.Set(1305, 20, -15);
		wall->scale.Set(5, 20, 220);
		instance->Object_list.push_back(wall);

		//Front fence >> Left
		wall = new AABBObject();
		wall->Object = AABBObject::OBJECT_TYPE::BOUNDARY;
		wall->active = true;
		wall->pos.Set(-1195, 20, 785);
		wall->scale.Set(5, 20, 50);
		instance->Object_list.push_back(wall);

		//Front fence >> Right
		wall = new AABBObject();
		wall->Object = AABBObject::OBJECT_TYPE::BOUNDARY;
		wall->active = true;
		wall->pos.Set(-1195, 20, -480);
		wall->scale.Set(5, 20, 120);
		instance->Object_list.push_back(wall);

		//left fence
		wall = new AABBObject();
		wall->Object = AABBObject::OBJECT_TYPE::BOUNDARY;
		wall->active = true;
		wall->pos.Set(55, 20, 1015);
		wall->scale.Set(250, 20, 5);
		instance->Object_list.push_back(wall);
		
		//right fence
		wall = new AABBObject();
		wall->Object = AABBObject::OBJECT_TYPE::BOUNDARY;
		wall->active = true;
		wall->pos.Set(55, 20, -1015);
		wall->scale.Set(250, 20, 5);
		instance->Object_list.push_back(wall);

		//Gate
		gatePtr = new AABBObject();
		gatePtr->Object = AABBObject::OBJECT_TYPE::BOUNDARY;
		gatePtr->active = true;
		gatePtr->pos.Set(-1190, 20, 335);
		gatePtr->scale.Set(5, 20, 120);
		instance->Object_list.push_back(gatePtr);

}

void SceneLevel02::Update(double dt)
{
	SceneBase::Update(dt);

    if (nightVision)
    {
        lights[0].power = 4.f;
        lights[0].color = (1.f, 1.f, 1.f);
        glUniform3fv(m_parameters[U_LIGHT0_COLOR], 1, &lights[0].color.r);
        glUniform1f(m_parameters[U_LIGHT0_POWER], lights[0].power);
    }
    else
    {
        lights[0].power = 0.5f;
        lights[0].color = (0.8f, 0.8f, 0.8f);
        glUniform3fv(m_parameters[U_LIGHT0_COLOR], 1, &lights[0].color.r);
        glUniform1f(m_parameters[U_LIGHT0_POWER], lights[0].power);
    }

	distanceLeft = (Vector3(-1300, 20, 335) - camera.position).Length();

	if (questToNextScene)
	{
		if (distanceLeft < 50)
		{
			fountainsfx->stopAllSounds();
			sound.stopMusic();
			Singleton::getInstance()->stateCheck = true;
			Singleton::getInstance()->program_state = Singleton::PROGRAM_HUB;
		}
	}
		
	if (Singleton::getInstance()->showInventory == false)
		camera.Update(dt);

	fountainsfx->setListenerPosition(irrklang::vec3df(camera.position.x, camera.position.y, camera.position.z), irrklang::vec3df(-camera.view.x, camera.view.y, -camera.view.z));
	gatesfx->setListenerPosition(irrklang::vec3df(camera.position.x, camera.position.y, camera.position.z), irrklang::vec3df(-camera.view.x, camera.view.y, -camera.view.z));

	static bool eButtonState = false;
	if (Application::IsKeyPressed('E'))
	{
		if (!eButtonState)
		{
			eButtonState = true;
			
			for (auto object : instance->Object_list)
			{
				if (object->active)
				{
					if (object->Object == AABBObject::OBJECT_TYPE::KEY && (keyPtr->pos - camera.position).Length() < 95 && cameraViewObject(keyPtr->pos, 80) == true)
					{
						sound.playSoundEffect2D("Sound/pickUp.mp3");
						Singleton::getInstance()->gotKey = true;
						Singleton::getInstance()->inventory2ndRow.push_back(Singleton::getInstance()->item_key);

						object->active = false;
					}
				}
			}

			if ((gatePtr->pos - camera.position).Length() < 95 && cameraViewObject(gatePtr->pos, 80) == true && Singleton::getInstance()->gotKey == true && openGate == false)
			{
				gatesfx->play3D("Sound/gateOpen.wav", irrklang::vec3df(-1190, 20, 335));
				openGate = true;
				gatePtr->active = false;
			}
		}
	}
	else if (!Application::IsKeyPressed('E'))
	{
		if (eButtonState)
			eButtonState = false;
	}

	if (openGate && rotateGate > 0)
	{
		rotateGate--;
	}

	/////////////////
	// for testing //
	/////////////////

	//add key to inventory
	static bool spaceButtonState2 = false;
	if (Application::IsKeyPressed('Y'))
	{
		if (!spaceButtonState2)
		{
			spaceButtonState2 = true;

			Singleton::getInstance()->gotKey = true;
			if (Singleton::getInstance()->inventory2ndRow.size() < 6)
				Singleton::getInstance()->inventory2ndRow.push_back(Singleton::getInstance()->item_key);
		}
	}
	else if (!Application::IsKeyPressed('Y'))
	{
		if (spaceButtonState2)
			spaceButtonState2 = false;
	}

	//add ghost1 to inventory
	static bool spaceButtonState3 = false;
	if (Application::IsKeyPressed('U'))
	{
		if (!spaceButtonState3)
		{
			spaceButtonState3 = true;

			Singleton::getInstance()->gotKey = true;
			if (Singleton::getInstance()->inventory.size() < 6)
				Singleton::getInstance()->inventory.push_back(Singleton::getInstance()->item_ghost1);
		}
	}
	else if (!Application::IsKeyPressed('U'))
	{
		if (spaceButtonState3)
			spaceButtonState3 = false;
	}

	//remove inventory slot 1
	static bool spaceButtonState4 = false;
	if (Application::IsKeyPressed('T'))
	{
		if (!spaceButtonState4)
		{
			spaceButtonState4 = true;
			if (Singleton::getInstance()->inventory.size() > 0)
				Singleton::getInstance()->inventory.erase(Singleton::getInstance()->inventory.begin());
		}
	}
	else if (!Application::IsKeyPressed('T'))
	{
		if (spaceButtonState4)
			spaceButtonState4 = false;
	}

	//Dialogues
	if (Dialogue_Selection != 2 &&
		Application::IsKeyPressed('E') &&
		Dialogue_Timer <= 0.0f)
	{
		Dialogue_Timer = 1.0f;
		Dialogue_Selection++;
	}
	else
	{
		if (!instance->gotKey)
		{
			Dialogue_Timer -= (float)dt;
		}
	}
	if (Dialogue_Selection == 2 &&
		Application::IsKeyPressed('E') &&
		Dialogue_Timer <= 0.0f &&
		!key_bool)
	{
		Dialogue_Timer = 1.0f;
		Dialogues = false;
	}
	if (instance->gotKey)
	{
		if (!key_bool)
		{
			Dialogues = true;
			key_bool = true;
		}
		Dialogue_Selection = 3;
	}
	if (Dialogue_Selection == 3 &&
		Application::IsKeyPressed('E') &&
		Dialogue_Timer2 <= 0.0f)
	{
		Dialogues = false;
		questToNextScene = true;
	}
	else
	{
		if (key_bool)
		{
			Dialogue_Timer2 -= (float)dt;
		}
	}

	UpdateParticle(dt);

	camera.Terrain = TERRAINSIZE.y * ReadHeightMap(m_heightMap, camera.position.x / TERRAINSIZE.x, camera.position.z / TERRAINSIZE.z);
	for (std::vector<Enemy *>::iterator it = instance->Enemy_list.begin(); it != instance->Enemy_list.end(); ++it)
	{
		Enemy *ghost = (Enemy *)*it;
		if (ghost->active)
		{
			ghost->pos.y = TERRAINSIZE.y * ReadHeightMap(m_heightMap, ghost->pos.x / TERRAINSIZE.x, ghost->pos.z / TERRAINSIZE.z);
		}
	}
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
	if (Application::IsKeyPressed('V'))
	{
		fountainsfx->stopAllSounds();
	}
	if (Application::IsKeyPressed('B'))
	{
		fountainsfx->stopAllSounds();
	}
	if (Application::IsKeyPressed('N'))
	{
		fountainsfx->stopAllSounds();
	}
	if (Application::IsKeyPressed('M'))
	{
		fountainsfx->stopAllSounds();
	}

	fps = (float)(1.f / dt);
}

void SceneLevel02::UpdateParticle(double dt)
{
	if (m_particlesCount < MAX_PARTICLE)
	{
		ParticleObject* particle = GetParticles();
		particle->type = PARTICLEOBJECT_TYPE::P_WATER;
		particle->pos.Set(Math::RandFloatMinMax(-1700.0f, 1700.0f), 1200.0f, Math::RandFloatMinMax(-1700.0f, 1700.0f));
		particle->vel.Set(0, -10, 0);
		particle->scale.Set(10, 10, 10);
		particle->rotateSpeed = Math::RandFloatMinMax(20.0f, 40.0f);

		ParticleObject* particleWater1 = GetParticles();
		particleWater1->type = PARTICLEOBJECT_TYPE::P_FOUNTAIN_WATER1;
		//particleSmoke->scale.Set(5, 5, 5);
		float range = 3;
		particleWater1->vel.Set(Math::RandFloatMinMax(-range, range), Math::RandFloatMinMax(10, 20),	Math::RandFloatMinMax(-range, range));
		particleWater1->rotateSpeed = Math::RandFloatMinMax(20.f, 40.f);
		//particleWater1->scale.Set(20, 20, 10);
		particleWater1->pos.Set(0, 0, 0);
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

				if (particle->pos.y < (ReadHeightMap(m_heightMap, particle->pos.x / TERRAINSIZE.x, particle->pos.z / TERRAINSIZE.z)  * TERRAINSIZE.y) - 50)
				{
					particle->active = false;
					m_particlesCount--;
				}
			}

			if (particle->type == PARTICLEOBJECT_TYPE::P_FOUNTAIN_WATER1)
			{
				particle->vel += m_gravity * ((float)dt * 2);
				particle->pos += particle->vel * (float)dt * 10.f;

				//if particle reaches the terrain, it should not be inacitve
				if (particle->pos.y < (ReadHeightMap(m_heightMap, particle->pos.x / TERRAINSIZE.x, particle->pos.z / TERRAINSIZE.z)  * TERRAINSIZE.y) - 50)
				{
					//particle->vel.y = particle->pos.y;
					particle->active = false;
					m_particlesCount--;
				}
			}
		}
	}
}

ParticleObject* SceneLevel02::GetParticles(void)
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

void SceneLevel02::RenderGround()
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

void SceneLevel02::RenderSkyplane()
{
	modelStack.PushMatrix();
	modelStack.Translate(camera.position.x, 1800, camera.position.z);
	modelStack.Rotate(rotateAngle, 0, 1, 0);
	RenderMesh(meshList[SKYPLANE], false);
	modelStack.PopMatrix();
}

void SceneLevel02::RenderTerrain()
{
	modelStack.PushMatrix();
	modelStack.Translate(0, -50, 0);
	modelStack.Scale(TERRAINSIZE.x, TERRAINSIZE.y, TERRAINSIZE.z);
	//RenderMesh(meshList[TERRAIN], true);
	modelStack.PopMatrix();
}

void SceneLevel02::RenderEnvironment(bool Light)
{
	modelStack.PushMatrix();
	modelStack.Translate(-1200, -48 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), 100);
	modelStack.Rotate(rotateGate, 0, 1, 0);
	modelStack.Scale(320, 320, 320);
	RenderMeshOutlined(meshList[METAL_GATE], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-1200, -48 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), 565);
	modelStack.Rotate(-rotateGate, 0, 1, 0);
	modelStack.Scale(320, 320, 320);
	RenderMeshOutlined(meshList[METAL_GATE], Light);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(1000, -48 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), 0);
	modelStack.Rotate(180, 0, 1, 0);
	modelStack.Scale(60, 60, 60);
	RenderMeshOutlined(meshList[HOUSE3], Light);
	modelStack.PopMatrix();

	for (int i = 0; i < 12; i++)
	{
		modelStack.PushMatrix();
		modelStack.Translate(i * 210 -1200, -50 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), -1000);
		modelStack.Scale(0.5, 0.5, 0.5);
		RenderMeshOutlined(meshList[METAL_FENCE], Light);
		modelStack.PopMatrix();
	}

	for (int i = 0; i < 12; i++)
	{
		modelStack.PushMatrix();
		modelStack.Translate(i * 210 - 1000, -50 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), 1000);
		modelStack.Rotate(180, 0, 1, 0);
		modelStack.Scale(0.5, 0.5, 0.5);
		RenderMeshOutlined(meshList[METAL_FENCE], Light);
		modelStack.PopMatrix();
	}

	for (int i = 0; i < 9; i++)
	{
		modelStack.PushMatrix();
		modelStack.Translate(-1200, -50 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), i * 225 - 800);
		modelStack.Rotate(90, 0, 1, 0);
		modelStack.Scale(0.5, 0.5, 0.5);
		if (i != 5 && i != 6)
			RenderMeshOutlined(meshList[METAL_FENCE], Light);
		modelStack.PopMatrix();
	}

	for (int i = 0; i < 9; i++)
	{
		modelStack.PushMatrix();
		modelStack.Translate(1300, -50 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), i * 225 - 1000);
		modelStack.Rotate(-90, 0, 1, 0);
		modelStack.Scale(0.5, 0.5, 0.5);
		RenderMeshOutlined(meshList[METAL_FENCE], Light);
		modelStack.PopMatrix();
	}
}
void SceneLevel02::RenderOthers(bool Light)
{
	Vector3 treePos1;
	Vector3 treePos2;
	Vector3 treePos3;
	Vector3 treePos4;

	treePos1.Set(-200, -60 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), -200);
	treePos2.Set(200, -60 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), -200);
	treePos3.Set(-200, -60 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), 200);
	treePos4.Set(200, -60 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), 200);

	modelStack.PushMatrix();
	modelStack.Translate(treePos1.x, treePos1.y + 75, treePos1.z);
	modelStack.Rotate(Math::RadianToDegree(atan2(camera.position.x - treePos1.x, camera.position.z - treePos1.z)), 0, 1, 0);
	modelStack.Scale(150, 150, 150);
	RenderMeshOutlined(meshList[COCONUT_TREE], Light);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(treePos2.x, treePos2.y + 75, treePos2.z);
	modelStack.Rotate(Math::RadianToDegree(atan2(camera.position.x - treePos2.x, camera.position.z - treePos2.z)), 0, 1, 0);
	modelStack.Scale(150, 150, 150);
	RenderMeshOutlined(meshList[COCONUT_TREE], Light);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(treePos3.x, treePos3.y + 75, treePos3.z);
	modelStack.Rotate(Math::RadianToDegree(atan2(camera.position.x - treePos3.x, camera.position.z - treePos3.z)), 0, 1, 0);
	modelStack.Scale(150, 150, 150);
	RenderMeshOutlined(meshList[COCONUT_TREE], Light);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(treePos4.x, treePos4.y + 75, treePos4.z);
	modelStack.Rotate(Math::RadianToDegree(atan2(camera.position.x - treePos4.x, camera.position.z - treePos4.z)), 0, 1, 0);
	modelStack.Scale(150, 150, 150);
	RenderMeshOutlined(meshList[COCONUT_TREE], Light);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0, -49 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), 0);
	modelStack.Rotate(-90, 1, 0, 0);
	modelStack.Scale(140, 140, 140);
	RenderMeshOutlined(meshList[FLOOR], Light);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-1300, treePos1.y + 120, 600);
	modelStack.Rotate(Math::RadianToDegree(atan2(-(600 - instance->player->getPosition().z), -1300 - instance->player->getPosition().x) - 90), 0, 1, 0);
	modelStack.Scale(250, 400, 250);
	RenderMeshOutlined(meshList[GEO_TREE_1], Light);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-1300, treePos1.y + 120, 130);
	modelStack.Rotate(Math::RadianToDegree(atan2(-(130 - instance->player->getPosition().z), -1300 - instance->player->getPosition().x) - 90), 0, 1, 0);
	modelStack.Scale(250, 400, 250);
	RenderMeshOutlined(meshList[GEO_TREE_1], Light);
	modelStack.PopMatrix();

	for (int i = 1; i < 5; ++i)
	{
		modelStack.PushMatrix();
		modelStack.Translate(-1400, -40, (600 - 94 * i));
		modelStack.Rotate(Math::RadianToDegree(atan2(-((600 - 94 * i) - instance->player->getPosition().z), -1400 - instance->player->getPosition().x) - 90), 0, 1, 0);
		modelStack.Scale(100, 100, 100);
		RenderMeshOutlined(meshList[GEO_BUSH], Light);
		modelStack.PopMatrix();
	}

	//paintings harambe
	modelStack.PushMatrix();
	modelStack.Translate(1229, 50 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), 95);
	modelStack.Scale(1, 80, 140);
	modelStack.Rotate(-90, 0, 1, 0);
	RenderMesh(meshList[PAINTING], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(1229, 50 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), -105);
	modelStack.Scale(1, 80, 140);
	modelStack.Rotate(-90, 0, 1, 0);
	RenderMesh(meshList[PAINTING2], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(1112, 50 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), -220);
	modelStack.Scale(140, 80, 1);
	RenderMesh(meshList[PAINTING], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(912, 50 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), -220);
	modelStack.Scale(140, 80, 1);
	RenderMesh(meshList[PAINTING2], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(1112, 50 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), 220);
	modelStack.Scale(140, 80, 1);
	modelStack.Rotate(180, 0, 1, 0);
	RenderMesh(meshList[PAINTING2], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(912, 50 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), 220);
	modelStack.Scale(140, 80, 1);
	modelStack.Rotate(180, 0, 1, 0);
	RenderMesh(meshList[PAINTING], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(768, 50 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), 142);
	modelStack.Scale(1, 80, 140);
	modelStack.Rotate(90, 0, 1, 0);
	RenderMesh(meshList[PAINTING2], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(768, 50 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), -142);
	modelStack.Scale(1, 80, 140);
	modelStack.Rotate(90, 0, 1, 0);
	RenderMesh(meshList[PAINTING], false);
	modelStack.PopMatrix();
}
void SceneLevel02::RenderHUD()
{
	RenderRadar();
}

void SceneLevel02::RenderSprite()
{
}

void SceneLevel02::RenderParticle(ParticleObject* particle)
{
	switch (particle->type)
	{
		posPartition = getPartition(particle->pos);
	case PARTICLEOBJECT_TYPE::P_FOUNTAIN_WATER1:
	{
		if (renderCheck(playerPartition, posPartition))
		{
			modelStack.PushMatrix();
			modelStack.Translate(particle->pos.x, particle->pos.y, particle->pos.z);
			//insert billboard code
			modelStack.Scale(particle->scale.x, particle->scale.y, particle->scale.z);
			RenderMesh(meshList[FOUNTAIN_WATER1], false);
			modelStack.PopMatrix();
		}
		break;
	}
	default:
		break;
	}
}

void SceneLevel02::RenderLight()
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

void SceneLevel02::RenderWorld()
{
	glUniform1f(m_parameters[U_FOG_ENABLE], 1);
	RenderSkyplane();
	RenderTerrain();
	RenderObjects(ShowHitbox);
	RenderEnvironment(false);
	RenderOthers(false);
	RenderEnemies(false);
	RenderBullets(false);
	//RenderSprite();
	glUniform1f(m_parameters[U_FOG_ENABLE], 0);
}

void SceneLevel02::RenderPassGPass()
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

void SceneLevel02::RenderPassMain()
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

	RenderWeapons(false);
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

	RenderWorld();

	for (std::vector<ParticleObject*>::iterator it = particleList.begin(); it != particleList.end(); ++it)
	{
		ParticleObject*particle = (ParticleObject*)*it;
		if (particle->active)
		{
			RenderParticle(particle);
		}
	}

	if (!Singleton::getInstance()->stateCheck)
	{
		RenderHUD();
	}

	// Render the crosshair
	RenderMeshIn2D(meshList[GEO_CROSSHAIR], false, 2.0f);

	//On screen text
	std::ostringstream ss;
    if (instance->stateCheck == false && instance->openDoor == false)
    {
        if (showText)
        {
            ss.precision(5);
            ss << "FPS: " << fps;
            RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 2, 3);

            ss.str("");
            ss.precision(5);
            ss << "POS: " << camera.position;
            RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 0, 0), 2.5f, 2, 6);
        }
        if (questToNextScene)
        {
            glDisable(GL_DEPTH_TEST);
            modelStack.PushMatrix();
            modelStack.Translate(-1500, 20, 335);
            modelStack.Rotate(Math::RadianToDegree(atan2(camera.position.x - -1500, camera.position.z - 335)), 0, 1, 0);
            modelStack.Scale(150, 150, 150);
            RenderMeshOutlined(meshList[QUEST_EXIT], false);
            modelStack.PopMatrix();
            glEnable(GL_DEPTH_TEST);
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
    }
	SceneBase::Render();
}

void SceneLevel02::Render()
{
	//PRE RENDER PASS
	RenderPassGPass();

	//MAIN RENDER PASS
	RenderPassMain();
}

void SceneLevel02::Exit()
{
//	delete level2_Heights;
	SceneBase::Exit();
}