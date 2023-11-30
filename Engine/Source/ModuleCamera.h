#pragma once
#include "Module.h"
#include "Math/float3.h"

class ModuleCamera : public Module
{
public:
	ModuleCamera();
	~ModuleCamera();

	bool Init();

	void SetFOV(float horizontalFov);
	void SetAspectRatio(float aspectRatio);
	void SetPlaneDistances(float distanceNear, float distanceFar);
	void SetPosition(float3 position);
	void SetOrientation(float3 orientation);
	void LookAt(float3 looking);
	void LookAt(float x, float y, float z);
	void GetProjectionMatrix() const;
	void GetViewMatrix() const;

private:
	void ComputeVerticalFov();

private:
	float horizontalFov;
	float verticalFov;
	float aspectRatio;
	float distanceNear;
	float distanceFar;
	float3 position;
	float3 orientation;
	float3 looking;

	float3 front;
	float3 up;
};
