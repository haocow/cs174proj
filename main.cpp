// ------------------------
// GLUT harness v. 1.0
// ------------------------

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include "Angel.h"
#include "MyFunctions.h"
#include "sphere.h"
#include "camera.h"
#include "lighting.h"

// Variables for window
int winHeight = 800;
int winWidth = 1066;

// Variables for shaders
GLint vPosition, vNormal,
	  vCamera, vPerspective;

Sphere sphere;
Camera camera;

void myInit( void )
{
	sphere.generateSphere(5);
    // Create a vertex array object
    GLuint vao;
    glGenVertexArrays( 1, &vao );
    glBindVertexArray( vao );

    // Create and initialize a buffer object
	GLuint buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(sphere.points) + sizeof(sphere.normals), NULL, GL_STATIC_DRAW);

	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(sphere.points), sphere.points);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(sphere.points), sizeof(sphere.normals), sphere.normals);

    // Load shaders and use the resulting shader program
    GLuint program = InitShader("vshader.glsl", "fshader.glsl");
    glUseProgram(program);

	vPosition = glGetAttribLocation(program, "vPosition");
	glEnableVertexAttribArray( vPosition );
	glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	vNormal = glGetAttribLocation( program, "vNormal" ); 
    glEnableVertexAttribArray( vNormal );
    glVertexAttribPointer( vNormal, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(sphere.points)) );

	vPerspective = glGetUniformLocation( program, "PerspectiveMatrix" );
	glUniformMatrix4fv(vPerspective, 1, false, Perspective(60, winWidth/winHeight, .1, 1000));

	vCamera = glGetUniformLocation( program, "CameraMatrix" );
	glUniformMatrix4fv( vCamera, 1, false, camera.matrixCamera() );

	//**************************
	//* SCALE VARIABLES ********
	//**************************
	ScaleMat = glGetUniformLocation( program, "ScaleMatrix" );
	setScale(5);

   	//********************************
	//* LIGHT/COLOR INITIALIZE *******
	//********************************
	fMaterialAmbient = glGetUniformLocation(program, "AmbientProduct");
	fMaterialDiffuse = glGetUniformLocation(program, "DiffuseProduct");
	fMaterialSpecular = glGetUniformLocation(program, "SpecularProduct");
	
    glUniform4fv( glGetUniformLocation(program, "lightPosition"),
		  1, light_position );

    glUniform1f( glGetUniformLocation(program, "Shininess"),
		 material_shininess );

   	//********************************
	//* BACKGROUND INITIALIZE ********
	//********************************
	glClearColor( 0.0, 0.0, 0.0, 1.0 );		// Black Background
}

void draw_sphere()
{
	glUniform4fv( fMaterialAmbient, 1, ambient_product );
    glUniform4fv( fMaterialDiffuse, 1, diffuse_product );
    glUniform4fv( fMaterialSpecular, 1, specular_product );

	glDrawArrays( GL_TRIANGLES , 0, numVertices );
}

//************************
//* GLUT HARNESS CODE ****
//************************

void initGlut(int& argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitContextVersion( 3, 2 );
    glutInitContextProfile( GLUT_CORE_PROFILE );
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 50);
	glutInitWindowSize(1066, 800);
	glutCreateWindow("TermProject");

	glewExperimental = GL_TRUE;
	glewInit();

	// Enable Depth
	glEnable( GL_DEPTH_TEST );

	// Stores the points for the original cube
	myInit();
}

// Called when the window needs to be redrawn.
// TODO: Don't forget to add glutPostRedisplay().
void callbackDisplay()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Update Camera
	glUniformMatrix4fv( vCamera, 1, false, camera.matrixCamera() );

	// Draw the main sphere
	setColor( 0.5, 0.8, 0.4, 1.0 , 1 );
	draw_sphere();

	glutWarpPointer(winWidth / 2, winHeight / 2);

	glutSwapBuffers();
}

// Called when the window is resized.
void callbackReshape(int width, int height)
{

}

