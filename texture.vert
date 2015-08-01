#version 330

in vec4 vPosition;
in vec3 vNormal;
in vec2 vcoordText;

uniform mat4 modelView;
uniform mat4 projectionMatrix;
uniform mat3 normalMatrix;

uniform vec4 lightPosition;

out vec3 fN;
out vec3 fE;
out vec3 fL;
out vec2 fTextCoord;

void main()
{
	vec4 eyePosition = modelView * vPosition;

	fN = normalMatrix * vNormal;
	fL = lightPosition.xyz - eyePosition.xyz;
	fE = -eyePosition.xyz;

	fTextCoord = vcoordText;
	gl_Position = projectionMatrix * eyePosition;
}
