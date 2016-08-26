#include "SceneBase.h"
#include "GL\glew.h"

#include "shader.hpp"
#include "MeshBuilder.h"
#include "Application.h"
#include "Utility.h"
#include "LoadTGA.h"
#include <sstream>

SceneBase::SceneBase() : TERRAINSIZE(4000.0f, 200.0f, 4000.0f)
{
}

SceneBase::~SceneBase()
{
}

void SceneBase::Init()
{
	instance = Singleton::getInstance();
	Math::InitRNG();
	instance->Object_list.clear();
	instance->Enemy_list.clear();
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

	/*m_parameters[U_LIGHT2_TYPE] = glGetUniformLocation(m_programID, "lights[2].type");
	m_parameters[U_LIGHT2_POSITION] = glGetUniformLocation(m_programID, "lights[2].position_cameraspace");
	m_parameters[U_LIGHT2_COLOR] = glGetUniformLocation(m_programID, "lights[2].color");
	m_parameters[U_LIGHT2_POWER] = glGetUniformLocation(m_programID, "lights[2].power");
	m_parameters[U_LIGHT2_KC] = glGetUniformLocation(m_programID, "lights[2].kC");
	m_parameters[U_LIGHT2_KL] = glGetUniformLocation(m_programID, "lights[2].kL");
	m_parameters[U_LIGHT2_KQ] = glGetUniformLocation(m_programID, "lights[2].kQ");
	m_parameters[U_LIGHT2_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[2].spotDirection");
	m_parameters[U_LIGHT2_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[2].cosCutoff");
	m_parameters[U_LIGHT2_COSINNER] = glGetUniformLocation(m_programID, "lights[2].cosInner");
	m_parameters[U_LIGHT2_EXPONENT] = glGetUniformLocation(m_programID, "lights[2].exponent");

	m_parameters[U_LIGHT3_TYPE] = glGetUniformLocation(m_programID, "lights[3].type");
	m_parameters[U_LIGHT3_POSITION] = glGetUniformLocation(m_programID, "lights[3].position_cameraspace");
	m_parameters[U_LIGHT3_COLOR] = glGetUniformLocation(m_programID, "lights[3].color");
	m_parameters[U_LIGHT3_POWER] = glGetUniformLocation(m_programID, "lights[3].power");
	m_parameters[U_LIGHT3_KC] = glGetUniformLocation(m_programID, "lights[3].kC");
	m_parameters[U_LIGHT3_KL] = glGetUniformLocation(m_programID, "lights[3].kL");
	m_parameters[U_LIGHT3_KQ] = glGetUniformLocation(m_programID, "lights[3].kQ");
	m_parameters[U_LIGHT3_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[3].spotDirection");
	m_parameters[U_LIGHT3_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[3].cosCutoff");
	m_parameters[U_LIGHT3_COSINNER] = glGetUniformLocation(m_programID, "lights[3].cosInner");
	m_parameters[U_LIGHT3_EXPONENT] = glGetUniformLocation(m_programID, "lights[3].exponent");*/

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
	lights[1].position.Set(1, 1, 1);
	lights[1].color.Set(1, 1, 1);
	lights[1].power = 0.0f;
	lights[1].kC = 1.f;
	lights[1].kL = 0.01f;
	lights[1].kQ = 0.001f;
	lights[1].cosCutoff = cos(Math::DegreeToRadian(18));
	lights[1].cosInner = cos(Math::DegreeToRadian(30));
	lights[1].exponent = 1.f;
	lights[1].spotDirection.Set(0.f, 1.f, 0.f);

	//level 1 lights
	/*lights[2].type = Light::LIGHT_POINT;
	lights[2].position.Set(20, 40, 20);
	lights[2].color.Set(1, 1, 1);
	lights[2].power = 1.0f;
	lights[2].kC = 1.f;
	lights[2].kL = 0.01f;
	lights[2].kQ = 0.001f;

	lights[3].type = Light::LIGHT_POINT;
	lights[3].position.Set(1300, 40, 97);
	lights[3].color.Set(1, 1, 1);
	lights[3].power = 1.0f;
	lights[3].kC = 1.f;
	lights[3].kL = 0.01f;
	lights[3].kQ = 0.001f;*/

	glUniform1i(m_parameters[U_NUMLIGHTS], 3);
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

	/*glUniform1i(m_parameters[U_LIGHT2_TYPE], lights[2].type);
	glUniform3fv(m_parameters[U_LIGHT2_COLOR], 1, &lights[2].color.r);
	glUniform1f(m_parameters[U_LIGHT2_POWER], lights[2].power);
	glUniform1f(m_parameters[U_LIGHT2_KC], lights[2].kC);
	glUniform1f(m_parameters[U_LIGHT2_KL], lights[2].kL);
	glUniform1f(m_parameters[U_LIGHT2_KQ], lights[2].kQ);
	glUniform1f(m_parameters[U_LIGHT2_COSCUTOFF], lights[2].cosCutoff);
	glUniform1f(m_parameters[U_LIGHT2_COSINNER], lights[2].cosInner);
	glUniform1f(m_parameters[U_LIGHT2_EXPONENT], lights[2].exponent);

	glUniform1i(m_parameters[U_LIGHT3_TYPE], lights[3].type);
	glUniform3fv(m_parameters[U_LIGHT3_COLOR], 1, &lights[3].color.r);
	glUniform1f(m_parameters[U_LIGHT3_POWER], lights[3].power);
	glUniform1f(m_parameters[U_LIGHT3_KC], lights[3].kC);
	glUniform1f(m_parameters[U_LIGHT3_KL], lights[3].kL);
	glUniform1f(m_parameters[U_LIGHT3_KQ], lights[3].kQ);
	glUniform1f(m_parameters[U_LIGHT3_COSCUTOFF], lights[3].cosCutoff);
	glUniform1f(m_parameters[U_LIGHT3_COSINNER], lights[3].cosInner);
	glUniform1f(m_parameters[U_LIGHT3_EXPONENT], lights[3].exponent);*/

	//FOG
	Color fogColor(0.8f, 0.8f, 0.8f);
	FogAmount = 1500.0f;
	glUniform3fv(m_parameters[U_FOG_COLOR], 1, &fogColor.r);
	glUniform1f(m_parameters[U_FOG_START], 10);
	glUniform1f(m_parameters[U_FOG_END], FogAmount);
	glUniform1f(m_parameters[U_FOG_DENSITY], 0.005f);
	glUniform1f(m_parameters[U_FOG_TYPE], 0);
	glUniform1f(m_parameters[U_FOG_ENABLE], 0);

	camera.Init(Vector3(1, 200, 10), Vector3(0, 200, 1), Vector3(0, 1, 0));

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
	/*meshList[TERRAIN] = MeshBuilder::GenerateTerrain("Terrain", "Image//Terrain_Default.raw", m_heightMap, level1_Heights);
	meshList[TERRAIN]->textureArray[0] = LoadTGA("Image//Forest//Grass.tga");*/

	meshList[TERRAIN] = MeshBuilder::GenerateTerrain("Terrain", "Image//Terrain_Level03.raw", m_heightMap_3, level3_Heights);
	meshList[TERRAIN]->textureArray[0] = LoadTGA("Image//Forest//Grass.tga");
	meshList[TERRAIN]->textureArray[1] = LoadTGA("Image//Forest//Dead_Leaves.tga");

	//weapon
	meshList[RIFLE] = MeshBuilder::GenerateOBJ("Rifle", "OBJ//rifle.obj");
	meshList[RIFLE]->textureArray[0] = LoadTGA("Image//rifle.tga");

	meshList[PISTOL] = MeshBuilder::GenerateOBJ("Pistol", "OBJ//pistol.obj");
	meshList[PISTOL]->textureArray[0] = LoadTGA("Image//pistol.tga");

	meshList[VACUUM] = MeshBuilder::GenerateQuad("VACUUM", Color(0, 0, 0), 1.f);
	meshList[VACUUM]->textureID = LoadTGA("Image//vacuum.tga");

	meshList[GEO_CACTUS] = MeshBuilder::GenerateOBJ("Cactus", "OBJ//Cactus.obj");
	meshList[GEO_CACTUS]->textureArray[0] = LoadTGA("Image//Cactus.tga");

	meshList[INVENTORY_UI] = MeshBuilder::GenerateQuad("Water", Color(0, 0, 0), 1.f);
	meshList[INVENTORY_UI]->textureID = LoadTGA("Image//inventory.tga");

	meshList[GAME_TITLE] = MeshBuilder::GenerateQuad("Water", Color(0, 0, 0), 1.f);
	meshList[GAME_TITLE]->textureID = LoadTGA("Image//title.tga");

	meshList[BUTTON_PLAY] = MeshBuilder::GenerateQuad("Water", Color(0, 0, 0), 1.f);
	meshList[BUTTON_PLAY]->textureID = LoadTGA("Image//play.tga");

	meshList[BUTTON_CREDITS] = MeshBuilder::GenerateQuad("Water", Color(0, 0, 0), 1.f);
	meshList[BUTTON_CREDITS]->textureID = LoadTGA("Image//credits.tga");

	meshList[BUTTON_INSTRUCTION] = MeshBuilder::GenerateQuad("Water", Color(0, 0, 0), 1.f);
	meshList[BUTTON_INSTRUCTION]->textureID = LoadTGA("Image//instruction.tga");

	meshList[BUTTON_OPTIONS] = MeshBuilder::GenerateQuad("Water", Color(0, 0, 0), 1.f);
	meshList[BUTTON_OPTIONS]->textureID = LoadTGA("Image//options.tga");

	meshList[BUTTON_EXIT] = MeshBuilder::GenerateQuad("Water", Color(0, 0, 0), 1.f);
	meshList[BUTTON_EXIT]->textureID = LoadTGA("Image//exit.tga");

	meshList[NIGHT_VISION] = MeshBuilder::GenerateQuad("NightVision", Color(1, 1, 1), 1.f);
	meshList[NIGHT_VISION]->textureID = LoadTGA("Image//nightVision.tga");

	//Particles
	meshList[GEO_PARTICLE_WATER] = MeshBuilder::GenerateSphere("lightball", Color(0.5, 0.5, 1), 18, 36, 1.f);
	meshList[GEO_PARTICLE_SAND] = MeshBuilder::GenerateSphere("Sand particle", Color(0.8f, 0.7f, 0.5f), 18, 36, 1.f);

	//Player
	meshList[GEO_STAMINA] = MeshBuilder::GenerateQuad("Stamina", Color(0, 1, 0), 1.f);

	//Minimap
	m_Minimap = new Minimap();
	m_Minimap->SetBackground(MeshBuilder::GenerateMinimap("Minimap", Color(1, 1, 1), 1.f));
	m_Minimap->GetBackground()->textureArray[0] = LoadTGA("Image//HUD//Radar.tga");
	m_Minimap->SetBorder(MeshBuilder::GenerateMinimapBorder("Minimap border", Color(1, 1, 0), 1.f));
	meshList[GEO_VIEW] = MeshBuilder::GenerateCircle("View on minimap", Color(0, 0, 0), 1.f);
	meshList[GEO_VIEW]->textureArray[0] = LoadTGA("Image//HUD//Radar.tga");
	meshList[GEO_GREENBALL] = MeshBuilder::GenerateCircle("You on minimap", Color(0, 1, 0), 1.f);
	meshList[GEO_REDBALL] = MeshBuilder::GenerateCircle("Enemy on minimap", Color(1, 0, 0), 1.f);
	meshList[GEO_BLUEBALL] = MeshBuilder::GenerateCircle("Enemy(WEAKEN) on minimap", Color(0, 1, 1), 1.f);
	meshList[GEO_BOSS_ICON] = MeshBuilder::GenerateQuad("Boss on minimap", Color(0, 0, 0), 1.f);
	meshList[GEO_BOSS_ICON]->textureArray[0] = LoadTGA("Image//HUD//Boss_icon.tga");

	//Loading screens
	meshList[GEO_LOAD_1] = MeshBuilder::GenerateQuad("Level 1 loading screen", Color(0, 0, 0), 1.f);
	meshList[GEO_LOAD_1]->textureID = LoadTGA("Image//Screen//Load_Screen01.tga");
	meshList[GEO_LOAD_2] = MeshBuilder::GenerateQuad("Level 2 loading screen", Color(0, 0, 0), 1.f);
	meshList[GEO_LOAD_2]->textureID = LoadTGA("Image//Screen//Load_Screen02.tga");
	meshList[GEO_LOAD_3] = MeshBuilder::GenerateQuad("Level 3 loading screen", Color(0, 0, 0), 1.f);
	meshList[GEO_LOAD_3]->textureID = LoadTGA("Image//Screen//Load_Screen03.tga");
	meshList[GEO_LOAD_4] = MeshBuilder::GenerateQuad("Level 4 loading screen", Color(0, 0, 0), 1.f);
	meshList[GEO_LOAD_4]->textureID = LoadTGA("Image//Screen//Load_Screen04.tga");

	//meshList[POCONG] = MeshBuilder::GenerateOBJ("POCONG", "OBJ//pocong.obj");
	//meshList[POCONG]->textureArray[0] = LoadTGA("Image//Graveyard//pocong.tga");

	//Enemy
	meshList[GEO_GHOST1] = MeshBuilder::GenerateSpriteAnimation("TumbleWeed", 4, 3);
	meshList[GEO_GHOST1]->textureArray[0] = LoadTGA("Image//Ghosts//Ghost_1.tga");
	meshList[GEO_GHOST2] = MeshBuilder::GenerateSpriteAnimation("Horsey", 4, 3);
	meshList[GEO_GHOST2]->textureArray[0] = LoadTGA("Image//Ghosts//Ghost_2.tga");
	meshList[GEO_GHOST3] = MeshBuilder::GenerateSpriteAnimation("Horsey", 3, 6);
	meshList[GEO_GHOST3]->textureArray[0] = LoadTGA("Image//Ghosts//Ghost_3.tga");
	meshList[GEO_HITBOX] = MeshBuilder::GenerateCube("Hitbox", Color(1, 0, 0));

	meshList[BOSS] = MeshBuilder::GenerateOBJ("Boss", "OBJ//Boss.obj");
	meshList[BOSS]->textureArray[0] = LoadTGA("Image//Ghosts//Boss.tga");
	meshList[WISP] = MeshBuilder::GenerateSpriteAnimation("Wisp bullet", 4, 4);
	meshList[WISP]->textureArray[0] = LoadTGA("Image//Ghosts//Wisp.tga");

	//Shadow stuff
	meshList[GEO_LIGHT_DEPTH_QUAD] = MeshBuilder::GenerateQuad("Shadow Test", 1, 1);
	meshList[GEO_LIGHT_DEPTH_QUAD]->textureArray[0] = m_lightDepthFBO.GetTexture();

	weaponType = 1;

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

	Wisp = dynamic_cast<SpriteAnimation*>(meshList[WISP]);
	if (Wisp)
	{
		Wisp->m_anim = new Animation();
		Wisp->m_anim->Set(0, 15, 0, 0.8f, true);
	}

	characterHeight = 7.f;

	Singleton::getInstance()->player->Init();

	//Loading text file
	ReadFile("Text//Ghost_Amount.csv", ghost_Amount);

	int Counter = 0;
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
	//		cout << ghost_Amount[Counter] << " , ";
			Counter++;
		}
	//	cout << endl;
	}

	Singleton::getInstance()->player->Init();

	delay = 0;

	rotatePistol = 5;
	rotateRifle = 4;

	gunUp = false;
	gunDown = false;

}

