#include "SceneLevel01.h"
#include "GL\glew.h"

#include "shader.hpp"
#include "Application.h"
#include "Utility.h"
#include <sstream>

SceneLevel01::SceneLevel01()
{
}

SceneLevel01::~SceneLevel01()
{
}

static const Vector3 TERRAINSIZE(4000.0f, 200.0f, 4000.0f);

void SceneLevel01::Init()
{
	sound.playMusic("Sound/level1bgm.mp3");

	Application::HideCursor();

	SceneBase::Init();
	
	lights[0].position.Set(0, -100, 0);

	timerstart = false;
	timer = 0.f;

	d1 = false;
	d2 = false;
	d3 = false;
	d4 = false;
	d5 = false;
	d6 = false;
	d7 = false;
	d8 = false;

	//level 1 terrain
	meshList[LEVEL01_TERRAIN] = MeshBuilder::GenerateTerrain("level01 terrain", "Image//Terrain//Terrain_Level01.raw", m_heightMap, level1_Heights);
	meshList[LEVEL01_TERRAIN]->textureArray[0] = LoadTGA("Image//Indoor//indoorFloor.tga");

	meshList[LEVEL01_WALLS] = MeshBuilder::GenerateQuad("walls", Color(0, 0, 0), 1.f);
	meshList[LEVEL01_WALLS]->textureArray[0] = LoadTGA("Image//Indoor//walltex.tga");

	meshList[STAIRS] = MeshBuilder::GenerateOBJ("stairs", "OBJ//Indoor//Stairs.obj");
	meshList[STAIRS]->textureArray[0] = LoadTGA("Image//Indoor//stairs.tga");

	meshList[LEVEL01] = MeshBuilder::GenerateOBJ("level01", "OBJ//Indoor//Level01.obj");
	meshList[LEVEL01]->textureArray[0] = LoadTGA("Image//Indoor//level1.tga");

	meshList[DOOR] = MeshBuilder::GenerateOBJ("Door", "OBJ//Indoor//door.obj");
	meshList[DOOR]->textureArray[0] = LoadTGA("Image//Indoor//door.tga");

	meshList[CHANDELIER] = MeshBuilder::GenerateOBJ("CHANDELIER", "OBJ//Indoor//chandelier.obj");
	meshList[CHANDELIER]->textureArray[0] = LoadTGA("Image//Indoor//chandelier.tga");

	meshList[CEILING] = MeshBuilder::GenerateQuad2("CEILING", Color(0, 0, 0), 1.f, 1.f, TexCoord(18, 6));
	meshList[CEILING]->textureArray[0] = LoadTGA("Image//Indoor//indoorCeiling.tga");

	meshList[ROOMCEILING] = MeshBuilder::GenerateQuad2("ROOMCEILING", Color(0, 0, 0), 1.f, 1.f, TexCoord(10, 10));
	meshList[ROOMCEILING]->textureArray[0] = LoadTGA("Image//Indoor//ceilingRoom.tga");

	meshList[BED] = MeshBuilder::GenerateOBJ("Bed", "OBJ//Indoor//bed.obj");
	meshList[BED]->textureArray[0] = LoadTGA("Image//Indoor//bed.tga");

	meshList[TOILETBOWL] = MeshBuilder::GenerateOBJ("TOILETBOWL", "OBJ//Indoor//toilet.obj");
	meshList[TOILETBOWL]->textureArray[0] = LoadTGA("Image//Indoor//toilet.tga");

	meshList[INDOORGATE] = MeshBuilder::GenerateOBJ("INDOORGATE", "OBJ//Indoor//indoorGate.obj");
	meshList[INDOORGATE]->textureArray[0] = LoadTGA("Image//Indoor//indoorGate.tga");

	meshList[BLOCKAGE] = MeshBuilder::GenerateOBJ("barricade", "OBJ//Indoor//barricade.obj");
	meshList[BLOCKAGE]->textureArray[0] = LoadTGA("Image//Indoor//Table.tga");

	meshList[TABLE] = MeshBuilder::GenerateOBJ("Table", "OBJ//Indoor//table.obj");
	meshList[TABLE]->textureArray[0] = LoadTGA("Image//Indoor//Table.tga");

	meshList[CHAIR] = MeshBuilder::GenerateOBJ("Chair", "OBJ//Indoor//chair.obj");
	meshList[CHAIR]->textureArray[0] = LoadTGA("Image//Indoor//chair.tga");

	meshList[ELEVATOR] = MeshBuilder::GenerateOBJ("ELEVATOR", "OBJ//Indoor//elevator.obj");
	meshList[ELEVATOR]->textureArray[0] = LoadTGA("Image//Indoor//elevator.tga");

	meshList[ELEVATORCOVER] = MeshBuilder::GenerateOBJ("ELEVATORCOVER", "OBJ//Indoor//elevatorCover.obj");
	meshList[ELEVATORCOVER]->textureArray[0] = LoadTGA("Image//Indoor//uvmap.tga");

	meshList[SOFA] = MeshBuilder::GenerateOBJ("SOFA", "OBJ//Indoor//sofa.obj");
	meshList[SOFA]->textureArray[0] = LoadTGA("Image//Indoor//sofa.tga");

	meshList[SHELF] = MeshBuilder::GenerateOBJ("SHELF", "OBJ//Indoor//shelf.obj");
	meshList[SHELF]->textureArray[0] = LoadTGA("Image//Indoor//shelf.tga");

	meshList[BEDSIDE] = MeshBuilder::GenerateOBJ("BEDSIDE", "OBJ//Indoor//bedside.obj");
	meshList[BEDSIDE]->textureArray[0] = LoadTGA("Image//Indoor//bedside.tga");

	meshList[SINK] = MeshBuilder::GenerateOBJ("SINK", "OBJ//Indoor//sink.obj");
	meshList[SINK]->textureArray[0] = LoadTGA("Image//Indoor//sink.tga");

	meshList[CRATE] = MeshBuilder::GenerateOBJ("CRATE", "OBJ//Indoor//crate.obj");
	meshList[CRATE]->textureArray[0] = LoadTGA("Image//Indoor//crate.tga");

	//harambe
	meshList[PAINTING] = MeshBuilder::GenerateQuad("PAINTING", Color(0, 0, 0), 1.f);
	meshList[PAINTING]->textureArray[0] = LoadTGA("Image//Indoor//paintingHarambe.tga");
	//starry night
	meshList[PAINTING2] = MeshBuilder::GenerateQuad("PAINTING2", Color(0, 0, 0), 1.f);
	meshList[PAINTING2]->textureArray[0] = LoadTGA("Image//Indoor//painting1.tga");
	//city lights
	meshList[PAINTING3] = MeshBuilder::GenerateQuad("PAINTING3", Color(0, 0, 0), 1.f);
	meshList[PAINTING3]->textureArray[0] = LoadTGA("Image//Indoor//painting2.tga");
	//ghost painting 1
	meshList[PAINTING4] = MeshBuilder::GenerateQuad("PAINTING4", Color(0, 0, 0), 1.f);
	meshList[PAINTING4]->textureArray[0] = LoadTGA("Image//Indoor//ghostpainting.tga");
	//ghost painting 1
	meshList[PAINTING5] = MeshBuilder::GenerateQuad("PAINTING5", Color(0, 0, 0), 1.f);
	meshList[PAINTING5]->textureArray[0] = LoadTGA("Image//Indoor//ghostpainting2.tga");

	InitPartitioning();

	camera.Init(Vector3(180, 5, -668), Vector3(179, 5, -668), Vector3(0, 1, 0));

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

	initSceneObjects();

	//SpawnGhost();

	//Loading dialogue
	ReadDialogue("Text//Dialogue_1.txt", Dialogue);
}

