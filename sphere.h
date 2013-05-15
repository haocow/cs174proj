#ifndef __SPHERE__
#define __SPHERE__

typedef Angel::vec4  color4;
typedef Angel::vec4  point4;

const int numTimesToSubdivideMax = 5;				// number of subdivisions
const int numTriangles = 4096;					// (number of faces (4)) * (numTimesToSubdivide + 1)
												// number of resulting points
const int numVertices = 3 * numTriangles;

class Sphere
{
public:
    Sphere()
    {
	}

    ~Sphere() {}

	void triangle( const point4& a, const point4& b, const point4& c );
	point4 unit( const point4& p );
	void divide_triangle( const point4& a, const point4& b, const point4& c, int count );
	void generateSphere(int count);

	// Variable Declarations
	point4 points[numVertices];
	vec3 normals[numVertices];
};

#endif