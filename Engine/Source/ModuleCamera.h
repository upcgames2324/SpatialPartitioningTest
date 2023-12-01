#pragma once
#include "Module.h"
#include "Math/float3.h"
#include "Geometry/Frustum.h"

class ModuleCamera : public Module
{
public:
	ModuleCamera();
	~ModuleCamera();

	bool Init();
	update_status Update();
	bool CleanUp();

	void SetFOV(float horizontalFov);
	void SetAspectRatio(float aspectRatio);
	void SetPlaneDistances(float distanceNear, float distanceFar);
	void SetPosition(float3 position);
	void SetOrientation(float3 orientation);
	void LookAt(float3 looking);
	void LookAt(float x, float y, float z);
	void GetProjectionMatrix() const;
	void GetViewMatrix() const;

	void RotateAngle(const float3& axis, const float angle);

private:
	void ComputeVerticalFov();
	void CheckInputs();

private:
	Frustum frustum;
	float3 orientation;
	float3 looking;
	float aspectRatio;
};