void SceneLevel01::Update(double dt)
{
	bLightEnabled = false;

	if (instance->openDoor == false)
		camera.Update(dt);

	SceneBase::Update(dt);

	UpdateParticle(dt);
	
	UpdateHitboxes(dt);

	camera.Terrain = TERRAINSIZE.y * ReadHeightMap(m_heightMap, camera.position.x / TERRAINSIZE.x, camera.position.z / TERRAINSIZE.z);
	
	for (std::vector<Enemy *>::iterator it = instance->Enemy_list.begin(); it != instance->Enemy_list.end(); ++it)
	{
		Enemy *ghost = (Enemy *)*it;
		if (ghost->active)
		{
			ghost->pos.y = TERRAINSIZE.y * ReadHeightMap(m_heightMap, ghost->pos.x / TERRAINSIZE.x, ghost->pos.z / TERRAINSIZE.z);
		}
	}

	static bool eButtonState = false;
	
	if (timerstart)
	{
		timer += dt;
		instance->openDoor = true;
	}

	if (timer > 3.f)
	{
		//left first front
		if (instance->singletonCamera->position.x < 460 && instance->singletonCamera->position.x > 340)
		{
			if (instance->singletonCamera->position.z > -200 && instance->singletonCamera->position.z < -100)
			{
				d1 = true;
				timer = 0.f;
				timerstart = false;
				instance->openDoor = false;
			}
		}
		//left first inside
		 if (instance->singletonCamera->position.x < 460 && instance->singletonCamera->position.x > 340)
		{
			if (instance->singletonCamera->position.z > -300 && instance->singletonCamera->position.z < -200)
			{
				d2 = true;
				timer = 0.f;
				timerstart = false;
				instance->openDoor = false;
			}
		}

		//left second front
		 if (instance->singletonCamera->position.x < 1360 && instance->singletonCamera->position.x > 1240)
		{
			if (instance->singletonCamera->position.z > -200 && instance->singletonCamera->position.z < -100)
			{
				d3 = true;
				timer = 0.f;
				timerstart = false;
				instance->openDoor = false;
			}
		}
		//left second inside
		 if (instance->singletonCamera->position.x < 1360 && instance->singletonCamera->position.x > 1240)
		{
			if (instance->singletonCamera->position.z > -300 && instance->singletonCamera->position.z < -200)
			{
				d4 = true;
				timer = 0.f;
				timerstart = false;
				instance->openDoor = false;
			}
		}

		//right first front
		 if (instance->singletonCamera->position.x < 460 && instance->singletonCamera->position.x > 340)
		{
			if (instance->singletonCamera->position.z > 300 && instance->singletonCamera->position.z < 400)
			{
				d5 = true;
				timer = 0.f;
				timerstart = false;
				instance->openDoor = false;
			}
		}
		//right first inside
		 if (instance->singletonCamera->position.x < 460 && instance->singletonCamera->position.x > 340)
		{
			if (instance->singletonCamera->position.z > 420 && instance->singletonCamera->position.z < 520)
			{
				d6 = true;
				timer = 0.f;
				timerstart = false;
				instance->openDoor = false;
			}
		}

		//right second front
		 if (instance->singletonCamera->position.x < 1360 && instance->singletonCamera->position.x > 1240)
		{
			if (instance->singletonCamera->position.z > 300 && instance->singletonCamera->position.z < 400)
			{
				d7 = true;
				timer = 0.f;
				timerstart = false;
				instance->openDoor = false;
			}
		}
		//right second inside
		 if (instance->singletonCamera->position.x < 1360 && instance->singletonCamera->position.x > 1240)
		{
			if (instance->singletonCamera->position.z > 420 && instance->singletonCamera->position.z < 520)
			{
				d8 = true;
				timer = 0.f;
				timerstart = false;
				instance->openDoor = false;
			}
		}
	}
	//left 1st
	if (d1)
	{
		instance->singletonCamera->position.x = 400;
		instance->singletonCamera->position.z = -250;
		instance->singletonCamera->target.z = -260;
		instance->singletonCamera->target.x = 400;
		d1 = false;
	}

	 if (d2)
	{
		instance->singletonCamera->position.x = 400;
		instance->singletonCamera->position.z = -180;
		instance->singletonCamera->target.z = -170;
		instance->singletonCamera->target.x = 400;
		d2 = false;
	}
	//left 2nd
	if (d3)
	{
		instance->singletonCamera->position.x = 1300;
		instance->singletonCamera->position.z = -250;
		instance->singletonCamera->target.z = -260;
		instance->singletonCamera->target.x = 1300;
		d3 = false;
	}

	 if (d4)
	{
		instance->singletonCamera->position.x = 1300;
		instance->singletonCamera->position.z = -180;
		instance->singletonCamera->target.z = -170;
		instance->singletonCamera->target.x = 1300;
		d4 = false;
	}
	//right 1st
	if (d5)
	{
		instance->singletonCamera->position.x = 400;
		instance->singletonCamera->position.z = 430;
		instance->singletonCamera->target.z = 440;
		instance->singletonCamera->target.x = 400;
		d5 = false;
	}

	 if (d6)
	{
		instance->singletonCamera->position.x = 400;
		instance->singletonCamera->position.z = 380;
		instance->singletonCamera->target.z = 370;
		instance->singletonCamera->target.x = 400;
		d6 = false;
	}
	//right 2nd
	if (d7)
	{
		instance->singletonCamera->position.x = 1300;
		instance->singletonCamera->position.z = 430;
		instance->singletonCamera->target.z = 440;
		instance->singletonCamera->target.x = 1300;
		d7 = false;
	}

	if (d8)
	{
		instance->singletonCamera->position.x = 1300;
		instance->singletonCamera->position.z = 380;
		instance->singletonCamera->target.z = 370;
		instance->singletonCamera->target.x = 1300;
		d8 = false;
	}

	if (Application::IsKeyPressed('E'))
	{
		if (!eButtonState)
		{
			eButtonState = true;
			for (auto object : instance->Object_list)
			{
				if (object->Object == AABBObject::OBJECT_TYPE::INDOORGATE && (exitPtr->pos - camera.position).Length() < 95 && cameraViewObject(exitPtr->pos, 80) == true)
				{
					if (instance->gotHammer)
					{
						sound.stopMusic();
						Singleton::getInstance()->stateCheck = true;
						Singleton::getInstance()->program_state = Singleton::PROGRAM_GAME2;
					}
				}

				if (object->Object == AABBObject::OBJECT_TYPE::DOOR && (door1->pos - camera.position).Length() < 95 && cameraViewObject(door1->pos, 80) == true)
				{
					timerstart = true;
				}

				if (object->Object == AABBObject::OBJECT_TYPE::DOOR && (door2->pos - camera.position).Length() < 95 && cameraViewObject(door2->pos, 80) == true)
				{
					timerstart = true;
				}

				if (object->Object == AABBObject::OBJECT_TYPE::DOOR && (door3->pos - camera.position).Length() < 95 && cameraViewObject(door3->pos, 80) == true)
				{
					timerstart = true;
				}

				if (object->Object == AABBObject::OBJECT_TYPE::DOOR && (door4->pos - camera.position).Length() < 95 && cameraViewObject(door4->pos, 80) == true)
				{
					timerstart = true;
				}

				if (object->Object == AABBObject::OBJECT_TYPE::DOOR && (door5->pos - camera.position).Length() < 95 && cameraViewObject(door5->pos, 80) == true)
				{
					timerstart = true;
				}

				if (object->Object == AABBObject::OBJECT_TYPE::DOOR && (door6->pos - camera.position).Length() < 95 && cameraViewObject(door6->pos, 80) == true)
				{
					timerstart = true;
				}

				if (object->Object == AABBObject::OBJECT_TYPE::DOOR && (door7->pos - camera.position).Length() < 95 && cameraViewObject(door7->pos, 80) == true)
				{
					timerstart = true;
				}

				if (object->Object == AABBObject::OBJECT_TYPE::DOOR && (door8->pos - camera.position).Length() < 95 && cameraViewObject(door8->pos, 80) == true)
				{
					timerstart = true;
				}
				if (object->active && cameraViewObject(hammerPtr->pos, 80) == true)
				{
					if (object->Object == AABBObject::OBJECT_TYPE::HAMMER && (hammerPtr->pos - camera.position).Length() < 95)
					{
						sound.playSoundEffect2D("Sound/pickUp.mp3");
						Singleton::getInstance()->gotHammer = true;
						Singleton::getInstance()->inventory2ndRow.push_back(Singleton::getInstance()->item_hammer);

						object->active = false;
					}
				}
			}	
		}
	}
	else if (!Application::IsKeyPressed('E'))
	{
		if (eButtonState)
			eButtonState = false;
	}

	if (Dialogue_Selection != 2 && 
		Application::IsKeyPressed('E')&&
		Dialogue_Timer <= 0.0f)
	{
		Dialogue_Timer = 1.0f;
		Dialogue_Selection++;
	}
	else
	{
		if (!instance->gotHammer)
		{
			Dialogue_Timer -= (float)dt;
		}
	}
	if (Dialogue_Selection == 2 &&
		Application::IsKeyPressed('E') &&
		Dialogue_Timer <= 0.0f &&
		!hammer_bool)
	{
		Dialogue_Timer = 1.0f;
		Dialogues = false;
	}
	if (instance->gotHammer)
	{
		if (!hammer_bool)
		{
			Dialogues = true;
			hammer_bool = true;
		}
		Dialogue_Selection = 3;
	}
	if (Dialogue_Selection == 3 &&
		Application::IsKeyPressed('E') &&
		Dialogue_Timer2 <= 0.0f)
	{
		Dialogues = false;
	}
	else
	{
		if (hammer_bool)
		{
			Dialogue_Timer2 -= (float)dt;
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
			lights[1].power = 15.0f;
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

void SceneLevel01::initSceneObjects()
{
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//[Left] first room 
	//(front doors)
	AABBObject* door = new AABBObject();
	door->Object = AABBObject::OBJECT_TYPE::DOOR;
	door->active = true;
	door->pos.Set(400, -50 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), -203);
	door->scale.Set(4, 4, 4);
	door1 = door;
	instance->Object_list.push_back(door);

	door = new AABBObject();
	door->Object = AABBObject::OBJECT_TYPE::DOOR;
	door->active = true;
	door->pos.Set(400, -50 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), -212);
	door->scale.Set(4, 4, 4);
	door2 = door;
	instance->Object_list.push_back(door);

	//(chairs)
	AABBObject* chair = new AABBObject();
	chair->Object = AABBObject::OBJECT_TYPE::CHAIR;
	chair->active = true;
	chair->pos.Set(590, -50 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), -630);
	chair->scale.Set(1, 1, 1);
	instance->Object_list.push_back(chair);

	chair = new AABBObject();
	chair->Object = AABBObject::OBJECT_TYPE::CHAIR;
	chair->active = true;
	chair->angle = 180;
	chair->rotate.Set(0, 1, 0);
	chair->pos.Set(590, -50 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), -570);
	chair->scale.Set(1, 1, 1);
	instance->Object_list.push_back(chair);

	//(table)
	AABBObject* table = new AABBObject();
	table->Object = AABBObject::OBJECT_TYPE::TABLE;
	table->active = true;
	table->pos.Set(590, -50 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), -600);
	table->scale.Set(1, 1, 1);
	instance->Object_list.push_back(table);

	//(Sofa)
	AABBObject* sofa = new AABBObject();
	sofa->Object = AABBObject::OBJECT_TYPE::SOFA;
	sofa->active = true;
	sofa->pos.Set(200, -50 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), -610);
	sofa->scale.Set(7, 7, 7);
	instance->Object_list.push_back(sofa);

	sofa = new AABBObject();
	sofa->Object = AABBObject::OBJECT_TYPE::SOFA;
	sofa->active = true;
	sofa->angle = 180;
	sofa->rotate.Set(0, 1, 0);
	sofa->pos.Set(250, -50 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), -450);
	sofa->scale.Set(7, 7, 7);
	instance->Object_list.push_back(sofa);

	//(Toiletbowl)
	AABBObject* toilet = new AABBObject();
	toilet->Object = AABBObject::OBJECT_TYPE::TOILETBOWL;
	toilet->active = true;
	toilet->angle = 180;
	toilet->rotate.Set(0, 1, 0);
	toilet->pos.Set(600, -50 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), -240);
	toilet->scale.Set(1, 1, 1);
	instance->Object_list.push_back(toilet);

	AABBObject* sink = new AABBObject();
	sink->Object = AABBObject::OBJECT_TYPE::SINK;
	sink->active = true;
	sink->pos.Set(650, -45 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), -380);
	sink->scale.Set(20, 20, 20);
	instance->Object_list.push_back(sink);

	//(bedside table)
	AABBObject* bedside = new AABBObject();
	bedside->Object = AABBObject::OBJECT_TYPE::BEDSIDE;
	bedside->active = true;
	bedside->angle = 90;
	bedside->rotate.Set(0, 1, 0);
	bedside->pos.Set(120, -50 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), -675);
	bedside->scale.Set(2, 2, 2);
	instance->Object_list.push_back(bedside);

	AABBObject* bed = new AABBObject();
	bed->Object = AABBObject::OBJECT_TYPE::BED;
	bed->active = true;
	bed->angle = 90;
	bed->rotate.Set(0, 1, 0);
	bed->pos.Set(150, -30 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), -750);
	bed->scale.Set(25, 25, 25);
	instance->Object_list.push_back(bed);

	//(Shelf)
	AABBObject* shelf = new AABBObject();
	shelf->Object = AABBObject::OBJECT_TYPE::SHELF;
	shelf->active = true;
	shelf->angle = 180;
	shelf->rotate.Set(0, 1, 0);
	shelf->pos.Set(330, -50 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), -656);
	shelf->scale.Set(10, 10, 10);
	instance->Object_list.push_back(shelf);

	shelf = new AABBObject();
	shelf->Object = AABBObject::OBJECT_TYPE::SHELF;
	shelf->active = true;
	shelf->angle = 180;
	shelf->rotate.Set(0, 1, 0);
	shelf->pos.Set(275, -50 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), -656);
	shelf->scale.Set(10, 10, 10);
	instance->Object_list.push_back(shelf);

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//[Left] second room 
	//(front doors)
	door = new AABBObject();
	door->Object = AABBObject::OBJECT_TYPE::DOOR;
	door->active = true;
	door->pos.Set(1300, -50 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), -203);
	door->scale.Set(4, 4, 4);
	door3 = door;
	instance->Object_list.push_back(door);

	door = new AABBObject();
	door->Object = AABBObject::OBJECT_TYPE::DOOR;
	door->active = true;
	door->pos.Set(1300, -50 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), -212);
	door->scale.Set(4, 4, 4);
	door4 = door;
	instance->Object_list.push_back(door);

	//(chairs)
	chair = new AABBObject();
	chair->Object = AABBObject::OBJECT_TYPE::CHAIR;
	chair->active = true;
	chair->pos.Set(1090, -50 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), -750);
	chair->scale.Set(1, 1, 1);
	instance->Object_list.push_back(chair);

	chair = new AABBObject();
	chair->Object = AABBObject::OBJECT_TYPE::CHAIR;
	chair->active = true;
	chair->angle = 180;
	chair->rotate.Set(0, 1, 0);
	chair->pos.Set(1090, -50 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), -690);
	chair->scale.Set(1, 1, 1);
	instance->Object_list.push_back(chair);

	//(table)
	table = new AABBObject();
	table->Object = AABBObject::OBJECT_TYPE::TABLE;
	table->active = true;
	table->pos.Set(1093, -50 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), -720);
	table->scale.Set(1, 1, 1);
	instance->Object_list.push_back(table);

	//(Sofa)
	sofa = new AABBObject();
	sofa->Object = AABBObject::OBJECT_TYPE::SOFA;
	sofa->active = true;
	sofa->angle = 180;
	sofa->rotate.Set(0, 1, 0);
	sofa->pos.Set(1050, -50 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), -250);
	sofa->scale.Set(7, 7, 7);
	instance->Object_list.push_back(sofa);

	sofa = new AABBObject();
	sofa->Object = AABBObject::OBJECT_TYPE::SOFA;
	sofa->active = true;
	sofa->angle = 180;
	sofa->rotate.Set(0, 1, 0);
	sofa->pos.Set(1530, -50 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), -250);
	sofa->scale.Set(7, 7, 7);
	instance->Object_list.push_back(sofa);

	sofa = new AABBObject();
	sofa->Object = AABBObject::OBJECT_TYPE::SOFA;
	sofa->active = true;
	sofa->pos.Set(1530, -50 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), -600);
	sofa->scale.Set(7, 7, 7);
	instance->Object_list.push_back(sofa);

	sofa = new AABBObject();
	sofa->Object = AABBObject::OBJECT_TYPE::SOFA;
	sofa->active = true;
	sofa->pos.Set(1050, -50 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), -600);
	sofa->scale.Set(7, 7, 7);
	instance->Object_list.push_back(sofa);

	//(Toiletbowl)
	toilet = new AABBObject();
	toilet->Object = AABBObject::OBJECT_TYPE::TOILETBOWL;
	toilet->active = true;
	toilet->angle = 90;
	toilet->rotate.Set(0, 1, 0);
	toilet->pos.Set(1200, -50 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), -405);
	toilet->scale.Set(1, 1, 1);
	instance->Object_list.push_back(toilet);

	//(bedside table)
	bedside = new AABBObject();
	bedside->Object = AABBObject::OBJECT_TYPE::BEDSIDE;
	bedside->active = true;
	bedside->angle = -90;
	bedside->rotate.Set(0, 1, 0);
	bedside->pos.Set(1560, -50 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), -680);
	bedside->scale.Set(2, 2, 2);
	instance->Object_list.push_back(bedside);

	bed = new AABBObject();
	bed->Object = AABBObject::OBJECT_TYPE::BED;
	bed->active = true;
	bed->angle = -90;
	bed->rotate.Set(0, 1, 0);
	bed->pos.Set(1530, -30 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), -750);
	bed->scale.Set(25, 25, 25);
	instance->Object_list.push_back(bed);

	//(Shelf)
	shelf = new AABBObject();
	shelf->Object = AABBObject::OBJECT_TYPE::SHELF;
	shelf->active = true;
	shelf->pos.Set(1260, -50 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), -782);
	shelf->scale.Set(10, 10, 10);
	instance->Object_list.push_back(shelf);

	shelf = new AABBObject();
	shelf->Object = AABBObject::OBJECT_TYPE::SHELF;
	shelf->active = true;
	shelf->pos.Set(1360, -50 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), -782);
	shelf->scale.Set(10, 10, 10);
	instance->Object_list.push_back(shelf);

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//[Right] first room (front doors)
	door = new AABBObject();
	door->Object = AABBObject::OBJECT_TYPE::DOOR;
	door->active = true;
	door->pos.Set(400, -50 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), 404);
	door->scale.Set(4, 4, 4);
	door5 = door;
	instance->Object_list.push_back(door);

	door = new AABBObject();
	door->Object = AABBObject::OBJECT_TYPE::DOOR;
	door->active = true;
	door->pos.Set(400, -50 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), 413);
	door->scale.Set(4, 4, 4);
	door6 = door;
	instance->Object_list.push_back(door);

	//(chairs)
	chair = new AABBObject();
	chair->Object = AABBObject::OBJECT_TYPE::CHAIR;
	chair->active = true;
	chair->pos.Set(500, -50 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), 610);
	chair->scale.Set(1, 1, 1);
	instance->Object_list.push_back(chair);

	chair = new AABBObject();
	chair->Object = AABBObject::OBJECT_TYPE::CHAIR;
	chair->active = true;
	chair->angle = 180;
	chair->rotate.Set(0, 1, 0);
	chair->pos.Set(500, -50 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), 670);
	chair->scale.Set(1, 1, 1);
	instance->Object_list.push_back(chair);

	//(table)
	table = new AABBObject();
	table->Object = AABBObject::OBJECT_TYPE::TABLE;
	table->active = true;
	table->pos.Set(500, -50 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), 640);
	table->scale.Set(1, 1, 1);
	instance->Object_list.push_back(table);

	//(sofa)
	sofa = new AABBObject();
	sofa->Object = AABBObject::OBJECT_TYPE::SOFA;
	sofa->active = true;
	sofa->angle = 180;
	sofa->rotate.Set(0, 1, 0);
	sofa->pos.Set(625, -50 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), 970);
	sofa->scale.Set(7, 7, 7);
	instance->Object_list.push_back(sofa);

	sofa = new AABBObject();
	sofa->Object = AABBObject::OBJECT_TYPE::SOFA;
	sofa->active = true;
	sofa->pos.Set(625, -50 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), 770);
	sofa->scale.Set(7, 7, 7);
	instance->Object_list.push_back(sofa);

	//(Toiletbowl)
	toilet = new AABBObject();
	toilet->Object = AABBObject::OBJECT_TYPE::TOILETBOWL;
	toilet->active = true;
	toilet->pos.Set(310, -50 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), 580);
	toilet->scale.Set(1, 1, 1);
	instance->Object_list.push_back(toilet);

	sink = new AABBObject();
	sink->Object = AABBObject::OBJECT_TYPE::SINK;
	sink->active = true;
	sink->angle = 180;
	sink->rotate.Set(0, 1, 0);
	sink->pos.Set(320, -45 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), 730);
	sink->scale.Set(20, 20, 20);
	instance->Object_list.push_back(sink);
	
	//(bedside table)
	bedside = new AABBObject();
	bedside->Object = AABBObject::OBJECT_TYPE::BEDSIDE;
	bedside->active = true;
	bedside->angle = 90;
	bedside->rotate.Set(0, 1, 0);
	bedside->pos.Set(120, -50 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), 520);
	bedside->scale.Set(2, 2, 2);
	instance->Object_list.push_back(bedside);

	bedside = new AABBObject();
	bedside->Object = AABBObject::OBJECT_TYPE::BEDSIDE;
	bedside->active = true;
	bedside->angle = 90;
	bedside->rotate.Set(0, 1, 0);
	bedside->pos.Set(120, -50 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), 690);
	bedside->scale.Set(2, 2, 2);
	instance->Object_list.push_back(bedside);

	//(bed)
	bed = new AABBObject();
	bed->Object = AABBObject::OBJECT_TYPE::BED;
	bed->active = true;
	bed->angle = 90;
	bed->rotate.Set(0, 1, 0);
	bed->pos.Set(150, -30 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), 460);
	bed->scale.Set(25, 25, 25);
	instance->Object_list.push_back(bed);

	bed = new AABBObject();
	bed->Object = AABBObject::OBJECT_TYPE::BED;
	bed->active = true;
	bed->angle = 90;
	bed->rotate.Set(0, 1, 0);
	bed->pos.Set(150, -30 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), 750);
	bed->scale.Set(25, 25, 25);
	instance->Object_list.push_back(bed);

	//(shelf)
	shelf = new AABBObject();
	shelf->Object = AABBObject::OBJECT_TYPE::SHELF;
	shelf->active = true;
	shelf->angle = 180;
	shelf->rotate.Set(0, 1, 0);
	shelf->pos.Set(220, -50 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), 950);
	shelf->scale.Set(10, 10, 10);
	instance->Object_list.push_back(shelf);

	shelf = new AABBObject();
	shelf->Object = AABBObject::OBJECT_TYPE::SHELF;
	shelf->active = true;
	shelf->angle = 180;
	shelf->rotate.Set(0, 1, 0);
	shelf->pos.Set(165, -50 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), 950);
	shelf->scale.Set(10, 10, 10);
	instance->Object_list.push_back(shelf);
	//475 957
	shelf = new AABBObject();
	shelf->Object = AABBObject::OBJECT_TYPE::SHELF;
	shelf->active = true;
	shelf->angle = 180;
	shelf->rotate.Set(0, 1, 0);
	shelf->pos.Set(475, -50 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), 980);
	shelf->scale.Set(10, 10, 10);
	instance->Object_list.push_back(shelf);

	shelf = new AABBObject();
	shelf->Object = AABBObject::OBJECT_TYPE::SHELF;
	shelf->active = true;
	shelf->angle = 180;
	shelf->rotate.Set(0, 1, 0);
	shelf->pos.Set(400, -50 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), 980);
	shelf->scale.Set(10, 10, 10);
	instance->Object_list.push_back(shelf);

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//[Right] second room (front doors)
	door = new AABBObject();
	door->Object = AABBObject::OBJECT_TYPE::DOOR;
	door->active = true;
	door->pos.Set(1300, -50 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), 404);
	door->scale.Set(4, 4, 4);
	door7 = door;
	instance->Object_list.push_back(door);

	door = new AABBObject();
	door->Object = AABBObject::OBJECT_TYPE::DOOR;
	door->active = true;
	door->pos.Set(1300, -50 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), 413);
	door->scale.Set(4, 4, 4);
	door8 = door;
	instance->Object_list.push_back(door);

	//(chairs)
	chair = new AABBObject();
	chair->Object = AABBObject::OBJECT_TYPE::CHAIR;
	chair->active = true;
	chair->pos.Set(1400, -50 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), 610);
	chair->scale.Set(1, 1, 1);
	instance->Object_list.push_back(chair);

	chair = new AABBObject();
	chair->Object = AABBObject::OBJECT_TYPE::CHAIR;
	chair->active = true;
	chair->angle = 180;
	chair->rotate.Set(0, 1, 0);
	chair->pos.Set(1400, -50 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), 670);
	chair->scale.Set(1, 1, 1);
	instance->Object_list.push_back(chair);

	//(table)
	table = new AABBObject();
	table->Object = AABBObject::OBJECT_TYPE::TABLE;
	table->active = true;
	table->pos.Set(1400, -50 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), 640);
	table->scale.Set(1, 1, 1);
	instance->Object_list.push_back(table);

	//(bedside)
	bedside = new AABBObject();
	bedside->Object = AABBObject::OBJECT_TYPE::BEDSIDE;
	bedside->active = true;
	bedside->angle = -90;
	bedside->rotate.Set(0, 1, 0);
	bedside->pos.Set(1570, -50 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), 510);
	bedside->scale.Set(2, 2, 2);
	instance->Object_list.push_back(bedside);

	//(bed)
	bed = new AABBObject();
	bed->Object = AABBObject::OBJECT_TYPE::BED;
	bed->active = true;
	bed->angle = -90;
	bed->rotate.Set(0, 1, 0);
	bed->pos.Set(1520, -30 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), 460);
	bed->scale.Set(25, 25, 25);
	instance->Object_list.push_back(bed);

	//(shelf)
	shelf = new AABBObject();
	shelf->Object = AABBObject::OBJECT_TYPE::SHELF;
	shelf->active = true;
	shelf->angle = 180;
	shelf->rotate.Set(0, 1, 0);
	shelf->pos.Set(1560, -50 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), 630);
	shelf->scale.Set(10, 10, 10);
	instance->Object_list.push_back(shelf);

	shelf = new AABBObject();
	shelf->Object = AABBObject::OBJECT_TYPE::SHELF;
	shelf->active = true;
	shelf->pos.Set(1560, -50 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), 655);
	shelf->scale.Set(10, 10, 10);
	instance->Object_list.push_back(shelf);

	//(sink)
	sink = new AABBObject();
	sink->Object = AABBObject::OBJECT_TYPE::SINK;
	sink->active = true;
	sink->pos.Set(1090, -45 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), 640);
	sink->scale.Set(20, 20, 20);
	instance->Object_list.push_back(sink);

	sink = new AABBObject();
	sink->Object = AABBObject::OBJECT_TYPE::SINK;
	sink->active = true;
	sink->angle = 90;
	sink->rotate.Set(0, 1, 0);
	sink->pos.Set(1005, -45 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), 720);
	sink->scale.Set(20, 20, 20);
	instance->Object_list.push_back(sink);

	//(Toiletbowl)
	toilet = new AABBObject();
	toilet->Object = AABBObject::OBJECT_TYPE::TOILETBOWL;
	toilet->active = true;
	toilet->pos.Set(1140, -50 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), 650);
	toilet->scale.Set(1, 1, 1);
	instance->Object_list.push_back(toilet);

	toilet = new AABBObject();
	toilet->Object = AABBObject::OBJECT_TYPE::TOILETBOWL;
	toilet->active = true;
	toilet->angle = 90;
	toilet->rotate.Set(0, 1, 0);
	toilet->pos.Set(1010, -50 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), 770);
	toilet->scale.Set(1, 1, 1);
	instance->Object_list.push_back(toilet);

	//(crates)
	AABBObject* crate = new AABBObject();
	crate->Object = AABBObject::OBJECT_TYPE::CRATE;
	crate->active = true;
	crate->pos.Set(1350, -10 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), 900);
	crate->scale.Set(40, 40, 40);
	instance->Object_list.push_back(crate);

	crate = new AABBObject();
	crate->Object = AABBObject::OBJECT_TYPE::CRATE;
	crate->active = true;
	crate->pos.Set(1350, -10 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), 950);
	crate->scale.Set(40, 40, 40);
	instance->Object_list.push_back(crate);

	crate = new AABBObject();
	crate->Object = AABBObject::OBJECT_TYPE::CRATE;
	crate->active = true;
	crate->pos.Set(1350, -50 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), 980);
	crate->scale.Set(40, 40, 40);
	instance->Object_list.push_back(crate);

	crate = new AABBObject();
	crate->Object = AABBObject::OBJECT_TYPE::CRATE;
	crate->active = true;
	crate->pos.Set(1350, -50 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), 930);
	crate->scale.Set(40, 40, 40);
	instance->Object_list.push_back(crate);

	crate = new AABBObject();
	crate->Object = AABBObject::OBJECT_TYPE::CRATE;
	crate->active = true;
	crate->pos.Set(1350, -50 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), 880);
	crate->scale.Set(40, 40, 40);
	instance->Object_list.push_back(crate);

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//Misc OBJs

	AABBObject* hammer = new AABBObject();
	hammer->Object = AABBObject::OBJECT_TYPE::HAMMER;
	hammer->active = true;
	hammer->pos.Set(1525, -30 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), 925);
	hammer->scale.Set(10, 10, 10);
	hammerPtr = hammer;
	instance->Object_list.push_back(hammer);

	AABBObject* indoorGate = new AABBObject();
	indoorGate->Object = AABBObject::OBJECT_TYPE::INDOORGATE;
	indoorGate->active = true;
	indoorGate->pos.Set(-400, -50 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), 97);
	indoorGate->scale.Set(1, 1.2, 1);
	exitPtr = indoorGate;
	instance->Object_list.push_back(indoorGate);
	//1
	AABBObject* elevatorCover = new AABBObject();
	elevatorCover->Object = AABBObject::OBJECT_TYPE::ELEVATORCOVER;
	elevatorCover->active = true;
	elevatorCover->pos.Set(630, -50 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), 10);
	elevatorCover->scale.Set(80, 80, 80);
	instance->Object_list.push_back(elevatorCover);

	AABBObject* elevator = new AABBObject();
	elevator->Object = AABBObject::OBJECT_TYPE::ELEVATOR;
	elevator->active = true;
	elevator->pos.Set(630, -50 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), 10);
	elevator->scale.Set(80, 80, 80);
	instance->Object_list.push_back(elevator);
	//2
	elevatorCover = new AABBObject();
	elevatorCover->Object = AABBObject::OBJECT_TYPE::ELEVATORCOVER;
	elevatorCover->active = true;
	elevatorCover->pos.Set(840, -50 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), 10);
	elevatorCover->scale.Set(80, 80, 80);
	instance->Object_list.push_back(elevatorCover);

	elevator = new AABBObject();
	elevator->Object = AABBObject::OBJECT_TYPE::ELEVATOR;
	elevator->active = true;
	elevator->pos.Set(840, -50 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), 10);
	elevator->scale.Set(80, 80, 80);
	instance->Object_list.push_back(elevator);
	//3
	elevatorCover = new AABBObject();
	elevatorCover->Object = AABBObject::OBJECT_TYPE::ELEVATORCOVER;
	elevatorCover->active = true;
	elevatorCover->pos.Set(1050, -50 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), 10);
	elevatorCover->scale.Set(80, 80, 80);
	instance->Object_list.push_back(elevatorCover);

	elevator = new AABBObject();
	elevator->Object = AABBObject::OBJECT_TYPE::ELEVATOR;
	elevator->active = true;
	elevator->pos.Set(1050, -50 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), 10);
	elevator->scale.Set(80, 80, 80);
	instance->Object_list.push_back(elevator);
	//4
	elevatorCover = new AABBObject();
	elevatorCover->Object = AABBObject::OBJECT_TYPE::ELEVATORCOVER;
	elevatorCover->active = true;
	elevatorCover->pos.Set(630, -50 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), 190.1);
	elevatorCover->scale.Set(80, 80, 80);
	instance->Object_list.push_back(elevatorCover);

	elevator = new AABBObject();
	elevator->Object = AABBObject::OBJECT_TYPE::ELEVATOR;
	elevator->active = true;
	elevator->angle = 180;
	elevator->rotate.Set(0, 1, 0);
	elevator->pos.Set(630, -50 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), 190.1);
	elevator->scale.Set(80, 80, 80);
	instance->Object_list.push_back(elevator);
	//5
	elevatorCover = new AABBObject();
	elevatorCover->Object = AABBObject::OBJECT_TYPE::ELEVATORCOVER;
	elevatorCover->active = true;
	elevatorCover->pos.Set(840, -50 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), 190.1);
	elevatorCover->scale.Set(80, 80, 80);
	instance->Object_list.push_back(elevatorCover);

	elevator = new AABBObject();
	elevator->Object = AABBObject::OBJECT_TYPE::ELEVATOR;
	elevator->active = true;
	elevator->angle = 180;
	elevator->rotate.Set(0, 1, 0);
	elevator->pos.Set(840, -50 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), 190.1);
	elevator->scale.Set(80, 80, 80);
	instance->Object_list.push_back(elevator);
	//6
	elevatorCover = new AABBObject();
	elevatorCover->Object = AABBObject::OBJECT_TYPE::ELEVATORCOVER;
	elevatorCover->active = true;
	elevatorCover->pos.Set(1050, -50 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), 190.1);
	elevatorCover->scale.Set(80, 80, 80);
	instance->Object_list.push_back(elevatorCover);

	elevator = new AABBObject();
	elevator->Object = AABBObject::OBJECT_TYPE::ELEVATOR;
	elevator->active = true;
	elevator->angle = 180;
	elevator->rotate.Set(0, 1, 0);
	elevator->pos.Set(1050, -50 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), 190.1);
	elevator->scale.Set(80, 80, 80);
	instance->Object_list.push_back(elevator);

