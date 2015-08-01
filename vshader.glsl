#version 330

uniform mat4 modelView;
uniform vec3 midPoint;
uniform float invDiag;
uniform mat4 projectionMatrix;

in vec4 vPosition;
in vec3 vColors;
in vec3 normal;

out vec4 v2fcolor;
out vec4 inout_color;

void main()
{
	//v2fcolor = vec4(abs(vColors.x),abs(vColors.y),abs(vColors.z), 1);
	inout_color = vec4(abs(normal), 1);
	gl_Position = projectionMatrix*modelView*vPosition;
}
