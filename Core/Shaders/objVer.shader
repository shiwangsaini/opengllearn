#version 330 core

layout (location = 0) in vec4 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

layout (location = 3) in vec3 aOffsets;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProj;

out vec2 TexCoord;
out vec3 Normal;
out vec3 FragPos;

void main()
{
	TexCoord = aTexCoord;
	vec4 worldPos = (aPos + vec4(aOffsets, 0.0)); 

	FragPos = vec3(uModel* worldPos);
	Normal = aNormal;

	gl_Position = uProj *  uView * uModel * worldPos;
}