///////////////////////////////////////////////////////////WALLS//////////////////////////////////////////////////////
	//FIRST WALLS, LEFT SIDE
	AABBObject* walls = new AABBObject();
	walls->Object = AABBObject::OBJECT_TYPE::BOUNDARY;
	walls->active = true;
	walls->pos.Set(-95, -30 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, -95 / TERRAINSIZE.x, -15 / TERRAINSIZE.z), -100);
	walls->scale.Set(70, 25, 22);
	instance->Object_list.push_back(walls);
	
	//FIRST WALLS, RIGHT SIDE
	walls = new AABBObject();
	walls->Object = AABBObject::OBJECT_TYPE::BOUNDARY;
	walls->active = true;
	walls->pos.Set(-95, -30 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, -95 / TERRAINSIZE.x, -15 / TERRAINSIZE.z), 305);
	walls->scale.Set(70, 25, 22);
	instance->Object_list.push_back(walls);

	//FIRST DOOR, LEFT SIDE, WALL
	walls = new AABBObject();
	walls->Object = AABBObject::OBJECT_TYPE::BOUNDARY;
	walls->active = true;
	walls->pos.Set(400, -50 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), -203);
	walls->scale.Set(29, 25, 5);
	instance->Object_list.push_back(walls);
	
	//FIRST DOOR, RIGHT SIDE, WALL
	walls = new AABBObject();
	walls->Object = AABBObject::OBJECT_TYPE::BOUNDARY;
	walls->active = true;
	walls->pos.Set(842, -30 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), -100);
	walls->scale.Set(61, 25, 22);
	instance->Object_list.push_back(walls);

	//SECOND SET OF WALLS, LEFT SIDE
	walls = new AABBObject();
	walls->Object = AABBObject::OBJECT_TYPE::BOUNDARY;
	walls->active = true;
	walls->pos.Set(842, -30 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), 305);
	walls->scale.Set(61, 25, 22);
	instance->Object_list.push_back(walls);

	//SECOND SET OF WALLS, RIGHT SIDE
	walls = new AABBObject();
	walls->Object = AABBObject::OBJECT_TYPE::BOUNDARY;
	walls->active = true;
	walls->pos.Set(400, -50 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), 404);
	walls->scale.Set(29, 25, 5);
	instance->Object_list.push_back(walls);

	//SECOND SET OF WALLS, LEFT SIDE (DOOR)
	walls = new AABBObject();
	walls->Object = AABBObject::OBJECT_TYPE::BOUNDARY;
	walls->active = true;
	walls->pos.Set(1290, -50 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), -203);
	walls->scale.Set(29, 25, 5);
	instance->Object_list.push_back(walls);

	//SECOND SET OF WALLS, RIGHT SIDE (DOOR)
	walls = new AABBObject();
	walls->Object = AABBObject::OBJECT_TYPE::BOUNDARY;
	walls->active = true;
	walls->pos.Set(1290, -50 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), 400);
	walls->scale.Set(29, 25, 2);
	instance->Object_list.push_back(walls);
	
	//Back wall [Main corridoor]
	walls = new AABBObject();
	walls->Object = AABBObject::OBJECT_TYPE::BOUNDARY;
	walls->active = true;
	walls->pos.Set(1455, -50 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), 90);
	walls->scale.Set(5, 25, 60);
	instance->Object_list.push_back(walls);

	//First left door, Barricaded room
	walls = new AABBObject();
	walls->Object = AABBObject::OBJECT_TYPE::BOUNDARY;
	walls->active = true;
	walls->pos.Set(210, -50 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), -330);
	walls->scale.Set(22, 20, 20);
	instance->Object_list.push_back(walls);

	//First left door, toilet, right wall
	walls = new AABBObject();
	walls->Object = AABBObject::OBJECT_TYPE::BOUNDARY;
	walls->active = true;
	walls->pos.Set(485, -50 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), -315);
	walls->scale.Set(5, 25, 20);
	instance->Object_list.push_back(walls);

	//First left door, toilet, front wall
	walls = new AABBObject();
	walls->Object = AABBObject::OBJECT_TYPE::BOUNDARY;
	walls->active = true;
	walls->pos.Set(625, -50 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), -405);
	walls->scale.Set(17, 25, 3);
	instance->Object_list.push_back(walls);

	//First left door, toilet, left wall
	walls = new AABBObject();
	walls->Object = AABBObject::OBJECT_TYPE::BOUNDARY;
	walls->active = true;
	walls->pos.Set(705, -50 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), -385);
	walls->scale.Set(3, 25, 80);
	instance->Object_list.push_back(walls);

	//First left door, toilet, back wall
	walls = new AABBObject();
	walls->Object = AABBObject::OBJECT_TYPE::BOUNDARY;
	walls->active = true;
	walls->pos.Set(600, -50 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), -210);
	walls->scale.Set(25, 25, 3);
	instance->Object_list.push_back(walls);

	//First left door, left most wall
	walls = new AABBObject();
	walls->Object = AABBObject::OBJECT_TYPE::BOUNDARY;
	walls->active = true;
	walls->pos.Set(90, -50 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), -490);
	walls->scale.Set(3, 25, 90);
	instance->Object_list.push_back(walls);
	
	//First left door, Bedroom Front wall
	walls = new AABBObject();
	walls->Object = AABBObject::OBJECT_TYPE::BOUNDARY;
	walls->active = true;
	walls->pos.Set(264, -50 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), -640);
	walls->scale.Set(32, 25, 3);
	instance->Object_list.push_back(walls);

	//First left door, Bedroom right wall

	walls = new AABBObject();
	walls->Object = AABBObject::OBJECT_TYPE::BOUNDARY;
	walls->active = true;
	walls->pos.Set(470, -50 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), -720);
	walls->scale.Set(4, 25, 19);
	instance->Object_list.push_back(walls);

	//First left door, Bedroom Back wall
	walls = new AABBObject();
	walls->Object = AABBObject::OBJECT_TYPE::BOUNDARY;
	walls->active = true;
	walls->pos.Set(390, -50 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), -805);
	walls->scale.Set(90, 25, 3);
	instance->Object_list.push_back(walls);

	//Second left door, wall with main door
	walls = new AABBObject();
	walls->Object = AABBObject::OBJECT_TYPE::BOUNDARY;
	walls->active = true;
	walls->pos.Set(1305, -50 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), -205);
	walls->scale.Set(90, 25, 3);
	instance->Object_list.push_back(walls);

	//Second left door, Barricaded room
	walls = new AABBObject();
	walls->Object = AABBObject::OBJECT_TYPE::BOUNDARY;
	walls->active = true;
	walls->pos.Set(1288, -50 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), -440);
	walls->scale.Set(26, 20, 16);
	instance->Object_list.push_back(walls);

	//Second left door, wall with door
	walls = new AABBObject();
	walls->Object = AABBObject::OBJECT_TYPE::BOUNDARY;
	walls->active = true;
	walls->pos.Set(1005, -50 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), -485);
	walls->scale.Set(3, 25, 60);
	instance->Object_list.push_back(walls);

	//Second left door, left wall
	walls = new AABBObject();
	walls->Object = AABBObject::OBJECT_TYPE::BOUNDARY;
	walls->active = true;
	walls->pos.Set(1600, -50 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), -485);
	walls->scale.Set(3, 25, 64);
	instance->Object_list.push_back(walls);

	//Second left door, right wall
	walls = new AABBObject();
	walls->Object = AABBObject::OBJECT_TYPE::BOUNDARY;
	walls->active = true;
	walls->pos.Set(1600, -50 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), -485);
	walls->scale.Set(3, 25, 64);
	instance->Object_list.push_back(walls);

	//Second left door, back wall
	walls = new AABBObject();
	walls->Object = AABBObject::OBJECT_TYPE::BOUNDARY;
	walls->active = true;
	walls->pos.Set(1298, -50 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), -800);
	walls->scale.Set(64, 25, 3);
	instance->Object_list.push_back(walls);

	//Second left door, room divider
	walls = new AABBObject();
	walls->Object = AABBObject::OBJECT_TYPE::BOUNDARY;
	walls->active = true;
	walls->pos.Set(1208, -50 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), -718);
	walls->scale.Set(3, 25, 20);
	instance->Object_list.push_back(walls);

	//Second left door, Bedroom front wall
	walls = new AABBObject();
	walls->Object = AABBObject::OBJECT_TYPE::BOUNDARY;
	walls->active = true;
	walls->pos.Set(1430, -50 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), -650);
	walls->scale.Set(32, 25, 3);
	instance->Object_list.push_back(walls);

	//Second left door, Dining room front wall
	walls = new AABBObject();
	walls->Object = AABBObject::OBJECT_TYPE::BOUNDARY;
	walls->active = true;
	walls->pos.Set(1070, -50 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), -632);
	walls->scale.Set(16, 25, 3);
	instance->Object_list.push_back(walls);
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
	RenderMesh(meshList[LEVEL01], Light);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(500, 60 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), 100);
	modelStack.Scale(1050, 1, 305);
	modelStack.Rotate(90, 1, 0, 0);
	RenderMesh(meshList[CEILING], true);
	modelStack.PopMatrix();
}

