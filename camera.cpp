#include "Angel.h"
#include "camera.h"

float initX = 0;
float initY = 0;
float initZ = 10;

mat4 Camera::matrixCamera()
{
	return LookAt( vec4( camX, camY, camZ, 1.0 ),
				   vec4( camAtX, camAtY, camAtZ, 1.0 ),
				   vec4( 0.0, 1.0, 0.0, 0.0 ) );
}

void Camera::resetCamera()
{
	camX = initX;
	camY = initY;
	camZ = initZ;
	camAtX = 0;
	camAtY = 0;
	camAtZ = 0;
	viewAngle = 0;
}