#include "SceneHub.h"
#include "GL\glew.h"

#include "shader.hpp"
#include "Application.h"
#include "Utility.h"
#include <sstream>

SceneHub::SceneHub()
{
}

SceneHub::~SceneHub()
{
}

static const Vector3 TERRAINSIZE(1400.f, 200.0f, 1400.f);

void SceneHub::Init()
{
	sound.playMusic("Sound/shopbgm.mp3");

	showBuy = false;
	showSell = false;
	showDefault = true;
	Application::HideCursor();

	SceneBase::Init();
	lights[0].position.Set(0, 500, 0);

	meshList[FLOOR] = MeshBuilder::GenerateQuad2("floor", Color(0, 0, 0), 10, 10, TexCoord(10, 10));
	meshList[FLOOR]->textureArray[0] = LoadTGA("Image//Outdoor//floor.tga");

	meshList[GEO_SHOPKEEPER] = MeshBuilder::GenerateQuad("HARAMBE", Color(1, 1, 1), 1);
	meshList[GEO_SHOPKEEPER]->textureArray[0] = LoadTGA("Image//Hub//shopkeeper.tga");

	meshList[GEO_HUB] = MeshBuilder::GenerateOBJ("HUB", "OBJ//Hub//Hub.obj");
	meshList[GEO_HUB]->textureArray[0] = LoadTGA("Image//Hub//hub.tga");
	meshList[GEO_HUB]->material.kAmbient.Set(0.4f, 0.4f, 0.4f);

	meshList[SHOP_BG] = MeshBuilder::GenerateQuad("Water", Color(0, 0, 0), 1.f);
	meshList[SHOP_BG]->textureArray[0] = LoadTGA("Image//Hub//shopBG.tga");

	meshList[SHOP_UI] = MeshBuilder::GenerateQuad("Water", Color(0, 0, 0), 1.f);
	meshList[SHOP_UI]->textureID = LoadTGA("Image//Hub//shopUI.tga");

	meshList[UI_BUY] = MeshBuilder::GenerateQuad("Water", Color(0, 0, 0), 1.f);
	meshList[UI_BUY]->textureID = LoadTGA("Image//Hub//buy.tga");

	meshList[UI_SELL] = MeshBuilder::GenerateQuad("Water", Color(0, 0, 0), 1.f);
	meshList[UI_SELL]->textureID = LoadTGA("Image//Hub//sell.tga");

	meshList[UI_HOVER] = MeshBuilder::GenerateQuad("Water", Color(0, 0, 0), 1.f);
	meshList[UI_HOVER]->textureID = LoadTGA("Image//Hub//hubHover.tga");

	meshList[UI_MONEY] = MeshBuilder::GenerateQuad("Level 1 loading screen", Color(0, 0, 0), 1.f);
	meshList[UI_MONEY]->textureID = LoadTGA("Image//Hub//money.tga");

	meshList[UI_AMMO_RIFLE] = MeshBuilder::GenerateQuad("Water", Color(0, 0, 0), 1.f);
	meshList[UI_AMMO_RIFLE]->textureID = LoadTGA("Image//Hub//ammoRifleUI.tga");

	meshList[UI_AMMO_PISTOL] = MeshBuilder::GenerateQuad("Water", Color(0, 0, 0), 1.f);
	meshList[UI_AMMO_PISTOL]->textureID = LoadTGA("Image//Hub//ammoPistolUI.tga");

	meshList[UI_POTION] = MeshBuilder::GenerateQuad("Water", Color(0, 0, 0), 1.f);
	meshList[UI_POTION]->textureID = LoadTGA("Image//Hub//potionUI.tga");

	//camera.Init(Vector3(-1190, 20, 335), Vector3(0, 5, 1), Vector3(0, 1, 0));
	//Random my random randomly using srand
	srand(time(NULL));
	camera.Init(Vector3(1, 5, 1), Vector3(350, 5, 15), Vector3(0, 1, 0));

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

	soundTimer = 0;

	buySize = 10;
	sellSize = 10;

	timeb4disappear = 0;

	initSceneObjects();

	camera.position.Set(1, 5, 1);

	InitPartitioning();
}

