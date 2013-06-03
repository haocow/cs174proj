/*
 CS 174A Final Project
 Jonathan Nguy, Jonathan Hao, Stephanie
 
 Music Visualizer
 
 */

// Music stuff

#include "fmod.hpp" //fmod c++ header
#include "fmod_errors.h"
#include <algorithm>

#ifdef __APPLE__
	#include <sys/time.h>
#endif

FMOD::System            *fmodSystem;
FMOD::Sound             *song;
FMOD::Channel           *channel = 0;
FMOD::ChannelGroup      *channelgroup;
FMOD::DSP               *dsp = 0, *dsphead;
FMOD::DSPConnection     *dspconnection = 0;
FMOD_RESULT             result;
unsigned int            version;
void                    *extradriverdata = 0;
bool musicPaused = true;

// getSpectrum() performs the frequency analysis, see explanation below
int sampleSize = 256;

float *specLeft, *specRight, *spec;
char *valueString;

// Beat threshold
float beatThresholdVolumeSmall = 0.3f;
float beatThresholdVolumeMedium = 0.6f;
float beatThresholdVolumeLarge = 0.9f;
int beatThresholdBar = 1;            // The bar in the volume distribution to examine
unsigned int beatPostIgnore = 250;   // Number of ms to ignore track for after a beat is recognized
int beatLastTick = 0;                // Time when last beat occurred

// 2nd threshold
float secondThresholdVolumeSmall = 0.03f;
float secondThresholdVolumeMedium = 0.08f;
float secondThresholdVolumeLarge = 0.10f;
int secondThresholdBar = 9;            // The bar in the volume distribution to examine
unsigned int secondPostIgnore = 250;   // Number of ms to ignore track for after a beat is recognized
int secondLastTick = 0;


// 3rd threshold
float thirdThresholdVolumeSmall = 0.01f;
float thirdThresholdVolumeMedium = 0.05f;
float thirdThresholdVolumeLarge = 0.10f;
int thirdThresholdBar = 13;            // The bar in the volume distribution to examine
unsigned int thirdPostIgnore = 250;   // Number of ms to ignore track for after a beat is recognized
int thirdLastTick = 0;



#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#ifdef __APPLE__ 
#include <GL/glew.h>
#endif
#include "Angel.h"
#include "MyFunctions.h"
#include "sphere.h"
#include "camera.h"
#include "lighting.h"
#include "texture.h"

// Variables for window
int winHeight = 900;
int winWidth = 900;

// Variable for program
GLuint program;

// Variables for shaders
GLuint vPosition, vNormal,
	   vCamera, vPerspective,
	   sphereID;

TgaImage stars;

Sphere sphere;
Camera camera;

GLuint textureBackground, uTex;

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
	glBufferData(GL_ARRAY_BUFFER, sizeof(sphere.points) + sizeof(sphere.normals) + sizeof(sphere.tex_coords), NULL, GL_STATIC_DRAW);

	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(sphere.points), sphere.points);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(sphere.points), sizeof(sphere.normals), sphere.normals);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(sphere.points) + sizeof(sphere.normals), sizeof(sphere.tex_coords), sphere.tex_coords);

    // Load shaders and use the resulting shader program
    program = InitShader("vshader.glsl", "fshader.glsl");
    glUseProgram(program);

	vPosition = glGetAttribLocation(program, "vPosition");
	glEnableVertexAttribArray( vPosition );
	glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	vNormal = glGetAttribLocation( program, "vNormal" ); 
    glEnableVertexAttribArray( vNormal );
    glVertexAttribPointer( vNormal, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(sphere.points)) );

	vPerspective = glGetUniformLocation( program, "PerspectiveMatrix" );
	glUniformMatrix4fv(vPerspective, 1, false, Perspective(60, winWidth/winHeight, .1, 4000));

	vCamera = glGetUniformLocation( program, "CameraMatrix" );
	glUniformMatrix4fv( vCamera, 1, false, camera.matrixCamera() );

	sphereID = glGetUniformLocation( program, "fsphereID");

	//**************************
	//* SCALE VARIABLES ********
	//**************************
	ScaleMat = glGetUniformLocation( program, "ScaleMatrix" );
	setScale(5);

	//**************************
	//* POSITION VARIABLES *****
	//**************************
	//Set variable pointers
	TransMat = glGetUniformLocation( program, "TranslationMatrix" );

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

	//**************************
	//* TEXTURE VARIABLES ******
	//**************************
