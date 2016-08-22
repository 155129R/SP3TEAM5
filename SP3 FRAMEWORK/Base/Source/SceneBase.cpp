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

	meshList[TERRAIN_LEVEL03] = MeshBuilder::GenerateTerrain("Terrain", "Image//Terrain_Level03.raw", m_heightMap_3, level3_Heights);
	meshList[TERRAIN_LEVEL03]->textureArray[0] = LoadTGA("Image//Forest//Grass.tga");
	meshList[TERRAIN_LEVEL03]->textureArray[1] = LoadTGA("Image//Forest//Dead_Leaves.tga");

	meshList[TERRAIN_LEVEL04] = MeshBuilder::GenerateTerrain("Terrain", "Image//Terrain_Level04.raw", m_heightMap_4, level4_Heights);
	meshList[TERRAIN_LEVEL04]->textureArray[0] = LoadTGA("Image//level4_ground.tga");

	//weapon
	meshList[RIFLE] = MeshBuilder::GenerateOBJ("Rifle", "OBJ//rifle.obj");
	meshList[RIFLE]->textureArray[0] = LoadTGA("Image//rifle.tga");

	meshList[PISTOL] = MeshBuilder::GenerateOBJ("Pistol", "OBJ//pistol.obj");
	meshList[PISTOL]->textureArray[0] = LoadTGA("Image//pistol.tga");

	//level 1 terrain
	meshList[LEVEL01_TERRAIN] = MeshBuilder::GenerateTerrain("level01 terrain", "Image//Terrain_Level01.raw", m_heightMap, level1_Heights);
	meshList[LEVEL01_TERRAIN]->textureArray[0] = LoadTGA("Image//Forest//Grass.tga");

	meshList[LEVEL01_WALLS] = MeshBuilder::GenerateQuad("walls", Color(0, 0, 0), 1.f);
	meshList[LEVEL01_WALLS]->textureArray[0] = LoadTGA("Image//walltex.tga");

	meshList[STAIRS] = MeshBuilder::GenerateOBJ("stairs", "OBJ//Stairs.obj");
	meshList[STAIRS]->textureArray[0] = LoadTGA("Image//stairs.tga");

	meshList[LEVEL01] = MeshBuilder::GenerateOBJ("level01", "OBJ//Level01.obj");
	meshList[LEVEL01]->textureArray[0] = LoadTGA("Image//uvmap.tga");

	meshList[DOOR] = MeshBuilder::GenerateOBJ("Door", "OBJ//door.obj");
	meshList[DOOR]->textureArray[0] = LoadTGA("Image//door.tga");

	//meshList[BED] = MeshBuilder::GenerateOBJ("Bed", "OBJ//Bed.obj");
	//meshList[BED]->textureArray[0] = LoadTGA("Image//bed.tga");

	meshList[INDOORGATE] = MeshBuilder::GenerateOBJ("INDOORGATE", "OBJ//indoorGate.obj");
	meshList[INDOORGATE]->textureArray[0] = LoadTGA("Image//indoorGate.tga");

	meshList[BLOCKAGE] = MeshBuilder::GenerateOBJ("barricade", "OBJ//barricade.obj");
	meshList[BLOCKAGE]->textureArray[0] = LoadTGA("Image//Table.tga");

	meshList[TABLE] = MeshBuilder::GenerateOBJ("Table", "OBJ//table.obj");
	meshList[TABLE]->textureArray[0] = LoadTGA("Image//Table.tga");

	meshList[CHAIR] = MeshBuilder::GenerateOBJ("Table", "OBJ//chair.obj");
	meshList[CHAIR]->textureArray[0] = LoadTGA("Image//chair.tga");

	//meshList[ELEVATORDOOR] = MeshBuilder::GenerateOBJ("elevator", "OBJ//elevator.obj");
	//meshList[ELEVATORDOOR]->textureArray[0] = LoadTGA("Image//elevator.tga");

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

	meshList[HOUSE3] = MeshBuilder::GenerateOBJ("house", "OBJ//house2.obj");
	meshList[HOUSE3]->textureArray[0] = LoadTGA("Image//house3.tga");

	meshList[METAL_FENCE] = MeshBuilder::GenerateOBJ("house", "OBJ//metalFence.obj");
	meshList[METAL_FENCE]->textureArray[0] = LoadTGA("Image//metalFence.tga");

	meshList[METAL_GATE] = MeshBuilder::GenerateOBJ("house", "OBJ//gate.obj");
	meshList[METAL_GATE]->textureArray[0] = LoadTGA("Image//metalFence.tga");

	meshList[HEDGE] = MeshBuilder::GenerateOBJ("house", "OBJ//hedge.obj");
	meshList[HEDGE]->textureArray[0] = LoadTGA("Image//hedge.tga");

	meshList[BENCHES] = MeshBuilder::GenerateOBJ("house", "OBJ//bench.obj");
	meshList[BENCHES]->textureArray[0] = LoadTGA("Image//bench.tga");

	meshList[POT] = MeshBuilder::GenerateOBJ("pot", "OBJ//pot.obj");
	meshList[POT]->textureArray[0] = LoadTGA("Image//pot.tga");

	meshList[COCONUT_TREE] = MeshBuilder::GenerateQuad("Water", Color(0, 0, 0), 1.f);
	meshList[COCONUT_TREE]->textureArray[0] = LoadTGA("Image//coconutTree.tga");

	meshList[NIGHT_VISION] = MeshBuilder::GenerateQuad("NightVision", Color(1,1,1));
	meshList[NIGHT_VISION]->textureArray[0] = LoadTGA("Image//nightVision.tga");

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
	m_Minimap->SetAvatar(MeshBuilder::GenerateMinimapAvatar("Minimap avatar", Color(1, 1, 0), 1.f));
	meshList[GEO_VIEW] = MeshBuilder::GenerateCircle("View on minimap", Color(0, 0, 0), 1.f);
	meshList[GEO_VIEW]->textureArray[0] = LoadTGA("Image//HUD//Radar.tga");
	meshList[GEO_GREENBALL] = MeshBuilder::GenerateCircle("You on minimap", Color(0, 1, 0), 1.f);
	meshList[GEO_REDBALL] = MeshBuilder::GenerateCircle("Enemy on minimap", Color(1, 0, 0), 1.f);

	//Screen
	meshList[GEO_LOAD_1] = MeshBuilder::GenerateQuad("Level 1 loading screen", Color(0, 0, 0), 1.f);
	meshList[GEO_LOAD_1]->textureArray[0] = LoadTGA("Image//Screen//Load_Screen01.tga");
	meshList[GEO_LOAD_2] = MeshBuilder::GenerateQuad("Level 2 loading screen", Color(0, 0, 0), 1.f);
	meshList[GEO_LOAD_2]->textureArray[0] = LoadTGA("Image//Screen//Load_Screen02.tga");
	meshList[GEO_LOAD_3] = MeshBuilder::GenerateQuad("Level 3 loading screen", Color(0, 0, 0), 1.f);
	meshList[GEO_LOAD_3]->textureArray[0] = LoadTGA("Image//Screen//Load_Screen03.tga");
	meshList[GEO_LOAD_4] = MeshBuilder::GenerateQuad("Level 4 loading screen", Color(0, 0, 0), 1.f);
	meshList[GEO_LOAD_4]->textureArray[0] = LoadTGA("Image//Screen//Load_Screen04.tga");

	//Forest
	meshList[GEO_TREE_1] = MeshBuilder::GenerateQuad("Thin Tree", Color(0, 0, 0), 1.f);
	meshList[GEO_TREE_1]->textureArray[0] = LoadTGA("Image//Forest//Tree_1.tga");
	meshList[GEO_TREE_2] = MeshBuilder::GenerateQuad("Fat Tree", Color(0, 0, 0), 1.f);
	meshList[GEO_TREE_2]->textureArray[0] = LoadTGA("Image//Forest//Tree_2.tga");
	meshList[GEO_TREE_3] = MeshBuilder::GenerateQuad("Dead Tree", Color(0, 0, 0), 1.f);
	meshList[GEO_TREE_3]->textureArray[0] = LoadTGA("Image//Forest//Dead_Tree.tga");
	meshList[GEO_BUSH] = MeshBuilder::GenerateQuad("Water", Color(0, 0, 0), 1.f);
	meshList[GEO_BUSH]->textureArray[0] = LoadTGA("Image//Forest//Bush.tga");
	meshList[WATER] = MeshBuilder::GenerateQuad("Water", Color(0, 0, 0), 1.f);
	meshList[WATER]->textureArray[0] = LoadTGA("Image//sea.tga");
	meshList[WATER_SURFACE] = MeshBuilder::GenerateQuad("Water Surace", Color(0, 0, 0), 1.f);
	meshList[WATER_SURFACE]->textureArray[0] = LoadTGA("Image//sea2.tga");
	meshList[GEO_BRIDGE] = MeshBuilder::GenerateOBJ("Bridge", "OBJ//Forest//Bridge.obj");
	meshList[GEO_BRIDGE]->textureArray[0] = LoadTGA("Image//Forest//Bridge.tga");
	meshList[GEO_LOGS] = MeshBuilder::GenerateOBJ("Logs", "OBJ//Forest//Logs.obj");
	meshList[GEO_LOGS]->textureArray[0] = LoadTGA("Image//Forest//Logs.tga");

	//Level 4 - Graveyard
	meshList[TOMBSTONE] = MeshBuilder::GenerateOBJ("Tombstone", "OBJ//Tombstone.obj");
	meshList[TOMBSTONE]->textureArray[0] = LoadTGA("Image//Graveyard//Tombstone.tga");
	meshList[FENCE] = MeshBuilder::GenerateOBJ("Fence", "OBJ//wooden_fence.obj");
	meshList[FENCE]->textureArray[0] = LoadTGA("Image//wood_1.tga");
	meshList[DEADTREE] = MeshBuilder::GenerateOBJ("DEADTREE", "OBJ//tree.obj");
	meshList[DEADTREE]->textureArray[0] = LoadTGA("Image//Graveyard//deadtree.tga");

	//Sprite
	meshList[GEO_GHOST1] = MeshBuilder::GenerateSpriteAnimation("TumbleWeed", 4, 3);
	meshList[GEO_GHOST1]->textureArray[0] = LoadTGA("Image//Ghosts//Ghost_1.tga");
	meshList[GEO_GHOST2] = MeshBuilder::GenerateSpriteAnimation("Horsey", 4, 3);
	meshList[GEO_GHOST2]->textureArray[0] = LoadTGA("Image//Ghosts//Ghost_2.tga");
	meshList[GEO_GHOST3] = MeshBuilder::GenerateSpriteAnimation("Horsey", 3, 6);
	meshList[GEO_GHOST3]->textureArray[0] = LoadTGA("Image//Ghosts//Ghost_3.tga");
	meshList[GEO_HITBOX] = MeshBuilder::GenerateCube("Hitbox", Color(1, 0, 0));

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

	for (int i = 0; i < 40; ++i)
	{
		int Random = Math::RandIntMinMax(1, 3);
		
		Enemy* Ghost = new Enemy(Enemy::ENEMY_TYPE::GHOST_1);
		switch (Random)
		{
			case 1:
			{
				Ghost->Type = Enemy::ENEMY_TYPE::GHOST_1;
				break;
			}
			case 2:
			{
				Ghost->Type = Enemy::ENEMY_TYPE::GHOST_2;
				break;
			}
			case 3:
			{
				Ghost->Type = Enemy::ENEMY_TYPE::GHOST_3;
				break;
			}
		}
		Ghost->active = true;
		Ghost->pos.Set(Math::RandFloatMinMax(-1800, 1800), 0, Math::RandFloatMinMax(-1100, 1800));
		Ghost->scale.Set(50, 50, 50);

		Enemy_list.push_back(Ghost);
	}

	Singleton::getInstance()->player->Init();
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
		mode = false;
	if (Application::IsKeyPressed('4'))
		mode = true;

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

	if (Application::IsKeyPressed('V'))
	{
		RenderImageOnScreen(meshList[GEO_LOAD_1], Vector3(100, 2, 1), Vector3(50 - (100), 1, 0), Vector3(0, 0, 0));
	}

	if (Application::IsKeyPressed('B'))
	{
		RenderImageOnScreen(meshList[GEO_LOAD_2], Vector3(100, 2, 1), Vector3(50 - (100), 1, 0), Vector3(0, 0, 0));
	}

	if (Application::IsKeyPressed('N'))
	{
		RenderImageOnScreen(meshList[GEO_LOAD_3], Vector3(100, 2, 1), Vector3(50 - (100), 1, 0), Vector3(0, 0, 0));
	}

	if (Application::IsKeyPressed('M'))
	{
		RenderImageOnScreen(meshList[GEO_LOAD_4], Vector3(100, 2, 1), Vector3(50 - (100), 1, 0), Vector3(0, 0, 0));
	}

	UpdatePlayer(dt);
	Singleton::getInstance()->player->setPosition(camera.position);

	Vector3 View = (camera.target - camera.position).Normalized();
	rotateAngle = Math::RadianToDegree(atan2(-View.z, View.x));
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
	//viewStack.PushMatrix();
	//	viewStack.LoadIdentity();
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
	//viewStack.PopMatrix();
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
	glPolygonOffset(-2.0f, -2.0f);
	glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 0);
	mesh->Render();
	glPolygonOffset(2.0f, 2.0f);
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

	UpdateFearEffect(dt);
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
void SceneBase::UpdateHitboxes(double dt)
{
	for (std::vector<AABBObject *>::iterator it = Object_list.begin(); it != Object_list.end(); ++it)
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
					obj->Hitbox.UpdateAABB(obj->pos - Vector3(-3,50,0));
					obj->Hitbox.Resize(Vector3(140, 65, 650));
					break;
				}
			}
		}
	}
	for (std::vector<Enemy *>::iterator it = Enemy_list.begin(); it != Enemy_list.end(); ++it)
	{
		Enemy *ghost = (Enemy *)*it;
		if (ghost->active)
		{
			ghost->pos.y = TERRAINSIZE.y *  ReadHeightMap(m_heightMap_3, ghost->pos.x / TERRAINSIZE.x, ghost->pos.z / TERRAINSIZE.z);
			ghost->Hitbox.Resize(ghost->scale);
		}
	}
}