void SceneBase::SpawnGhost()
{
	int Counter = 0;
	if (Singleton::getInstance()->program_state == Singleton::PROGRAM_GAME1)
	{
		Counter = 0;
	}
	if (Singleton::getInstance()->program_state == Singleton::PROGRAM_GAME2)
	{
		Counter = 3;
	}
	if (Singleton::getInstance()->program_state == Singleton::PROGRAM_GAME3)
	{
		Counter = 6;
	}

	int Type = 1;
	for (int j = 0; j < 3; j++)
	{
		for (int i = 0; i < ghost_Amount[Counter]; i++)
		{
			switch (Type)
			{
			case 1:
			{
				Enemy* Ghost = new Enemy(Enemy::ENEMY_TYPE::GHOST_1, Enemy::PATROL);
				instance->Enemy_list.push_back(Ghost);
				break;
			}
			case 2:
			{
				Enemy* Ghost = new Enemy(Enemy::ENEMY_TYPE::GHOST_2, Enemy::PATROL);
				instance->Enemy_list.push_back(Ghost);
				break;
			}
			case 3:
			{
				Enemy* Ghost = new Enemy(Enemy::ENEMY_TYPE::GHOST_3, Enemy::PATROL);
				instance->Enemy_list.push_back(Ghost);
				break;
			}
			}
		}
		Counter++;
		Type++;
	}
}

