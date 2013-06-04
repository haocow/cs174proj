
#ifndef __LIGHTING__
#define __LIGHTING__

#include "Angel.h"

GLint fMaterialAmbient, fMaterialDiffuse, fMaterialSpecular;

// Variables for light
point4 light_position( 0, 100, 0, 0.0 );
color4 light_ambient( 0.15, 0.15, 0.15, 1.0 );
color4 light_diffuse( .5, .58, .58, 1 );
color4 light_specular;// = (1.0, 1.0, 1.0, 1.0);

color4 material_ambient( 0.5, 0.5, 0.5, 1.0 );
color4 material_diffuse( 1.0, 1.0, 1.0, 1.0 );
color4 material_specular( 1.0, 0.0, 1.0, 1.0 );
float  material_shininess = 10.0;

color4 ambient_product = light_ambient * material_ambient;
color4 diffuse_product = light_diffuse * material_diffuse;
color4 specular_product = light_specular * material_specular;

// colorArray
float scaleVal[3] = {4.0, 2.0, 1.0};
vec4 colorAdj( 1.0, 0.0, 1.0 );

bool colorDir[9] = {false, false, false, false, false, false, false, false, false};

color4 colorArray[3] = {color4( .26, .40, .82, 1 ),
    color4( .71, .41, .90, 1 ),
    color4( .30, .71, .53, 1 )};
// Light related functions
void setColor(float r, float g, float b, float a, float spec)
{
	material_ambient = color4( r, g, b, a);
	material_diffuse = color4( r, g, b, a);
	material_specular = color4(1, 1, 1, 1);
    
	if (spec == 1)
	{
		material_specular = color4(r, g, b, a);
		light_specular = color4(0, 0, 0, 0);
	}
    
	// update all products
	ambient_product = light_ambient * material_ambient;
	diffuse_product = light_diffuse * material_diffuse;
	specular_product = light_specular * material_specular;
}

// Collision test
bool testCollisions( vec4 pta, vec4 ptb )   // vec4 = ( x, y, z, scale)
{
	// Calculate squared distance between centers
	float xdiff = pta[0] - ptb[0];
	float ydiff = pta[1] - ptb[1];
	float zdiff = pta[2] - ptb[2];
	float dist2 = (xdiff*xdiff) + (ydiff*ydiff) + (zdiff*zdiff);
	// Spheres intersect if squared distance is less than
	// squared sum of radii
	float radiusSum = pta[3] + ptb[3];
	return dist2 <= radiusSum * radiusSum;
}


#endif