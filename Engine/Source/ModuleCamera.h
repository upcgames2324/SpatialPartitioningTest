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

	void SetFOV(const float horizontalFov);
	void SetAspectRatio(const float aspectRatio);
	void SetPlaneDistances(const float distanceNear, const float distanceFar);
	void SetPosition(const float3& position);
	void LookAt(const float3& looking);
	void LookAt(const float x, const float y, const float z);
	const float4x4 GetProjectionMatrix() const;
	const float4x4 GetViewMatrix() const;
	const Frustum GetFrustum() const;


	void RotateAngle(const float3& axis, const float angle);

private:
	void ComputeVerticalFov();
	void CheckInputs();

private:
	Frustum frustum;
	float aspectRatio;
};
