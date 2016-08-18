#ifndef SCENE_BASE_H
#define SCENE_BASE_H

#include "Scene.h"
#include "Mtx44.h"
#include "Mesh.h"
#include "MatrixStack.h"
#include "Light.h"
#include <vector>
#include "Particle.h"
#include "DepthFBO.h"
#include <map>

struct Partition
{
	Vector3 MINPOS;
	Vector3 MAXPOS;
};

#include "MeshBuilder.h"

#include "Player.h"
#include "Enemy.h"

class SceneBase : public Scene
{
protected:
	enum UNIFORM_TYPE
	{
		U_MVP = 0,
		U_MODELVIEW,
		U_MODELVIEW_INVERSE_TRANSPOSE,
		U_MATERIAL_AMBIENT,
		U_MATERIAL_DIFFUSE,
		U_MATERIAL_SPECULAR,
		U_MATERIAL_SHININESS,
		U_LIGHTENABLED,
		U_NUMLIGHTS,
		U_LIGHT0_TYPE,
		U_LIGHT0_POSITION,
		U_LIGHT0_COLOR,
		U_LIGHT0_POWER,
		U_LIGHT0_KC,
		U_LIGHT0_KL,
		U_LIGHT0_KQ,
		U_LIGHT0_SPOTDIRECTION,
		U_LIGHT0_COSCUTOFF,
		U_LIGHT0_COSINNER,
		U_LIGHT0_EXPONENT,
		U_LIGHT1_TYPE,
		U_LIGHT1_POSITION,
		U_LIGHT1_COLOR,
		U_LIGHT1_POWER,
		U_LIGHT1_KC,
		U_LIGHT1_KL,
		U_LIGHT1_KQ,
		U_LIGHT1_SPOTDIRECTION,
		U_LIGHT1_COSCUTOFF,
		U_LIGHT1_COSINNER,
		U_LIGHT1_EXPONENT,

		U_COLOR_TEXTURE_ENABLED,
		U_COLOR_TEXTURE_ENABLED1,

		U_COLOR_TEXTURE,
		U_COLOR_TEXTURE1,

		U_TEXT_ENABLED,
		U_TEXT_COLOR,

		U_FOG_COLOR,
		U_FOG_START,
		U_FOG_END,
		U_FOG_DENSITY,
		U_FOG_TYPE,
		U_FOG_ENABLE,

		U_LIGHT_DEPTH_MVP_GPASS,
		U_LIGHT_DEPTH_MVP,
		U_SHADOW_MAP,

		U_SHADOW_COLOR_TEXTURE,
		U_SHADOW_COLOR_TEXTURE1,
		U_SHADOW_COLOR_TEXTURE2,

		U_SHADOW_COLOR_TEXTURE_ENABLED,
		U_SHADOW_COLOR_TEXTURE_ENABLED1,
		U_SHADOW_COLOR_TEXTURE_ENABLED2,

		U_TOTAL,
	};

public:
	enum GEOMETRY_TYPE
	{
		GEO_AXES,
		GEO_CROSSHAIR,
		GEO_LIGHTBALL,
		GEO_SPHERE,
		GEO_QUAD,
		GEO_CUBE,
		GEO_RING,
		GEO_CONE,
		GEO_LEFT,
		GEO_RIGHT,
		GEO_TOP,
		GEO_BOTTOM,
		GEO_FRONT,
		GEO_BACK,
		GEO_GRASS_DARKGREEN,
		GEO_GRASS_LIGHTGREEN,
		GEO_TEXT,

		SKYPLANE,
		TERRAIN,
		TERRAIN_LEVEL01,
		TERRAIN_LEVEL02,
		TERRAIN_LEVEL03,
		TERRAIN_LEVEL04,

		CACTUS,
		TOMBSTONE,

		PISTOL,
		RIFLE,

