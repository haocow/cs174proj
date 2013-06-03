#include "Angel.h"
#include "sphere.h"

int Index = 0;

point4 v[4] = {
	vec4( 0.0, 0.0, 1.0, 1.0 ),
	vec4( 0.0, 0.942809, -0.333333, 1.0 ),
	vec4( -0.816497, -0.471405, -0.333333, 1.0 ),
	vec4( 0.816497, -0.471405, -0.333333, 1.0 )
    };

void Sphere::triangle( const point4& a, const point4& b, const point4& c )
{
	points[Index] = a;  Index++;
	points[Index] = b;  Index++;
	points[Index] = c;  Index++;
}

point4 Sphere::unit( const point4& p )
{
    float len = p.x*p.x + p.y*p.y + p.z*p.z;
    
    point4 t;
    if ( len > DivideByZeroTolerance ) {
	t = p / sqrt(len);
	t.w = 1.0;
    }

    return t;
}

void Sphere::divide_triangle( const point4& a, const point4& b,
		 const point4& c, int count )
{
    if ( count > 0 ) {
        point4 v1 = unit( a + b );
        point4 v2 = unit( a + c );
        point4 v3 = unit( b + c );
        divide_triangle(  a, v1, v2, count - 1 );
        divide_triangle(  c, v2, v3, count - 1 );
        divide_triangle(  b, v3, v1, count - 1 );
        divide_triangle( v1, v3, v2, count - 1 );
    }
    else {
        triangle( a, b, c );
    }
}

void Sphere::generateSphere(int count)
{
    divide_triangle( v[0], v[1], v[2], count );
    divide_triangle( v[3], v[2], v[1], count );
    divide_triangle( v[0], v[3], v[1], count );
    divide_triangle( v[0], v[2], v[3], count );

	// Normals
    for (int i = 0; i < numVertices; i++)
    {
        normals[i] = point3(points[i].x, points[i].y, points[i].z);
    }

	// TexCoords
    double u, v;
    for (int i = 0; i < numVertices; i++)
    {
        v = 10*(0.5 - asin(points[i].y)/M_PI) ; //0~10
        u = 10*(0.5*(atan2(points[i].z,points[i].x)/M_PI + 1)); //0~10
        tex_coords[i] = point2(u,v);
    }

	Index = 0;
}