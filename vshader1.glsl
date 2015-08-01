#version 330

in vec4 vPosition;
in vec4 vColors;

out vec4 v2fcolor;

void main()
{
    gl_Position = vPosition;
//  float gray = (vPosition.y + 1) * 0.5;
    v2fcolor = vColors;
}