		//level 01
		LEVEL01_TERRAIN,
		LEVEL01_WALLS,
		LEVEL01,
		STAIRS,
		DOOR,
		BED,
		WINDOW,
		TABLE,
		CHAIR,
		INDOORPLANT,
		BLOCKAGE,
		INDOORGATE,
		CHANDELIER,
		ELEVATORDOOR,

		//PLAYER
		GEO_STAMINA,

		//level 03
		GEO_CACTUS,
		GEO_TREE_1,
		GEO_TREE_2,
		GEO_TREE_3,
		GEO_BUSH,
		GEO_BRIDGE,
		GEO_LOGS,
		WATER,
		WATER_SURFACE,

		//SPRITE / ENEMY
		GEO_GHOST1,
		GEO_GHOST2,
		GEO_GHOST3,
		GEO_HITBOX,

		//level 2
		FOUNTAIN,
		FOUNTAIN_WATER1,
		FLOOR,
		HOUSE1,
		HOUSE2,
		POT,
		COCONUT_TREE,
		HEDGE,
		BENCHES,
		HOUSE3,
		METAL_FENCE,
		METAL_GATE,

		GEO_PARTICLE_WATER,
		GEO_PARTICLE_SAND,

		GEO_LIGHT_DEPTH_QUAD,

		NUM_GEOMETRY,
	};
	enum RENDER_PASS
	{
		RENDER_PASS_PRE,
		RENDER_PASS_MAIN,
		NUM_RENDER_PASS,
	};

	SceneBase();
	~SceneBase();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

	void RenderText(Mesh* mesh, std::string text, Color color);
	void RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y);
	void RenderMeshIn2D(Mesh *mesh, bool enableLight, float size = 1.0f, float x = 0.0f, float y = 0.0f);
	void RenderImageOnScreen(Mesh* mesh, Vector3 Scale, Vector3 Translate, Vector3 Rotate);
	void RenderMeshOutlined(Mesh* mesh, bool enableLight);
	void RenderMesh(Mesh *mesh, bool enableLight);

	//Used in all scenes
	void UpdatePlayer(double dt);
	void UpdateFearEffect(double dt);

	void RenderEnemies(bool ShowHitbox = false);

	float getBaryCentricInterpolation(Vector3 p1, Vector3 p2, Vector3 p3, Vector3 pos);
	float getHeightofTerrain(float terrainscale, float ** heights);

	void InitPartitioning();
	char getPartition(Vector3 pos);

protected:
	unsigned m_vertexArrayID;
	Mesh* meshList[NUM_GEOMETRY];
	unsigned m_programID;
	unsigned m_parameters[U_TOTAL];

	Camera3 camera;

	MS modelStack;
	MS viewStack;
	MS projectionStack;

	Light lights[2];

	//For particles
	std::vector<ParticleObject*> particleList;  //Used to store particles
	Vector3 m_gravity;							//Gravity affecting the particles
	int m_particlesCount;						//Number of particles
	unsigned MAX_PARTICLE;						//Max no of particles

	//Terrain
	std::vector<unsigned char>m_heightMap;
	std::vector<unsigned char>m_heightMap_2;
	std::vector<unsigned char>m_heightMap_3;
	std::vector<unsigned char>m_heightMap_4;

	float **level1_Heights;
	float **level2_Heights;
	float **level3_Heights;
	float **level4_Heights;
	float characterHeight;

	float terrainHeight;
	Vector3 Terrainsize;

	//Shadow Stuff
	unsigned m_gPassShaderID;
	DepthFBO m_lightDepthFBO;
	RENDER_PASS m_renderPass;

	Mtx44 m_lightDepthProj;
	Mtx44 m_lightDepthView;

	bool bLightEnabled;

	float FogAmount;
	float fps;

	//Player
	Player* player;
	Color Black;

	//Enemy
	std::vector<Enemy *> Enemy_list;

	//Sprite Pointers
	SpriteAnimation* G1;
	SpriteAnimation* G2;
	SpriteAnimation* G3;

	std::map<char, Partition> partitioning;
};

#endif