#include "cameracontroller.h"

CameraController::CameraController()
	: pCamera(NULL)
	, cOffset(-400.0f, -300.0f, 0.0f)
{
}

CameraController::~CameraController()
{
}

void CameraController::SetCamera(Camera *cam)
{
	pCamera = cam;
}

void CameraController::LookAt(const Vector3f &pos)
{
	pCamera->SetPosition(pos + cOffset);
}
