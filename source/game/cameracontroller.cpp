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

//	Rect4f r(x, y, pScreen->GetWidth(), pScreen->GetHeight());

//	s32 halfScreenWidth = pScreen->GetWidth() / 2;
//	s32 halfScreenHeight = pScreen->GetHeight() / 2;

//	if(x < cArea.x1)
//		x = cArea.x1;

//	if(y < cArea.y1)
//		y = cArea.y1;

//	if(y > cArea.y2 - 32 - halfScreenHeight * 2)
//		y = cArea.y2 - 32 - halfScreenHeight * 2;

//	if(x > (cArea.x2 - 32 - halfScreenWidth * 2))
//		x = cArea.x2 - 32 - halfScreenWidth * 2;

	Vector3f np(static_cast<f32>(x), static_cast<f32>(y), z);

	pCamera->SetPosition(np);
}
