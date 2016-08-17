#include "SceneBase.h"
#include "GL\glew.h"

#include "shader.hpp"
#include "MeshBuilder.h"
#include "Application.h"
#include "Utility.h"
#include "LoadTGA.h"
#include <sstream>

SceneBase::SceneBase()
{
}

SceneBase::~SceneBase()
{
}

void SceneBase::Init()
{
	Math::InitRNG();

	// Black background
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);

	glEnable(GL_CULL_FACE);
	glEnable(GL_SAMPLE_ALPHA_TO_COVERAGE);

	glEnable(GL_POLYGON_OFFSET_FILL);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glGenVertexArrays(1, &m_vertexArrayID);
	glBindVertexArray(m_vertexArrayID);

	m_gPassShaderID = LoadShaders("Shader//GPass.vertexshader", "Shader//GPass.fragmentshader");
	m_parameters[U_LIGHT_DEPTH_MVP_GPASS] = glGetUniformLocation(m_gPassShaderID, "lightDepthMVP");

	m_programID = LoadShaders("Shader//Shadow.vertexshader", "Shader//Shadow.fragmentshader");
	m_parameters[U_LIGHT_DEPTH_MVP] = glGetUniformLocation(m_programID, "lightDepthMVP");

	m_parameters[U_SHADOW_MAP] = glGetUniformLocation(m_programID, "shadowMap");

	// Get a handle for our uniform
	m_parameters[U_MVP] = glGetUniformLocation(m_programID, "MVP");
	m_parameters[U_MODELVIEW] = glGetUniformLocation(m_programID, "MV");
	m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE] = glGetUniformLocation(m_programID, "MV_inverse_transpose");
	m_parameters[U_MATERIAL_AMBIENT] = glGetUniformLocation(m_programID, "material.kAmbient");
	m_parameters[U_MATERIAL_DIFFUSE] = glGetUniformLocation(m_programID, "material.kDiffuse");
	m_parameters[U_MATERIAL_SPECULAR] = glGetUniformLocation(m_programID, "material.kSpecular");
	m_parameters[U_MATERIAL_SHININESS] = glGetUniformLocation(m_programID, "material.kShininess");
	m_parameters[U_LIGHTENABLED] = glGetUniformLocation(m_programID, "lightEnabled");
	m_parameters[U_NUMLIGHTS] = glGetUniformLocation(m_programID, "numLights");
	m_parameters[U_LIGHT0_TYPE] = glGetUniformLocation(m_programID, "lights[0].type");
	m_parameters[U_LIGHT0_POSITION] = glGetUniformLocation(m_programID, "lights[0].position_cameraspace");
	m_parameters[U_LIGHT0_COLOR] = glGetUniformLocation(m_programID, "lights[0].color");
	m_parameters[U_LIGHT0_POWER] = glGetUniformLocation(m_programID, "lights[0].power");
	m_parameters[U_LIGHT0_KC] = glGetUniformLocation(m_programID, "lights[0].kC");
	m_parameters[U_LIGHT0_KL] = glGetUniformLocation(m_programID, "lights[0].kL");
	m_parameters[U_LIGHT0_KQ] = glGetUniformLocation(m_programID, "lights[0].kQ");
	m_parameters[U_LIGHT0_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[0].spotDirection");
	m_parameters[U_LIGHT0_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[0].cosCutoff");
	m_parameters[U_LIGHT0_COSINNER] = glGetUniformLocation(m_programID, "lights[0].cosInner");
	m_parameters[U_LIGHT0_EXPONENT] = glGetUniformLocation(m_programID, "lights[0].exponent");
	m_parameters[U_LIGHT1_TYPE] = glGetUniformLocation(m_programID, "lights[1].type");
	m_parameters[U_LIGHT1_POSITION] = glGetUniformLocation(m_programID, "lights[1].position_cameraspace");
	m_parameters[U_LIGHT1_COLOR] = glGetUniformLocation(m_programID, "lights[1].color");
	m_parameters[U_LIGHT1_POWER] = glGetUniformLocation(m_programID, "lights[1].power");
	m_parameters[U_LIGHT1_KC] = glGetUniformLocation(m_programID, "lights[1].kC");
	m_parameters[U_LIGHT1_KL] = glGetUniformLocation(m_programID, "lights[1].kL");
	m_parameters[U_LIGHT1_KQ] = glGetUniformLocation(m_programID, "lights[1].kQ");
	m_parameters[U_LIGHT1_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[1].spotDirection");
	m_parameters[U_LIGHT1_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[1].cosCutoff");
	m_parameters[U_LIGHT1_COSINNER] = glGetUniformLocation(m_programID, "lights[1].cosInner");
	m_parameters[U_LIGHT1_EXPONENT] = glGetUniformLocation(m_programID, "lights[1].exponent");
	// Get a handle for our "colorTexture" uniform
	m_parameters[U_COLOR_TEXTURE_ENABLED] = glGetUniformLocation(m_programID, "colorTextureEnabled[0]");
	m_parameters[U_COLOR_TEXTURE] = glGetUniformLocation(m_programID, "colorTexture[0]");
	m_parameters[U_COLOR_TEXTURE_ENABLED1] = glGetUniformLocation(m_programID, "colorTextureEnabled[1]");
	m_parameters[U_COLOR_TEXTURE1] = glGetUniformLocation(m_programID, "colorTexture[1]");
	// Get a handle for our "textColor" uniform
	m_parameters[U_TEXT_ENABLED] = glGetUniformLocation(m_programID, "textEnabled");
	m_parameters[U_TEXT_COLOR] = glGetUniformLocation(m_programID, "textColor");

	m_parameters[U_SHADOW_COLOR_TEXTURE_ENABLED] = glGetUniformLocation(m_gPassShaderID, "colorTextureEnabled[0]");
	m_parameters[U_SHADOW_COLOR_TEXTURE] = glGetUniformLocation(m_gPassShaderID, "colorTexture[0]");
	m_parameters[U_SHADOW_COLOR_TEXTURE_ENABLED1] = glGetUniformLocation(m_gPassShaderID, "colorTextureEnabled[1]");
	m_parameters[U_SHADOW_COLOR_TEXTURE1] = glGetUniformLocation(m_gPassShaderID, "colorTexture[1]");
	m_parameters[U_SHADOW_COLOR_TEXTURE_ENABLED2] = glGetUniformLocation(m_gPassShaderID, "colorTextureEnabled[2]");
	m_parameters[U_SHADOW_COLOR_TEXTURE2] = glGetUniformLocation(m_gPassShaderID, "colorTexture[2]");

	m_parameters[U_FOG_COLOR] = glGetUniformLocation(m_programID, "fogParam.color");
	m_parameters[U_FOG_START] = glGetUniformLocation(m_programID, "fogParam.start");
	m_parameters[U_FOG_END] = glGetUniformLocation(m_programID, "fogParam.end");
	m_parameters[U_FOG_DENSITY] = glGetUniformLocation(m_programID, "fogParam.density");
	m_parameters[U_FOG_TYPE] = glGetUniformLocation(m_programID, "fogParam.type");
	m_parameters[U_FOG_ENABLE] = glGetUniformLocation(m_programID, "fogParam.enabled");

	// Use our shader
	glUseProgram(m_programID);

	lights[0].type = Light::LIGHT_DIRECTIONAL;
	lights[0].position.Set(20, 40, 20);
	lights[0].color.Set(1, 1, 1);
	lights[0].power = 1.0f;
	lights[0].kC = 1.f;
	lights[0].kL = 0.01f;
	lights[0].kQ = 0.001f;
	//lights[0].cosCutoff = cos(Math::DegreeToRadian(45));
	//lights[0].cosInner = cos(Math::DegreeToRadian(30));
	//lights[0].exponent = 1.f;
	//lights[0].spotDirection.Set(0.f, 1.f, 0.f);

	lights[1].type = Light::LIGHT_SPOT;
	lights[1].position.Set(0, 0, 0);
	lights[1].color.Set(1, 1, 1);
	lights[1].power = 0.0f;
	lights[1].kC = 1.f;
	lights[1].kL = 0.01f;
	lights[1].kQ = 0.001f;
	lights[1].cosCutoff = cos(Math::DegreeToRadian(18));
	lights[1].cosInner = cos(Math::DegreeToRadian(30));
	lights[1].exponent = 1.f;
	lights[1].spotDirection.Set(0.f, 1.f, 0.f);

	glUniform1i(m_parameters[U_NUMLIGHTS], 2);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);

	glUniform1i(m_parameters[U_LIGHT0_TYPE], lights[0].type);
	glUniform3fv(m_parameters[U_LIGHT0_COLOR], 1, &lights[0].color.r);
	glUniform1f(m_parameters[U_LIGHT0_POWER], lights[0].power);
	glUniform1f(m_parameters[U_LIGHT0_KC], lights[0].kC);
	glUniform1f(m_parameters[U_LIGHT0_KL], lights[0].kL);
	glUniform1f(m_parameters[U_LIGHT0_KQ], lights[0].kQ);
	glUniform1f(m_parameters[U_LIGHT0_COSCUTOFF], lights[0].cosCutoff);
	glUniform1f(m_parameters[U_LIGHT0_COSINNER], lights[0].cosInner);
	glUniform1f(m_parameters[U_LIGHT0_EXPONENT], lights[0].exponent);

	glUniform1i(m_parameters[U_LIGHT1_TYPE], lights[1].type);
	glUniform3fv(m_parameters[U_LIGHT1_COLOR], 1, &lights[1].color.r);
	glUniform1f(m_parameters[U_LIGHT1_POWER], lights[1].power);
	glUniform1f(m_parameters[U_LIGHT1_KC], lights[1].kC);
	glUniform1f(m_parameters[U_LIGHT1_KL], lights[1].kL);
	glUniform1f(m_parameters[U_LIGHT1_KQ], lights[1].kQ);
	glUniform1f(m_parameters[U_LIGHT1_COSCUTOFF], lights[1].cosCutoff);
	glUniform1f(m_parameters[U_LIGHT1_COSINNER], lights[1].cosInner);
	glUniform1f(m_parameters[U_LIGHT1_EXPONENT], lights[1].exponent);

	//FOG
	Color fogColor(0.8f, 0.8f, 0.8f);
	FogAmount = 1500.0f;
	glUniform3fv(m_parameters[U_FOG_COLOR], 1, &fogColor.r);
	glUniform1f(m_parameters[U_FOG_START], 10);
	glUniform1f(m_parameters[U_FOG_END], FogAmount);
	glUniform1f(m_parameters[U_FOG_DENSITY], 0.005f);
	glUniform1f(m_parameters[U_FOG_TYPE], 0);
	glUniform1f(m_parameters[U_FOG_ENABLE], 0);

	camera.Init(Vector3(0, 100, 10), Vector3(0, 100, 1), Vector3(0, 1, 0));

	for (int i = 0; i < NUM_GEOMETRY; ++i)
	{
		meshList[i] = NULL;
	}
	meshList[GEO_AXES] = MeshBuilder::GenerateAxes("reference");//, 1000, 1000, 1000);
	meshList[GEO_CROSSHAIR] = MeshBuilder::GenerateCrossHair("crosshair");
	meshList[GEO_QUAD] = MeshBuilder::GenerateQuad("quad", Color(1, 1, 1), 1.f);
	//meshList[GEO_QUAD]->textureID = LoadTGA("Image//calibri.tga");
	meshList[GEO_TEXT] = MeshBuilder::GenerateText("text", 16, 16);
	meshList[GEO_TEXT]->textureID = LoadTGA("Image//calibri.tga");
	meshList[GEO_TEXT]->material.kAmbient.Set(1, 0, 0);
	meshList[GEO_RING] = MeshBuilder::GenerateRing("ring", Color(1, 0, 1), 36, 1, 0.5f);
	meshList[GEO_LIGHTBALL] = MeshBuilder::GenerateSphere("lightball", Color(0, 0, 0), 18, 36, 1.f);
	meshList[GEO_SPHERE] = MeshBuilder::GenerateSphere("sphere", Color(1, 0, 0), 18, 36, 10.f);
	meshList[GEO_CUBE] = MeshBuilder::GenerateCube("cube", Color(1, 1, 1), 4);
	//meshList[GEO_TORUS] = MeshBuilder::GenerateCylinder("torus", 36, 36, 5, 1);
	meshList[GEO_CONE] = MeshBuilder::GenerateCone("cone", Color(0.5f, 1, 0.3f), 36, 10.f, 10.f);
	//meshList[GEO_CONE]->material.kDiffuse.Set(0.99f, 0.99f, 0.99f);
	//meshList[GEO_CONE]->material.kSpecular.Set(0.f, 0.f, 0.f);

	//Skyplane
	meshList[SKYPLANE] = MeshBuilder::GenerateSkyplane("WEEE", Color(1, 1, 1), 256, 1000.0f, 4000.0f, 1.0f, 1.0f);
	meshList[SKYPLANE]->textureArray[0] = LoadTGA("Image//Sky.tga");

	//Terrain 
	meshList[TERRAIN] = MeshBuilder::GenerateTerrain("Terrain", "Image//Terrain_Default.raw", m_heightMap, level1_Heights);
	meshList[TERRAIN]->textureArray[0] = LoadTGA("Image//Forest//Grass.tga");
	meshList[TERRAIN_LEVEL04] = MeshBuilder::GenerateTerrain("Terrain", "Image//Terrain_Level4.raw", m_heightMap_4, level4_Heights);
	meshList[TERRAIN_LEVEL04]->textureArray[0] = LoadTGA("Image//level4_ground.tga");

	//level 1 terrain
	meshList[LEVEL01_TERRAIN] = MeshBuilder::GenerateTerrain("level01 terrain", "Image//Terrain_Level01.raw", m_heightMap, level1_Heights);
	meshList[LEVEL01_TERRAIN]->textureArray[0] = LoadTGA("Image//Forest//Grass.tga");

	meshList[LEVEL01_WALLS] = MeshBuilder::GenerateQuad("walls", Color(0, 0, 0), 1.f);
	meshList[LEVEL01_WALLS]->textureArray[0] = LoadTGA("Image//walltex.tga");

	meshList[STAIRS] = MeshBuilder::GenerateOBJ("stairs", "OBJ//Stairs.obj");
	meshList[STAIRS]->textureArray[0] = LoadTGA("Image//stairs.tga");

	meshList[DOOR] = MeshBuilder::GenerateOBJ("Door", "OBJ//door.obj");
	meshList[DOOR]->textureArray[0] = LoadTGA("Image//door.tga");

	//meshList[ELEVATORDOOR] = MeshBuilder::GenerateOBJ("elevator", "OBJ//elevator.obj");
	//meshList[ELEVATORDOOR]->textureArray[0] = LoadTGA("Image//elevator.tga");

	meshList[WATER] = MeshBuilder::GenerateQuad("Water", Color(0, 0, 0), 1.f);
	meshList[WATER]->textureArray[0] = LoadTGA("Image//sea.tga");
	meshList[WATER_SURFACE] = MeshBuilder::GenerateQuad("Water Surace", Color(0, 0, 0), 1.f);
	meshList[WATER_SURFACE]->textureArray[0] = LoadTGA("Image//sea2.tga");

	meshList[GEO_CACTUS] = MeshBuilder::GenerateOBJ("Cactus", "OBJ//Cactus.obj");
	meshList[GEO_CACTUS]->textureArray[0] = LoadTGA("Image//Cactus.tga");

	meshList[FOUNTAIN] = MeshBuilder::GenerateOBJ("fountain", "OBJ//fountain.obj");
	meshList[FOUNTAIN]->textureArray[0] = LoadTGA("Image//fountain.tga");

	meshList[FOUNTAIN_WATER1] = MeshBuilder::GenerateSphere("sphere", Color(0.2f, 0.7f, 1), 18, 36, 1.f);
	meshList[FOUNTAIN_WATER1]->textureArray[0] = LoadTGA("Image//water.tga");

	meshList[FLOOR] = MeshBuilder::GenerateQuad2("floor", Color(0, 0, 0), 10,10,TexCoord(10,10));
	meshList[FLOOR]->textureArray[0] = LoadTGA("Image//floor.tga");

	meshList[HOUSE1] = MeshBuilder::GenerateOBJ("house", "OBJ//house.obj");
	meshList[HOUSE1]->textureArray[0] = LoadTGA("Image//houseTex.tga");

	meshList[HOUSE2] = MeshBuilder::GenerateOBJ("house", "OBJ//house.obj");
	meshList[HOUSE2]->textureArray[0] = LoadTGA("Image//houseTex2.tga");

	meshList[POT] = MeshBuilder::GenerateOBJ("pot", "OBJ//pot.obj");
	meshList[POT]->textureArray[0] = LoadTGA("Image//pot.tga");

	meshList[COCONUT_TREE] = MeshBuilder::GenerateQuad("Water", Color(0, 0, 0), 1.f);
	meshList[COCONUT_TREE]->textureArray[0] = LoadTGA("Image//coconutTree.tga");

	meshList[TOMBSTONE] = MeshBuilder::GenerateOBJ("Cactus", "OBJ//Tombstone.obj");
	meshList[TOMBSTONE]->textureArray[0] = LoadTGA("Image//Tombstone.tga");

	//Particles
	meshList[GEO_PARTICLE_WATER] = MeshBuilder::GenerateSphere("lightball", Color(0.5, 0.5, 1), 18, 36, 1.f);
	meshList[GEO_PARTICLE_SAND] = MeshBuilder::GenerateSphere("Sand particle", Color(0.8f, 0.7f, 0.5f), 18, 36, 1.f);

	//Player
	meshList[GEO_STAMINA] = MeshBuilder::GenerateQuad("Stamina", Color(0, 1, 0), 1.f);

	//Forest
	meshList[GEO_TREE_1] = MeshBuilder::GenerateQuad("Thin Tree", Color(0, 0, 0), 1.f);
	meshList[GEO_TREE_1]->textureArray[0] = LoadTGA("Image//Forest//Tree.tga");
	meshList[GEO_TREE_2] = MeshBuilder::GenerateQuad("Fat Tree", Color(0, 0, 0), 1.f);
	meshList[GEO_TREE_2]->textureArray[0] = LoadTGA("Image//Forest//Fat_Tree.tga");
	meshList[GEO_TREE_3] = MeshBuilder::GenerateQuad("Dead Tree", Color(0, 0, 0), 1.f);
	meshList[GEO_TREE_3]->textureArray[0] = LoadTGA("Image//Forest//Dead_Tree.tga");
	meshList[GEO_BUSH] = MeshBuilder::GenerateQuad("Water", Color(0, 0, 0), 1.f);
	meshList[GEO_BUSH]->textureArray[0] = LoadTGA("Image//Forest//Bush.tga");

	//Sprite
	meshList[GEO_GHOST1] = MeshBuilder::GenerateSpriteAnimation("TumbleWeed", 4, 3);
	meshList[GEO_GHOST1]->textureArray[0] = LoadTGA("Image//Ghosts//Ghost_1.tga");
	meshList[GEO_GHOST2] = MeshBuilder::GenerateSpriteAnimation("Horsey", 4, 3);
	meshList[GEO_GHOST2]->textureArray[0] = LoadTGA("Image//Ghosts//Ghost_2.tga");
	meshList[GEO_GHOST3] = MeshBuilder::GenerateSpriteAnimation("Horsey", 3, 6);
	meshList[GEO_GHOST3]->textureArray[0] = LoadTGA("Image//Ghosts//Ghost_3.tga");

	//Shadow stuff
	meshList[GEO_LIGHT_DEPTH_QUAD] = MeshBuilder::GenerateQuad("Shadow Test", 1, 1);
	meshList[GEO_LIGHT_DEPTH_QUAD]->textureArray[0] = m_lightDepthFBO.GetTexture();

	G1 = dynamic_cast<SpriteAnimation*>(meshList[GEO_GHOST1]);
	G2 = dynamic_cast<SpriteAnimation*>(meshList[GEO_GHOST2]);
	G3 = dynamic_cast<SpriteAnimation*>(meshList[GEO_GHOST3]);

	if (G1)
	{
		G1->m_anim = new Animation();
		G1->m_anim->Set(0, 11, 0, 2.0f, true);
	}

	if (G2)
	{
		G2->m_anim = new Animation();
		G2->m_anim->Set(0, 11, 0, 2.0f, true);
	}
	
	if (G3)
	{
		G3->m_anim = new Animation();
		G3->m_anim->Set(0, 17, 0, 2.0f, true);
	}

	characterHeight = 7.f;
}

void SceneBase::Update(double dt)
{
	if (Application::IsKeyPressed('I'))
	{
		lights[0].position.z -= (float)50 * dt;
	}
	if (Application::IsKeyPressed('K'))
	{
		lights[0].position.z += (float)50 * dt;
	}
	if (Application::IsKeyPressed('J'))
	{
		lights[0].position.x -= (float)50 * dt;
	}
	if (Application::IsKeyPressed('L'))
	{
		lights[0].position.x += (float)50 * dt;
	}
	if (Application::IsKeyPressed('O'))
	{
		lights[0].position.y -= (float)50 * dt;
	}
	if (Application::IsKeyPressed('P'))
	{
		lights[0].position.y += (float)50 * dt;
	}

	if (Application::IsKeyPressed('1'))
		glEnable(GL_CULL_FACE);
	if (Application::IsKeyPressed('2'))
		glDisable(GL_CULL_FACE);
	if (Application::IsKeyPressed('3'))
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	if (Application::IsKeyPressed('4'))
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}

	else if (Application::IsKeyPressed('8'))
	{
		bLightEnabled = true;
	}
	else if (Application::IsKeyPressed('9'))
	{
		bLightEnabled = false;
	}
}

void SceneBase::RenderText(Mesh* mesh, std::string text, Color color)
{
	if (!mesh || mesh->textureID <= 0)
		return;

	glDisable(GL_DEPTH_TEST);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 1);
	glUniform3fv(m_parameters[U_TEXT_COLOR], 1, &color.r);
	glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh->textureID);
	glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	for (unsigned i = 0; i < text.length(); ++i)
	{
		Mtx44 characterSpacing;
		characterSpacing.SetToTranslation(i * 1.0f, 0, 0); //1.0f is the spacing of each character, you may change this value
		Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top() * characterSpacing;
		glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);

		mesh->Render((unsigned)text[i] * 6, 6);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);
	glEnable(GL_DEPTH_TEST);
}
void SceneBase::RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y)
{
	if (!mesh || mesh->textureID <= 0)
		return;

	glDisable(GL_DEPTH_TEST);
	Mtx44 ortho;
	ortho.SetToOrtho(0, 80, 0, 60, -10, 10);
	projectionStack.PushMatrix();
	projectionStack.LoadMatrix(ortho);
	viewStack.PushMatrix();
	viewStack.LoadIdentity();
	modelStack.PushMatrix();
	modelStack.LoadIdentity();
	modelStack.Translate(x, y, 0);
	modelStack.Scale(size, size, size);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 1);
	glUniform3fv(m_parameters[U_TEXT_COLOR], 1, &color.r);
	glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh->textureID);
	glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	for (unsigned i = 0; i < text.length(); ++i)
	{
		Mtx44 characterSpacing;
		characterSpacing.SetToTranslation(i * 1.0f + 0.5f, 0.5f, 0); //1.0f is the spacing of each character, you may change this value
		Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top() * characterSpacing;
		glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);

		mesh->Render((unsigned)text[i] * 6, 6);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);
	modelStack.PopMatrix();
	viewStack.PopMatrix();
	projectionStack.PopMatrix();
	glEnable(GL_DEPTH_TEST);
}
void SceneBase::RenderMeshIn2D(Mesh *mesh, bool enableLight, float size, float x, float y)
{
	Mtx44 ortho;
	ortho.SetToOrtho(-80, 80, -60, 60, -10, 10);
	projectionStack.PushMatrix();
	projectionStack.LoadMatrix(ortho);
	viewStack.PushMatrix();
	viewStack.LoadIdentity();
	modelStack.PushMatrix();
	modelStack.LoadIdentity();
	modelStack.Scale(size, size, size);
	modelStack.Translate(x, y, 0);

	Mtx44 MVP, modelView, modelView_inverse_transpose;

	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	if (mesh->textureID > 0)
	{
		glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mesh->textureID);
		glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	}
	else
	{
		glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 0);
	}
	mesh->Render();
	if (mesh->textureID > 0)
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	modelStack.PopMatrix();
	viewStack.PopMatrix();
	projectionStack.PopMatrix();
}
void SceneBase::RenderImageOnScreen(Mesh* mesh, Vector3 Scale, Vector3 Translate, Vector3 Rotate)
{
	Mtx44 ortho;
	ortho.SetToOrtho(0, 80, 0, 60, -100, 100); //size of screen UI
	projectionStack.PushMatrix();
	projectionStack.LoadMatrix(ortho);
	viewStack.PushMatrix();
	viewStack.LoadIdentity(); //No need camera for ortho mode
	modelStack.PushMatrix();
	modelStack.LoadIdentity(); //Reset modelStack
	modelStack.Translate(Translate.x, Translate.y, Translate.z);
	modelStack.Rotate(Rotate.x, 1, 0, 0);
	modelStack.Rotate(Rotate.y, 0, 1, 0);
	modelStack.Rotate(Rotate.z, 0, 0, 1);
	modelStack.Scale(Scale.x, Scale.y, Scale.z);

	Mtx44 MVP, modelView, modelView_inverse_transpose;

	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	modelView = viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MODELVIEW], 1, GL_FALSE, &modelView.a[0]);

	if (mesh->textureID > 0)
	{
		glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mesh->textureID);
		glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	}
	else
	{
		glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 0);
	}

	mesh->Render(); //this line should only be called once 

	if (mesh->textureID > 0)
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	modelStack.PopMatrix();
	projectionStack.PopMatrix();
	viewStack.PopMatrix();
}
void SceneBase::RenderMeshOutlined(Mesh* mesh, bool enableLight)
{
	RenderMesh(mesh, enableLight);

	//OUTLINING THE MESH
	glFrontFace(GL_CW);
	glPolygonOffset(-4.0f, -4.0f);
	glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 0);
	mesh->Render();
	glPolygonOffset(4.0f, 4.0f);
	glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 0);
	mesh->Render();

	glFrontFace(GL_CCW);
}
void SceneBase::RenderMesh(Mesh *mesh, bool enableLight)
{
	Mtx44 MVP, modelView, modelView_inverse_transpose;

	//For Gpass/shadow textures
	if (m_renderPass == RENDER_PASS_PRE)
	{
		for (int i = 0; i < Mesh::MAX_TEXTURES; ++i)
		{
			if (mesh->textureArray[i] > 0)
			{
				glUniform1i(m_parameters[U_SHADOW_COLOR_TEXTURE_ENABLED + i], 1);
				glActiveTexture(GL_TEXTURE0 + i);
				glBindTexture(GL_TEXTURE_2D, mesh->textureArray[i]);
				glUniform1i(m_parameters[U_SHADOW_COLOR_TEXTURE + i], i);
			}
			else
				glUniform1i(m_parameters[U_SHADOW_COLOR_TEXTURE_ENABLED + i], 0);
		}

		Mtx44 lightDepthMVP = m_lightDepthProj * m_lightDepthView * modelStack.Top();
		glUniformMatrix4fv(m_parameters[U_LIGHT_DEPTH_MVP_GPASS], 1, GL_FALSE, &lightDepthMVP.a[0]);

		mesh->Render();
		return;
	}

	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);

	modelView = viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MODELVIEW], 1, GL_FALSE, &modelView.a[0]);

	if (enableLight && bLightEnabled)
	{
		glUniform1i(m_parameters[U_LIGHTENABLED], 1);

		modelView_inverse_transpose = modelView.GetInverse().GetTranspose();
		glUniformMatrix4fv(m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE], 1, GL_FALSE, &modelView_inverse_transpose.a[0]);

		//Shadow
		Mtx44 lightDepthMVP = m_lightDepthProj * m_lightDepthView * modelStack.Top();
		glUniformMatrix4fv(m_parameters[U_LIGHT_DEPTH_MVP], 1, GL_FALSE, &lightDepthMVP.a[0]);

		//load material
		glUniform3fv(m_parameters[U_MATERIAL_AMBIENT], 1, &mesh->material.kAmbient.r);
		glUniform3fv(m_parameters[U_MATERIAL_DIFFUSE], 1, &mesh->material.kDiffuse.r);
		glUniform3fv(m_parameters[U_MATERIAL_SPECULAR], 1, &mesh->material.kSpecular.r);
		glUniform1f(m_parameters[U_MATERIAL_SHININESS], mesh->material.kShininess);
	}
	else
	{
		glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	}

	for (int i = 0; i < Mesh::MAX_TEXTURES; ++i)
	{
		if (mesh->textureArray[i] > 0)
		{
			glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED + i], 1);
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, mesh->textureArray[i]);
			glUniform1i(m_parameters[U_COLOR_TEXTURE + i], i);
		}
		else
		{
			glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED + i], 0);
		}

	}

	mesh->Render();
}

