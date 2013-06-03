#ifndef __CAMERA_H__
#define __CAMERA_H__

const float radiusOfCS = 130;

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

	// Variable Declarations
	float camX, camY, camZ,
		  angleTheta, anglePhi;
};

#endif