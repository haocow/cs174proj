#version 150

in  vec4 vPosition;
in  vec3 vNormal;
out vec4 color;

// Lighting
uniform vec4  lightPosition;

out vec3 fN;
out vec3 fE;
out vec3 fL;

// Textures
in  vec2 vTexCoords;
out vec2 st;

uniform mat4 ScaleMatrix;
uniform mat4 TranslationMatrix;
uniform mat4 CameraMatrix;
uniform mat4 PerspectiveMatrix;

void main() 
{
	gl_Position = ScaleMatrix * vPosition * TranslationMatrix * CameraMatrix * PerspectiveMatrix;
	
	fN = normalize( vec4(vNormal, 0.0) * TranslationMatrix ).xyz;
	fE = -(vPosition * TranslationMatrix).xyz;
	fL = lightPosition.xyz - (vPosition * TranslationMatrix).xyz;

	st = vTexCoords;
} 
