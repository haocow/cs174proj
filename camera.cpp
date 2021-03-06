#include "Angel.h"
#include "camera.h"

float radiusOfCS = 130;

mat4 Camera::matrixCamera( void )
{
	return LookAt( vec4( camX, camY, camZ, 1.0 ),
				   vec4( 0,0,0,1 ),
				   vec4( 0,1,0,1 ));
}

void Camera::resetCamera( void )
{
	angleTheta = M_PI/90;
	anglePhi = 0;
	radiusOfCS = 130;
}

void Camera::updateCameraPos( void )
{
	camX = radiusOfCS*sin(angleTheta)*cos(anglePhi);
	camY = radiusOfCS*sin(angleTheta)*sin(anglePhi);
	camZ = radiusOfCS*cos(angleTheta);
}

void Camera::autoRotateCam( void )
{
	angleTheta += M_PI/200;
	if (angleTheta > 2*M_PI)
		angleTheta -= 2*M_PI;
	updateCameraPos();
}

void Camera::zoomIn( void )
{
	radiusOfCS -= zoomDelta;
	if (radiusOfCS < zoomThreshhold)
		radiusOfCS += zoomDelta;
}

void Camera::zoomOut( void )
{
	radiusOfCS += zoomDelta;
}