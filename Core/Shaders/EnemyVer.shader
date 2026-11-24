#version 330 core
layout (location = 0) in vec3 aPos;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProj;

//out vec3 FragPos;

void main()
{
    //FragPos = (uModel*aPos).rgb;
    gl_Position = uProj *  uView *  uModel * vec4(aPos,1.0);
}