void SceneBase::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

float SceneBase::getBaryCentricInterpolation(Vector3 p1, Vector3 p2, Vector3 p3, Vector3 pos)
{
	float det = (p2.z - p3.z) * (p1.x - p3.x) + (p3.x - p2.x) * (p1.z - p3.z);
	float l1 = ((p2.z - p3.z) * (pos.x - p3.x) + (p3.x - p2.x) * (pos.z - p3.z)) / det;
	float l2 = ((p3.z - p1.z) * (pos.x - p3.x) + (p1.x - p3.x) * (pos.z - p3.z)) / det;
	float l3 = 1.0f - l1 - l2;
	return l1 * p1.y + l2 * p2.y + l3 * p3.y;
}

float SceneBase::getHeightofTerrain(float terrainscale, float ** heights)
{
	float terrainX = camera.position.x + (terrainscale / 2.f);
	float terrainZ = camera.position.z + (terrainscale / 2.f);
	float gridSquSiz = terrainscale / 255.f;
	int gridX = floor(terrainX / gridSquSiz);
	int gridZ = floor(terrainZ / gridSquSiz);
	if (gridX >= 255.f || gridZ >= 255.f || gridX < 0.f || gridZ < 0.f)
	{
		return 0.f;
	}
	float xCoord = (fmod(terrainX, gridSquSiz)) / gridSquSiz;
	float zCoord = (fmod(terrainZ, gridSquSiz)) / gridSquSiz;
	float answer;

	if (xCoord <= (1.f - zCoord))
	{
		answer = getBaryCentricInterpolation(Vector3(0, heights[gridX][gridZ], 0), Vector3(1, heights[gridX + 1][gridZ], 0), Vector3(0, heights[gridX][gridZ + 1], 1), Vector3(xCoord, 0, zCoord));
	}
	else
	{
		answer = getBaryCentricInterpolation(Vector3(1, heights[gridX + 1][gridZ], 0), Vector3(1, heights[gridX + 1][gridZ + 1], 1), Vector3(0, heights[gridX][gridZ + 1], 1), Vector3(xCoord, 0, zCoord));
	}
	answer *= terrainHeight;
	answer += characterHeight * 2;

	return answer;
}

void SceneBase::InitPartitioning()
{
	Partition A;
	A.MINPOS.Set();
}

void SceneBase::updatePartition(Vector3 pos)
{

}

void SceneBase::Exit()
{
	// Cleanup VBO
	for (int i = 0; i < NUM_GEOMETRY; ++i)
	{
		if (meshList[i])
			delete meshList[i];
	}

	while (particleList.size() > 0)
	{
		ParticleObject* particle = particleList.back();
		delete particle;
		particleList.pop_back();
	}

	glDeleteProgram(m_programID);
	glDeleteProgram(m_gPassShaderID);
	glDeleteVertexArrays(1, &m_vertexArrayID);
}