//	texInit();
#ifdef __APPLE__
    glGenTextures( 1, &textureBackground );
    glBindTexture( GL_TEXTURE_2D, textureBackground );
    glTexImage2D(GL_TEXTURE_2D, 0, 4, stars.width, stars.height, 0,
                 (stars.byteCount == 3) ? GL_BGR : GL_BGRA,
                 GL_UNSIGNED_BYTE, stars.data );
    
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    
    glUniform1i( uTex, 0);
    glUniform1i( glGetUniformLocation( program, "texMap" ), 0);
    
	GLuint vTexCoord = glGetAttribLocation( program, "vTexCoords" );
    glEnableVertexAttribArray( vTexCoord );
    glVertexAttribPointer( vTexCoord, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(sphere.points) + sizeof(sphere.normals)) );
#else
	if (!stars.loadTGA("stars.tga")){
        printf("Couldn't load tga file");
    }

	texInit();

	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );

	glUniform1i( glGetUniformLocation( program, "texMap" ), 0);

	glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, stars.width, stars.height, 0, GL_RGB, GL_UNSIGNED_BYTE, stars.data );
	glGenerateMipmap( GL_TEXTURE_2D );

	GLuint vTexCoord = glGetAttribLocation( program, "vTexCoords" );
			glEnableVertexAttribArray( vTexCoord );
            glVertexAttribPointer( vTexCoord, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(sphere.points) + sizeof(sphere.normals)) );
#endif 	

	//********************************
	//* BACKGROUND INITIALIZE ********
	//********************************
	glClearColor( 0.0, 0.0, 0.0, 1.0 );		// Black Background
    
	//********************************
	//* MUSIC INITIALIZE *************
	//********************************
    result = FMOD::System_Create(&fmodSystem);
    
    result = fmodSystem->getVersion(&version);
    result = fmodSystem->init(32, FMOD_INIT_NORMAL, extradriverdata);
    result = fmodSystem->createSound("brave.mp3", FMOD_SOFTWARE | FMOD_LOOP_NORMAL , 0, &song);
    
    result = fmodSystem->playSound(FMOD_CHANNEL_FREE, song, true, &channel);
    //    result = fmodSystem->playSound(song, 0, true, &channel);
    fmodSystem->update(); // Per-frame FMOD update ('system' is a pointer to FMOD::System)
    
    
    // For spectrum analyzer
    specLeft = new float[sampleSize];
    specRight = new float[sampleSize];
    spec = new float[sampleSize];
    
    for (int i = 0; i<sampleSize; i++){
        specLeft[i] = 0.0;
        specRight[i] = 0.0;
        spec[i] = 0.0;
    }
}

void spawn_sphere()
{
	glDrawArrays( GL_TRIANGLES, 0, numVertices );
}

