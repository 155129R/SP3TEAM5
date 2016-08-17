#include "SceneShadow.h"
#include "GL\glew.h"

#include "shader.hpp"
#include "Application.h"
#include "Utility.h"
#include "LoadTGA.h"
#include <sstream>
#include "LoadHmap.h"
#include "Bullet.h"

SceneShadow::SceneShadow()
{
}

SceneShadow::~SceneShadow()
{
}

static const Vector3 TERRAINSIZE(4000.0f, 200.0f, 4000.0f);

void SceneShadow::Init()
{
	SceneBase::Init();

	player = new Player();

	player->Init();

	terrainHeight = TERRAINSIZE.y;

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

	for (int i = 0; i < 400; ++i)
	{
		Tree[i].Set(Math::RandIntMinMax(-1800, 1800), 0, Math::RandIntMinMax(-800, 1800));
		Tree_Type[i] = Math::RandIntMinMax(1, 3);
	}
	for (int i = 0; i < 400; ++i)
	{
		Bush[i].Set(Math::RandIntMinMax(-2000, 2000), 0, Math::RandIntMinMax(-1000, 2000));
	}
}

void SceneShadow::Update(double dt)
{
	SceneBase::Update(dt);

	UpdateParticle(dt);
	UpdateBullet(dt);


	UpdatePlayer(dt);

	//Update sprites
	if (G1)
	{
		G1->Update(dt);
		G1->m_anim->animActive = true;
	}
	if (G2)
	{
		G2->Update(dt);
		G2->m_anim->animActive = true;
	}
	if (G3)
	{
		G3->Update(dt);
		G3->m_anim->animActive = true;
	}

	//camera.Terrain = TERRAINSIZE.y * ReadHeightMap(m_heightMap, camera.position.x / TERRAINSIZE.x, camera.position.z / TERRAINSIZE.z);
	camera.Terrain = getHeightofTerrain(TERRAINSIZE.x, level3_Heights);

	camera.Update(dt);

	//shoot
	if (Application::IsKeyPressed(VK_SPACE))
	{
		Bullet::bulletList.push_back(new Bullet(Vector3(camera.position.x, camera.position.y - 2, camera.position.z), Vector3(1, 0, 1), 150, 100, 10));
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

void SceneShadow::UpdateBullet(double dt)
{
	for (vector<Bullet*>::iterator it = Bullet::bulletList.begin(); it != Bullet::bulletList.end();){
		if ((*it)->deleteBullet == true){
			delete *it;
			it = Bullet::bulletList.erase(it);
		}
		else{
			(*it)->Update(dt);
			it++;
		}
	}
}

void SceneShadow::UpdateParticle(double dt)
{
	if (m_particlesCount < MAX_PARTICLE)
	{
		ParticleObject* particle = GetParticles();
		particle->type = PARTICLEOBJECT_TYPE::P_WATER;
		particle->pos.Set(Math::RandFloatMinMax(-1700.0f, 1700.0f), 1200.0f, Math::RandFloatMinMax(-1700.0f, 1700.0f));
		particle->vel.Set(0, -10, 0);
		particle->scale.Set(10, 10, 10);
		particle->rotateSpeed = Math::RandFloatMinMax(20.0f, 40.0f);

		ParticleObject* particleSmoke = GetParticles();
		particleSmoke->type = PARTICLEOBJECT_TYPE::P_FOUNTAIN_WATER1;
		//particleSmoke->scale.Set(5, 5, 5);
		float range = 2;
		particleSmoke->vel.Set(Math::RandFloatMinMax(-range, range),
			Math::RandFloatMinMax(-range, range),
			Math::RandFloatMinMax(-range, range));
		particleSmoke->rotateSpeed = Math::RandFloatMinMax(20.f, 40.f);
		particleSmoke->pos.Set(0, 20 + 350.f * ReadHeightMap(m_heightMap, -20.f / 4000, -20.f / 4000), 0);
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

void SceneShadow::UpdatePlayer(double dt)
{
	player->GetCamera(camera);
	player->Update(dt);

	if (Application::IsKeyPressed(VK_NUMPAD0))
	{
		player->InflictFear(5);
	}

	if (player->GetStamina() <= 0.0f)
	{
		camera.Tired = true;
	}
	else
	{
		camera.Tired = false;
	}

	UpdateFearEffect(dt);
}

void SceneShadow::UpdateFearEffect(double dt)
{
	switch (player->GetFear())
	{
		case 1:
			break;

		case 2:
			FogAmount = 1000.0f;
			glUniform1f(m_parameters[U_FOG_END], FogAmount);
			Black.Set(0.0f, 0.0f, 0.0f);
			glUniform3fv(m_parameters[U_FOG_COLOR], 1, &Black.r);
			break;

		case 3:
			FogAmount = 700.0f;
			glUniform1f(m_parameters[U_FOG_END], FogAmount);
			Black.Set(0.0f, 0.0f, 0.0f);
			glUniform3fv(m_parameters[U_FOG_COLOR], 1, &Black.r);
			break;

		case 4:
			FogAmount = 500.0f;
			glUniform1f(m_parameters[U_FOG_END], FogAmount);
			Black.Set(0.0f, 0.0f, 0.0f);
			glUniform3fv(m_parameters[U_FOG_COLOR], 1, &Black.r);
			break;

		case 5:
			FogAmount = 100.0f;
			glUniform1f(m_parameters[U_FOG_END], FogAmount);
			Black.Set(0.0f, 0.0f, 0.0f);
			glUniform3fv(m_parameters[U_FOG_COLOR], 1, &Black.r);
			break;
	}
}

ParticleObject* SceneShadow::GetParticles(void)
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

void SceneShadow::RenderGround()
{
	modelStack.PushMatrix();
	modelStack.Rotate(-90, 1, 0, 0);
	modelStack.Translate(0, 0, -20);
	modelStack.Rotate(-90, 0, 0, 1);
	modelStack.Scale(100.0f, 100.0f, 100.0f);

	for (int x=0; x<10; x++)
	{
		for (int z=0; z<10; z++)
		{
			modelStack.PushMatrix();
			modelStack.Translate(x-5.0f, z-5.0f, 0.0f);
			if ( ((x*9+z) % 2) == 0)
				RenderMesh(meshList[GEO_GRASS_DARKGREEN], false);
			else
				RenderMesh(meshList[GEO_GRASS_LIGHTGREEN], false);
			modelStack.PopMatrix();
		}
	}
	modelStack.PopMatrix();
}

void SceneShadow::RenderSkyplane(bool inverted)
{
	modelStack.PushMatrix();
	modelStack.Translate(camera.position.x , 1800, camera.position.z);
	modelStack.Rotate(rotateAngle, 0, 1, 0);
	if (inverted)
	{
		modelStack.Rotate(180, 1, 0, 0);
	}
	RenderMesh(meshList[SKYPLANE], false);
	modelStack.PopMatrix();
}

void SceneShadow::RenderTerrain()
{
	modelStack.PushMatrix();
	modelStack.Translate(0, -50, 0);
	modelStack.Scale(TERRAINSIZE.x, TERRAINSIZE.y, TERRAINSIZE.z);
	RenderMesh(meshList[TERRAIN], true);
	modelStack.PopMatrix();
}

void SceneShadow::RenderEnvironment(bool Light, bool inverted)
{
	if (!inverted)
	{
		for (int i = 0; i < 400; ++i)
		{
			float Degree = Math::RadianToDegree(atan2(-(Tree[i].z - player->pos.z), Tree[i].x - player->pos.x));
			switch (Tree_Type[i])
			{
				case 1:
				{
					modelStack.PushMatrix();
					modelStack.Translate(Tree[i].x, 100 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, Tree[i].x / TERRAINSIZE.x, Tree[i].z / TERRAINSIZE.z), Tree[i].z);
					modelStack.Rotate(Degree - 90, 0, 1, 0);
					modelStack.Scale(200, 300, 200);
					RenderMeshOutlined(meshList[GEO_TREE_1], false);
					modelStack.PopMatrix();
					break;
				}
				case 2:
				{
					modelStack.PushMatrix();
					modelStack.Translate(Tree[i].x, 80 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, Tree[i].x / TERRAINSIZE.x, Tree[i].z / TERRAINSIZE.z), Tree[i].z);
					modelStack.Rotate(Degree - 90, 0, 1, 0);
					modelStack.Scale(300, 300, 300);
					RenderMeshOutlined(meshList[GEO_TREE_2], false);
					modelStack.PopMatrix();
					break;
				}
				case 3:
				{
					modelStack.PushMatrix();
					modelStack.Translate(Tree[i].x, 100 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, Tree[i].x / TERRAINSIZE.x, Tree[i].z / TERRAINSIZE.z), Tree[i].z);
					modelStack.Rotate(Degree - 90, 0, 1, 0);
					modelStack.Scale(100, 300, 100);
					RenderMeshOutlined(meshList[GEO_TREE_3], false);
					modelStack.PopMatrix();
					break;
				}
			}
		}

		for (int i = 0; i < 400; ++i)
		{
			float Degree = Math::RadianToDegree(atan2(-(Bush[i].z - player->pos.z), Bush[i].x - player->pos.x));
			modelStack.PushMatrix();
			modelStack.Translate(Bush[i].x, -50 + TERRAINSIZE.y * ReadHeightMap(m_heightMap, Bush[i].x / TERRAINSIZE.x, Bush[i].z / TERRAINSIZE.z), Bush[i].z);
			modelStack.Rotate(Degree - 90, 0, 1, 0);
			modelStack.Scale(100, 100, 100);
			RenderMeshOutlined(meshList[GEO_BUSH], false);
			modelStack.PopMatrix();
		}

		//modelStack.PushMatrix();
		//modelStack.Translate(0, 100, -1400);
		//modelStack.Scale(10, 30, 10);
		//RenderMeshOutlined(meshList[GEO_CACTUS], false);
		//modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(1900, 0, -1400);
		modelStack.Rotate(90, 0, 1, 0);
		modelStack.Scale(4, 5, 5);
		RenderMeshOutlined(meshList[GEO_LOGS], false);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(-1900, 0, -1400);
		modelStack.Rotate(90, 0, 1, 0);
		modelStack.Scale(4, 5, 5);
		RenderMeshOutlined(meshList[GEO_LOGS], false);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(0, 60, -1420);
		modelStack.Scale(5, 5, 5);
		RenderMeshOutlined(meshList[GEO_BRIDGE], false);
		modelStack.PopMatrix();
	}


	if (inverted)
	{
		//modelStack.PushMatrix();
		//modelStack.Translate(0, 100, -1400);
		//modelStack.Rotate(180, 1, 0, 0);
		//modelStack.Scale(10, 30, 10);
		//RenderMesh(meshList[GEO_CACTUS], false);
		//modelStack.PopMatrix();
	}
}

void SceneShadow::RenderHUD()
{
	//std::cout << player->GetStamina() << std::endl;
	RenderImageOnScreen(meshList[GEO_STAMINA], Vector3(100, 2, 1), Vector3(50 - (100 - player->GetStamina() / 3) , 1, 0), Vector3(0, 0, 0));
}

void SceneShadow::RenderSprite()
{
	modelStack.PushMatrix();
	modelStack.Translate(150, 0, 0);
	modelStack.Scale(25, 25, 25);
	RenderMesh(meshList[GEO_GHOST1], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(100, 0, 0);
	modelStack.Scale(25, 25, 25);
	RenderMesh(meshList[GEO_GHOST2], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(50, 0, 0);
	modelStack.Scale(25, 25, 25);
	RenderMesh(meshList[GEO_GHOST3], false);
	modelStack.PopMatrix();
}

void SceneShadow::RenderParticle(ParticleObject* particle)
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

	default :
		break;
	}
}

void SceneShadow::RenderLight()
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

void SceneShadow::RenderWorld()
{
	glUniform1f(m_parameters[U_FOG_ENABLE], 1);
	RenderSkyplane(); 
	RenderTerrain();
	RenderEnvironment(false);
	//RenderSprite();

	glUniform1f(m_parameters[U_FOG_ENABLE], 0);
}

void SceneShadow::RenderPassGPass()
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

void SceneShadow::RenderPassMain()
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

	//bullet
	for (vector<Bullet*>::iterator it = Bullet::bulletList.begin(); it != Bullet::bulletList.end(); ++it){
		modelStack.PushMatrix();
		modelStack.Translate(
			(*it)->position.x,
			(*it)->position.y,
			(*it)->position.z
			);
		modelStack.Scale(1, 1, 1);
		RenderMesh(meshList[GEO_LIGHTBALL], false);
		modelStack.PopMatrix();
	}


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

	glDisable(GL_CULL_FACE);
	
	glDepthMask(GL_FALSE); // Don't write to depth buffer
	glEnable(GL_STENCIL_TEST);
	
	// Reflection surface
	glStencilFunc(GL_ALWAYS, 1, 0xFF); // Set any stencil to 1
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	glStencilMask(0xFF); // Write to stencil buffer
	glClear(GL_STENCIL_BUFFER_BIT); // Clear stencil buffer (0 by default)
	
	modelStack.PushMatrix();
	modelStack.Translate(0, 100, -1440);
	modelStack.Rotate(-90, 1, 0, 0);
	modelStack.Rotate(-90, 0, 0, 1);
	modelStack.Scale(800, 4000, 1);
	RenderMesh(meshList[WATER], false);
	modelStack.PopMatrix();
	
	// Reflection
	glStencilFunc(GL_EQUAL, 1, 0xFF); // Pass test if stencil value is 1
	glStencilMask(0x00); // Don't write anything to stencil buffer
	glDepthMask(GL_TRUE); // Write to depth buffer
	
	//modelStack.PushMatrix();
	//modelStack.Scale(1,-1,1);
	//RenderSkyplane();
	//RenderEnvironment(false);
	//modelStack.PopMatrix();


	modelStack.PushMatrix();
	RenderEnvironment(false,true);
	modelStack.PopMatrix();
	
	glDisable(GL_STENCIL_TEST);
	
	glEnable(GL_CULL_FACE);

	modelStack.PushMatrix();
	modelStack.Translate(0, 100, -1440);
	modelStack.Rotate(-90, 1, 0, 0);
	modelStack.Rotate(-90, 0, 0, 1);
	modelStack.Scale(800, 4000, 1);
	RenderMesh(meshList[WATER], false);
	modelStack.PopMatrix();

	RenderHUD();

	//On screen text
	std::ostringstream ss;
	ss.precision(5);
	ss << "FPS: " << fps;
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 2, 3);

}

void SceneShadow::Render()
{
	//PRE RENDER PASS
	RenderPassGPass();

	//MAIN RENDER PASS
	RenderPassMain();
}

void SceneShadow::Exit()
{
	SceneBase::Exit();
}