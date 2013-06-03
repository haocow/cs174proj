#version 120

varying	vec3	fN;
varying	vec3	fL;
varying	vec3	fE;

uniform vec4 AmbientProduct, DiffuseProduct, SpecularProduct;

uniform float Shininess;

// Texture
uniform  vec2  st;
uniform sampler2D Tex;
uniform int fsphereID;

varying vec4 fColor;

void main() 
{ 
	if (fsphereID == 0)
	{
        gl_FragColor = texture2D(Tex, vec2(gl_TexCoord[0]));
		gl_FragColor.a = 1.0;
	}
	else
	{
		// Normalize the input lighting vectors
		vec3 N = normalize(fN);
		vec3 E = normalize(fE);
		vec3 L = normalize(fL);
	
		vec3 H = normalize( L + E );
	
		vec4 ambient = AmbientProduct;
	
		float Kd = max(dot(N, L), 0.0);
		vec4 diffuse = Kd * DiffuseProduct;
	 
		float Ks = pow(max(dot(N, H), 0.0), Shininess);
		vec4 specular = Ks*SpecularProduct;

		// discard the specular highlight if the light's behind the vertex
		if( dot(L, N) < 0.0 ) {
			specular = vec4(0.0, 0.0, 0.0, 1.0);
		}
	
		gl_FragColor = ambient + diffuse + specular;
		gl_FragColor.a = 1.0;
	}
} 