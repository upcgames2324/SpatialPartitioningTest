#include "Application.h"
#include "ModuleCamera.h"
#include "ModuleInput.h"
#include "ModuleWindow.h"
#include "MathGeoLib.h"
#include <GL/glew.h>
#include <SDL.h>

ModuleCamera::ModuleCamera()
{
}

ModuleCamera::~ModuleCamera()
{
}

bool ModuleCamera::Init()
{
	frustum.type = FrustumType::PerspectiveFrustum;
	frustum.nearPlaneDistance = 0.1f;
	frustum.farPlaneDistance = 200.0f;
	// TODO
	//frustum.horizontalFov = math::DegToRad(90.0f);
	//ComputeVerticalFov();
	frustum.verticalFov = math::pi / 4.0f;
	frustum.horizontalFov = 2.f * atanf(tanf(frustum.verticalFov * 0.5f) * App->GetWindow()->GetAspectRatio());
	frustum.pos = float3(0.0f, 1.0f, 8.0f);
	frustum.front = -float3::unitZ;
	frustum.up = float3::unitY;

	this->orientation = float3::zero;
	this->looking = float3::zero;

	return true;
}

update_status ModuleCamera::Update()
{
	CheckInputs();

	return UPDATE_CONTINUE;
}

bool ModuleCamera::CleanUp()
{
	return true;
}

void ModuleCamera::SetFOV(float horizontalFov)
{
	frustum.horizontalFov = horizontalFov;
	ComputeVerticalFov();
}

void ModuleCamera::SetAspectRatio(float aspectRatio)
{
	aspectRatio = aspectRatio;
	ComputeVerticalFov();
}

void ModuleCamera::SetPlaneDistances(float distanceNear, float distanceFar)
{
	frustum.nearPlaneDistance = distanceNear;
	frustum.farPlaneDistance = distanceFar;
}

void ModuleCamera::SetPosition(float3 position)
{
	frustum.pos = position;
}

void ModuleCamera::SetOrientation(float3 orientation)
{
	this->orientation = orientation; // TODO
}

void ModuleCamera::LookAt(float3 looking)
{
	this->looking = looking; // TODO
}

void ModuleCamera::LookAt(float x, float y, float z)
{
	LookAt(float3(x, y, z));
}

float4x4 ModuleCamera::GetProjectionMatrix() const
{
	float4x4 projectionGL = frustum.ProjectionMatrix();
	//glMatrixMode(GL_PROJECTION); // TODO: check uniform assign
	//glLoadMatrixf(*projectionGL.Transposed().v);
	return projectionGL;
}

float4x4 ModuleCamera::GetViewMatrix() const
{
	float4x4 viewGL = float4x4(frustum.ViewMatrix());
	//glMatrixMode(GL_MODELVIEW);
	//glLoadMatrixf(*viewGL.Transposed().v);
	return viewGL;
}

void ModuleCamera::RotateAngle(const float3& axis, const float angle)
{
	float3x3 rotationDeltaMatrix = float3x3::RotateAxisAngle(axis, math::DegToRad(angle));
	float3 oldFront = frustum.front.Normalized();
	frustum.front = rotationDeltaMatrix.MulDir(oldFront);
	float3 oldUp = frustum.up.Normalized();
	frustum.up = rotationDeltaMatrix.MulDir(oldUp);
}

void ModuleCamera::ComputeVerticalFov()
{
	frustum.verticalFov = frustum.horizontalFov / aspectRatio;
}

void ModuleCamera::CheckInputs()
{
	float moveSpeed = 2.0f;
	float deltaTime = 1.0f / 60.0f; // TODO: get from SDL
	COORD mouseMovement = App->GetInput()->GetMouseMotion();
	float mouseSensibility = -0.5f;

	float3 position = frustum.pos;

	if (App->GetInput()->GetMouseButtonDown(SDL_BUTTON_RIGHT) == KeyState::KEY_REPEAT)
	{
		if (App->GetInput()->GetKey(SDL_SCANCODE_LSHIFT) == KeyState::KEY_REPEAT || App->GetInput()->GetKey(SDL_SCANCODE_RSHIFT) == KeyState::KEY_REPEAT)
		{
			moveSpeed *= 2;
		}

		if (App->GetInput()->GetKey(SDL_SCANCODE_W) == KeyState::KEY_REPEAT)
		{
			position += frustum.front * (moveSpeed * deltaTime);
		}
		if (App->GetInput()->GetKey(SDL_SCANCODE_S) == KeyState::KEY_REPEAT)
		{
			position -= frustum.front * (moveSpeed * deltaTime);
		}
		if (App->GetInput()->GetKey(SDL_SCANCODE_A) == KeyState::KEY_REPEAT)
		{
			position -= frustum.WorldRight() * (moveSpeed * deltaTime);
		}
		if (App->GetInput()->GetKey(SDL_SCANCODE_D) == KeyState::KEY_REPEAT)
		{
			position += frustum.WorldRight() * (moveSpeed * deltaTime);
		}
		if (App->GetInput()->GetKey(SDL_SCANCODE_Q) == KeyState::KEY_REPEAT)
		{
			position -= frustum.up * (moveSpeed * deltaTime);
		}
		if (App->GetInput()->GetKey(SDL_SCANCODE_E) == KeyState::KEY_REPEAT)
		{
			position += frustum.up * (moveSpeed * deltaTime);
		}
		frustum.pos = position;

		if (mouseMovement.X != 0)
		{
			RotateAngle(float3::unitY, mouseMovement.X * mouseSensibility);
		}
		if (mouseMovement.Y != 0)
		{
			RotateAngle(frustum.WorldRight(), mouseMovement.Y * mouseSensibility);
		}
	}
}
