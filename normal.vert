#version 330

in vec4 vPosition;
in vec3 vNormal;
in vec2 vTextCoord;
in vec4 vTangent;
uniform mat4 modelView;
uniform mat4 projectionMatrix;
uniform mat3 normalMatrix;
uniform vec4 lightPosition;
out vec3 fE;
out vec3 fL;
out vec2 fTextCoord;

void main ()
{
    vec3 bitangent = vTangent.w * cross(vNormal, vTangent.xyz);
    vec3 T = normalMatrix * vTangent.xyz ;
    vec3 B = normalMatrix * bitangent;
    vec3 N = normalMatrix * vNormal;

    mat3 TBN = mat3 ( T.x, B.x, N.x,
                      T.y, B.y, N.y,
                      T.z, B.z, N.z);

    vec4 eyePosition = modelView * vPosition;

    fL = TBN * (lightPosition.xyz - eyePosition.xyz);
    fE = TBN * (-eyePosition.xyz);
    fTextCoord = vTextCoord;
    gl_Position = projectionMatrix * eyePosition;
}