void SceneHub::initSceneObjects()
{
	//Counter
	AABBObject * Counter = new AABBObject();
	Counter->Object = AABBObject::OBJECT_TYPE::BOUNDARY;
	Counter->active = true;
	Counter->pos.Set(285, -60, 25);
	Counter->scale.Set(6, 5, 52);
	instance->Object_list.push_back(Counter);

	//Back
	AABBObject * Boundary = new AABBObject();
	Boundary->Object = AABBObject::OBJECT_TYPE::BOUNDARY;
	Boundary->active = true;
	Boundary->pos.Set(-405, 35, 30);
	Boundary->scale.Set(1, 30, 55);
	instance->Object_list.push_back(Boundary);

	//Left
	Boundary = new AABBObject();
	Boundary->Object = AABBObject::OBJECT_TYPE::BOUNDARY;
	Boundary->active = true;
	Boundary->pos.Set(-45, 35, -245);
	Boundary->scale.Set(65, 30, 1);
	instance->Object_list.push_back(Boundary);

	//Right
	Boundary = new AABBObject();
	Boundary->Object = AABBObject::OBJECT_TYPE::BOUNDARY;
	Boundary->active = true;
	Boundary->pos.Set(-45, 35, 290);
	Boundary->scale.Set(65, 30, 1);
	instance->Object_list.push_back(Boundary);

	//Portal 1
	Portal_1 = new AABBObject();
	Portal_1->Object = AABBObject::OBJECT_TYPE::BOUNDARY;
	Portal_1->active = true;
	Portal_1->pos.Set(-170, -6, -240);
	Portal_1->scale.Set(10, 10, 1);
	instance->Object_list.push_back(Portal_1);

	//Portal 2
	Portal_2 = new AABBObject();
	Portal_2->Object = AABBObject::OBJECT_TYPE::BOUNDARY;
	Portal_2->active = true;
	Portal_2->pos.Set(100, -6, -240);
	Portal_2->scale.Set(10, 10, 1);
	instance->Object_list.push_back(Portal_2);

	//Portal 3
	Portal_3 = new AABBObject();
	Portal_3->Object = AABBObject::OBJECT_TYPE::BOUNDARY;
	Portal_3->active = true;
	Portal_3->pos.Set(100, -6, 286);
	Portal_3->scale.Set(10, 10, 1);
	instance->Object_list.push_back(Portal_3);

	//Portal 4
	Portal_4 = new AABBObject();
	Portal_4->Object = AABBObject::OBJECT_TYPE::BOUNDARY;
	Portal_4->active = true;
	Portal_4->pos.Set(-170, -6, 286);
	Portal_4->scale.Set(10, 10, 1);
	instance->Object_list.push_back(Portal_4);
}

void SceneHub::Update(double dt)
{
	timeb4disappear += dt;
	distanceLeft = (Vector3(-2000, 20, 335) - camera.position).Length();

	if (Singleton::getInstance()->showShop == false && Singleton::getInstance()->showInventory == false)
		camera.Update(dt);

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

	static bool eButtonState = false;
	if (Application::IsKeyPressed('E'))
	{
		if (!eButtonState)
		{
			eButtonState = true;
			
			if ((Vector3(350, -40 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), 15) - camera.position).Length() < 150)
			{
				if (rand() % 2)
				{
					//sound.playSoundEffect3D("Sound/gorilla1.mp3", irrklang::vec3df(350, -40 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), 15), false);
					sound.playSoundEffect2D("Sound/gorilla1.mp3");
				}
				else
				{
					//sound.playSoundEffect3D("Sound/gorilla2.wav", irrklang::vec3df(350, -40 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), 15), false);
					sound.playSoundEffect2D("Sound/gorilla2.wav");
				}
					
				if (Singleton::getInstance()->showShop == false)
				{
					Application::SetMousePosition(0, 0);
					Application::ShowCursor();
					Singleton::getInstance()->showShop = true;
				}
				else
				{
					Application::SetMousePosition(0, 0);
					Application::HideCursor();
					Singleton::getInstance()->showShop = false;
				}
			}
		}
	}
	else if (!Application::IsKeyPressed('E'))
	{
		if (eButtonState)
			eButtonState = false;
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

			if (Singleton::getInstance()->inventory.size() < 6)
				Singleton::getInstance()->inventory.push_back(Singleton::getInstance()->item_ghost2);

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

			if (Singleton::getInstance()->inventory.size() < 6)
				Singleton::getInstance()->inventory.push_back(Singleton::getInstance()->item_ghost3);
		}
	}
	else if (!Application::IsKeyPressed('T'))
	{
		if (spaceButtonState4)
			spaceButtonState4 = false;
	}

	////////////////////////////////////////////////////////
	//						Portals						  //
	////////////////////////////////////////////////////////
	distance_1 = (Portal_1->pos - Singleton::getInstance()->player->getPosition()).Length();
	if (Application::IsKeyPressed('E') && distance_1 <= 50)
	{
		sound.stopMusic();
		Singleton::getInstance()->stateCheck = true;
		Singleton::getInstance()->program_state = Singleton::PROGRAM_GAME1;
	}

	distance_2 = (Portal_2->pos - Singleton::getInstance()->player->getPosition()).Length();
	if (Application::IsKeyPressed('E') && distance_2 <= 50 && instance->gotHammer)
	{
		sound.stopMusic();
		Singleton::getInstance()->stateCheck = true;
		Singleton::getInstance()->program_state = Singleton::PROGRAM_GAME2;
	}

	distance_3 = (Portal_3->pos - Singleton::getInstance()->player->getPosition()).Length();
	if (Application::IsKeyPressed('E') && distance_3 <= 50 && instance->gotKey)
	{
		sound.stopMusic();
		Singleton::getInstance()->stateCheck = true;
		Singleton::getInstance()->program_state = Singleton::PROGRAM_GAME3;
	}

	distance_4 = (Portal_4->pos - Singleton::getInstance()->player->getPosition()).Length();
	if (Application::IsKeyPressed('E') && distance_4 <= 50 && instance->gotClear)
	{
		sound.stopMusic();
		Singleton::getInstance()->stateCheck = true;
		Singleton::getInstance()->program_state = Singleton::PROGRAM_GAME4;
	}

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


	fps = (float)(1.f / dt);
}

