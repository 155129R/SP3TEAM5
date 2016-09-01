#ifndef SCENE_BASE_H
#define SCENE_BASE_H

#include "Scene.h"
#include "Mtx44.h"
#include "Mesh.h"
#include "MatrixStack.h"
#include "Light.h"
#include "Particle.h"
#include "DepthFBO.h"
#include "LoadHmap.h"
#include "LoadTGA.h"
#include "LoadCSV.h"
#include "MeshBuilder.h"
#include "Minimap.h"
#include "Bullet.h"
#include "Capture.h"
#include "Singleton.h"

struct Partition 
{
	Vector3 MINPOS;
	Vector3 MAXPOS;
};

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

		U_LIGHT2_TYPE,
		U_LIGHT2_POSITION,
		U_LIGHT2_COLOR,
		U_LIGHT2_POWER,
		U_LIGHT2_KC,
		U_LIGHT2_KL,
		U_LIGHT2_KQ,
		U_LIGHT2_SPOTDIRECTION,
		U_LIGHT2_COSCUTOFF,
		U_LIGHT2_COSINNER,
		U_LIGHT2_EXPONENT,

		U_LIGHT3_TYPE,
		U_LIGHT3_POSITION,
		U_LIGHT3_COLOR,
		U_LIGHT3_POWER,
		U_LIGHT3_KC,
		U_LIGHT3_KL,
		U_LIGHT3_KQ,
		U_LIGHT3_SPOTDIRECTION,
		U_LIGHT3_COSCUTOFF,
		U_LIGHT3_COSINNER,
		U_LIGHT3_EXPONENT,

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

		//weapons
		PISTOL,
		RIFLE,
		VACUUM,
		PISTOLBULLET,

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
		CEILING,
		TOILETBOWL,
		ELEVATOR,
		ELEVATORCOVER,
		HAMMER,
		ROOMCEILING,
		PAINTING,
		PAINTING2,
		PAINTING3,
		PAINTING4,
		PAINTING5,
		SOFA,
		BEDSIDE,
		SHELF,
		SINK,
		CRATE,

		//PLAYER
		GEO_STAMINA,
		NIGHT_VISION,
		GEO_TEXT_BOX,

		//Minimap
		GEO_VIEW,
		GEO_GREENBALL,
		GEO_REDBALL,
		GEO_BLUEBALL,
		GEO_BOSS_ICON,

		//Screen
		GEO_LOAD_1,
		GEO_LOAD_2,
		GEO_LOAD_3,
		GEO_LOAD_4,
		GAME_WIN,
		TO_HUB,
		GAME_CREDITS,

		//level 03
		GEO_CACTUS,
		GEO_TREE_1,
		GEO_TREE_2,
		GEO_TREE_3,
		GEO_BUSH,
		GEO_BRIDGE,
		GEO_LOGS,
		WATER_FLOW,

		//SPRITE / ENEMY
		GEO_GHOST1_ATTACK,
		GEO_GHOST1_LEFT,
		GEO_GHOST1_RIGHT,
		GEO_GHOST2_ATTACK,
		GEO_GHOST2_LEFT,
		GEO_GHOST2_RIGHT,
		GEO_GHOST3_ATTACK,
		GEO_GHOST3_LEFT,
		GEO_GHOST3_RIGHT,
		GEO_HITBOX,
		BOSS,
		WISP,
		GEO_HP,
		GEO_BOSS_HP,

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
		INVENTORY_UI,
		GAME_TITLE,
		BUTTON_PLAY,
		BUTTON_OPTIONS,
		BUTTON_CREDITS,
		BUTTON_INSTRUCTION,
		BUTTON_EXIT,
		GEO_KEY,
		QUEST_EXIT,
		
		//level4
		TOMBSTONE,
		FENCE,
		DEADTREE,
		POCONG,

		//inventory
		INV_GHOST1,
		INV_GHOST2,
		INV_GHOST3,
		INV_HOVER,
		INV_TORCH_LIGHT,
		INV_NIGHT_VISION,
		INV_RADAR,

		//shop
		SHOP_UI,
		UI_BUY,
		UI_SELL,
		UI_HOVER,
		UI_MONEY,
		UI_AMMO_PISTOL,
		UI_AMMO_RIFLE,
		UI_POTION,

		//HUB
		GEO_HUB,
		GEO_SHOPKEEPER,
		SHOP_BG,

		//UI
		UI_VACUUM,
		UI_PISTOL,
		UI_RIFLE,
		UI_BOX,
		UI_BOX2,
		UI_STAMINA_BAR,
		UI_FEAR_BAR,
		UI_BLACK_BAR,

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
	void RenderMeshIn2D(Mesh *mesh, bool enableLight, Vector3 Scale = (1.0f, 1.0f, 1.0f), float x = 0.0f, float y = 0.0f, bool rotate_Mini = false, float rotate = 0.0f);
	void RenderImageOnScreen(Mesh* mesh, Vector3 Scale, Vector3 Translate, Vector3 Rotate);
	void RenderMeshOutlined(Mesh* mesh, bool enableLight);
	void RenderMesh(Mesh *mesh, bool enableLight);
	void RenderOBJOnScreen(Mesh* mesh, float scale ,float x, float y, float z, float rotateX, float rotateY, float rotateZ, bool enableLight);
	void RenderNightVision();

	//Used in all scenes
	void SpawnGhost();

	void UpdatePlayer(double dt);
	void UpdateFearEffect(double dt);
	void UpdateEnemy(double dt);
	void UpdateBoss(double dt);
	void UpdateHitboxes(double dt);
	void UpdateShoot(double dt);
	void UpdateCapture(double dt);
	void UpdateWeaponType(double dt);

	void RenderObjects(bool ShowHitbox = false);
	void RenderEnemies(bool ShowHitbox = false);
	void RenderBoss(bool ShowHitbox = false);
	void RenderRadar(void);
	void RenderBullets(bool light);
	void RenderWeapons(bool light);
	void RenderInventory();
	void RenderGUI();
	void FootStep1();
	void FootStep2();
	void FootStep3();
	float getBaryCentricInterpolation(Vector3 p1, Vector3 p2, Vector3 p3, Vector3 pos);
	float getHeightofTerrain(float terrainscale, float ** heights);

	void InitPartitioning();
	char getPartition(Vector3 pos);
	bool renderCheck(char playerPartition, char posPartition);

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

	bool mode = false;

	//For particles
	std::vector<ParticleObject*> particleList;  //Used to store particles
	Vector3 m_gravity;							//Gravity affecting the particles
	int m_particlesCount;						//Number of particles
	unsigned MAX_PARTICLE;						//Max no of particles

	//Terrain
	const Vector3 TERRAINSIZE;

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
	bool ShowHitbox = false;

	//Player
	float radarAngle = 0;
	float rotateAngle = 0;
	float rotateKey = 0;
	Minimap* m_Minimap;
	float HealthpackCD = 0.0f;
	bool Ready = true;
	Color Black;

	//Enemy 
	int ghost_Amount[9];
	float delay = 0;

	//shooting 
	float readyToShoot = 0.f;
	int fireRate = 1;
	int rotatePistol;
	int rotateRifle;
	bool gunUp;
	bool gunDown;

	float Degree;

	bool reloading = false;
	float reloadTime;

	double scaleWin = 5;

	//bullet
	std::vector<Bullet*> bulletList;
	//capture
	std::vector<Capture*> captureList;

	//Sprite Pointers
	SpriteAnimation* G1_A;
	SpriteAnimation* G1_L;
	SpriteAnimation* G1_R;
	SpriteAnimation* G2_A;
	SpriteAnimation* G2_L;
	SpriteAnimation* G2_R;
	SpriteAnimation* G3_A;
	SpriteAnimation* G3_L;
	SpriteAnimation* G3_R;

	SpriteAnimation* Wisp;

	std::map<char, Partition> partitioning;

	bool nightVision;

	char playerPartition;
	char posPartition;
	bool spatialPartitioning;

	//weapon
	int weaponType;

	Singleton* instance;
	bool cameraViewObject(Vector3 pos, float degree);

	Sound sound;

	Color fogColor;

	bool showText = false;
	bool Dialogues = true;
};

#endif