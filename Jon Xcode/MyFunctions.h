#ifndef __MYFUNC__
#define __MYFUNC__

#include "Angel.h"

#if defined(_MSC_VER) || defined(_MSC_EXTENSIONS)
  #define DELTA_EPOCH_IN_MICROSECS  11644473600000000Ui64
#else
  #define DELTA_EPOCH_IN_MICROSECS  11644473600000000ULL
#endif

mat4 identityMatrix = mat4 (vec4(1, 0, 0, 0),
						    vec4(0, 1, 0, 0),
							vec4(0, 0, 1, 0),
							vec4(0, 0, 0, 1));

GLuint ScaleMat, 
	   TransMat;

void setScale(float timesLarger)
{
	glUniformMatrix4fv(ScaleMat, 1, false, Scale(timesLarger, timesLarger, timesLarger));
}

void setTranslation(float x, float y, float z)
{
	glUniformMatrix4fv(TransMat, 1, false, Translate(x, y, z));
}

#ifdef __APPLE__
#else
	#include <time.h>
	#include <Winsock2.h>

	struct timezone
	{
	  int  tz_minuteswest; /* minutes W of Greenwich */
	  int  tz_dsttime;     /* type of dst correction */
	};

	int gettimeofday(struct timeval *tv, struct timezone *tz)
	{
	// Define a structure to receive the current Windows filetime
	  FILETIME ft;
 
	// Initialize the present time to 0 and the timezone to UTC
	  unsigned __int64 tmpres = 0;
	  static int tzflag = 0;

	  if (NULL != tv)
	  {
		GetSystemTimeAsFileTime(&ft);
 
	// The GetSystemTimeAsFileTime returns the number of 100 nanosecond 
	// intervals since Jan 1, 1601 in a structure. Copy the high bits to 
	// the 64 bit tmpres, shift it left by 32 then or in the low 32 bits.
		tmpres |= ft.dwHighDateTime;
		tmpres <<= 32;
		tmpres |= ft.dwLowDateTime;
 
	// Convert to microseconds by dividing by 10
		tmpres /= 10;
 
	// The Unix epoch starts on Jan 1 1970.  Need to subtract the difference 
	// in seconds from Jan 1 1601.
		tmpres -= DELTA_EPOCH_IN_MICROSECS;
 
	// Finally change microseconds to seconds and place in the seconds value. 
	// The modulus picks up the microseconds.
		tv->tv_sec = (long)(tmpres / 1000000UL);
		tv->tv_usec = (long)(tmpres % 1000000UL);
	  }
 
	  if (NULL != tz)
	  {
		if (!tzflag)
		{
		  _tzset();
		  tzflag++;
		}
  
	// Adjust for the timezone west of Greenwich
		  tz->tz_minuteswest = _timezone / 60;
		tz->tz_dsttime = _daylight;
	  }
 
	  return 0;
	}
#endif

#endif