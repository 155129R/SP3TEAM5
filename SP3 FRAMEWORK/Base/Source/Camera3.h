#ifndef CAMERA_3_H
#define CAMERA_3_H

#include "Camera.h"

class Camera3 : public Camera
{
public:
	Vector3 defaultPosition;
	Vector3 defaultTarget;
	Vector3 defaultUp;

	bool Jump;
	bool Jumped;

	bool Tired = false;
	int RUN_SPEED;
	float Terrain;

	Camera3();
	~Camera3();
	virtual void Init(const Vector3& pos, const Vector3& target, const Vector3& up);
	virtual void Update(double dt);
	virtual void Reset();

	void Forward(double dt);
	void Backward(double dt);
	void Sideway(double dt, bool left);

	void Yaw(double dt);
	void Pitch(double dt);
};

#endif