#include "cameracontroller.h"

CameraController::CameraController()
	: pCamera(NULL)
	, cOffset(-400.0f, -300.0f, 0.0f)
	, cArea(0.0f, 0.0f, pScreen->GetWidth(), pScreen->GetHeight())
{
}

CameraController::~CameraController()
{
}

void CameraController::SetCamera(Camera *cam)
{
	pCamera = cam;
}

void CameraController::SetArea(const Rect4f &area)
{
	cArea = area;
}

void CameraController::LookAt(const Vector3f &pos)
{
	/*
	Texel/Raster bug - arrendodar a posicao da camera para pixel-perfect.
	*/
	Vector3f p = pos + cOffset;
	s32 x = static_cast<s32>(p.getX());
	s32 y = static_cast<s32>(p.getY());
	f32 z = p.getZ();

	pCamera->SetPosition(static_cast<f32>(x), static_cast<f32>(y), z);
}