void draw_sphere()
{
	// 0 = background
	// 1 = central sphere
    
	// Background
	glUniform1i( sphereID, 0 );
	setScale( 1000 );
    setColor( 0, 0, 0, 1, 1 );
	setTranslation( 0, 0, 0 );
	spawn_sphere();
    
	// Origin Sphere
	glUniform1i( sphereID, 1 );
	setColor( colorArray[0].x, colorArray[0].y, colorArray[0].z, 1.0, 1.0 );
	setScale( scaleVal[0] );
	setTranslation( 0, 0, 0 );
	// Lighting
	glUniform4fv( fMaterialAmbient, 1, ambient_product );
    glUniform4fv( fMaterialDiffuse, 1, diffuse_product );
    glUniform4fv( fMaterialSpecular, 1, specular_product );
	spawn_sphere();
    
	// Neg-Neg-Neg
	glUniform1i( sphereID, 1 );
	setColor( colorArray[1].x, colorArray[1].y, colorArray[1].z, 1.0, 1.0 );
	setScale( 0.6*scaleVal[1] );
	setTranslation( -21, -39, -26 );
	// Lighting
	glUniform4fv( fMaterialAmbient, 1, ambient_product );
    glUniform4fv( fMaterialDiffuse, 1, diffuse_product );
    glUniform4fv( fMaterialSpecular, 1, specular_product );
	spawn_sphere();
    
	// Neg-Neg-Pos
	glUniform1i( sphereID, 1 );
	setColor( colorArray[1].x, colorArray[1].y, colorArray[1].z, 1.0, 1.0 );
	setScale( 1.3*scaleVal[1] );
	setTranslation( -31, -36, +30 );
	// Lighting
	glUniform4fv( fMaterialAmbient, 1, ambient_product );
    glUniform4fv( fMaterialDiffuse, 1, diffuse_product );
    glUniform4fv( fMaterialSpecular, 1, specular_product );
	spawn_sphere();
    
    // Neg-Pos-Neg
	glUniform1i( sphereID, 1 );
	setColor( colorArray[0].x, colorArray[0].y, colorArray[0].z, 1.0, 1.0 );
	setScale ( 1.2*scaleVal[0] );
	setTranslation( -14, 37, -41 );
	// Lighting
	glUniform4fv( fMaterialAmbient, 1, ambient_product );
    glUniform4fv( fMaterialDiffuse, 1, diffuse_product );
    glUniform4fv( fMaterialSpecular, 1, specular_product );
	spawn_sphere();
    
    // Neg-Pos-Pos
	glUniform1i( sphereID, 1 );
	setColor( colorArray[0].x, colorArray[0].y, colorArray[0].z, 1.0, 1.0 );
	setScale ( 0.9*scaleVal[0] );
	setTranslation( -30, 22, 38 );
	// Lighting
	glUniform4fv( fMaterialAmbient, 1, ambient_product );
    glUniform4fv( fMaterialDiffuse, 1, diffuse_product );
    glUniform4fv( fMaterialSpecular, 1, specular_product );
	spawn_sphere();
    
	// Pos-Neg-Neg
	glUniform1i( sphereID, 1 );
	setColor( colorArray[2].x, colorArray[2].y, colorArray[2].z, 1.0, 1.0 );
	setScale( 1.1*scaleVal[2] );
	setTranslation( 30, -30, -30 );
	// Lighting
	glUniform4fv( fMaterialAmbient, 1, ambient_product );
    glUniform4fv( fMaterialDiffuse, 1, diffuse_product );
    glUniform4fv( fMaterialSpecular, 1, specular_product );
	spawn_sphere();
    
	// Pos-Neg-Pos
	glUniform1i( sphereID, 1 );
	setColor( colorArray[2].x, colorArray[2].y, colorArray[2].z, 1.0, 1.0 );
	setScale( 0.7*scaleVal[0] );
	setTranslation( 31, -20, +37 );
	// Lighting
	glUniform4fv( fMaterialAmbient, 1, ambient_product );
    glUniform4fv( fMaterialDiffuse, 1, diffuse_product );
    glUniform4fv( fMaterialSpecular, 1, specular_product );
	spawn_sphere();
    
    // Pos-Pos-Neg
	glUniform1i( sphereID, 1 );
	setColor( colorArray[2].x, colorArray[2].y, colorArray[2].z, 1.0, 1.0 );
	setScale ( 0.9*scaleVal[2] );
	setTranslation( 36, 30, -35 );
	// Lighting
	glUniform4fv( fMaterialAmbient, 1, ambient_product );
    glUniform4fv( fMaterialDiffuse, 1, diffuse_product );
    glUniform4fv( fMaterialSpecular, 1, specular_product );
	spawn_sphere();
    
    // Pos-Pos-Pos
	glUniform1i( sphereID, 1 );
	setColor( colorArray[1].x, colorArray[1].y, colorArray[1].z, 1.0, 1.0 );
	setScale ( scaleVal[1] );
	setTranslation( 40, 24, 40 );
	// Lighting
	glUniform4fv( fMaterialAmbient, 1, ambient_product );
    glUniform4fv( fMaterialDiffuse, 1, diffuse_product );
    glUniform4fv( fMaterialSpecular, 1, specular_product );
	spawn_sphere();

}

