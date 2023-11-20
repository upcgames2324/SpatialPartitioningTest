#include "ModuleCamera.h"
#include <GL/glew.h>

ModuleCamera::ModuleCamera()
{
}

ModuleCamera::~ModuleCamera()
{
}

bool ModuleCamera::Init()
{
	this->position = float3(0.0f, 1.0f, -2.0f);
	this->planeDistances = float2(0.1f, 200.0f);
	//this->horizontalFov = DEGTORAD * 90.0f;
	//this->aspectRatio = 1.3f;

	this->front = float3::unitZ;
	this->up = float3::unitY;
	return true;
}

void ModuleCamera::SetFOV(float horizontalFov)
{
	this->horizontalFov = horizontalFov;
}

void ModuleCamera::SetAspectRatio(float aspectRatio)
{
	this->aspectRatio = aspectRatio;
}

void ModuleCamera::SetPlaneDistances(float2 planeDistances)
{
	this->planeDistances = planeDistances;
}

void ModuleCamera::SetPosition(float3 position)
{
	this->position = position;
}

void ModuleCamera::SetOrientation(float3 orientation)
{
	this->orientation = orientation;
}

void ModuleCamera::LookAt(float x, float y, float z)
{
	this->looking = float3(x, y, z);
}

void ModuleCamera::LookAt(float3 looking)
{
	this->looking = looking;
}

void ModuleCamera::GetProjectionMatrix() const
{
	Frustum frustum;
	//frustum.SetKind(FrustumSpaceGL, FrustumRightHanded);
	frustum.type = FrustumType::PerspectiveFrustum;
	frustum.nearPlaneDistance = this->planeDistances.x;
	frustum.farPlaneDistance = this->planeDistances.y;
	//frustum.horizontalFov = this->horizontalFov;
	//frustum.aspectRatio = this->aspectRatio;
	frustum.pos = position;
	frustum.front = front;
	frustum.up = up;
	float4x4 projectionGL = frustum.ProjectionMatrix().Transposed(); // < --Important to transpose!
	//Send the frustum projection matrix to OpenGL
	// direct mode would be:
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(*projectionGL.v);
}

void ModuleCamera::GetViewMatrix() const
{
	Frustum frustum;
	frustum.pos = position;
	float3x3 rotationMatrix; // TODO: some rotation value (or LookAt matrix)
	frustum.front = rotationMatrix.WorldX();
	frustum.up = rotationMatrix.WorldY();

	// Handling rotations
	float3x3 rotationDeltaMatrix; // TODO: some rotation delta value
	float3 oldFront = frustum.front.Normalized();
	frustum.front = rotationDeltaMatrix.MulDir(oldFront);
	float3 oldUp = frustum.up.Normalized();
	frustum.up = rotationDeltaMatrix.MulDir(oldUp);

	//Send the frustum view matrix to OpenGL
	// direct mode would be:
	float4x4 viewGL = float4x4(frustum.ViewMatrix()).Transposed();
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(*viewGL.v);
}