void SceneLevel01::RenderRoomObjects(bool Light)
{
	//paintings harambe
	modelStack.PushMatrix();
	modelStack.Translate(530, 0 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), -795);
	modelStack.Scale(70, 40, 1);
	RenderMesh(meshList[PAINTING], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(295, 0 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), -785);
	modelStack.Scale(70, 40, 1);
	RenderMesh(meshList[PAINTING], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(460, 0 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), -735);
	modelStack.Scale(1, 40, 70);
	modelStack.Rotate(270, 0, 1, 0);
	RenderMesh(meshList[PAINTING], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(690, 0 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), -635);
	modelStack.Scale(1, 40, 70);
	modelStack.Rotate(270, 0, 1, 0);
	RenderMesh(meshList[PAINTING], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(480, 0 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), -305);
	modelStack.Scale(1, 40, 70);
	modelStack.Rotate(270, 0, 1, 0);
	RenderMesh(meshList[PAINTING], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(310, 0 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), -305);
	modelStack.Scale(1, 40, 70);
	modelStack.Rotate(90, 0, 1, 0);
	RenderMesh(meshList[PAINTING], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(100, 0 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), -530);
	modelStack.Scale(1, 40, 70);
	modelStack.Rotate(90, 0, 1, 0);
	RenderMesh(meshList[PAINTING], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(317, 0 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), -635);
	modelStack.Scale(70, 40, 1);
	RenderMesh(meshList[PAINTING], true);
	modelStack.PopMatrix();

	//paintings starrynight
	modelStack.PushMatrix();
	modelStack.Translate(730, 10 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), 4.5);
	modelStack.Scale(70, 50, 1);
	RenderMesh(meshList[PAINTING2], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(945, 10 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), 195);
	modelStack.Scale(70, 50, 1);
	modelStack.Rotate(180, 0, 1, 0);
	RenderMesh(meshList[PAINTING2], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(690, 0 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), -735);
	modelStack.Scale(1, 50, 70);
	modelStack.Rotate(270, 0, 1, 0);
	RenderMesh(meshList[PAINTING2], true);
	modelStack.PopMatrix();

	//paintings city light
	modelStack.PushMatrix();
	modelStack.Translate(945, 10 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), 4.5);
	modelStack.Scale(70, 50, 1);
	RenderMesh(meshList[PAINTING3], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(730, 10 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), 195);
	modelStack.Scale(70, 50, 1);
	modelStack.Rotate(180, 0, 1, 0);
	RenderMesh(meshList[PAINTING3], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(640, 0 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), -795);
	modelStack.Scale(70, 50, 1);
	RenderMesh(meshList[PAINTING3], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(690, 0 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), -535);
	modelStack.Scale(1, 50, 70);
	modelStack.Rotate(270, 0, 1, 0);
	RenderMesh(meshList[PAINTING3], true);
	modelStack.PopMatrix();

	//ghost painting
	modelStack.PushMatrix();
	modelStack.Translate(320, 0 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), -203);
	modelStack.Scale(70, 50, 1);
	RenderMesh(meshList[PAINTING4], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(480, 0 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), -203);
	modelStack.Scale(70, 50, 1);
	RenderMesh(meshList[PAINTING], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(1220, 0 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), -203);
	modelStack.Scale(70, 50, 1);
	RenderMesh(meshList[PAINTING5], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(1380, 0 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), -203);
	modelStack.Scale(70, 50, 1);
	RenderMesh(meshList[PAINTING], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(320, 0 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), 404);
	modelStack.Scale(70, 50, 1);
	modelStack.Rotate(180, 0, 1, 0);
	RenderMesh(meshList[PAINTING5], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(480, 0 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), 404);
	modelStack.Scale(70, 50, 1);
	modelStack.Rotate(180, 0, 1, 0);
	RenderMesh(meshList[PAINTING], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(1220, 0 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), 403);
	modelStack.Scale(70, 50, 1);
	modelStack.Rotate(180, 0, 1, 0);
	RenderMesh(meshList[PAINTING4], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(1380, 0 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), 403);
	modelStack.Scale(70, 50, 1);
	modelStack.Rotate(180, 0, 1, 0);
	RenderMesh(meshList[PAINTING], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(1435, 0 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), -104);
	modelStack.Scale(1, 50, 70);
	modelStack.Rotate(-90, 0, 1, 0);
	RenderMesh(meshList[PAINTING], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(1435, 0 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), -4);
	modelStack.Scale(1, 50, 70);
	modelStack.Rotate(-90, 0, 1, 0);
	RenderMesh(meshList[PAINTING3], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(1435, 0 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), 96);
	modelStack.Scale(1, 50, 70);
	modelStack.Rotate(-90, 0, 1, 0);
	RenderMesh(meshList[PAINTING2], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(1435, 0 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), 196);
	modelStack.Scale(1, 50, 70);
	modelStack.Rotate(-90, 0, 1, 0);
	RenderMesh(meshList[PAINTING], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(1435, 0 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), 296);
	modelStack.Scale(1, 50, 70);
	modelStack.Rotate(-90, 0, 1, 0);
	RenderMesh(meshList[PAINTING3], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(1152, 0 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), 296);
	modelStack.Scale(1, 50, 70);
	modelStack.Rotate(90, 0, 1, 0);
	RenderMesh(meshList[PAINTING3], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(1152, 0 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), -104);
	modelStack.Scale(1, 50, 70);
	modelStack.Rotate(90, 0, 1, 0);
	RenderMesh(meshList[PAINTING2], true);
	modelStack.PopMatrix();
	//535 255
	modelStack.PushMatrix();
	modelStack.Translate(535, 0 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), 296);
	modelStack.Scale(1, 50, 70);
	modelStack.Rotate(-90, 0, 1, 0);
	RenderMesh(meshList[PAINTING2], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(255, 0 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), 296);
	modelStack.Scale(1, 50, 70);
	modelStack.Rotate(90, 0, 1, 0);
	RenderMesh(meshList[PAINTING3], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(535, 0 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), -104);
	modelStack.Scale(1, 50, 70);
	modelStack.Rotate(-90, 0, 1, 0);
	RenderMesh(meshList[PAINTING2], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(255, 0 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), -104);
	modelStack.Scale(1, 50, 70);
	modelStack.Rotate(90, 0, 1, 0);
	RenderMesh(meshList[PAINTING], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(166, 0 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), 5);
	modelStack.Scale(70, 50, 1);
	RenderMesh(meshList[PAINTING], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(66, 0 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), 5);
	modelStack.Scale(70, 50, 1);
	RenderMesh(meshList[PAINTING], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-34, 0 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), 5);
	modelStack.Scale(70, 50, 1);
	RenderMesh(meshList[PAINTING], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-134, 0 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), 5);
	modelStack.Scale(70, 50, 1);
	RenderMesh(meshList[PAINTING], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-234, 0 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), 5);
	modelStack.Scale(70, 50, 1);
	RenderMesh(meshList[PAINTING], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-334, 0 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), 5);
	modelStack.Scale(70, 50, 1);
	RenderMesh(meshList[PAINTING], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(166, 0 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), 195);
	modelStack.Scale(70, 50, 1);
	modelStack.Rotate(180, 0, 1, 0);
	RenderMesh(meshList[PAINTING], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(66, 0 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), 195);
	modelStack.Scale(70, 50, 1);
	modelStack.Rotate(180, 0, 1, 0);
	RenderMesh(meshList[PAINTING], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-34, 0 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), 195);
	modelStack.Scale(70, 50, 1);
	modelStack.Rotate(180, 0, 1, 0);
	RenderMesh(meshList[PAINTING], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-134, 0 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), 195);
	modelStack.Scale(70, 50, 1);
	modelStack.Rotate(180, 0, 1, 0);
	RenderMesh(meshList[PAINTING], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-234, 0 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), 195);
	modelStack.Scale(70, 50, 1);
	modelStack.Rotate(180, 0, 1, 0);
	RenderMesh(meshList[PAINTING], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-334, 0 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), 195);
	modelStack.Scale(70, 50, 1);
	modelStack.Rotate(180, 0, 1, 0);
	RenderMesh(meshList[PAINTING], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(1000, 0 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), -400);
	modelStack.Scale(1, 50, 70);
	modelStack.Rotate(90, 0, 1, 0);
	RenderMesh(meshList[PAINTING2], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(1000, 0 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), -300);
	modelStack.Scale(1, 50, 70);
	modelStack.Rotate(90, 0, 1, 0);
	RenderMesh(meshList[PAINTING3], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(1000, 0 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), -500);
	modelStack.Scale(1, 50, 70);
	modelStack.Rotate(90, 0, 1, 0);
	RenderMesh(meshList[PAINTING], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(1585, 0 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), -400);
	modelStack.Scale(1, 50, 70);
	modelStack.Rotate(-90, 0, 1, 0);
	RenderMesh(meshList[PAINTING], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(1585, 0 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), -300);
	modelStack.Scale(1, 50, 70);
	modelStack.Rotate(-90, 0, 1, 0);
	RenderMesh(meshList[PAINTING2], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(1585, 0 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), -500);
	modelStack.Scale(1, 50, 70);
	modelStack.Rotate(-90, 0, 1, 0);
	RenderMesh(meshList[PAINTING3], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(1410, 0 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), -630);
	modelStack.Scale(70, 50, 1);
	RenderMesh(meshList[PAINTING2], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(1068, 0 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), -630);
	modelStack.Scale(70, 50, 1);
	RenderMesh(meshList[PAINTING], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(1410, 0 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), -215);
	modelStack.Scale(70, 50, 1);
	modelStack.Rotate(180, 0, 1, 0);
	RenderMesh(meshList[PAINTING3], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(1510, 0 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), -215);
	modelStack.Scale(70, 50, 1);
	modelStack.Rotate(180, 0, 1, 0);
	RenderMesh(meshList[PAINTING], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(1168, 0 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), -215);
	modelStack.Scale(70, 50, 1);
	modelStack.Rotate(180, 0, 1, 0);
	RenderMesh(meshList[PAINTING3], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(1068, 0 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), -215);
	modelStack.Scale(70, 50, 1);
	modelStack.Rotate(180, 0, 1, 0);
	RenderMesh(meshList[PAINTING2], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(1300, 0 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), -360);
	modelStack.Scale(70, 50, 1);
	RenderMesh(meshList[PAINTING2], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(1300, 0 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), -520);
	modelStack.Scale(70, 50, 1);
	modelStack.Rotate(180, 0, 1, 0);
	RenderMesh(meshList[PAINTING], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(1400, 0 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), 413);
	modelStack.Scale(70, 50, 1);
	RenderMesh(meshList[PAINTING], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(1440, 0 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), 485);
	modelStack.Scale(1, 50, 70);
	modelStack.Rotate(-90, 0, 1, 0);
	RenderMesh(meshList[PAINTING2], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(1440, 0 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), 585);
	modelStack.Scale(1, 50, 70);
	modelStack.Rotate(-90, 0, 1, 0);
	RenderMesh(meshList[PAINTING], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(1440, 0 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), 685);
	modelStack.Scale(1, 50, 70);
	modelStack.Rotate(-90, 0, 1, 0);
	RenderMesh(meshList[PAINTING3], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(1215, 0 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), 485);
	modelStack.Scale(1, 50, 70);
	modelStack.Rotate(90, 0, 1, 0);
	RenderMesh(meshList[PAINTING2], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(1215, 0 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), 585);
	modelStack.Scale(1, 50, 70);
	modelStack.Rotate(90, 0, 1, 0);
	RenderMesh(meshList[PAINTING], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(1215, 0 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), 685);
	modelStack.Scale(1, 50, 70);
	modelStack.Rotate(90, 0, 1, 0);
	RenderMesh(meshList[PAINTING3], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(1300, 0 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), 830);
	modelStack.Scale(70, 50, 1);
	modelStack.Rotate(180, 0, 1, 0);
	RenderMesh(meshList[PAINTING3], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(520, 0 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), 550);
	modelStack.Scale(70, 50, 1);
	RenderMesh(meshList[PAINTING3], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(430, 0 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), 650);
	modelStack.Scale(1, 50, 70);
	modelStack.Rotate(90, 0, 1, 0);
	RenderMesh(meshList[PAINTING], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(695, 0 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), 448);
	modelStack.Scale(1, 50, 70);
	modelStack.Rotate(-90, 0, 1, 0);
	RenderMesh(meshList[PAINTING3], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(695, 0 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), 548);
	modelStack.Scale(1, 50, 70);
	modelStack.Rotate(-90, 0, 1, 0);
	RenderMesh(meshList[PAINTING], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(695, 0 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), 648);
	modelStack.Scale(1, 50, 70);
	modelStack.Rotate(-90, 0, 1, 0);
	RenderMesh(meshList[PAINTING2], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(695, 0 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), 648);
	modelStack.Scale(1, 50, 70);
	modelStack.Rotate(-90, 0, 1, 0);
	RenderMesh(meshList[PAINTING2], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(695, 0 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), 748);
	modelStack.Scale(1, 50, 70);
	modelStack.Rotate(-90, 0, 1, 0);
	RenderMesh(meshList[PAINTING], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(695, 0 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), 848);
	modelStack.Scale(1, 50, 70);
	modelStack.Rotate(-90, 0, 1, 0);
	RenderMesh(meshList[PAINTING3], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(695, 0 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), 948);
	modelStack.Scale(1, 50, 70);
	modelStack.Rotate(-90, 0, 1, 0);
	RenderMesh(meshList[PAINTING2], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(590, 0 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), 415);
	modelStack.Scale(70, 50, 1);
	RenderMesh(meshList[PAINTING2], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(490, 0 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), 415);
	modelStack.Scale(70, 50, 1);
	RenderMesh(meshList[PAINTING], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(490, 0 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), 530);
	modelStack.Scale(70, 50, 1);
	modelStack.Rotate(180, 0, 1, 0);
	RenderMesh(meshList[PAINTING3], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(390, 0 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), 530);
	modelStack.Scale(70, 50, 1);
	modelStack.Rotate(180, 0, 1, 0);
	RenderMesh(meshList[PAINTING], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(290, 0 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), 485);
	modelStack.Scale(1, 50, 70);
	modelStack.Rotate(90, 0, 1, 0);
	RenderMesh(meshList[PAINTING3], true);
	modelStack.PopMatrix();

	//[Left] first room
	
	modelStack.PushMatrix();
	modelStack.Translate(463, -50 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), -656);
	modelStack.Scale(4, 3.5, 4);
	modelStack.Rotate(-75, 0, 1, 0);
	RenderMeshOutlined(meshList[DOOR], Light);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(398, 60 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), -505);
	modelStack.Scale(300, 1, 300);
	modelStack.Rotate(90, 1, 0, 0);
	RenderMesh(meshList[ROOMCEILING], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(500, -50 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), -390);
	modelStack.Scale(4, 3.5, 4);
	modelStack.Rotate(105, 0, 1, 0);
	RenderMeshOutlined(meshList[DOOR], Light);
	modelStack.PopMatrix();

	/*modelStack.PushMatrix();
	modelStack.Translate(120, -50 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), -420);
	modelStack.Scale(1.2, 1, 1);
	RenderMeshOutlined(meshList[BLOCKAGE], Light);
	modelStack.PopMatrix();*/

	//[Left] Second room

	modelStack.PushMatrix();
	modelStack.Translate(1295, 60 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), -505);
	modelStack.Scale(300, 1, 300);
	modelStack.Rotate(90, 1, 0, 0);
	RenderMesh(meshList[ROOMCEILING], true);
	modelStack.PopMatrix();

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
	modelStack.Translate(1410, -50 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), -485);
	modelStack.Scale(1.2, 1, 1);
	modelStack.Rotate(90, 0, 1, 0);
	RenderMeshOutlined(meshList[BLOCKAGE], Light);
	modelStack.PopMatrix();


	//[Right] first room

	modelStack.PushMatrix();
	modelStack.Translate(398, 60 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), 705);
	modelStack.Scale(300, 1, 300);
	modelStack.Rotate(90, 1, 0, 0);
	RenderMesh(meshList[ROOMCEILING], true);
	modelStack.PopMatrix();

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

	//[Right] second room

	modelStack.PushMatrix();
	modelStack.Translate(1295, 60 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), 705);
	modelStack.Scale(300, 1, 300);
	modelStack.Rotate(90, 1, 0, 0);
	RenderMesh(meshList[ROOMCEILING], true);
	modelStack.PopMatrix();

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
	modelStack.Translate(1020, -50 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), 630);
	modelStack.Scale(1.2, 1, 1);
	RenderMeshOutlined(meshList[BLOCKAGE], Light);
	modelStack.PopMatrix();

	//test obj
	/*modelStack.PushMatrix();
	modelStack.Translate(400, -30 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), -500);
	modelStack.Scale(40, 40, 40);
	RenderMesh(meshList[CRATE], false);
	modelStack.PopMatrix();*/
}

