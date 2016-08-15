#include "SceneTexture.h"
#include "GL\glew.h"

#include "shader.hpp"
#include "MeshBuilder.h"
#include "Application.h"
#include "Utility.h"
#include "LoadTGA.h"

SceneTexture::SceneTexture()
{
}

SceneTexture::~SceneTexture()
{
}

void SceneTexture::Init()
{
	// Black background
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS); 
	
	glEnable(GL_CULL_FACE);
	
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// Enable blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glGenVertexArrays(1, &m_vertexArrayID);
	glBindVertexArray(m_vertexArrayID);

	m_programID = LoadShaders( "Shader//Texture.vertexshader", "Shader//Texture.fragmentshader" );
	
	// Get a handle for our uniform
	m_parameters[U_MVP] = glGetUniformLocation(m_programID, "MVP");
	//m_parameters[U_MODEL] = glGetUniformLocation(m_programID, "M");
	//m_parameters[U_VIEW] = glGetUniformLocation(m_programID, "V");
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
	m_parameters[U_COLOR_TEXTURE_ENABLED] = glGetUniformLocation(m_programID, "colorTextureEnabled");
	m_parameters[U_COLOR_TEXTURE] = glGetUniformLocation(m_programID, "colorTexture");
	
	// Use our shader
	glUseProgram(m_programID);

	lights[0].type = Light::LIGHT_SPOT;
	lights[0].position.Set(0, 20, 0);
	lights[0].color.Set(1, 1, 1);
	lights[0].power = 1;
	lights[0].kC = 1.f;
	lights[0].kL = 0.01f;
	lights[0].kQ = 0.001f;
	lights[0].cosCutoff = cos(Math::DegreeToRadian(45));
	lights[0].cosInner = cos(Math::DegreeToRadian(30));
	lights[0].exponent = 3.f;
	lights[0].spotDirection.Set(0.f, 1.f, 0.f);

	lights[1].type = Light::LIGHT_DIRECTIONAL;
	lights[1].position.Set(1, 1, 0);
	lights[1].color.Set(1, 1, 0.5f);
	lights[1].power = 0.4f;
	//lights[1].kC = 1.f;
	//lights[1].kL = 0.01f;
	//lights[1].kQ = 0.001f;
	//lights[1].cosCutoff = cos(Math::DegreeToRadian(45));
	//lights[1].cosInner = cos(Math::DegreeToRadian(30));
	//lights[1].exponent = 3.f;
	//lights[1].spotDirection.Set(0.f, 1.f, 0.f);
	
	glUniform1i(m_parameters[U_NUMLIGHTS], 1);

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

	camera.Init(Vector3(0, 0, 50), Vector3(0, 0, 0), Vector3(0, 1, 0));

	for(int i = 0; i < NUM_GEOMETRY; ++i)
	{
		meshList[i] = NULL;
	}
	meshList[GEO_AXES] = MeshBuilder::GenerateAxes("reference");//, 1000, 1000, 1000);
	meshList[GEO_QUAD] = MeshBuilder::GenerateQuad("quad", Color(1, 1, 1), 100.f);
	//meshList[GEO_QUAD]->textureID = LoadTGA("Image//color2.tga");
	meshList[GEO_CUBE] = MeshBuilder::GenerateCube("cube", 1);
	meshList[GEO_RING] = MeshBuilder::GenerateRing("ring", Color(1, 0, 1), 36, 1, 0.5f);
	meshList[GEO_LIGHTBALL] = MeshBuilder::GenerateSphere("lightball", Color(1, 1, 1), 18, 36, 1.f);
	meshList[GEO_SPHERE] = MeshBuilder::GenerateSphere("sphere", Color(1, 0, 0), 18, 36, 10.f);
	//meshList[GEO_CUBE] = MeshBuilder::GenerateCube("cube", 1, 1, 1);
	//meshList[GEO_TORUS] = MeshBuilder::GenerateCylinder("torus", 36, 36, 5, 1);
	meshList[GEO_SPHERE2] = MeshBuilder::GenerateSphere("sphere2", Color(1, 0, 1), 18, 36, 10.f);
	meshList[GEO_SPHERE2]->material.kDiffuse.Set(0.3f, 0.3f, 0.3f);
	meshList[GEO_SPHERE2]->material.kSpecular.Set(0.6f, 0.6f, 0.6f);
	meshList[GEO_SPHERE3] = MeshBuilder::GenerateSphere("sphere3", Color(0, 1, 0), 18, 36, 10.f);
	meshList[GEO_SPHERE3]->material.kDiffuse.Set(0.6f, 0.6f, 0.6f);
	meshList[GEO_SPHERE3]->material.kSpecular.Set(0.9f, 0.9f, 0.9f);
	meshList[GEO_SPHERE4] = MeshBuilder::GenerateSphere("sphere4", Color(1, 1, 0), 18, 36, 10.f);
	meshList[GEO_SPHERE4]->material.kDiffuse.Set(0.6f, 0.6f, 0.6f);
	meshList[GEO_SPHERE4]->material.kSpecular.Set(0.9f, 0.9f, 0.9f);
	meshList[GEO_SPHERE4]->material.kShininess = 8.f;
	meshList[GEO_SPHERE5] = MeshBuilder::GenerateSphere("sphere5", Color(0.8f, 0.4f, 0.11f), 18, 36, 10.f);
	meshList[GEO_SPHERE5]->material.kDiffuse.Set(0.99f, 0.99f, 0.99f);
	meshList[GEO_SPHERE5]->material.kSpecular.Set(0.1f, 0.1f, 0.1f);
	meshList[GEO_SPHERE6] = MeshBuilder::GenerateSphere("sphere6", Color(0.22f, 0.56f, 0.56f), 18, 36, 10.f);
	meshList[GEO_SPHERE6]->material.kDiffuse.Set(0.99f, 0.99f, 0.99f);
	meshList[GEO_SPHERE6]->material.kSpecular.Set(0.1f, 0.1f, 0.1f);
	meshList[GEO_SPHERE7] = MeshBuilder::GenerateSphere("sphere7", Color(0.7f, 0.23f, 0.93f), 18, 36, 10.f);
	meshList[GEO_SPHERE7]->material.kDiffuse.Set(0.99f, 0.99f, 0.99f);
	meshList[GEO_SPHERE7]->material.kSpecular.Set(0.1f, 0.1f, 0.1f);
	meshList[GEO_SPHERE8] = MeshBuilder::GenerateSphere("sphere8", Color(1.f, 0.08f, 0.58f), 18, 36, 10.f);
	meshList[GEO_SPHERE8]->material.kDiffuse.Set(0.99f, 0.99f, 0.99f);
	meshList[GEO_SPHERE8]->material.kSpecular.Set(0.1f, 0.1f, 0.1f);
	meshList[GEO_SPHERE9] = MeshBuilder::GenerateSphere("sphere9", Color(0, 0, 1), 18, 36, 10.f);
	meshList[GEO_SPHERE9]->material.kDiffuse.Set(0.99f, 0.99f, 0.99f);
	meshList[GEO_SPHERE9]->material.kSpecular.Set(0.1f, 0.1f, 0.1f);
	meshList[GEO_CONE] = MeshBuilder::GenerateCone("cone", Color(0.5f, 1, 0.3f), 36, 10.f, 10.f);
	meshList[GEO_CONE]->material.kDiffuse.Set(0.99f, 0.99f, 0.99f);
	meshList[GEO_CONE]->material.kSpecular.Set(0.f, 0.f, 0.f);
	
	meshList[GEO_QUAD1] = MeshBuilder::GenerateQuad("quad", Color(1, 0, 0), 2.f);
	meshList[GEO_QUAD1]->textureID = LoadTGA("Image//marioalpha.tga");
	meshList[GEO_CIRCLE1] = MeshBuilder::GenerateRing("quad", Color(0, 1, 0), 360, 1.f, 0);
	
	// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 1000 units
	Mtx44 perspective;
	//perspective.SetToPerspective(45.0f, 4.0f / 3.0f, 0.1f, 1000.0f);
	perspective.SetToOrtho(-40, 40, -30, 30, -1000, 1000);
	projectionStack.LoadMatrix(perspective);
	
	rotateAngle = 0;
	circlePos.SetZero();
	quadPos.SetZero();
}

