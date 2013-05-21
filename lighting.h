#ifndef __LIGHTING__
#define __LIGHTING__

#include "Angel.h"

GLint fMaterialAmbient, fMaterialDiffuse, fMaterialSpecular;

// Variables for light
point4 light_position( 10, 10, 5, 0.0 );
color4 light_ambient( 0.2, 0.2, 0.2, 1.0 );
color4 light_diffuse( 1, 1, 1, 1 );
color4 light_specular = (1.0, 1.0, 1.0, 1.0);

color4 material_ambient( 0.5, 0.5, 0.5, 1.0 );
color4 material_diffuse( 1.0, 1.0, 1.0, 1.0 );
color4 material_specular( 1.0, 0.0, 1.0, 1.0 );
float  material_shininess = 5.0;

color4 ambient_product = light_ambient * material_ambient;
color4 diffuse_product = light_diffuse * material_diffuse;
color4 specular_product = light_specular * material_specular;

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

#endif