void SceneLevel01::RenderHUD()
{
	RenderRadar();
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
	RenderSkyplane();
	RenderTerrain();
	RenderLevel(true);
	RenderRoomObjects(true);
	RenderBullets(true);
	//RenderSprite();
	RenderEnemies(false);

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

	RenderWeapons(true);
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

	RenderWorld();

	RenderObjects(ShowHitbox);

	if (!Singleton::getInstance()->stateCheck)
	{
		RenderHUD();
	}

	std::ostringstream ss;

	if (timerstart && timer < 3.f)
	{
		RenderImageOnScreen(meshList[GEO_LOAD_1], Vector3(80, 60, 1), Vector3(40, 30, 0), Vector3(0, 0, 0));
	}

	if (showText)
	{
		//On screen text
		ss.precision(5);
		ss << "FPS: " << fps;
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 2, 3);

		ss.str("");
		ss << "pistol mag: " << pistolMag;
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 2, 6);

		ss.str("");
		ss << "pistol ammo: " << pistolAmmo;
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 2, 9);

		ss.str("");
		ss << "rifle mag: " << rifleMag;
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 2, 12);

		ss.str("");
		ss.precision(5);
		ss << "rifle ammo: " << rifleAmmo;
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 2, 15);

		ss.str("");
		ss.precision(5);
		ss << "Position z: " << camera.position.z;
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 2, 18);

		ss.str("");
		ss.precision(5);
		ss << "Position x: " << camera.position.x;
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 2, 21);

		if (reloading){
			std::ostringstream ss;
			ss << "Reloading";
			RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 2, 22);
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

void SceneLevel01::Render()
{
	//PRE RENDER PASS
	RenderPassGPass();

	//MAIN RENDER PASS
	RenderPassMain();
}

void SceneLevel01::Exit()
{
	delete level1_Heights;
	SceneBase::Exit();
}