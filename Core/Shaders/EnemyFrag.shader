#version 330 core
out vec4 FragColor;
 
uniform vec3 color;

void main()
{
    //FragColor = mix(texture(texture1, TexCoord) , texture(texture2, TexCoord), 0.2);
    FragColor = vec4(color, 1.0);
}