//************************
//* GLUT HARNESS CODE ****
//************************

// Called when the window needs to be redrawn.
// TODO: Don't forget to add glutPostRedisplay().
void callbackDisplay()
{
    // Per-frame FMOD update ('system' is a pointer to FMOD::System)
    fmodSystem->update();
    //        std::transform(&spec[0], &spec[sampleSize], &spec[0], normalize);
    //[maxVol] (float dB) -> float { return dB / maxVol; });
    //    if (!musicPaused)
    //        printf("%f \n", maxVol);
    
    // Get spectrum for left and right stereo channels
    channel->getSpectrum(specLeft, sampleSize, 0, FMOD_DSP_FFT_WINDOW_RECT);
    channel->getSpectrum(specRight, sampleSize, 1, FMOD_DSP_FFT_WINDOW_RECT);
    
    // Find max volume
    auto maxIterator = std::max_element(&spec[0], &spec[sampleSize]);
    float maxVol = *maxIterator;
    
    // Normalize
    if (maxVol != 0)
        for (int i = 0; i < sampleSize; i++)
            spec[i] = spec[i]/maxVol;
    //
    //    if (!musicPaused)
    //        printf("Max vol: %f \n", maxVol);
    
    if (!musicPaused) {
        int location = 9;
//		if(spec[location] > .1)
//			printf("%f \n", spec[location]);
        
        bool beatDetectedSmall = false;
        bool beatDetectedMedium = false;
        bool beatDetectedLarge = false;
        
        timeval t;
        
        // Test for threshold volume being exceeded (if not currently ignoring track)
        if ((spec[beatThresholdBar] >= beatThresholdVolumeLarge ||
             spec[beatThresholdBar+1] >= beatThresholdVolumeLarge) && beatLastTick == 0)
        {
            beatLastTick = gettimeofday(&t,NULL);
            beatDetectedLarge = true;
        } else if ((spec[beatThresholdBar] >= beatThresholdVolumeMedium ||
                    spec[beatThresholdBar+1] >= beatThresholdVolumeMedium) && beatLastTick == 0) {
            beatLastTick = gettimeofday(&t,NULL);
            beatDetectedMedium = true;
        } else if ((spec[beatThresholdBar] >= beatThresholdVolumeSmall ||
                    spec[beatThresholdBar+1] >= beatThresholdVolumeSmall) && beatLastTick == 0) {
            beatLastTick = gettimeofday(&t,NULL);
            beatDetectedSmall = true;
        }
        
        if (beatDetectedLarge) {
            if (spec[beatThresholdBar] >= beatThresholdVolumeLarge)
                scaleVal[0] += spec[beatThresholdBar];
            else
                scaleVal[0] += spec[beatThresholdBar+1];
        } else if (beatDetectedMedium) {
            if (spec[beatThresholdBar] >= beatThresholdVolumeMedium)
                scaleVal[0] += spec[beatThresholdBar]/5;
            else
                scaleVal[0] += spec[beatThresholdBar+1]/5;
        } else if (beatDetectedSmall) {
            if (spec[beatThresholdBar] >= beatThresholdVolumeMedium)
                scaleVal[0] += spec[beatThresholdBar]/10;
            else
                scaleVal[0] += spec[beatThresholdBar+1]/10;
        }
        
        // Test for threshold volume being exceeded (if not currently ignoring track)
        if ((spec[secondThresholdBar] >= secondThresholdVolumeLarge ||
             spec[secondThresholdBar+1] >= secondThresholdVolumeLarge) && secondLastTick == 0)
        {
            secondLastTick = gettimeofday(&t,NULL);
            if (spec[secondThresholdBar] >= secondThresholdVolumeLarge)
                scaleVal[1] += spec[secondThresholdBar];
            else
                scaleVal[1] += spec[secondThresholdBar+1];
        } else if ((spec[secondThresholdBar] >= secondThresholdVolumeMedium ||
                    spec[secondThresholdBar+1] >= secondThresholdVolumeMedium) && secondLastTick == 0) {
            secondLastTick = gettimeofday(&t,NULL);
            if (spec[secondThresholdBar] >= secondThresholdVolumeMedium)
                scaleVal[1] += spec[secondThresholdBar];
            else
                scaleVal[1] += spec[secondThresholdBar+1];
        } else if ((spec[secondThresholdBar] >= secondThresholdVolumeSmall ||
                    spec[secondThresholdBar+1] >= secondThresholdVolumeSmall) && secondLastTick == 0) {
            secondLastTick = gettimeofday(&t,NULL);
            if (spec[secondThresholdBar] >= secondThresholdVolumeSmall)
                scaleVal[1] += spec[secondThresholdBar];
            else
                scaleVal[1] += spec[secondThresholdBar+1];
        }
        
        // Test for threshold volume being exceeded (if not currently ignoring track)
        if ((spec[thirdThresholdBar] >= thirdThresholdVolumeLarge ||
             spec[thirdThresholdBar+1] >= thirdThresholdVolumeLarge) && thirdLastTick == 0)
        {
            thirdLastTick = gettimeofday(&t,NULL);
            if (spec[thirdThresholdBar] >= thirdThresholdVolumeLarge)
                scaleVal[2] += spec[thirdThresholdBar];
            else
                scaleVal[2] += spec[thirdThresholdBar+1];
        } else if ((spec[thirdThresholdBar] >= thirdThresholdVolumeMedium ||
                    spec[thirdThresholdBar+1] >= thirdThresholdVolumeMedium) && thirdLastTick == 0) {
            thirdLastTick = gettimeofday(&t,NULL);
            if (spec[thirdThresholdBar] >= thirdThresholdVolumeMedium)
                scaleVal[2] += spec[thirdThresholdBar];
            else
                scaleVal[2] += spec[thirdThresholdBar+1];
        } else if ((spec[thirdThresholdBar] >= thirdThresholdVolumeSmall ||
                    spec[thirdThresholdBar+1] >= thirdThresholdVolumeSmall) && thirdLastTick == 0) {
            thirdLastTick = gettimeofday(&t,NULL);
            if (spec[thirdThresholdBar] >= thirdThresholdVolumeSmall)
                scaleVal[2] += spec[thirdThresholdBar];
            else
                scaleVal[2] += spec[thirdThresholdBar+1];
        }
        
        // If the ignore time has expired, allow testing for a beat again
        if (gettimeofday(&t,NULL) - beatLastTick >= beatPostIgnore)
            beatLastTick = 0;
        if (gettimeofday(&t,NULL) - secondLastTick >= secondPostIgnore)
            secondLastTick = 0;
        if (gettimeofday(&t,NULL) - thirdLastTick >= thirdPostIgnore)
            thirdLastTick = 0;
	}

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Update Camera
	glUniformMatrix4fv( vCamera, 1, false, camera.matrixCamera() );

	// Draw the main sphere
	setColor( 1, 1, 1, 1.0 , 1 );
	draw_sphere();
    

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
			camera.anglePhi += M_PI/90;
			if (camera.anglePhi > 2*M_PI)
				camera.anglePhi -= 2*M_PI;
			camera.updateCameraPos();
			break;
		case 'a':
			camera.angleTheta -= M_PI/90;
			if (camera.angleTheta < 0)
				camera.angleTheta += 2*M_PI;
			camera.updateCameraPos();
			break;
		case 's':
			camera.anglePhi -= M_PI/90;
			if (camera.anglePhi < 0)
				camera.anglePhi += 2*M_PI;
			camera.updateCameraPos();
			break;
		case 'd':
			camera.angleTheta += M_PI/90;
			if (camera.angleTheta > 2*M_PI)
				camera.angleTheta -= 2*M_PI;
			camera.updateCameraPos();
			break;       
		case 'p':
            // Play or pause the sound
            if (!musicPaused){
                result = channel->setPaused(musicPaused = true);
            } else {
                result = channel->setPaused(musicPaused = false);
            }
            break;
		case ' ':
			camera.resetCamera();
			camera.updateCameraPos();
			break;
	}

	glutPostRedisplay();
}