void SceneBase::RenderObjects(bool ShowHitbox)
{
	for (std::vector<AABBObject *>::iterator it = Object_list.begin(); it != Object_list.end(); ++it)
	{
		AABBObject *obj = (AABBObject *)*it;
		if (obj->active)
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
			}
		}
	}
}
void SceneBase::RenderEnemies(bool ShowHitbox)
{
	for (std::vector<Enemy *>::iterator it = Enemy_list.begin(); it != Enemy_list.end(); ++it)
	{
		Enemy *ghost = (Enemy *)*it;
		if (ghost->active)
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
	RenderMeshIn2D(meshList[GEO_GREENBALL], false, Vector3(1,1,1), 65, 45, true);

	for (std::vector<Enemy *>::iterator it = Enemy_list.begin(); it != Enemy_list.end(); ++it)
	{
		Enemy *ghost = (Enemy *)*it;
		if (ghost->active)
		{
			modelStack.PushMatrix();
			modelStack.Translate(65, 45, 0);
			modelStack.Rotate(rotateAngle, 0, 0, 1);
			modelStack.Translate(-ghost->pos.x / 70 + camera.position.x / 70, -ghost->pos.z / 70 + camera.position.z / 70, 0);
			RenderMesh(meshList[GEO_REDBALL], false);
			modelStack.PopMatrix();
		}
	}

	modelStack.PushMatrix();
	modelStack.Translate(65, 45, 0);
	modelStack.Rotate(rotateAngle, 0, 0, 1);
	modelStack.Translate(camera.position.x / 70, camera.position.z / 70, 0);
	modelStack.Scale(100, 100, 100);
	RenderMesh(m_Minimap->GetBackground(), false);
	modelStack.PopMatrix();

	glDisable(GL_STENCIL_TEST);

	RenderMeshIn2D(m_Minimap->GetBorder(), false, Vector3(30,30,30), 65, 45, false);

	//modelStack.PushMatrix();
	//modelStack.Translate(65, 45, 0);
	//modelStack.Rotate(rotateAngle, 0, 0, 1);
	//modelStack.Translate(camera.position.x / 70, camera.position.z / 70, 0);
	//modelStack.Scale(100, 100, 100);
	//RenderMesh(m_Minimap->GetBackground(), false);
	//modelStack.PopMatrix();

	projectionStack.PopMatrix();

	viewStack.PopMatrix();

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
	G.MAXPOS.Set(Terrainsize.x * 0.5f, 0,0);
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
	if
		(
		(partition1 == partition2) || (partition1 + 1 == partition2) || (partition1 + 3 == partition2) || (partition1 + 4 == partition2) || (partition1 + 5 == partition2)
		|| (partition1 - 1 == partition2) || (partition1 - 3 == partition2) || (partition1 - 4 == partition2) || (partition1 - 5 == partition2)
		)
		return true;
	else return false;
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