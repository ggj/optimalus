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
	pCamera->SetPosition(pos + cOffset);
}
