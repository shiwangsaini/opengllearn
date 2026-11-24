#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;

layout (location = 5) in vec3 InstanceOffset;

out vec3 FragPos;
out vec2 TexCoord;
out mat3 TBN;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProj;

void main()
{
    vec4 worldPos = uModel * vec4(aPos + InstanceOffset, 1.0);
    FragPos  = worldPos.xyz;
    TexCoord = aTexCoord;

    mat3 normalMat = mat3(transpose(inverse(uModel)));
    vec3 N = normalize(normalMat * aNormal);
    vec3 T = normalize(normalMat * aTangent);
    T = normalize(T - dot(T, N) * N);          // re-orthogonalize
    vec3 B = normalize(normalMat * aBitangent);
    TBN = mat3(T, B, N);

    gl_Position = uProj * uView * worldPos;
}