void SceneBase::Update(double dt)
{
	instance->singletonCamera = &camera;
	UpdatePlayer(dt);
	Singleton::getInstance()->player->setPosition(camera.position);
	Application::GetCursorPos(&Singleton::getInstance()->mousex, &Singleton::getInstance()->mousey);

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
	if (Wisp)
	{
		Wisp->Update(dt);
		Wisp->m_anim->animActive = true;
	}

	/*if (Application::IsKeyPressed('I'))
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
	}*/

	static bool inventoryButtonState = false;
	if (Application::IsKeyPressed('I'))
	{
		if (!inventoryButtonState)
		{
			inventoryButtonState = true;
			if (Singleton::getInstance()->showInventory == false)
			{
				Application::SetMousePosition(0, 0);
				Application::ShowCursor();
				Singleton::getInstance()->showInventory = true;
			}	
			else
			{
				Application::SetMousePosition(0, 0);
				Application::HideCursor();
				Singleton::getInstance()->showInventory = false;
			}
		}
	}
	else if (!Application::IsKeyPressed('I'))
	{
		if (inventoryButtonState)
			inventoryButtonState = false;
	}

	if (Application::IsKeyPressed('1'))
		//glEnable(GL_CULL_FACE);
		weaponType = 1;
	if (Application::IsKeyPressed('2'))
		//glDisable(GL_CULL_FACE);
		weaponType = 2;
	if (Application::IsKeyPressed('3'))
		//mode = false;
		weaponType = 3;
	if (Application::IsKeyPressed('4'))
		//mode = true;

		if (Application::IsKeyPressed('G'))
		{
			mode = false;
		}

	if (Application::IsKeyPressed('H'))
	{
		mode = true;
	}

	if (mode)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	if (Application::IsKeyPressed('8'))
	{
		bLightEnabled = true;
	}
	else if (Application::IsKeyPressed('9'))
	{
		bLightEnabled = false;
	}

	if (Application::IsKeyPressed(VK_NUMPAD1))
	{
		spatialPartitioning = true;
	}
	if (Application::IsKeyPressed(VK_NUMPAD2))
	{
		spatialPartitioning = false;
	}

	if (Application::IsKeyPressed(VK_F1))
	{
		ShowHitbox = true;
	}
	if (Application::IsKeyPressed(VK_F2))
	{
		ShowHitbox = false;
	}

	if (weaponType == 1)
	{
		if (Application::IsKeyPressed(VK_SPACE) && readyToShoot >= (float)(0.5f / fireRate))
		{
			sound.playSoundEffect2D("Sound/shoot.wav");

			gunDown = true;
			readyToShoot = 0.f;
			bulletList.push_back(new Bullet(
				Vector3(camera.position.x, camera.position.y, camera.position.z),
				Vector3(camera.view.x, camera.view.y, camera.view.z),
				300,
				1000,
				10
				));
		}
		else if (readyToShoot < (float)(1.f / fireRate)){
			readyToShoot += (float)(dt);
		}
		if (gunDown == true)
		{
			rotatePistol += (float)(500 * dt);
			if (rotatePistol >= 20)
			{
				gunDown = false;
				gunUp = true;
			}
		}

		if (gunUp == true)
		{
			rotatePistol -= (float)(100 * dt);
			if (rotatePistol <= 5)
			{
				gunUp = false;
				gunDown = false;
			}
		}
	}
	else if (weaponType == 2)
	{
		if (Application::IsKeyPressed(VK_SPACE) && readyToShoot >= (float)(0.1f / fireRate))
		{
			//sound.playSoundEffect2D("Sound/spray.wav");
			sound.playSoundEffect2D("Sound/shoot.mp3");
			gunDown = true;
			readyToShoot = 0.f;
			bulletList.push_back(new Bullet(
				Vector3(camera.position.x, camera.position.y, camera.position.z),
				Vector3(camera.view.x, camera.view.y, camera.view.z),
				300,
				1000,
				10
				));
		}
		else if (readyToShoot < (float)(1.f / fireRate)){
			readyToShoot += (float)(dt);
		}

		if (gunDown == true)
		{
			rotateRifle += (float)(100 * dt);
			if (rotateRifle >= 10)
			{
				gunDown = false;
				gunUp = true;
			}
		}

		if (gunUp == true)
		{
			rotateRifle -= (float)(50 * dt);
			if (rotateRifle <= 4)
			{
				gunUp = false;
				gunDown = false;
			}
		}
	}
	else if (weaponType == 3)
	{
		if (Application::IsKeyPressed(VK_SPACE) && readyToShoot >= (float)(0.5f / fireRate))
		{
			readyToShoot = 0.f;
			sound.playSoundEffect2D("Sound/vacuum.wav");
			captureList.push_back(new Capture(
				Vector3(camera.position.x, camera.position.y, camera.position.z),
				Vector3(camera.view.x, camera.view.y, camera.view.z),
				300,
				1000
				));
		}
		else if (readyToShoot < (float)(1.f / fireRate)){
			readyToShoot += (float)(dt);
		}
	}

	Vector3 View = (camera.target - camera.position).Normalized();
	radarAngle = Math::RadianToDegree(atan2(-View.z, View.x));

	UpdateEnemy(dt);
	UpdateHitboxes(dt);

	UpdateCapture(dt);

	UpdateShoot(dt);

	rotateKey += (float)(1 * dt);
}