void SceneHub::UpdateParticle(double dt)
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
		particleWater1->vel.Set(Math::RandFloatMinMax(-range, range),
			Math::RandFloatMinMax(-range, range),
			Math::RandFloatMinMax(-range, range));
		particleWater1->rotateSpeed = Math::RandFloatMinMax(20.f, 40.f);
		particleWater1->pos.Set(0, 80 + 350.f * ReadHeightMap(m_heightMap, -20.f / 4000, -20.f / 4000), 0);
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
				particle->vel += m_gravity * (float)dt;
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

ParticleObject* SceneHub::GetParticles(void)
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

void SceneHub::RenderGround()
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

void SceneHub::RenderSkyplane()
{
	modelStack.PushMatrix();
	modelStack.Translate(camera.position.x, 1800, camera.position.z);
	modelStack.Rotate(rotateAngle, 0, 1, 0);
	RenderMesh(meshList[SKYPLANE], false);
	modelStack.PopMatrix();
}

void SceneHub::RenderTerrain()
{
	modelStack.PushMatrix();
	modelStack.Translate(0, -50, 0);
	modelStack.Scale(TERRAINSIZE.x, TERRAINSIZE.y, TERRAINSIZE.z);
	//RenderMesh(meshList[TERRAIN], true);
	modelStack.PopMatrix();
}

