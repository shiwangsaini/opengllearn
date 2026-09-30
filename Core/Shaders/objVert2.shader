#version 330 core
layout (location = 0) in vec4 aPos;
layout (location = 1) in vec3 aNormal;

layout (location = 2) in vec3 objPos;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProj;

out vec3 Normal;
out vec3 FragPos;

void main()
{
	vec4 worldPos = uModel * aPos;
    worldPos.xyz += objPos;

	FragPos = vec3(uModel * aPos);
	Normal = mat3(transpose(inverse(uModel))) * aNormal;

	gl_Position = uProj * uView * worldPos;
}