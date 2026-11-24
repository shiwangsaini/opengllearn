
#version 330 core

out vec4 FragColor;
  
struct Material {
    sampler2D diffuse;
    sampler2D specular;
    vec3 ambient;
    float shininess;
};

struct Light{
    vec3 position;
    vec3 ambient;
    vec3 diffusion;
    vec3 specular;
};

uniform Material material;
uniform Light light;

uniform vec3 uViewPos;

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoord;


void main()
{
    // Ambient
    vec3 ambient = light.ambient * material.ambient *vec3(texture(material.diffuse, TexCoord));

    vec3 Norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);

    // Diffusion
    float Diff = max(dot(Norm, lightDir), 0.0);
    vec3 Diffusion = Diff * light.diffusion * vec3(texture(material.diffuse, TexCoord));

    // Speculation Diriction
    vec3 viewDir = normalize(uViewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, Norm);

    // Specular
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess); 
    vec3 specular = spec * light.specular * vec3(texture(material.specular, TexCoord));
    // Final
    vec3 lighting = (specular + ambient + Diffusion);
    FragColor = vec4(lighting, 1.0);
    
}