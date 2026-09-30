#version 330 core

layout (location = 0) in vec4 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec3 aColor;
layout (location = 3) in vec2 aTexCoord;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProj;

out vec3 Color;
out vec2 TexCoord;
out vec3 Normal;
out vec3 FragPos;

void main()
{
	Color = aColor;
	TexCoord = aTexCoord;
	FragPos = vec3(uModel * aPos);
	Normal = aNormal;
	gl_Position = uProj *  uView *  uModel * aPos;
}
