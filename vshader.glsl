#version 150

in  vec4 vPosition;
in  vec3 vNormal;
out vec4 color;

uniform vec4  lightPosition;

// Phong
out vec3 fN;
out vec3 fE;
out vec3 fL;

uniform mat4 ScaleMatrix;
uniform mat4 CameraMatrix;
uniform mat4 PerspectiveMatrix;

void main() 
{
	gl_Position = ScaleMatrix * vPosition * CameraMatrix * PerspectiveMatrix;
	
	fN = (vec4(vNormal, 0.0)).xyz;
	fE = -(vPosition).xyz;
	fL = lightPosition.xyz - (vPosition).xyz;
} 
