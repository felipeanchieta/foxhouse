#version 330

in vec4 vPosition;
in vec3 vNormal;
uniform mat4 modelView;
uniform mat4 projectionMatrix;
uniform mat3 normalMatrix;
uniform vec4 ambientProduct;
uniform vec4 diffuseProduct;
uniform vec4 specularProduct;
uniform float shininess;
uniform vec4 lightPosition;
flat out vec4 inoutColor;

void main()
{
	vec4 eyePosition = modelView * vPosition;
	vec3 N = normalMatrix * vNormal;
	vec3 L = lightPosition.xyz - eyePosition.xyz;
	vec3 E = -eyePosition.xyz;
	vec3 H = normalize(L+E);
	N = normalize(N);
	L = normalize(L);
	E = normalize(E);
	float NdotL = dot(N, L);
	float Kd = max(NdotL , 0.0);
	float Ks = (NdotL < 0.0) ? 0.0 : pow(max(dot(H, E), 0.0), shininess);
	vec4 diffuse = Kd * diffuseProduct;
	vec4 specular = max(Ks * specularProduct ,0.0);
	vec4 ambient = ambientProduct;
	gl_Position = projectionMatrix * eyePosition;
	inoutColor = vec4((ambient + diffuse + specular).rgb, 1.0);

}
