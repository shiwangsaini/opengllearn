#version 330 core

out vec4 FragColor;

struct Material {
    sampler2D texture_diffuse1;
    sampler2D texture_specular1;
    sampler2D texture_normal1;
    vec3  diffuse;
    vec3  specular;
    vec3  ambient;
    float shininess;
};

struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffusion;
    vec3 specular;
};

uniform Material material;
uniform Light    light;
uniform vec3     uViewPos;
uniform bool     hasSpecular;
uniform bool     useMaterialColor;
uniform bool     hasNormalMap;

in vec3 FragPos;
in vec2 TexCoord;
in mat3 TBN;

void main()
{
    vec3 diffuseColor = useMaterialColor ? material.diffuse : texture(material.texture_diffuse1, TexCoord).rgb;
    vec3 specularColor = useMaterialColor ? material.specular : (hasSpecular ? texture(material.texture_specular1, TexCoord).rgb : vec3(0.0));

    vec3 norm = normalize(TBN[2]);
    if (hasNormalMap) {
        vec3 n = texture(material.texture_normal1, TexCoord).rgb * 2.0 - 1.0;
        norm = normalize(TBN * n);
    }

    vec3 lightDir = normalize(light.position - FragPos);
    vec3 viewDir  = normalize(uViewPos - FragPos);

    vec3 ambient = light.ambient * material.ambient * diffuseColor;

    float diff   = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * light.diffusion * diffuseColor;

    // Blinn-Phong: looks better than Phong at high shininess like 225
    vec3 halfDir = normalize(lightDir + viewDir);
    float spec   = pow(max(dot(norm, halfDir), 0.0), material.shininess);
    vec3 specular = spec * light.specular * specularColor;

    FragColor = vec4(ambient + diffuse + specular, 1.0);
}