void SceneHub::RenderEnvironment(bool Light)
{
	//Portal 1
	modelStack.PushMatrix();
	modelStack.Translate(-170, -6, -240);
	modelStack.Scale(115, 120, 140);
	RenderMeshOutlined(meshList[GEO_LOAD_1], Light);
	modelStack.PopMatrix();

	//Portal 2
	modelStack.PushMatrix();
	modelStack.Translate(100, -6, -240);
	modelStack.Scale(115, 120, 140);
	RenderMeshOutlined(meshList[GEO_LOAD_2], Light);
	modelStack.PopMatrix();

	//Portal 3
	modelStack.PushMatrix();
	modelStack.Translate(100, -6, 286);
	modelStack.Rotate(180, 0, 1, 0);
	modelStack.Scale(115, 120, 140);
	RenderMeshOutlined(meshList[GEO_LOAD_3], Light);
	modelStack.PopMatrix();

	//Portal 4
	modelStack.PushMatrix();
	modelStack.Translate(-170, -6, 286);
	modelStack.Rotate(180, 0, 1, 0);
	modelStack.Scale(115, 120, 140);
	RenderMeshOutlined(meshList[GEO_LOAD_4], Light);
	modelStack.PopMatrix();
}
void SceneHub::RenderOthers(bool Light)
{
	modelStack.PushMatrix();
	modelStack.Translate(0, -49 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), 0);
	modelStack.Rotate(-90, 1, 0, 0);
	modelStack.Scale(140, 140, 140);
	RenderMeshOutlined(meshList[FLOOR], Light);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(350, -40 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), 15);
	shopkeeperAngle = Math::RadianToDegree(atan2(-(15 - instance->player->getPosition().z), 350 - instance->player->getPosition().x));
	modelStack.Scale(140, 140, 140);
	modelStack.Rotate(shopkeeperAngle - 90, 0, 1, 0);
	RenderMeshOutlined(meshList[GEO_SHOPKEEPER], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(400, 100, 0);
	modelStack.Rotate(-90, 0, 1, 0);
	modelStack.Scale(600, 300, 140);
	RenderMesh(meshList[SHOP_BG], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0, -52 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), 0);
	modelStack.Scale(60, 40, 60);
	RenderMeshOutlined(meshList[GEO_HUB], true);
	modelStack.PopMatrix();
}
void SceneHub::RenderHUD()
{
	RenderRadar();
}
void SceneHub::RenderShop()
{
	std::ostringstream ss;
	static bool bLButtonState = false;

	if (Singleton::getInstance()->showShop == true)
	{
		RenderImageOnScreen(meshList[SHOP_UI], Vector3(70, 50, 1), Vector3(40, 30, 0), Vector3(0, 0, 0));
		ss.str(""); ss.precision(5); ss << Singleton::getInstance()->money;
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 2.5f, 65, 51.2);

		/////////////////
		// SELL BUTTON //
		/////////////////
		if ((725 * Application::GetWindowWidth() / 800> Singleton::getInstance()->mousex && 585 * Application::GetWindowWidth() / 800< Singleton::getInstance()->mousex) &&
			(182 * Application::GetWindowHeight() / 600> Singleton::getInstance()->mousey && 117 * Application::GetWindowHeight() / 600< Singleton::getInstance()->mousey))
		{
			//MOUSE CLICK	
			if (!bLButtonState && Application::IsMousePressed(0))
			{
				bLButtonState = true;

				showSell = true;
				showBuy = false;
				showDefault = false;
				buySize = 5;
				sellSize = 15;
				sound.playSoundEffect2D("Sound/button.mp3");
				RenderImageOnScreen(meshList[UI_SELL], Vector3(sellSize, sellSize, 1), Vector3(65, 45, 1), Vector3(0, 0, 0));
			}
			else if (bLButtonState && !Application::IsMousePressed(0))
			{
				bLButtonState = false;
			}

			//MOUSE HOVER
			else
			{
				RenderImageOnScreen(meshList[UI_SELL], Vector3(sellSize, sellSize, 1), Vector3(65, 45, 1), Vector3(0, 0, 0));
			}
		}
		//DEFAULT
		else
		{
			RenderImageOnScreen(meshList[UI_SELL], Vector3(sellSize, sellSize, 1), Vector3(65, 45, 1), Vector3(0, 0, 0));
		}

		////////////////
		// BUY BUTTON //
		////////////////
		if ((565 * Application::GetWindowWidth() / 800> Singleton::getInstance()->mousex && 425 * Application::GetWindowWidth() / 800< Singleton::getInstance()->mousex) &&
			(182 * Application::GetWindowHeight() / 600> Singleton::getInstance()->mousey && 117 * Application::GetWindowHeight() / 600< Singleton::getInstance()->mousey))
		{
			//MOUSE CLICK
			if (!bLButtonState && Application::IsMousePressed(0))
			{
				bLButtonState = true;

				showSell = false;
				showBuy = true;
				showDefault = false;
				buySize = 15;
				sellSize = 5;
				sound.playSoundEffect2D("Sound/button.mp3");
				RenderImageOnScreen(meshList[UI_BUY], Vector3(buySize, buySize, 1), Vector3(50, 45, 1), Vector3(0, 0, 0));
			}
			else if (bLButtonState && !Application::IsMousePressed(0))
			{
				bLButtonState = false;
			}
			//MOUSE HOVER
			else
			{
				RenderImageOnScreen(meshList[UI_BUY], Vector3(buySize, buySize, 1), Vector3(50, 45, 1), Vector3(0, 0, 0));
			}
		}
		//DEFAULT
		else
		{
			RenderImageOnScreen(meshList[UI_BUY], Vector3(buySize, buySize, 1), Vector3(50, 45, 1), Vector3(0, 0, 0));
		}

		/////////////////
		// EXIT BUTTON //
		/////////////////
		if ((272 * Application::GetWindowWidth() / 800> Singleton::getInstance()->mousex && 65 * Application::GetWindowWidth() / 800< Singleton::getInstance()->mousex) &&
			(91 * Application::GetWindowHeight() / 600> Singleton::getInstance()->mousey && 57 * Application::GetWindowHeight() / 600< Singleton::getInstance()->mousey))
		{
			//MOUSE CLICK
			if (!bLButtonState && Application::IsMousePressed(0))
			{
				bLButtonState = true;

				Application::SetMousePosition(0, 0);
				Application::HideCursor();
				Singleton::getInstance()->showShop = false;
			}
			else if (bLButtonState && !Application::IsMousePressed(0))
			{
				bLButtonState = false;
			}
		}


		if (showDefault)
		{
			ss.str(""); ss.precision(5); ss << "How can I";
			RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 0, 0), 2.3f, 19, 40);
			ss.str(""); ss.precision(5); ss << "help you?";
			RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 0, 0), 2.3f, 19, 37);

		}
		if (showBuy)
		{
			ss.str(""); ss.precision(5); ss << "What would you";
			RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 0, 0), 2.3f, 19, 40);
			ss.str(""); ss.precision(5); ss << "like to buy?";
			RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 0, 0), 2.3f, 19, 37);

			if (timeb4disappear < 0.5)
			{
				ss.str(""); ss.precision(5); ss << "NOT ENOUGH MATERIAL!";
				RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 0, 0), 2.5f, 45, 20);
			}
			
			ss.str(""); ss.precision(5); ss << "------------------------";
			RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 2.5f, 42.5, 18);

			ss.str(""); ss.precision(5); ss << "INVENTORY";
			RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 2.5f, 52, 16);

			RenderImageOnScreen(meshList[UI_AMMO_PISTOL], Vector3(5, 4, 1), Vector3(48, 13, 1), Vector3(0, 0, 0));
			ss.str(""); ss.precision(5); ss << instance->pistolMag;
			RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 2.5f, 48, 8);

			RenderImageOnScreen(meshList[UI_AMMO_RIFLE], Vector3(9, 4, 1), Vector3(58, 13, 1), Vector3(0, 0, 0));
			ss.str(""); ss.precision(5); ss << instance->rifleMag;
			RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 2.5f, 58, 8);

			RenderImageOnScreen(meshList[UI_POTION], Vector3(4, 4, 1), Vector3(68, 13, 1), Vector3(0, 0, 0));
			ss.str(""); ss.precision(5); ss << instance->player->getHealthPack();
			RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 2.5f, 68, 8);

			//row 1
			RenderImageOnScreen(meshList[UI_MONEY], Vector3(4, 4, 1), Vector3(47, 36, 1), Vector3(0, 0, 0));
			ss.str(""); ss.precision(5); ss << "5";
			RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 2.5f, 50, 35);
			ss.str(""); ss.precision(5); ss << "----->";
			RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 2.5f, 53, 35);
			RenderImageOnScreen(meshList[UI_AMMO_PISTOL], Vector3(5, 4, 1), Vector3(65, 36, 1), Vector3(0, 0, 0));

			//row 2
			RenderImageOnScreen(meshList[UI_MONEY], Vector3(4, 4, 1), Vector3(47, 31, 1), Vector3(0, 0, 0));
			ss.str(""); ss.precision(5); ss << "10";
			RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 2.5f, 50, 30);
			ss.str(""); ss.precision(5); ss << "----->";
			RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 2.5f, 53, 30);
			RenderImageOnScreen(meshList[UI_AMMO_RIFLE], Vector3(9, 4, 1), Vector3(65, 31, 1), Vector3(0, 0, 0));
			
			//row 3
			RenderImageOnScreen(meshList[UI_MONEY], Vector3(4, 4, 1), Vector3(47, 26, 1), Vector3(0, 0, 0));
			ss.str(""); ss.precision(5); ss << "15";
			RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 2.5f, 50, 25);
			ss.str(""); ss.precision(5); ss << "----->";
			RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 2.5f, 53, 25);
			RenderImageOnScreen(meshList[UI_POTION], Vector3(4, 4, 1), Vector3(65, 26, 1), Vector3(0, 0, 0));

			//SLOT 1
			if ((725 * Application::GetWindowWidth() / 800 > Singleton::getInstance()->mousex && 425 * Application::GetWindowWidth() / 800 < Singleton::getInstance()->mousex) &&
				(264 * Application::GetWindowHeight() / 600 > Singleton::getInstance()->mousey && 217 * Application::GetWindowHeight() / 600 < Singleton::getInstance()->mousey))
			{
				//MOUSE CLICK	
				if (!bLButtonState && Application::IsMousePressed(0))
				{
					bLButtonState = true;

					if (Singleton::getInstance()->money >= 5)
					{
						Singleton::getInstance()->money -= 5;
						instance->pistolMag++;
						sound.playSoundEffect2D("Sound/buy.mp3");
					}
					else
					{
						timeb4disappear = 0;
					}
				}
				else if (bLButtonState && !Application::IsMousePressed(0))
				{
					bLButtonState = false;
				}

				//MOUSE HOVER
				RenderImageOnScreen(meshList[UI_HOVER], Vector3(30, 6.5, 1), Vector3(57.5, 36, 1), Vector3(0, 0, 0));
			}

			//SLOT 2
			if ((725 * Application::GetWindowWidth() / 800 > Singleton::getInstance()->mousex && 425 * Application::GetWindowWidth() / 800 < Singleton::getInstance()->mousex) &&
				(313 * Application::GetWindowHeight() / 600 > Singleton::getInstance()->mousey && 266 * Application::GetWindowHeight() / 600 < Singleton::getInstance()->mousey))
			{
				//MOUSE CLICK	
				if (!bLButtonState && Application::IsMousePressed(0))
				{
					bLButtonState = true;

					if (Singleton::getInstance()->money >= 10)
					{
						Singleton::getInstance()->money -= 10;
						instance->rifleMag++;
						sound.playSoundEffect2D("Sound/buy.mp3");
					}
					else
					{
						timeb4disappear = 0;
					}
				}
				else if (bLButtonState && !Application::IsMousePressed(0))
				{
					bLButtonState = false;
				}

				//MOUSE HOVER
				RenderImageOnScreen(meshList[UI_HOVER], Vector3(30, 6.5, 1), Vector3(57.5, 31, 1), Vector3(0, 0, 0));
			}

			//SLOT 3
			if ((725 * Application::GetWindowWidth() / 800 > Singleton::getInstance()->mousex && 425 * Application::GetWindowWidth() / 800 < Singleton::getInstance()->mousex) &&
				(362 * Application::GetWindowHeight() / 600 > Singleton::getInstance()->mousey && 315 * Application::GetWindowHeight() / 600 < Singleton::getInstance()->mousey))
			{
				//MOUSE CLICK	
				if (!bLButtonState && Application::IsMousePressed(0))
				{
					bLButtonState = true;

					if (Singleton::getInstance()->money >= 15)
					{
						Singleton::getInstance()->money -= 15;
						instance->player->AddHealthpack(1);
						sound.playSoundEffect2D("Sound/buy.mp3");
					}
					else
					{
						timeb4disappear = 0;
					}
				}
				else if (bLButtonState && !Application::IsMousePressed(0))
				{
					bLButtonState = false;
				}

				//MOUSE HOVER
				RenderImageOnScreen(meshList[UI_HOVER], Vector3(30, 6.5, 1), Vector3(57.5, 26, 1), Vector3(0, 0, 0));
			}

			
		}
		if (showSell)
		{
			int sz = Singleton::getInstance()->inventory.size();
			
			ss.str(""); ss.precision(5); ss << "What would you";
			RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0,0,0), 2.3f, 19, 40);
			ss.str(""); ss.precision(5); ss << "like to sell?";
			RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 0, 0), 2.3f, 19, 37);

			for (int i = 1; i <= sz; i++)
			{
				if (Singleton::getInstance()->inventory[i - 1]->name == "ghost1")
				{
					RenderImageOnScreen(meshList[INV_GHOST1], Vector3(4, 4, 1), Vector3(47, 41 - i * 5, 1), Vector3(0, 0, 0));
					RenderImageOnScreen(meshList[UI_MONEY], Vector3(4, 4, 1), Vector3(65, 41 - i * 5, 1), Vector3(0, 0, 0));

					ss.str(""); ss.precision(5); ss << "----->      " << Singleton::getInstance()->inventory[i - 1]->howMuchItWorth;
					RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 2.5f, 53, 40 - i * 5);
				}

				if (Singleton::getInstance()->inventory[i - 1]->name == "ghost2")
				{
					RenderImageOnScreen(meshList[INV_GHOST2], Vector3(4, 4, 1), Vector3(47, 41 - i * 5, 1), Vector3(0, 0, 0));
					RenderImageOnScreen(meshList[UI_MONEY], Vector3(4, 4, 1), Vector3(65, 41 - i * 5, 1), Vector3(0, 0, 0));

					ss.str(""); ss.precision(5); ss << "----->      " << Singleton::getInstance()->inventory[i - 1]->howMuchItWorth;
					RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 2.5f, 53, 40 - i * 5);
				}
				if (Singleton::getInstance()->inventory[i - 1]->name == "ghost3")
				{
					RenderImageOnScreen(meshList[INV_GHOST3], Vector3(4, 4, 1), Vector3(47, 41 - i * 5, 1), Vector3(0, 0, 0));
					RenderImageOnScreen(meshList[UI_MONEY], Vector3(4, 4, 1), Vector3(65, 41 - i * 5, 1), Vector3(0, 0, 0));

					ss.str(""); ss.precision(5); ss << "----->      " << Singleton::getInstance()->inventory[i - 1]->howMuchItWorth;
					RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 1, 1), 2.5f, 53, 40 - i * 5);
				}

				//SLOT 1
				if ((725 * Application::GetWindowWidth() / 800 > Singleton::getInstance()->mousex && 425 * Application::GetWindowWidth() / 800 < Singleton::getInstance()->mousex) &&
					(264 * Application::GetWindowHeight() / 600 > Singleton::getInstance()->mousey && 217 * Application::GetWindowHeight() / 600 < Singleton::getInstance()->mousey) && i == 1)
				{
					//MOUSE CLICK	
					if (!bLButtonState && Application::IsMousePressed(0))
					{
						bLButtonState = true;

						sound.playSoundEffect2D("Sound/sell.mp3");

						if (Singleton::getInstance()->inventory[0]->name == "ghost1") Singleton::getInstance()->money += Singleton::getInstance()->inventory[0]->howMuchItWorth;
						if (Singleton::getInstance()->inventory[0]->name == "ghost2") Singleton::getInstance()->money += Singleton::getInstance()->inventory[0]->howMuchItWorth;
						if (Singleton::getInstance()->inventory[0]->name == "ghost3") Singleton::getInstance()->money += Singleton::getInstance()->inventory[0]->howMuchItWorth;

						if (Singleton::getInstance()->inventory.size() > 0)
							Singleton::getInstance()->inventory.erase(Singleton::getInstance()->inventory.begin());
						break;
					}
					else if (bLButtonState && !Application::IsMousePressed(0))
					{
						bLButtonState = false;
					}

					//MOUSE HOVER
					RenderImageOnScreen(meshList[UI_HOVER], Vector3(30, 6.5, 1), Vector3(57.5, 36, 1), Vector3(0, 0, 0));
				}

				//SLOT 2
				if ((725 * Application::GetWindowWidth() / 800 > Singleton::getInstance()->mousex && 425 * Application::GetWindowWidth() / 800 < Singleton::getInstance()->mousex) &&
					(313 * Application::GetWindowHeight() / 600 > Singleton::getInstance()->mousey && 266 * Application::GetWindowHeight() / 600 < Singleton::getInstance()->mousey) && i == 2)
				{
					//MOUSE CLICK	
					if (!bLButtonState && Application::IsMousePressed(0))
					{
						bLButtonState = true;

						sound.playSoundEffect2D("Sound/sell.mp3");

						if (Singleton::getInstance()->inventory[1]->name == "ghost1") Singleton::getInstance()->money += Singleton::getInstance()->inventory[1]->howMuchItWorth;
						if (Singleton::getInstance()->inventory[1]->name == "ghost2") Singleton::getInstance()->money += Singleton::getInstance()->inventory[1]->howMuchItWorth;
						if (Singleton::getInstance()->inventory[1]->name == "ghost3") Singleton::getInstance()->money += Singleton::getInstance()->inventory[1]->howMuchItWorth;

						if (Singleton::getInstance()->inventory.size() > 0)
							Singleton::getInstance()->inventory.erase(Singleton::getInstance()->inventory.begin() + 1);
						break;
					}
					else if (bLButtonState && !Application::IsMousePressed(0))
					{
						bLButtonState = false;
					}

					//MOUSE HOVER
					RenderImageOnScreen(meshList[UI_HOVER], Vector3(30, 6.5, 1), Vector3(57.5, 31, 1), Vector3(0, 0, 0));
				}

				//SLOT 3
				if ((725 * Application::GetWindowWidth() / 800 > Singleton::getInstance()->mousex && 425 * Application::GetWindowWidth() / 800 < Singleton::getInstance()->mousex) &&
					(362 * Application::GetWindowHeight() / 600 > Singleton::getInstance()->mousey && 315 * Application::GetWindowHeight() / 600 < Singleton::getInstance()->mousey) && i == 3)
				{
					//MOUSE CLICK	
					if (!bLButtonState && Application::IsMousePressed(0))
					{
						bLButtonState = true;

						sound.playSoundEffect2D("Sound/sell.mp3");

						if (Singleton::getInstance()->inventory[2]->name == "ghost1") Singleton::getInstance()->money += Singleton::getInstance()->inventory[2]->howMuchItWorth;
						if (Singleton::getInstance()->inventory[2]->name == "ghost2") Singleton::getInstance()->money += Singleton::getInstance()->inventory[2]->howMuchItWorth;
						if (Singleton::getInstance()->inventory[2]->name == "ghost3") Singleton::getInstance()->money += Singleton::getInstance()->inventory[2]->howMuchItWorth;

						if (Singleton::getInstance()->inventory.size() > 0)
							Singleton::getInstance()->inventory.erase(Singleton::getInstance()->inventory.begin() + 2);
						break;
					}
					else if (bLButtonState && !Application::IsMousePressed(0))
					{
						bLButtonState = false;
					}

					//MOUSE HOVER
					RenderImageOnScreen(meshList[UI_HOVER], Vector3(30, 6.5, 1), Vector3(57.5, 26, 1), Vector3(0, 0, 0));
				}

				//SLOT 4
				if ((725 * Application::GetWindowWidth() / 800 > Singleton::getInstance()->mousex && 425 * Application::GetWindowWidth() / 800 < Singleton::getInstance()->mousex) &&
					(411 * Application::GetWindowHeight() / 600 > Singleton::getInstance()->mousey && 364 * Application::GetWindowHeight() / 600 < Singleton::getInstance()->mousey) && i == 4)
				{
					//MOUSE CLICK	
					if (!bLButtonState && Application::IsMousePressed(0))
					{
						bLButtonState = true;

						sound.playSoundEffect2D("Sound/sell.mp3");

						if (Singleton::getInstance()->inventory[3]->name == "ghost1") Singleton::getInstance()->money += Singleton::getInstance()->inventory[3]->howMuchItWorth;
						if (Singleton::getInstance()->inventory[3]->name == "ghost2") Singleton::getInstance()->money += Singleton::getInstance()->inventory[3]->howMuchItWorth;
						if (Singleton::getInstance()->inventory[3]->name == "ghost3") Singleton::getInstance()->money += Singleton::getInstance()->inventory[3]->howMuchItWorth;

						if (Singleton::getInstance()->inventory.size() > 0)
							Singleton::getInstance()->inventory.erase(Singleton::getInstance()->inventory.begin() + 3);
						break;
					}
					else if (bLButtonState && !Application::IsMousePressed(0))
					{
						bLButtonState = false;
					}

					//MOUSE HOVER
					RenderImageOnScreen(meshList[UI_HOVER], Vector3(30, 6.5, 1), Vector3(57.5, 21, 1), Vector3(0, 0, 0));
				}

				//SLOT 5
				if ((725 * Application::GetWindowWidth() / 800 > Singleton::getInstance()->mousex && 425 * Application::GetWindowWidth() / 800 < Singleton::getInstance()->mousex) &&
					(460 * Application::GetWindowHeight() / 600 > Singleton::getInstance()->mousey && 413 * Application::GetWindowHeight() / 600 < Singleton::getInstance()->mousey) && i == 5)
				{
					//MOUSE CLICK	
					if (!bLButtonState && Application::IsMousePressed(0))
					{
						bLButtonState = true;

						sound.playSoundEffect2D("Sound/sell.mp3");

						if (Singleton::getInstance()->inventory[4]->name == "ghost1") Singleton::getInstance()->money += Singleton::getInstance()->inventory[4]->howMuchItWorth;
						if (Singleton::getInstance()->inventory[4]->name == "ghost2") Singleton::getInstance()->money += Singleton::getInstance()->inventory[4]->howMuchItWorth;
						if (Singleton::getInstance()->inventory[4]->name == "ghost3") Singleton::getInstance()->money += Singleton::getInstance()->inventory[4]->howMuchItWorth;

						if (Singleton::getInstance()->inventory.size() > 0)
							Singleton::getInstance()->inventory.erase(Singleton::getInstance()->inventory.begin() + 4);
						break;
					}
					else if (bLButtonState && !Application::IsMousePressed(0))
					{
						bLButtonState = false;
					}

					//MOUSE HOVER
					RenderImageOnScreen(meshList[UI_HOVER], Vector3(30, 6.5, 1), Vector3(57.5, 16, 1), Vector3(0, 0, 0));
				}

				//SLOT 6
				if ((725 * Application::GetWindowWidth() / 800 > Singleton::getInstance()->mousex && 425 * Application::GetWindowWidth() / 800 < Singleton::getInstance()->mousex) &&
					(509 * Application::GetWindowHeight() / 600 > Singleton::getInstance()->mousey && 462 * Application::GetWindowHeight() / 600 < Singleton::getInstance()->mousey) && i == 6)
				{
					//MOUSE CLICK	
					if (!bLButtonState && Application::IsMousePressed(0))
					{
						bLButtonState = true;

						sound.playSoundEffect2D("Sound/sell.mp3");

						if (Singleton::getInstance()->inventory[5]->name == "ghost1") Singleton::getInstance()->money += Singleton::getInstance()->inventory[5]->howMuchItWorth;
						if (Singleton::getInstance()->inventory[5]->name == "ghost2") Singleton::getInstance()->money += Singleton::getInstance()->inventory[5]->howMuchItWorth;
						if (Singleton::getInstance()->inventory[5]->name == "ghost3") Singleton::getInstance()->money += Singleton::getInstance()->inventory[5]->howMuchItWorth;

						if (Singleton::getInstance()->inventory.size() > 0)
							Singleton::getInstance()->inventory.erase(Singleton::getInstance()->inventory.begin() + 5);
						break;
					}
					else if (bLButtonState && !Application::IsMousePressed(0))
					{
						bLButtonState = false;
					}

					//MOUSE HOVER
					RenderImageOnScreen(meshList[UI_HOVER], Vector3(30, 6.5, 1), Vector3(57.5, 11, 1), Vector3(0, 0, 0));
				}


			}
		}
	}
	else
	{
		buySize = 10;
		sellSize = 10;
		showBuy = false;
		showSell = false;
		showDefault = true;
	}

}
void SceneHub::RenderSprite()
{
}

