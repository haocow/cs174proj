#include "Angel.h"
#include "camera.h"

mat4 Camera::matrixCamera( void )
{
	return LookAt( vec4( camX, camY, camZ, 1.0 ),
				   vec4( 0,0,0,1.0 ),
				   vec4(normalize(vec4(0,1,0, 0.0))) );
}

void Camera::resetCamera( void )
{
	angleTheta = M_PI/90;
	anglePhi = 0;
}

void Camera::updateCameraPos( void )
{
	camX = radiusOfCS*sin(angleTheta)*cos(anglePhi);
	camY = radiusOfCS*sin(angleTheta)*sin(anglePhi);
	camZ = radiusOfCS*cos(angleTheta);
}

void Camera::autoRotateCam( void )
{
	//angleTheta += M_PI/75;
	angleTheta += M_PI/200;
	if (angleTheta > 2*M_PI)
		angleTheta -= 2*M_PI;
	updateCameraPos();
}