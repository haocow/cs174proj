#version 120

attribute  vec4 vPosition;
attribute  vec3 vNormal;
attribute  vec2 vTexCoords;

// Lighting
uniform vec4  lightPosition;

varying vec3 fN;
varying vec3 fE;
varying vec3 fL;

// Textures
varying vec2 st;

uniform mat4 ScaleMatrix;
uniform mat4 TranslationMatrix;
uniform mat4 CameraMatrix;
uniform mat4 PerspectiveMatrix;

void main() 
{

	fN = normalize( vec4(vNormal, 0.0) * TranslationMatrix ).xyz;
	fE = -(vPosition * TranslationMatrix).xyz;
	fL = lightPosition.xyz - (vPosition * TranslationMatrix).xyz;
    
	st = vTexCoords;
    
    gl_TexCoord[0].xy = vTexCoords;
	gl_Position = ScaleMatrix * vPosition * TranslationMatrix * CameraMatrix * PerspectiveMatrix;
}
