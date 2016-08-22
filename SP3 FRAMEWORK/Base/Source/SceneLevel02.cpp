#include "SceneLevel02.h"
#include "GL\glew.h"

#include "shader.hpp"
#include "MeshBuilder.h"
#include "Application.h"
#include "Utility.h"
#include "LoadTGA.h"
#include <sstream>
#include "LoadHmap.h"

SceneLevel02::SceneLevel02()
{
}

SceneLevel02::~SceneLevel02()
{
}

static const Vector3 TERRAINSIZE(4000.0f, 200.0f, 4000.0f);

void SceneLevel02::Init()
{
	SceneBase::Init();
	sound.Init();
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

	item1pos = Vector3(0, 200, 0);
	item1 = new AABB(item1pos, Vector3(10, 20, 10));

	item2pos = Vector3(200, 0, 0);
	item2 = new AABB(item2pos, Vector3(10, 20, 10));

	item3pos = Vector3(0, 0, 200);
	item3 = new AABB(item3pos, Vector3(10, 20, 10));

	sound.playSoundEffect3D("Sound/fountain.wav",
		irrklang::vec3df(camera.position.x, camera.position.y, camera.position.z),
		irrklang::vec3df(camera.target.x, camera.target.y, camera.target.z),
		irrklang::vec3df(0, 0, 0),
		true);
}

void SceneLevel02::Update(double dt)
{
	SceneBase::Update(dt);

	sound.Update(irrklang::vec3df(camera.position.x, camera.position.y, camera.position.z), 
		irrklang::vec3df(camera.view.x, camera.view.y, camera.view.z));

	if (Application::IsKeyPressed('Q'))
	{
		openGate = true;
	}
	if (openGate && rotateGate > 0)
	{
		rotateGate--;
	}

	static bool spaceButtonState = false;
	if (Application::IsKeyPressed('E'))
	{
		if (!spaceButtonState)
		{
			spaceButtonState = true;
			Inventory::addObject(item1);
		}
	}
	else if (!Application::IsKeyPressed('E'))
	{
		if (spaceButtonState)
			spaceButtonState = false;
	}

	static bool spaceButtonState2 = false;
	if (Application::IsKeyPressed('Y'))
	{
		if (!spaceButtonState2)
		{
			spaceButtonState2 = true;
			Inventory::addObject(item2);
		}
	}
	else if (!Application::IsKeyPressed('Y'))
	{
		if (spaceButtonState2)
			spaceButtonState2 = false;
	}
	UpdateParticle(dt);

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
	RenderMesh(meshList[TERRAIN], true);
	modelStack.PopMatrix();
}

