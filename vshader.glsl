#version 150

in  vec4 vPosition;
in  vec3 vNormal;
out vec4 color;

uniform vec4 vColor;
uniform mat4 CameraMatrix;
uniform mat4 PerspectiveMatrix;

void main() 
{
	gl_Position = vPosition * CameraMatrix * PerspectiveMatrix;
	color = vColor;
} 
