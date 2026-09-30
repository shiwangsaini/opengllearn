
#version 330 core

out vec4 FragColor;
  
uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 uLightPos;
uniform vec3 uViewPos;

in vec3 Normal;
in vec3 FragPos;
in vec3 Color;
in vec2 TexCoord;

uniform sampler2D uTexture;

void main()
{
    // Ambient
    float ambientStrength = 0.2;
    vec3 ambient = ambientStrength * lightColor;

    vec3 Norm = normalize(Normal);
    vec3 lightDir = normalize(uLightPos - FragPos);

    // Diffusion
    float Diff = max(dot(Norm, lightDir), 0.0);
    vec3 Diffusion = (Diff) * lightColor;

    // Speculation Diriction
    float speculStrength = 0.5;
    vec3 viewDir = normalize(uViewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, Norm);

    // Specular
    float spec = pow(max(dot(viewDir, reflectDir), 0.0),36.0); 
    vec3 specular = speculStrength * spec * lightColor;

    // Final
    vec3 lighting = (specular + ambient + Diffusion);
    vec3 TextureColor = texture(uTexture, TexCoord).rgb * objectColor;

    FragColor = vec4(lighting * TextureColor, 1.0);
}