// Called when a special button is pressed
void callbackSpecial(int key, int x, int y)
{
	switch ( key ) {
	case GLUT_KEY_LEFT:
		break;
	case GLUT_KEY_RIGHT:
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
	/*
	float delta = M_PI/90;

	if (x < winWidth/2)
	{
		camera.angleTheta -= delta;
		camera.camAtX = camera.camX + sin(camera.angleTheta);
		camera.camAtZ = camera.camZ - cos(camera.angleTheta);
	}
	else if (x > winWidth/2)
	{
		camera.angleTheta += delta;
		camera.camAtX = camera.camX + sin(camera.angleTheta);
		camera.camAtZ = camera.camZ - cos(camera.angleTheta);
	}

	if (y < winHeight/2)
	{
		camera.anglePhi -= delta;
		camera.camAtY = camera.camY - sin(camera.anglePhi);
	}
	else if (y > winHeight/2)
	{
		camera.anglePhi += delta;
		camera.camAtY = camera.camY - sin(camera.anglePhi);
	}
	*/
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
    
	// Automatic rescaling
//	if ( scaleInc ) {
//		scaleVal += 0.1; }
//	else {
//		scaleVal -= 0.3;
//}
    
    //1st is for the beats
    
    for (int i = 0; i < 3; i++){
        scaleVal[i] -= .2;
        if ( scaleVal[i] > 15.0 ) {
            scaleVal[i] = 2;}
        if ( scaleVal[i] < 8.0 ) {
            scaleVal[i] = 8.0;}
        
        // x
		if (!colorDir[i*3])
			colorArray[i].x += ((i*.03) + .02);
		else
			colorArray[i].x -= ((i*.01) + .05);
		if (colorArray[i].x >= 1)
			colorDir[(i*3)] = true;
		else if (colorArray[i].x <= 0)
			colorDir[(i*3)] = false;
		
		// y
		if (!colorDir[(i*3) + 1])
			colorArray[i].y += ((i*.03) + .01);
		else
			colorArray[i].y -= ((i*.02) + .01);
		if (colorArray[i].y >= 1)
			colorDir[(i*3) + 1] = true;
		else if (colorArray[i].y <= 0)
			colorDir[(i*3) + 1] = false;
        
		// z
		if (!colorDir[(i*3) + 2])
			colorArray[i].z += ((i*.02) + .02);
		else
			colorArray[i].z -= ((i*.01) + .04);
		if (colorArray[i].z >= 1)
			colorDir[(i*3) + 2] = true;
		else if (colorArray[i].z <= 0)
			colorDir[(i*3) + 2] = false;
    }
	// Automatic camera rotation
	camera.autoRotateCam();
    
	glutPostRedisplay();
}


void initGlut(int& argc, char** argv)
{
	glutInit(&argc, argv);
#ifdef __APPLE__  // include Mac OS X verions of headers
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH );
#else // non-Mac OS X operating systems
    glutInitContextVersion( 3, 2 );
    glutInitContextProfile( GLUT_CORE_PROFILE );
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
#endif  // __APPLE__
    
	glutInitWindowPosition(400, 0);
	glutInitWindowSize(winWidth, winHeight);
	glutCreateWindow("Music Visualizer - Hao, Nguy, Sabatine");
    
	glewExperimental = GL_TRUE;
	glewInit();
    
	// Enable Depth
	glEnable( GL_DEPTH_TEST );
    
	// Stores the points for the original cube
	myInit();
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