// Called when a key is pressed. x, y is the current mouse position.
void callbackKeyboard(unsigned char key, int x, int y)
{
	switch ( key ) {
		case 27:
		case 'q': case 'Q':
			exit(EXIT_SUCCESS);
			break;
		case 'w':
			camera.camZ -= 1 * cos(camera.viewAngle);
			camera.camX += 1 * sin(camera.viewAngle);
			camera.camAtZ -= 1 * cos(camera.viewAngle);
			camera.camAtX += 1 * sin(camera.viewAngle);
			camera.camY -= 1 * sin(camera.viewAngleY);
			camera.camAtY -= 1 * sin(camera.viewAngleY);
			break;
		case 'a':
			camera.camX -= 1 * cos(camera.viewAngle);
			camera.camZ -= 1 * sin(camera.viewAngle);
			camera.camAtX -= 1 * cos(camera.viewAngle);
			camera.camAtZ -= 1 * sin(camera.viewAngle);
			break;
		case 's':
			camera.camZ += 1 * cos(camera.viewAngle);
			camera.camX -= 1 * sin(camera.viewAngle);
			camera.camAtZ += 1 * cos(camera.viewAngle);
			camera.camAtX -= 1 * sin(camera.viewAngle);
			camera.camY += 1 * sin(camera.viewAngleY);
			camera.camAtY += 1 * sin(camera.viewAngleY);
			break;
		case 'd':
			camera.camX += 1 * cos(camera.viewAngle);
			camera.camZ += 1 * sin(camera.viewAngle);
			camera.camAtX += 1 * cos(camera.viewAngle);
			camera.camAtZ += 1 * sin(camera.viewAngle);
			break;
		case ' ':
			camera.resetCamera();
			break;
	}

	glutPostRedisplay();
}

// Called when a special button is pressed
void callbackSpecial(int key, int x, int y)
{
	switch ( key ) {
		case GLUT_KEY_UP:
			camera.camY += 1;
			camera.camAtY += 1;
			break;
		case GLUT_KEY_DOWN:
			camera.camY -= 1;
			camera.camAtY -= 1;
			break;
	}

	glutPostRedisplay();
}

// Called when a mouse button is pressed or released
void callbackMouse(int button, int state, int x, int y)
{

}

// Called when the mouse is moved with a button pressed
void callbackMotion(int x, int y)
{

}

// Called when the mouse is moved with no buttons pressed
void callbackPassiveMotion(int x, int y)
{
	float delta = M_PI/90;

	if (x < winWidth/2)
	{
		camera.viewAngle -= delta;
		camera.camAtX = camera.camX + sin(camera.viewAngle);
		camera.camAtZ = camera.camZ - cos(camera.viewAngle);
	}
	else if (x > winWidth/2)
	{
		camera.viewAngle += delta;
		camera.camAtX = camera.camX + sin(camera.viewAngle);
		camera.camAtZ = camera.camZ - cos(camera.viewAngle);
	}

	if (y < winHeight/2)
	{
		camera.viewAngleY -= delta;
		camera.camAtY = camera.camY - sin(camera.viewAngleY);
	}
	else if (y > winHeight/2)
	{
		camera.viewAngleY += delta;
		camera.camAtY = camera.camY - sin(camera.viewAngleY);
	}
}

// Called when the system is idle. Can be called many times per frame.
void callbackIdle()
{
	glutPostRedisplay();
}

// Called when the timer expires
void callbackTimer(int)
{
	glutTimerFunc(1000/30, callbackTimer, 0);
	glutPostRedisplay();
}

void initCallbacks()
{
	glutDisplayFunc(callbackDisplay);
	glutReshapeFunc(callbackReshape);
	glutSpecialFunc(callbackSpecial);
	glutKeyboardFunc(callbackKeyboard);
	glutMouseFunc(callbackMouse);
	glutMotionFunc(callbackMotion);
	glutPassiveMotionFunc(callbackPassiveMotion);
	glutIdleFunc(callbackIdle);
	glutTimerFunc(1000/30, callbackTimer, 0);
}

int main(int argc, char** argv)
{
	initGlut(argc, argv);
	initCallbacks();

	glutMainLoop();
	return 0;
}
