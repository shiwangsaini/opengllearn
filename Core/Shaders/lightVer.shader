#version 330 core
layout (location = 0) in vec4 aPos;
layout (location = 1) in vec3 aNormal;


uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProj;

void main()
{
	gl_Position = uProj *  uView *  uModel * aPos;
}