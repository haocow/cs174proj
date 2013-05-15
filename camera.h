#ifndef __CAMERA_H__
#define __CAMERA_H__

class Camera
{
public:
	Camera()
    {
		resetCamera();
	}

	~Camera(){}

	mat4 matrixCamera( void );
	void resetCamera( void );

	// Variable Declarations
	float camX, camY, camZ,
		  camAtX, camAtY, camAtZ,
		  viewAngle;
};

#endif