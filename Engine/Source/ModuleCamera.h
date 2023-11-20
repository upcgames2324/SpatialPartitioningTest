#pragma once
#include "Module.h"
#include "MathGeoLib.h"

class ModuleCamera : public Module
{
public:
	ModuleCamera();
	~ModuleCamera();

	bool Init();

	void SetFOV(float horizontalFov);
	void SetAspectRatio(float aspectRatio);
	void SetPlaneDistances(float2 planeDistances);
	void SetPosition(float3 position);
	void SetOrientation(float3 orientation);
	void LookAt(float x, float y, float z);
	void LookAt(float3 looking);
	void GetProjectionMatrix() const;
	void GetViewMatrix() const;

private:
	float horizontalFov;
	float aspectRatio;
	float2 planeDistances;
	float3 position;
	float3 orientation;
	float3 looking;

	float3 front;
	float3 up;
};