void SceneBase::UpdateShoot(double dt)
{
	for (vector<Bullet*>::iterator it = bulletList.begin(); it != bulletList.end();){
		if ((*it)->deleteBullet == true){
			delete *it;
			it = bulletList.erase(it);
		}
		else{
			(*it)->Update(dt);
			it++;
		}
	}
}

void SceneBase::UpdateCapture(double dt)
{
	for (vector<Capture*>::iterator it = captureList.begin(); it != captureList.end();){
		if ((*it)->deleteProj == true){
			delete *it;
			it = captureList.erase(it);
		}
		else{
			(*it)->Update(dt);
			it++;
		}
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

void SceneBase::RenderMeshIn2D(Mesh *mesh, bool enableLight, Vector3 Scale, float x, float y, bool rotate_Mini, float rotate)
{
	modelStack.PushMatrix();
	modelStack.LoadIdentity();
	modelStack.Translate(x, y, 0);
	if (rotate_Mini)
	{
		modelStack.Rotate(rotateAngle, 0, 0, 1);
	}
	else
	{
		modelStack.Rotate(rotate, 0, 0, 1);
	}

	modelStack.Scale(Scale.x, Scale.y, Scale.z);

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

void SceneBase::RenderOBJOnScreen(Mesh* mesh, float scale, float x, float y, float z, float rotateX, float rotateY, float rotateZ, bool enableLight)
{

	Mtx44 ortho;
	ortho.SetToOrtho(0, 80, 0, 60, -100, 100);

	projectionStack.PushMatrix();
	projectionStack.LoadMatrix(ortho);

	viewStack.PushMatrix();
	viewStack.LoadIdentity();

	modelStack.PushMatrix();
	modelStack.LoadIdentity();
	modelStack.Translate(x, y, z);
	modelStack.Rotate(rotateX, 1, 0, 0);
	modelStack.Rotate(rotateY, 0, 1, 0);
	modelStack.Rotate(rotateZ, 0, 0, 1);
	modelStack.Scale(scale, scale, scale);

	Mtx44 MVP, modelView, modelView_inverse_transpose;

	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	modelView = viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MODELVIEW], 1, GL_FALSE, &modelView.a[0]);

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

	mesh->Render(); //this line should only be called once 

	if (mesh->textureID > 0)
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	modelStack.PopMatrix();
	projectionStack.PopMatrix();
	viewStack.PopMatrix();

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
	
	if (Singleton::getInstance()->stateCheck)
	{
		if (Singleton::getInstance()->program_state == Singleton::PROGRAM_MENU)
		{
			RenderImageOnScreen(meshList[GEO_LOAD_1], Vector3(80, 60, 1), Vector3(40, 30, 0), Vector3(0, 0, 0));
		}
		if (Singleton::getInstance()->program_state == Singleton::PROGRAM_GAME1)
		{
			RenderImageOnScreen(meshList[GEO_LOAD_1], Vector3(80, 60, 1), Vector3(40, 30, 0), Vector3(0, 0, 0));
		}
		if (Singleton::getInstance()->program_state == Singleton::PROGRAM_GAME2)
		{
			RenderImageOnScreen(meshList[GEO_LOAD_2], Vector3(80, 60, 1), Vector3(40, 30, 0), Vector3(0, 0, 0));
		}
		if (Singleton::getInstance()->program_state == Singleton::PROGRAM_GAME3)
		{
			RenderImageOnScreen(meshList[GEO_LOAD_3], Vector3(80, 60, 1), Vector3(40, 30, 0), Vector3(0, 0, 0));
		}
		if (Singleton::getInstance()->program_state == Singleton::PROGRAM_GAME4)
		{
			RenderImageOnScreen(meshList[GEO_LOAD_4], Vector3(80, 60, 1), Vector3(40, 30, 0), Vector3(0, 0, 0));
		}
	}

}

void SceneBase::UpdatePlayer(double dt)
{
	Singleton::getInstance()->player->Update(dt);
	if (Application::IsKeyPressed(VK_NUMPAD0))
	{
		Singleton::getInstance()->player->InflictFear(5);
	}

	if (Singleton::getInstance()->player->GetStamina() <= 0.0f)
	{
		camera.Tired = true;
	}
	else
	{
		camera.Tired = false;
	}

	//UpdateFearEffect(dt);
}
void SceneBase::UpdateFearEffect(double dt)
{
	switch (Singleton::getInstance()->player->GetFear())
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
void SceneBase::UpdateEnemy(double dt)
{
	for (std::vector<Enemy *>::iterator it = instance->Enemy_list.begin(); it != instance->Enemy_list.end(); ++it)
	{
		Enemy *ghost = (Enemy *)*it;
		if (ghost->active)
		{
			ghost->Update(dt);
		}
	}
}
void SceneBase::UpdateBoss(double dt)
{
	if (instance->boss->active)
	{
		instance->boss->Update(dt);
	}
}
void SceneBase::UpdateWeaponType(double dt)
{
	switch (Singleton::getInstance()->player->GetWeaponType())
	{
	case 1:
		weaponType = 1;
		break;
	case 2:
		weaponType = 2;
		break;
	case 3:
		weaponType = 3;
	default:
		break;
	}
}
void SceneBase::UpdateHitboxes(double dt)
{
	for (std::vector<AABBObject *>::iterator it = instance->Object_list.begin(); it != instance->Object_list.end(); ++it)
	{
		AABBObject *obj = (AABBObject *)*it;
		if (obj->active)
		{
			switch (obj->Object)
			{
			case AABBObject::OBJECT_TYPE::LOGS:
			{
				obj->Hitbox.UpdateAABB(obj->pos - Vector3(0, 40, 0));
				obj->Hitbox.Resize(Vector3(180, 100, 400));
				break;
			}
			case AABBObject::OBJECT_TYPE::BRIDGE:
			{
				obj->Hitbox.UpdateAABB(obj->pos - Vector3(-3, 50, 0));
				obj->Hitbox.Resize(Vector3(140, 75, 650));
				break;
			}
			case AABBObject::OBJECT_TYPE::FOUNTAIN:
			{
				obj->Hitbox.UpdateAABB(obj->pos - Vector3(0, 50, 0));
				obj->Hitbox.Resize(Vector3(100, 70, 100));
				break;
			}
			case AABBObject::OBJECT_TYPE::HEDGE:
			{
				obj->Hitbox.UpdateAABB(obj->pos - Vector3(0, 100, 0));
				obj->Hitbox.Resize(Vector3(40, 40, 80));
				break;
			}
			case AABBObject::OBJECT_TYPE::BENCH:
			{
				obj->Hitbox.UpdateAABB(obj->pos - Vector3(0, 75, 0));
				obj->Hitbox.Resize(Vector3(40, 40, 50));
				break;
			}
			case AABBObject::OBJECT_TYPE::POT:
			{
				obj->Hitbox.UpdateAABB(obj->pos - Vector3(0, 50, 0));
				obj->Hitbox.Resize(Vector3(20, 100, 20));
				break;
			}
			case AABBObject::OBJECT_TYPE::HOUSE1:
			case AABBObject::OBJECT_TYPE::HOUSE2:
			{
				obj->Hitbox.UpdateAABB(obj->pos - Vector3(0, 0, 0));
				obj->Hitbox.Resize(Vector3(410, 410, 410));
				break;
			}

			case AABBObject::OBJECT_TYPE::TOMBSTONE:
			{
				obj->Hitbox.UpdateAABB(obj->pos - Vector3(0, 40, 0));
				obj->Hitbox.Resize(Vector3(45, 55, 25));
				break;
			}
			case AABBObject::OBJECT_TYPE::DEADTREE:
			{
				obj->Hitbox.UpdateAABB(obj->pos - Vector3(0, 0, 0));
				obj->Hitbox.Resize(Vector3(160, 240, 160));
				break;
			}

			case AABBObject::OBJECT_TYPE::BARRICADE:
			{
				break;
			}

			case AABBObject::OBJECT_TYPE::BED:
			{
				obj->Hitbox.UpdateAABB(obj->pos - Vector3(0, 100, 0));
				obj->Hitbox.Resize(Vector3(100, 30, 60));
				break;
			}

			case AABBObject::OBJECT_TYPE::DOOR:
			{
				obj->Hitbox.UpdateAABB(obj->pos - Vector3(0, 55, 0));
				obj->Hitbox.Resize(Vector3(50, 90, 10));
				break;
			}

			case AABBObject::OBJECT_TYPE::CHAIR:
			{
				obj->Hitbox.UpdateAABB(obj->pos - Vector3(0, 80, 0));
				obj->Hitbox.Resize(Vector3(25, 45, 25));
				break;
			}

			case AABBObject::OBJECT_TYPE::TABLE:
			{
				obj->Hitbox.UpdateAABB(obj->pos - Vector3(0, 80, 0));
				obj->Hitbox.Resize(Vector3(75, 40, 55));
				break;
			}

			case AABBObject::OBJECT_TYPE::KEY:
			{
				obj->Hitbox.UpdateAABB(obj->pos - Vector3(0, 80, 0));
				obj->Hitbox.Resize(Vector3(15, 50, 15));
				break;
			}

			case AABBObject::OBJECT_TYPE::FENCE:
			{
				obj->Hitbox.UpdateAABB(obj->pos - Vector3(0, 80, 0));
				obj->Hitbox.Resize(Vector3(80, 150, 900));
				break;
			}
			case AABBObject::OBJECT_TYPE::BOUNDARY:
			{
				obj->Hitbox.UpdateAABB(obj->pos - Vector3(0, 80, 0));
				obj->Hitbox.Resize(Vector3(obj->scale.x * 10, obj->scale.y * 10, obj->scale.z * 10));
				break;
			}
			default:
			{
				break;
			}
			}
		}
	}
	for (std::vector<Enemy *>::iterator it = instance->Enemy_list.begin(); it != instance->Enemy_list.end(); ++it)
	{
		Enemy *ghost = (Enemy *)*it;
		if (ghost->active)
		{
			ghost->pos.y = TERRAINSIZE.y *  ReadHeightMap(m_heightMap_3, ghost->pos.x / TERRAINSIZE.x, ghost->pos.z / TERRAINSIZE.z);
			ghost->Hitbox.Resize(ghost->scale - Vector3(5, 5, 5));
		}
	}

	if (instance->boss->active)
	{
		instance->boss->Hitbox.Resize(instance->boss->scale + Vector3(200, 300, 200));
	}
	for (std::vector<AABBObject *>::iterator it = instance->boss->Boss_Bullet.begin(); it != instance->boss->Boss_Bullet.end(); ++it)
	{
		AABBObject *bullet = (AABBObject *)*it;
		if (bullet->active)
		{
			if (bullet->Object == AABBObject::OBJECT_TYPE::WISP)
			{
				bullet->Hitbox.UpdateAABB(bullet->pos - Vector3(0,15,0));
				bullet->Hitbox.Resize(Vector3(20, 40, 20));
			}
		}
	}
}

void SceneBase::RenderObjects(bool ShowHitbox)
{
	playerPartition = getPartition(camera.position);

	for (std::vector<AABBObject *>::iterator it = instance->Object_list.begin(); it != instance->Object_list.end(); ++it)
	{
		AABBObject *obj = (AABBObject *)*it;
		if (obj->active)
		{
			posPartition = getPartition(obj->pos);
			if (renderCheck(playerPartition, posPartition))
			{
				if (ShowHitbox)
				{
					modelStack.PushMatrix();
					glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
					modelStack.Translate(obj->Hitbox.pos.x, obj->Hitbox.pos.y + 100, obj->Hitbox.pos.z);
					modelStack.Scale(obj->Hitbox.size.x, obj->Hitbox.size.y, obj->Hitbox.size.z);
					RenderMesh(meshList[GEO_HITBOX], false);
					if (!mode)
					{
						glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
					}
					modelStack.PopMatrix();
				}
				switch (obj->Object)
				{

					case AABBObject::OBJECT_TYPE::LOGS:
					{
						modelStack.PushMatrix();
						modelStack.Translate(obj->pos.x, obj->pos.y, obj->pos.z);
						modelStack.Rotate(90, 0, 1, 0);
						modelStack.Scale(obj->scale.x, obj->scale.y, obj->scale.z);
						RenderMeshOutlined(meshList[GEO_LOGS], true);
						modelStack.PopMatrix();
						break;
					}
					case AABBObject::OBJECT_TYPE::BRIDGE:
					{
						modelStack.PushMatrix();
						modelStack.Translate(obj->pos.x, obj->pos.y, obj->pos.z);
						modelStack.Scale(obj->scale.x, obj->scale.y, obj->scale.z);
						RenderMeshOutlined(meshList[GEO_BRIDGE], true);
						modelStack.PopMatrix();
						break;
					}
					case AABBObject::OBJECT_TYPE::FOUNTAIN:
					{
						modelStack.PushMatrix();
						modelStack.Translate(obj->pos.x, obj->pos.y, obj->pos.z);
						modelStack.Scale(obj->scale.x, obj->scale.y, obj->scale.z);
						RenderMeshOutlined(meshList[FOUNTAIN], false);
						modelStack.PopMatrix();
						break;
					}
					case AABBObject::OBJECT_TYPE::HEDGE:
					{
						modelStack.PushMatrix();
						modelStack.Translate(obj->pos.x, obj->pos.y, obj->pos.z);
						modelStack.Scale(obj->scale.x, obj->scale.y, obj->scale.z);
						RenderMeshOutlined(meshList[HEDGE], false);
						modelStack.PopMatrix();
						break;
					}
					case AABBObject::OBJECT_TYPE::BENCH:
					{
						modelStack.PushMatrix();
						modelStack.Translate(obj->pos.x, obj->pos.y, obj->pos.z);
						modelStack.Rotate(obj->angle, obj->rotate.x, obj->rotate.y, obj->rotate.z);
						modelStack.Scale(obj->scale.x, obj->scale.y, obj->scale.z);
						RenderMeshOutlined(meshList[BENCHES], false);
						modelStack.PopMatrix();
						break;
					}
					case AABBObject::OBJECT_TYPE::POT:
					{
						modelStack.PushMatrix();
						modelStack.Translate(obj->pos.x, obj->pos.y, obj->pos.z);
						modelStack.Scale(obj->scale.x, obj->scale.y, obj->scale.z);
						RenderMeshOutlined(meshList[POT], false);
						modelStack.PopMatrix();
						break;
					}
					case AABBObject::OBJECT_TYPE::HOUSE1:
					{
						modelStack.PushMatrix();
						modelStack.Translate(obj->pos.x, obj->pos.y, obj->pos.z);
						modelStack.Rotate(obj->angle, obj->rotate.x, obj->rotate.y, obj->rotate.z);
						modelStack.Scale(obj->scale.x, obj->scale.y, obj->scale.z);
						RenderMeshOutlined(meshList[HOUSE1], false);
						modelStack.PopMatrix();
						break;
					}
					case AABBObject::OBJECT_TYPE::HOUSE2:
					{
						modelStack.PushMatrix();
						modelStack.Translate(obj->pos.x, obj->pos.y, obj->pos.z);
						modelStack.Rotate(obj->angle, obj->rotate.x, obj->rotate.y, obj->rotate.z);
						modelStack.Scale(obj->scale.x, obj->scale.y, obj->scale.z);
						RenderMeshOutlined(meshList[HOUSE2], false);
						modelStack.PopMatrix();
						break;
					}
					case AABBObject::OBJECT_TYPE::FENCE:
					{
						modelStack.PushMatrix();
						modelStack.Translate(obj->pos.x, obj->pos.y, obj->pos.z);
						modelStack.Rotate(obj->angle, obj->rotate.x, obj->rotate.y, obj->rotate.z);
						modelStack.Scale(obj->scale.x, obj->scale.y, obj->scale.z);
						RenderMeshOutlined(meshList[FENCE], true);
						modelStack.PopMatrix();
						break;
					}
					case AABBObject::OBJECT_TYPE::KEY:
					{
						modelStack.PushMatrix();
						modelStack.Translate(obj->pos.x, obj->pos.y, obj->pos.z);
						modelStack.Rotate(rotateKey * 20, 0, 1, 0);
						modelStack.Scale(obj->scale.x, obj->scale.y, obj->scale.z);
						RenderMeshOutlined(meshList[GEO_KEY], true);
						modelStack.PopMatrix();
						break;
					}
					case AABBObject::OBJECT_TYPE::TOMBSTONE:
					{
						modelStack.PushMatrix();
						modelStack.Translate(obj->pos.x, obj->pos.y, obj->pos.z);
						modelStack.Rotate(obj->angle, obj->rotate.x, obj->rotate.y, obj->rotate.z);
						modelStack.Scale(obj->scale.x, obj->scale.y, obj->scale.z);
						RenderMeshOutlined(meshList[TOMBSTONE], true);
						modelStack.PopMatrix();
						break;
					}
					case AABBObject::OBJECT_TYPE::DEADTREE:
					{
						modelStack.PushMatrix();
						modelStack.Translate(obj->pos.x, obj->pos.y, obj->pos.z);
						modelStack.Rotate(obj->angle, obj->rotate.x, obj->rotate.y, obj->rotate.z);
						modelStack.Scale(obj->scale.x, obj->scale.y, obj->scale.z);
						RenderMeshOutlined(meshList[DEADTREE], true);
						modelStack.PopMatrix();
						break;
					}
				default:
				{
					break;
				}

				}
			}
		}
	}
}

void SceneBase::RenderEnemies(bool ShowHitbox)
{
	playerPartition = getPartition(camera.position);
	for (std::vector<Enemy *>::iterator it = instance->Enemy_list.begin(); it != instance->Enemy_list.end(); ++it)
	{
		Enemy *ghost = (Enemy *)*it;
		if (ghost->active)
		{
			posPartition = getPartition(ghost->pos);
			if (renderCheck(playerPartition, posPartition))
			{
				float Degree = Math::RadianToDegree(atan2(-(ghost->pos.z - Singleton::getInstance()->player->getPosition().z), ghost->pos.x - Singleton::getInstance()->player->getPosition().x));

				if (ShowHitbox)
				{
					modelStack.PushMatrix();
					glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
					modelStack.Translate(ghost->Hitbox.pos.x, ghost->Hitbox.pos.y, ghost->Hitbox.pos.z);
					modelStack.Scale(ghost->Hitbox.size.x, ghost->Hitbox.size.y, ghost->Hitbox.size.z);
					RenderMesh(meshList[GEO_HITBOX], false);
					if (!mode)
					{
						glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
					}
					modelStack.PopMatrix();
				}

				switch (ghost->Type)
				{
				case Enemy::ENEMY_TYPE::GHOST_1:
				{
					modelStack.PushMatrix();
					modelStack.Translate(ghost->pos.x, ghost->pos.y, ghost->pos.z);
					if (ghost->Enemy::ENEMY_STATE::CAPTURED)
					{
						modelStack.Rotate(ghost->rotate, 1, 0, 0);
						modelStack.Rotate(ghost->rotate, 0, 0, 1);
					}
					modelStack.Rotate(Degree - 90, 0, 1, 0);
					modelStack.Scale(ghost->scale.x, ghost->scale.y, ghost->scale.z);
					RenderMesh(meshList[GEO_GHOST1], false);
					modelStack.PopMatrix();
					break;
				}
				case Enemy::ENEMY_TYPE::GHOST_2:
				{
					modelStack.PushMatrix();
					modelStack.Translate(ghost->pos.x, ghost->pos.y, ghost->pos.z);
					if (ghost->Enemy::ENEMY_STATE::CAPTURED)
					{
						modelStack.Rotate(ghost->rotate, 1, 0, 0);
						modelStack.Rotate(ghost->rotate, 0, 0, 1);
					}
					modelStack.Rotate(Degree - 90, 0, 1, 0);
					modelStack.Scale(ghost->scale.x, ghost->scale.y, ghost->scale.z);
					RenderMesh(meshList[GEO_GHOST2], false);
					modelStack.PopMatrix();
					break;
				}
				case Enemy::ENEMY_TYPE::GHOST_3:
				{
					modelStack.PushMatrix();
					modelStack.Translate(ghost->pos.x, ghost->pos.y, ghost->pos.z);
					if (ghost->Enemy::ENEMY_STATE::CAPTURED)
					{
						modelStack.Rotate(ghost->rotate, 1, 0, 0);
						modelStack.Rotate(ghost->rotate, 0, 0, 1);
					}
					modelStack.Rotate(Degree - 90, 0, 1, 0);
					modelStack.Scale(ghost->scale.x, ghost->scale.y, ghost->scale.z);
					RenderMesh(meshList[GEO_GHOST3], false);
					modelStack.PopMatrix();
					break;
				}
				default:
				{
					break;
				}
				}
			}
		}
	}
}
void SceneBase::RenderBoss(bool ShowHitbox)
{
	if (instance->boss->active)
	{
		float Yaw = Math::RadianToDegree(atan2(-(instance->boss->pos.z - instance->player->getPosition().z), instance->boss->pos.x - instance->player->getPosition().x));
		Vector3 view = (instance->player->getPosition() - instance->boss->pos).Normalized();
		float Pitch = Math::RadianToDegree(-atan2(view.y, sqrt(Math::Square(view.x) + Math::Square(view.z))));

		if (ShowHitbox)
		{
			modelStack.PushMatrix();
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			modelStack.Translate(instance->boss->Hitbox.pos.x, instance->boss->Hitbox.pos.y, instance->boss->Hitbox.pos.z);
			modelStack.Scale(instance->boss->Hitbox.size.x, instance->boss->Hitbox.size.y, instance->boss->Hitbox.size.z);
			RenderMesh(meshList[GEO_HITBOX], false);
			if (!mode)
			{
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			}
			modelStack.PopMatrix();
		}

		modelStack.PushMatrix();
		modelStack.Translate(instance->boss->pos.x, instance->boss->pos.y, instance->boss->pos.z);
		modelStack.Rotate(Yaw - 90, 0, 1, 0);
		modelStack.Rotate(Pitch, 1, 0, 0);
		modelStack.Scale(instance->boss->scale.x, instance->boss->scale.y, instance->boss->scale.z);
		RenderMeshOutlined(meshList[BOSS], true);
		modelStack.PopMatrix();
	}

	for (std::vector<AABBObject *>::iterator it = instance->boss->Boss_Bullet.begin(); it != instance->boss->Boss_Bullet.end(); ++it)
	{
		AABBObject *bullet = (AABBObject *)*it;
		if (bullet->active && 
			bullet->Object == AABBObject::OBJECT_TYPE::WISP)
		{
			if (ShowHitbox)
			{
				modelStack.PushMatrix();
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				modelStack.Translate(bullet->Hitbox.pos.x, bullet->Hitbox.pos.y, bullet->Hitbox.pos.z);
				modelStack.Scale(bullet->Hitbox.size.x, bullet->Hitbox.size.y, bullet->Hitbox.size.z);
				RenderMesh(meshList[GEO_HITBOX], false);
				if (!mode)
				{
					glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				}
				modelStack.PopMatrix();
			}

			float Yaw = Math::RadianToDegree(atan2(-(bullet->pos.z - instance->player->getPosition().z), bullet->pos.x - instance->player->getPosition().x));
			Vector3 view = (instance->player->getPosition() - bullet->pos).Normalized();
			float Pitch = Math::RadianToDegree(-atan2(view.y, sqrt(Math::Square(view.x) + Math::Square(view.z))));

			modelStack.PushMatrix();
			modelStack.Translate(bullet->pos.x, bullet->pos.y, bullet->pos.z);
			modelStack.Rotate(Yaw - 90, 0, 1, 0);
			modelStack.Rotate(Pitch, 1, 0, 0);
			modelStack.Scale(bullet->scale.x, bullet->scale.y, bullet->scale.z);
			RenderMeshOutlined(meshList[WISP], false);
			modelStack.PopMatrix();
		}
	}

}
void SceneBase::RenderRadar()
{
	Mtx44 ortho;
	ortho.SetToOrtho(-80, 80, -60, 60, -10, 10);
	projectionStack.PushMatrix();
	projectionStack.LoadMatrix(ortho);

	viewStack.PushMatrix();
	viewStack.LoadIdentity();

	//STENCILNG STUFF
	glEnable(GL_STENCIL_TEST);

	glStencilFunc(GL_ALWAYS, 1, 0xFF); // Set any stencil to 1
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	glStencilMask(0xFF); // Write to stencil buffer
	glDepthMask(GL_FALSE); // Don't write to depth buffer
	glClear(GL_STENCIL_BUFFER_BIT); // Clear stencil buffer (0 by default)

	//What is visible
	modelStack.PushMatrix();
	modelStack.Translate(65, 45, 0);
	modelStack.Scale(30, 30, 30);
	RenderMesh(meshList[GEO_VIEW], false);
	modelStack.PopMatrix();

	glStencilFunc(GL_EQUAL, 1, 0xFF); // Pass test if stencil value is 1
	glStencilMask(0x00); // Don't write anything to stencil buffer
	glDepthMask(GL_TRUE); // Write to depth buffer

	//What is going to be seen
	RenderMeshIn2D(meshList[GEO_GREENBALL], false, Vector3(1, 1, 1), 65, 45, true);

	for (std::vector<Enemy *>::iterator it = instance->Enemy_list.begin(); it != instance->Enemy_list.end(); ++it)
	{
		Enemy *ghost = (Enemy *)*it;
		if (ghost->active)
		{
			modelStack.PushMatrix();
			modelStack.Translate(65, 45, 0);
			modelStack.Rotate(radarAngle - 90, 0, 0, 1);
			modelStack.Translate(-ghost->pos.x / 70 + camera.position.x / 70, -ghost->pos.z / 70 + camera.position.z / 70, 0);
			if (ghost->State == Enemy::ENEMY_STATE::WEAKEN)
			{
				RenderMesh(meshList[GEO_BLUEBALL], false);
			}
			else
			{
				RenderMesh(meshList[GEO_REDBALL], false);
			}
			modelStack.PopMatrix();
		}
	}

	if (Singleton::getInstance()->program_state == Singleton::PROGRAM_GAME4)
	{
		Vector3 view = (instance->player->getPosition() - instance->boss->pos).Normalized();
		float Degree = Math::RadianToDegree(-atan2(view.x, view.z));
		modelStack.PushMatrix();
		modelStack.Translate(65, 45, 0);
		modelStack.Rotate(radarAngle - 90, 0, 0, 1);
		modelStack.Translate(-instance->boss->pos.x / 70 + camera.position.x / 70, -instance->boss->pos.z / 70 + camera.position.z / 70, 0);
		modelStack.Rotate(Degree, 0, 0, 1);
		modelStack.Scale(3, 3, 3);
		RenderMesh(meshList[GEO_BOSS_ICON], false);
		modelStack.PopMatrix();
	}

	modelStack.PushMatrix();
	modelStack.Translate(65, 45, 0);
	modelStack.Rotate(radarAngle - 90, 0, 0, 1);
	modelStack.Translate(camera.position.x / 70, camera.position.z / 70, 0);
	modelStack.Scale(100, 100, 100);
	RenderMesh(m_Minimap->GetBackground(), false);
	modelStack.PopMatrix();

	glDisable(GL_STENCIL_TEST);

	RenderMeshIn2D(m_Minimap->GetBorder(), false, Vector3(30, 30, 30), 65, 45, false);

	projectionStack.PopMatrix();

	viewStack.PopMatrix();
}

void SceneBase::RenderBullets(bool light)
{
	//bullet
	for (vector<Bullet*>::iterator it = bulletList.begin(); it != bulletList.end(); ++it)
	{

		modelStack.PushMatrix();
		modelStack.Translate(
			(*it)->position.x,
			(*it)->position.y,
			(*it)->position.z
			);
		modelStack.Scale(1, 1, 1);
		RenderMesh(meshList[GEO_LIGHTBALL], true);
		modelStack.PopMatrix();
	}
	//testing capture projectile
	/*for (vector<Capture*>::iterator it = captureList.begin(); it != captureList.end(); ++it)
	{
		modelStack.PushMatrix();
		modelStack.Translate(
			(*it)->position.x,
			(*it)->position.y,
			(*it)->position.z
			);
		modelStack.Scale(1, 1, 1);
		RenderMesh(meshList[GEO_LIGHTBALL], true);
		modelStack.PopMatrix();
	}*/
}

void SceneBase::RenderWeapons(bool light)
{
	switch (weaponType)
	{
	case 1:
		RenderOBJOnScreen(meshList[PISTOL], 1.2, 70, 5, -80, 0, 110, rotatePistol, light);
		break;
	case 2:
		RenderOBJOnScreen(meshList[RIFLE], 3, 68, -33, 10, rotateRifle, -170, 0, light);
		break;
	case 3:
		RenderImageOnScreen(meshList[VACUUM], Vector3(50, 50, 1), Vector3(70, 5, 0), Vector3(0, 0, 0));
		break;
	}
}
void SceneBase::RenderInventory()
{
	if (Singleton::getInstance()->showInventory == true)
	{
		RenderImageOnScreen(meshList[INVENTORY_UI], Vector3(50, 40, 1), Vector3(40, 30, 0), Vector3(0, 0, 0));
		if (Singleton::getInstance()->gotKey == true)
			RenderOBJOnScreen(meshList[GEO_KEY], 1, 20, 38, 10, 0, rotateKey * 20, 0, false);
	}
	else
	{

	}

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
//		*----*----*			*----*----**----*----*
//		|  A |  B |			|  A |  B ||  C |  D |
//		*----*----*			*----*----**----*----*
//		|  C |  D |			|  E |  F ||  G |  H |
//		*----*----*			*----*----**----*----*
//							|  I |  J ||  K |  L |
//			2*2				*----*----**----*----*
//							|  M |  N ||  O |  P |
//							*----*----**----*----*
//									 4*4

void SceneBase::InitPartitioning()
{
	//2*2
	//Partition A;												
	//A.MINPOS.Set(-Terrainsize.x, 0, -Terrainsize.z);
	//A.MAXPOS.Set(0, 0, 0);
	//partitioning['A'] = A;

	//Partition B;
	//B.MINPOS.Set(0, 0, -Terrainsize.z);
	//B.MAXPOS.Set(Terrainsize.x, 0, 0);
	//partitioning['B'] = B;

	//Partition C;
	//C.MINPOS.Set(-Terrainsize.x, 0, 0);
	//C.MAXPOS.Set(0, 0, Terrainsize.z);
	//partitioning['C'] = C;

	//Partition D;
	//D.MINPOS.Set(0, 0, 0);
	//D.MAXPOS.Set(Terrainsize.x, 0, Terrainsize.z);
	//partitioning['D'] = D;

	//4*4
	Partition A;
	A.MINPOS.Set(-Terrainsize.x, 0, -Terrainsize.z);
	A.MAXPOS.Set(-Terrainsize.x * 0.5f, 0, -Terrainsize.z * 0.5f);
	partitioning['A'] = A;

	Partition B;
	B.MINPOS.Set(-Terrainsize.x * 0.5f, 0, -Terrainsize.z);
	B.MAXPOS.Set(0, 0, -Terrainsize.z * 0.5f);
	partitioning['B'] = B;

	Partition C;
	C.MINPOS.Set(0, 0, -Terrainsize.z);
	C.MAXPOS.Set(Terrainsize.x * 0.5f, 0, -Terrainsize.z * 0.5f);
	partitioning['C'] = C;

	Partition D;
	D.MINPOS.Set(Terrainsize.x * 0.5f, 0, -Terrainsize.z);
	D.MAXPOS.Set(Terrainsize.x, 0, -Terrainsize.z * 0.5f);
	partitioning['D'] = D;
	/////////////////////////////////////////////////////////////////////////
	Partition E;
	E.MINPOS.Set(-Terrainsize.x, 0, -Terrainsize.z * 0.5f);
	E.MAXPOS.Set(-Terrainsize.x * 0.5f, 0, 0);
	partitioning['E'] = E;

	Partition F;
	F.MINPOS.Set(-Terrainsize.x * 0.5f, 0, -Terrainsize.z * 0.5f);
	F.MAXPOS.SetZero();
	partitioning['F'] = F;

	Partition G;
	G.MINPOS.Set(0, 0, -Terrainsize.z * 0.5f);
	G.MAXPOS.Set(Terrainsize.x * 0.5f, 0, 0);
	partitioning['G'] = G;

	Partition H;
	H.MINPOS.Set(Terrainsize.x * 0.5f, 0, -Terrainsize.z * 0.5f);
	H.MAXPOS.Set(Terrainsize.x, 0, 0);
	partitioning['H'] = H;
	/////////////////////////////////////////////////////////////////////////
	Partition I;
	I.MINPOS.Set(-Terrainsize.x, 0, 0);
	I.MAXPOS.Set(-Terrainsize.x * 0.5f, 0, Terrainsize.z * 0.5f);
	partitioning['I'] = I;

	Partition J;
	J.MINPOS.Set(-Terrainsize.x * 0.5f, 0, 0);
	J.MAXPOS.Set(0, 0, Terrainsize.z * 0.5f);
	partitioning['J'] = J;

	Partition K;
	K.MINPOS.SetZero();
	K.MAXPOS.Set(Terrainsize.x * 0.5f, 0, Terrainsize.z * 0.5f);
	partitioning['K'] = K;

	Partition L;
	L.MINPOS.Set(Terrainsize.x * 0.5f, 0, 0);
	L.MAXPOS.Set(Terrainsize.x, 0, Terrainsize.z * 0.5f);
	partitioning['L'] = L;
	///////////////////////////////////////////////////////////////////////
	Partition M;
	M.MINPOS.Set(-Terrainsize.x, 0, Terrainsize.z * 0.5f);
	M.MAXPOS.Set(-Terrainsize.x * 0.5f, 0, Terrainsize.z);
	partitioning['M'] = M;

	Partition N;
	N.MINPOS.Set(-Terrainsize.x * 0.5f, 0, Terrainsize.z * 0.5f);
	N.MAXPOS.Set(0, 0, Terrainsize.z);
	partitioning['N'] = N;

	Partition O;
	O.MINPOS.Set(0, 0, Terrainsize.z * 0.5f);
	O.MAXPOS.Set(Terrainsize.x * 0.5f, 0, Terrainsize.z);
	partitioning['O'] = O;

	Partition P;
	P.MINPOS.Set(Terrainsize.x * 0.5f, 0, Terrainsize.z * 0.5f);
	P.MAXPOS.Set(Terrainsize.x, 0, Terrainsize.z);
	partitioning['P'] = P;
	///////////////////////////////////////////////////////////////////////

}

char SceneBase::getPartition(Vector3 pos)
{
	for (auto partition : partitioning)
	{
		if ((pos.x >= partition.second.MINPOS.x && pos.z >= partition.second.MINPOS.z) && (pos.x <= partition.second.MAXPOS.x && pos.z <= partition.second.MAXPOS.z))
		{
			return partition.first;
		}
	}
}

bool SceneBase::renderCheck(char partition1, char partition2)
{
	if (spatialPartitioning == false)
		return true;
	if
		(
		(partition1 == partition2) || (partition1 + 1 == partition2) || (partition1 + 3 == partition2) || (partition1 + 4 == partition2) || (partition1 + 5 == partition2)
		|| (partition1 - 1 == partition2) || (partition1 - 3 == partition2) || (partition1 - 4 == partition2) || (partition1 - 5 == partition2)
		)
		return true;
	else return false;
}
bool  SceneBase::cameraViewObject(Vector3 pos, float degree)
{
	if (pos != camera.position)
	{
		Vector3 view = (pos - camera.position).Normalized();

		

		float angleX = Math::RadianToDegree(acos(view.Dot(camera.view)));
		//std::cout << angleX << std::endl;


		if (angleX <= degree)
		{
			return true;
		}
		if (angleX > degree)
		{
			return false;
		}

	}
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
	instance->Object_list.clear();
	instance->Enemy_list.clear();
	glDeleteProgram(m_programID);
	glDeleteProgram(m_gPassShaderID);
	glDeleteVertexArrays(1, &m_vertexArrayID);
}