void SceneHub::RenderParticle(ParticleObject* particle)
{
	switch (particle->type)
	{
		posPartition = getPartition(particle->pos);
	case PARTICLEOBJECT_TYPE::P_FOUNTAIN_WATER1:
	{
		if (renderCheck(playerPartition, posPartition))
		{

		}
		break;
	}
	default:
		break;
	}
}

void SceneHub::RenderLight()
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

void SceneHub::RenderWorld()
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

void SceneHub::RenderPassGPass()
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

void SceneHub::RenderPassMain()
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

	modelStack.PushMatrix();
	modelStack.Translate(0, 10000, 0);
	modelStack.Scale(1, 1, 1);
	RenderMesh(meshList[GEO_AXES], false);
	modelStack.PopMatrix();

	if (Singleton::getInstance()->showShop == false)
	{
		RenderWeapons(false);
		RenderInventory();
		RenderGUI();
	}
	//Render objects
	SceneBase::Render();
	RenderLight();
	RenderShop();
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
		if (Singleton::getInstance()->showShop == false)
		RenderHUD();
	}

	// Render the crosshair
	//RenderMeshIn2D(meshList[GEO_CROSSHAIR], false, 2.0f);

	//On screen text
	std::ostringstream ss;
    if (instance->stateCheck == false && instance->openDoor == false)
    {
        if (showText)
        {
            ss.str("");
            ss.precision(5);
            ss << "FPS: " << fps;
            RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 2, 3);

            ss.str("");
            ss.precision(5);
            ss << "POS: " << camera.position;
            RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 0, 0), 2.5f, 2, 6);

        }
        if (Singleton::getInstance()->showShop == false)
        {
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
        }

        if (distance_1 <= 50)
        {
            ss.str("");
            ss.precision(5);
            ss << "Press E to travel to the House";
            RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0.8f, 0.8f, 0.8f), 2.5, 20, 30);
        }
        if (distance_2 <= 50)
        {
            if (instance->gotHammer)
            {
                ss.str("");
                ss.precision(5);
                ss << "Press E to travel to Town Square";
                RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0.8f, 0.8f, 0.8f), 2.5, 20, 30);
            }
            else
            {
                ss.str("");
                ss.precision(5);
                ss << "Get the Hammer and clear the House first!";
                RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0.8f, 0.8f, 0.8f), 2.5, 20, 30);
            }
        }
        if (distance_3 <= 50)
        {
            if (instance->gotKey)
            {
                ss.str("");
                ss.precision(5);
                ss << "Press E to travel to the Forest";
                RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0.8f, 0.8f, 0.8f), 2.5, 20, 30);
            }
            else
            {
                ss.str("");
                ss.precision(5);
                ss << "Get the Key and clear Town Square first!";
                RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0.8f, 0.8f, 0.8f), 2.5, 20, 30);
            }
        }
        if (distance_4 <= 50)
        {
            if (instance->gotClear)
            {
                ss.str("");
                ss.precision(5);
                ss << "Press E to travel to the Cemetery";
                RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0.8f, 0.8f, 0.8f), 2.5, 20, 30);
            }
            else
            {
                ss.str("");
                ss.precision(5);
                ss << "Clear the Forest first!";
                RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0.8f, 0.8f, 0.8f), 2.5, 20, 30);
            }
        }
    }
}

void SceneHub::Render()
{
	//PRE RENDER PASS
	RenderPassGPass();

	//MAIN RENDER PASS
	RenderPassMain();
}

void SceneHub::Exit()
{
	//	delete level2_Heights;
	SceneBase::Exit();
}