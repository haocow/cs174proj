#ifndef __CAMERA_H__
#define __CAMERA_H__

const float zoomDelta = 2;
const float zoomThreshhold = 40;

class Camera
{
public:
	Camera()
    {
		resetCamera();
		updateCameraPos();
	}

	~Camera(){}

	mat4 matrixCamera( void );
	void resetCamera( void );
	void updateCameraPos( void );
	void autoRotateCam( void );
	void zoomIn( void );
	void zoomOut( void );

	// Variable Declarations
	float camX, camY, camZ,
		  angleTheta, anglePhi;
};

#endif