void SceneLevel02::RenderEnvironment(bool Light)
{
	/*modelStack.PushMatrix();
	modelStack.Translate(0, -50 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), 0);
	modelStack.Scale(10, 30, 10);
	RenderMeshOutlined(meshList[CACTUS], Light);
	modelStack.PopMatrix();*/

	modelStack.PushMatrix();
	modelStack.Translate(0, -48 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), 0);
	modelStack.Scale(2, 2, 2);
	RenderMeshOutlined(meshList[FOUNTAIN], Light);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-1200, -48 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), 100);
	modelStack.Rotate(rotateGate, 0, 1, 0);
	modelStack.Scale(320, 320, 320);
	RenderMeshOutlined(meshList[METAL_GATE], Light);
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

	modelStack.PushMatrix();
	modelStack.Translate(200, -30 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), 100);
	modelStack.Scale(20, 20, 40);
	RenderMeshOutlined(meshList[HEDGE], Light);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(200, -50 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), 0);
	modelStack.Rotate(-90, 0, 1, 0);
	modelStack.Scale(5, 5, 5);
	RenderMeshOutlined(meshList[BENCHES], Light);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(200, -30 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), -100);
	modelStack.Scale(20, 20, 40);
	RenderMeshOutlined(meshList[HEDGE], Light);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-200, -30 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), 100);
	modelStack.Scale(20, 20, 40);
	RenderMeshOutlined(meshList[HEDGE], Light);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-200, -50 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), 0);
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.Scale(5, 5, 5);
	RenderMeshOutlined(meshList[BENCHES], Light);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(-200, -30 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), -100);
	modelStack.Scale(20, 20, 40);
	RenderMeshOutlined(meshList[HEDGE], Light);
	modelStack.PopMatrix();

	for (int i = 0; i < 4; i++)
	{

		modelStack.PushMatrix();
		modelStack.Translate(i * 450 - 750, -50 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), -700);
		modelStack.Rotate(-90, 0, 1, 0);
		modelStack.Scale(200, 200, 200);

		if ((i % 2) == 0)
			RenderMeshOutlined(meshList[HOUSE1], Light);
		else
			RenderMeshOutlined(meshList[HOUSE2], Light);
		
		modelStack.PopMatrix();
	}

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
	Vector3 treePos1;
	Vector3 treePos2;
	Vector3 treePos3;
	Vector3 treePos4;

	treePos1.Set(-200, -60 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), -200);
	treePos2.Set(200, -60 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), -200);
	treePos3.Set(-200, -60 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), 200);
	treePos4.Set(200, -60 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, 1 / TERRAINSIZE.x, 1 / TERRAINSIZE.z), 200);

	modelStack.PushMatrix();
	modelStack.Translate(treePos1.x, treePos1.y, treePos1.z);
	modelStack.Scale(20, 10, 20);
	RenderMeshOutlined(meshList[POT], Light);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(treePos1.x, treePos1.y + 75, treePos1.z);
	modelStack.Rotate(Math::RadianToDegree(atan2(camera.position.x - treePos1.x, camera.position.z - treePos1.z)), 0, 1, 0);
	modelStack.Scale(150, 150, 150);
	RenderMeshOutlined(meshList[COCONUT_TREE], Light);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(treePos2.x, treePos2.y, treePos2.z);
	modelStack.Scale(20, 10, 20);
	RenderMeshOutlined(meshList[POT], Light);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(treePos2.x, treePos2.y + 75, treePos2.z);
	modelStack.Rotate(Math::RadianToDegree(atan2(camera.position.x - treePos2.x, camera.position.z - treePos2.z)), 0, 1, 0);
	modelStack.Scale(150, 150, 150);
	RenderMeshOutlined(meshList[COCONUT_TREE], Light);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(treePos3.x, treePos3.y, treePos3.z);
	modelStack.Scale(20, 10, 20);
	RenderMeshOutlined(meshList[POT], Light);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(treePos3.x, treePos3.y + 75, treePos3.z);
	modelStack.Rotate(Math::RadianToDegree(atan2(camera.position.x - treePos3.x, camera.position.z - treePos3.z)), 0, 1, 0);
	modelStack.Scale(150, 150, 150);
	RenderMeshOutlined(meshList[COCONUT_TREE], Light);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(treePos4.x, treePos4.y, treePos4.z);
	modelStack.Scale(20, 10, 20);
	RenderMeshOutlined(meshList[POT], Light);
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
}

void SceneLevel02::RenderHUD()
{
}

void SceneLevel02::RenderSprite()
{
}

void SceneLevel02::RenderParticle(ParticleObject* particle)
{
	switch (particle->type)
	{
	case PARTICLEOBJECT_TYPE::P_FOUNTAIN_WATER1:
		modelStack.PushMatrix();
		modelStack.Translate(particle->pos.x, particle->pos.y, particle->pos.z);
		//insert billboard code
		modelStack.Scale(particle->scale.x, particle->scale.y, particle->scale.z);
		RenderMesh(meshList[FOUNTAIN_WATER1], false);
		modelStack.PopMatrix();
		break;

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
	RenderEnvironment(false);
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

	for (std::vector<ParticleObject*>::iterator it = particleList.begin(); it != particleList.end(); ++it)
	{
		ParticleObject*particle = (ParticleObject*)*it;
		if (particle->active)
		{
			RenderParticle(particle);
		}
	}

	//On screen text
	std::ostringstream ss;
	ss.precision(5);
	ss << "FPS: " << fps;
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 2, 3);

	ss.str("");
	ss.precision(5);
	ss << "ITEM 1: " << std::to_string(Singleton::getInstance()->objectCount[item1]);
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 0, 0), 3, 2, 6);

	ss.str("");
	ss.precision(5);
	ss << "ITEM 2: " << std::to_string(Singleton::getInstance()->objectCount[item2]);
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 0, 0), 3, 2, 9);

	ss.str("");
	ss.precision(5);
	ss << "ITEM 3: " << std::to_string(Singleton::getInstance()->objectCount[item3]);
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 0, 0), 3, 2, 12);
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
	SceneBase::Exit();
}