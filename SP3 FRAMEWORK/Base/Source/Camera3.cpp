#include "Camera3.h"
#include "Application.h"
#include "Mtx44.h"

static const float CAMERA_SPEED = 100.f;
static const float MOUSE_SPEED = 200.f;

Camera3::Camera3()
{
}

Camera3::~Camera3()
{
}

void Camera3::Init(const Vector3& pos, const Vector3& target, const Vector3& up)
{
	this->position = defaultPosition = pos;
	this->target = defaultTarget = target;
	Vector3 view = (target - position).Normalized();
	Vector3 right = view.Cross(up);
	right.y = 0;
	right.Normalize();
	this->up = defaultUp = right.Cross(view).Normalized();

	Jump = false;
	Jumped = false;
	RUN_SPEED = 1.0f;
}

void Camera3::Update(double dt)
{
	//if (Application::IsKeyPressed('W'))
	//{
	//	Vector3 view = (target - position).Normalized();
	//	target.x += view.x * CAMERA_SPEED * (float)dt;
	//	target.z += view.z * CAMERA_SPEED * (float)dt;
	//	position.x += view.x * CAMERA_SPEED * (float)dt;
	//	position.z += view.z * CAMERA_SPEED * (float)dt;
	//}
	//if (Application::IsKeyPressed('S'))
	//{
	//	Vector3 view = (target - position).Normalized();
	//	target.x -= view.x * CAMERA_SPEED * (float)dt;
	//	target.z -= view.z * CAMERA_SPEED * (float)dt;
	//	position.x -= view.x * CAMERA_SPEED * (float)dt;
	//	position.z -= view.z * CAMERA_SPEED * (float)dt;
	//}
	//if (Application::IsKeyPressed('A'))
	//{
	//	Vector3 view = (target - position).Normalized();
	//	Vector3 right = view.Cross(up);
	//	right.y = 0;
	//	right.Normalize();
	//	target.x -= right.x * CAMERA_SPEED * (float)dt;
	//	target.z -= right.z * CAMERA_SPEED * (float)dt;
	//	position.x -= right.x * CAMERA_SPEED * (float)dt;
	//	position.z -= right.z * CAMERA_SPEED * (float)dt;
	//}
	//if (Application::IsKeyPressed('D'))
	//{
	//	Vector3 view = (target - position).Normalized();
	//	Vector3 right = view.Cross(up);
	//	right.y = 0;
	//	right.Normalize();
	//	target.x += right.x * CAMERA_SPEED * (float)dt;
	//	target.z += right.z * CAMERA_SPEED * (float)dt;
	//	position.x += right.x * CAMERA_SPEED * (float)dt;
	//	position.z += right.z * CAMERA_SPEED * (float)dt;
	//}

	if(Application::IsKeyPressed('W'))
	{
		Vector3 view = (target - position).Normalized();
		target.x += view.x * CAMERA_SPEED * RUN_SPEED * (float)dt;
		target.z += view.z * CAMERA_SPEED * RUN_SPEED * (float)dt;
		position.x += view.x * CAMERA_SPEED * RUN_SPEED * (float)dt;
		position.z += view.z * CAMERA_SPEED * RUN_SPEED * (float)dt;

		if (Terrain > position.y)
		{
			target.y += Terrain - position.y;
		}
		else
		{
			target.y -= position.y - Terrain;
		}
		position.y = Terrain;
	}
	if (Application::IsKeyPressed('S'))
	{
		Vector3 view = (target - position).Normalized();
		target.x -= view.x * CAMERA_SPEED * RUN_SPEED * (float)dt;
		target.z -= view.z * CAMERA_SPEED * RUN_SPEED * (float)dt;
		position.x -= view.x * CAMERA_SPEED * RUN_SPEED * (float)dt;
		position.z -= view.z * CAMERA_SPEED * RUN_SPEED * (float)dt;

		if (Terrain > position.y)
		{
			target.y += Terrain - position.y;
		}
		else
		{
			target.y -= position.y - Terrain;
		}
		position.y = Terrain;
	}
	if(Application::IsKeyPressed('A'))
	{
		Vector3 view = (target - position).Normalized();
		Vector3 right = view.Cross(up);
		right.y = 0;
		right.Normalize();
		target.x -= right.x * CAMERA_SPEED * RUN_SPEED * (float)dt;
		target.z -= right.z * CAMERA_SPEED * RUN_SPEED * (float)dt;
		position.x -= right.x * CAMERA_SPEED * RUN_SPEED * (float)dt;
		position.z -= right.z * CAMERA_SPEED * RUN_SPEED * (float)dt;

		if (Terrain > position.y)
		{
			target.y += Terrain - position.y;
		}
		else
		{
			target.y -= position.y - Terrain;
		}
		position.y = Terrain;
	}
	if(Application::IsKeyPressed('D'))
	{
		Vector3 view = (target - position).Normalized();
		Vector3 right = view.Cross(up);
		right.y = 0;
		right.Normalize();
		target.x += right.x * CAMERA_SPEED * RUN_SPEED * (float)dt;
		target.z += right.z * CAMERA_SPEED * RUN_SPEED * (float)dt;
		position.x += right.x * CAMERA_SPEED * RUN_SPEED * (float)dt;
		position.z += right.z * CAMERA_SPEED * RUN_SPEED * (float)dt;

		if (Terrain > position.y)
		{
			target.y += Terrain - position.y;
		}
		else
		{
			target.y -= position.y - Terrain;
		}
		position.y = Terrain;
	}

	if (Application::IsKeyPressed(VK_LSHIFT))
	{
		RUN_SPEED = 2.0f;
	}
	else
	{
		RUN_SPEED = 1.0f;
	}

	//if (Application::IsKeyPressed(VK_SPACE))
	//{
	//	target.y += CAMERA_SPEED * 0.5 * (float)dt;
	//	position.y += CAMERA_SPEED * 0.5 * (float)dt;
	//}
	//if (Jump == false && Jumped == false)
	//{
	//	if (Application::IsKeyPressed(VK_SPACE))
	//	{
	//		Jump = true;
	//	}
	//}

	//if (position.y <= 20.f && Jump == true)
	//{
	//	target.y += CAMERA_SPEED * 0.5 * (float)dt;
	//	position.y += CAMERA_SPEED * 0.5 * (float)dt;
	//}
	//else
	//{
	//	Jump = false;
	//	Jumped = true;
	//}

	//if (position.y >= 0.f && Jumped == true)
	//{
	//	target.y -= CAMERA_SPEED * 0.5 * (float)dt;
	//	position.y -= CAMERA_SPEED * 0.5 * (float)dt;
	//}
	//else
	//{
	//	Jumped = false;
	//}

	//if(Application::IsKeyPressed(VK_LEFT))
	//{
	//	Vector3 view = (target - position).Normalized();
	//	float yaw = (float)(CAMERA_SPEED * (float)dt);
	//	Mtx44 rotation;
	//	rotation.SetToRotation(yaw, 0, 1, 0);
	//	view = rotation * view;
	//	target = position + view;
	//	Vector3 right = view.Cross(up);
	//	right.y = 0;
	//	right.Normalize();
	//	up = right.Cross(view).Normalized();
	//}
	//if(Application::IsKeyPressed(VK_RIGHT))
	//{
	//	Vector3 view = (target - position).Normalized();
	//	float yaw = (float)(-CAMERA_SPEED * (float)dt);
	//	Mtx44 rotation;
	//	rotation.SetToRotation(yaw, 0, 1, 0);
	//	view = rotation * view;
	//	target = position + view;
	//	Vector3 right = view.Cross(up);
	//	right.y = 0;
	//	right.Normalize();
	//	up = right.Cross(view).Normalized();
	//}
	//if(Application::IsKeyPressed(VK_UP))
	//{
	//	float pitch = (float)(CAMERA_SPEED * (float)dt);
	//	Vector3 view = (target - position).Normalized();
	//	Vector3 right = view.Cross(up);
	//	right.y = 0;
	//	right.Normalize();
	//	up = right.Cross(view).Normalized();
	//	Mtx44 rotation;
	//	rotation.SetToRotation(pitch, right.x, right.y, right.z);
	//	view = rotation * view;
	//	target = position + view;
	//}
	//if(Application::IsKeyPressed(VK_DOWN))
	//{
	//	float pitch = (float)(-CAMERA_SPEED * (float)dt);
	//	Vector3 view = (target - position).Normalized();
	//	Vector3 right = view.Cross(up);
	//	right.y = 0;
	//	right.Normalize();
	//	up = right.Cross(view).Normalized();
	//	Mtx44 rotation;
	//	rotation.SetToRotation(pitch, right.x, right.y, right.z);
	//	view = rotation * view;
	//	target = position + view;
	//}

	//Update the camera direction based on mouse move
	// left-right rotate

	{
		Vector3 view = (target - position).Normalized();
		float yaw = (float)(-MOUSE_SPEED * Application::camera_yaw * (float)dt);
		Mtx44 rotation;
		rotation.SetToRotation(yaw, 0, 1, 0);
		view = rotation * view;
		target = position + view;
		Vector3 right = view.Cross(up);
		right.y = 0;
		right.Normalize();
		up = right.Cross(view).Normalized();
	}
	{
		float pitch = (float)(-MOUSE_SPEED * Application::camera_pitch * (float)dt);
		Vector3 view = (target - position).Normalized();
		Vector3 right = view.Cross(up);
		right.y = 0;
		right.Normalize();
		up = right.Cross(view).Normalized();
		Mtx44 rotation;
		rotation.SetToRotation(pitch, right.x, right.y, right.z);
		view = rotation * view;
		if (view.y <= 0.9f && view.y >= -0.6f)
		{
			target = position + view;
		}
	}
	if(Application::IsKeyPressed('R'))
	{
		Reset();
	}
}

void Camera3::Reset()
{
	position = defaultPosition;
	target = defaultTarget;
	up = defaultUp;
}