void SceneTexture::UpdatePhysics(double dt)
{
	static const float SPEED = 10.f;
	//Write Maths/Physics code
	circlePos.x += SPEED * (float)dt;
}

void SceneTexture::Update(double dt)
{
	if(Application::IsKeyPressed('1'))
		glEnable(GL_CULL_FACE);
	if(Application::IsKeyPressed('2'))
		glDisable(GL_CULL_FACE);
	if(Application::IsKeyPressed('3'))
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	if(Application::IsKeyPressed('4'))
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	
	if(Application::IsKeyPressed('5'))
	{
		lights[0].type = Light::LIGHT_POINT;
		glUniform1i(m_parameters[U_LIGHT0_TYPE], lights[0].type);
	}
	else if(Application::IsKeyPressed('6'))
	{
		lights[0].type = Light::LIGHT_DIRECTIONAL;
		glUniform1i(m_parameters[U_LIGHT0_TYPE], lights[0].type);
	}
	else if(Application::IsKeyPressed('7'))
	{
		lights[0].type = Light::LIGHT_SPOT;
		glUniform1i(m_parameters[U_LIGHT0_TYPE], lights[0].type);
	}

	if(Application::IsKeyPressed('I'))
		lights[0].position.z -= (float)(10.f * dt);
	if(Application::IsKeyPressed('K'))
		lights[0].position.z += (float)(10.f * dt);
	if(Application::IsKeyPressed('J'))
		lights[0].position.x -= (float)(10.f * dt);
	if(Application::IsKeyPressed('L'))
		lights[0].position.x += (float)(10.f * dt);
	if(Application::IsKeyPressed('O'))
		lights[0].position.y -= (float)(10.f * dt);
	if(Application::IsKeyPressed('P'))
		lights[0].position.y += (float)(10.f * dt);

	rotateAngle += (float)(10 * dt);

	camera.Update(dt);

	//UpdatePhysics(dt);
}

