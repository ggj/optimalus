#include "cameracontroller.h"

CameraController::CameraController()
	: pCamera(NULL)
	, cOffset(-400.0f, -300.0f, 0.0f)
	, cArea(-400.0f, -300.0f, 800.0f, 600.0f)
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

	Rect4f r(x, y, pScreen->GetWidth(), pScreen->GetHeight());
	Rect4f overlap;
	cArea.GetOverlappedRect(r, overlap);

	f32 h = pScreen->GetHeight() - overlap.Height();
	f32 w = pScreen->GetWidth() - overlap.Width();
	Vector3f np(static_cast<f32>(x) + w, static_cast<f32>(y) + h, z);

	pCamera->SetPosition(np);	
}
