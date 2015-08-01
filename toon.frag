#version 330

in vec3 fN;
in vec3 fE;
in vec3 fL;

uniform vec4 ambientProduct;
uniform vec4 diffuseProduct;
uniform vec4 specularProduct;
uniform float shininess;

out vec4 frag_color;

const int levels = 3;
const float scaleFactor = 1.0 / levels;

vec3 toon()
{
	vec3 N = normalize(fN);
	vec3 E = normalize(fE);
	vec3 L = normalize(fL);
	vec3 R = normalize(2.0 * dot(L, N) * N - L);
	float NdotL = dot(N, L);
	float Kd = max(NdotL , 0.0);
	vec3 s = normalize(L - E);
	float cosine = max(0.0, dot(s, R));
	vec4 diffuse = diffuseProduct * floor(cosine * levels) * scaleFactor;
	//vec4 specular = Ks * specularProduct;
	vec4 ambient = ambientProduct;

	return (ambient.xyz + diffuse.xyz);
}

void main()
{
	frag_color = vec4(toon(), 1.0);
}
