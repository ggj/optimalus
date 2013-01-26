#ifndef CAMERACONTROLLER_H
#define CAMERACONTROLLER_H

#include "../defines.h"

class CameraController
{
	public:
		CameraController();
		~CameraController();

		void SetCamera(Camera *cam);
		void LookAt(const Vector3f &pos);

	protected:
		Camera		*pCamera;
		Vector3f	cOffset;
};

#endif // CAMERACONTROLLER_H
