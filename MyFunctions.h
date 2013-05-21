#ifndef __MYFUNC__
#define __MYFUNC__

#include "Angel.h"

mat4 identityMatrix = mat4 (vec4(1, 0, 0, 0),
						    vec4(0, 1, 0, 0),
							vec4(0, 0, 1, 0),
							vec4(0, 0, 0, 1));

GLuint ScaleMat;

void setScale(float timesLarger)
{
	glUniformMatrix4fv(ScaleMat, 1, false, Scale(timesLarger, timesLarger, timesLarger));
}

#endif