void SceneTexture::RenderMesh(Mesh *mesh, bool enableLight)
{
	Mtx44 MVP, modelView, modelView_inverse_transpose;
	
	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	if(mesh->textureID > 0)
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
	if(enableLight)
	{
		glUniform1i(m_parameters[U_LIGHTENABLED], 1);
		modelView = viewStack.Top() * modelStack.Top();
		glUniformMatrix4fv(m_parameters[U_MODELVIEW], 1, GL_FALSE, &modelView.a[0]);
		modelView_inverse_transpose = modelView.GetInverse().GetTranspose();
		glUniformMatrix4fv(m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE], 1, GL_FALSE, &modelView.a[0]);

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
	mesh->Render();
	if(mesh->textureID > 0)
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

void SceneTexture::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	// Camera matrix
	viewStack.LoadIdentity();
	viewStack.LookAt(
						camera.position.x, camera.position.y, camera.position.z,
						camera.target.x, camera.target.y, camera.target.z,
						camera.up.x, camera.up.y, camera.up.z
					);
	// Model matrix : an identity matrix (model will be at the origin)
	modelStack.LoadIdentity();

	if(lights[0].type == Light::LIGHT_DIRECTIONAL)
	{
		Vector3 lightDir(lights[0].position.x, lights[0].position.y, lights[0].position.z);
		Vector3 lightDirection_cameraspace = viewStack.Top() * lightDir;
		glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, &lightDirection_cameraspace.x);
	}
	else if(lights[0].type == Light::LIGHT_SPOT)
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
	if(lights[1].type == Light::LIGHT_DIRECTIONAL)
	{
		Vector3 lightDir(lights[1].position.x, lights[1].position.y, lights[1].position.z);
		Vector3 lightDirection_cameraspace = viewStack.Top() * lightDir;
		glUniform3fv(m_parameters[U_LIGHT1_POSITION], 1, &lightDirection_cameraspace.x);
	}
	else if(lights[1].type == Light::LIGHT_SPOT)
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
	
	//RenderMesh(meshList[GEO_AXES], false);
	
	//modelStack.PushMatrix();
	//modelStack.Translate(lights[0].position.x, lights[0].position.y, lights[0].position.z);
	//RenderMesh(meshList[GEO_LIGHTBALL], false);
	//modelStack.PopMatrix();
	
	glDisable(GL_BLEND);
	modelStack.PushMatrix();
	modelStack.Translate(0, 0, -10);
	//modelStack.Rotate(-90, 1, 0, 0);
	RenderMesh(meshList[GEO_QUAD], false);
	modelStack.PopMatrix();
	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	modelStack.PushMatrix();
	modelStack.Translate(0, 0, -5);
	modelStack.Scale(5, 5, 5);
	modelStack.Rotate(90, 1, 0, 0);
	RenderMesh(meshList[GEO_CIRCLE1], false);
	modelStack.PopMatrix();
	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	modelStack.PushMatrix();
	modelStack.Translate(5, 0, 0);
	modelStack.Scale(5, 5, 5);
	RenderMesh(meshList[GEO_QUAD1], false);
	modelStack.PopMatrix();

	//modelStack.PushMatrix();
	//modelStack.Translate(0, 0, 0);
	//RenderMesh(meshList[GEO_CONE], true);
	//modelStack.PopMatrix();
	
	//modelStack.PushMatrix();
	//modelStack.Translate(0, 0, 0);
	//RenderMesh(meshList[GEO_SPHERE], true);
	//modelStack.PopMatrix();
	//
	//modelStack.PushMatrix();
	//modelStack.Translate(-20, 0, 0);
	//RenderMesh(meshList[GEO_SPHERE2], true);
	//modelStack.PopMatrix();
	//
	//modelStack.PushMatrix();
	//modelStack.Translate(20, 0, 0);
	//RenderMesh(meshList[GEO_SPHERE3], true);
	//modelStack.PopMatrix();
	//
	//modelStack.PushMatrix();
	//modelStack.Translate(0, 0, 20);
	//RenderMesh(meshList[GEO_SPHERE4], true);
	//modelStack.PopMatrix();
	//
	//modelStack.PushMatrix();
	//modelStack.Translate(-20, 0, 20);
	//RenderMesh(meshList[GEO_SPHERE5], true);
	//modelStack.PopMatrix();
	//
	//modelStack.PushMatrix();
	//modelStack.Translate(20, 0, 20);
	//RenderMesh(meshList[GEO_SPHERE6], true);
	//modelStack.PopMatrix();
	//
	//modelStack.PushMatrix();
	//modelStack.Translate(0, 0, -20);
	//RenderMesh(meshList[GEO_SPHERE7], true);
	//modelStack.PopMatrix();
	//
	//modelStack.PushMatrix();
	//modelStack.Translate(-20, 0, -20);
	//RenderMesh(meshList[GEO_SPHERE8], true);
	//modelStack.PopMatrix();
	//
	//modelStack.PushMatrix();
	//modelStack.Translate(20, 0, -20);
	//RenderMesh(meshList[GEO_SPHERE9], true);
	//modelStack.PopMatrix();
}

void SceneTexture::Exit()
{
	// Cleanup VBO
	for(int i = 0; i < NUM_GEOMETRY; ++i)
	{
		if(meshList[i])
			delete meshList[i];
	}
	glDeleteProgram(m_programID);
	glDeleteVertexArrays(1, &m_vertexArrayID);
}
