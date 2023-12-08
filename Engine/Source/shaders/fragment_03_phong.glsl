#version 440

layout(binding=3) uniform vec3 viewPos;
layout(binding=4) uniform Material material;
layout(binding=6) uniform Light light;
layout(binding=5) uniform sampler2D mytexture;

out vec4 color;

in vec2 uv0;
in vec3 normal;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

void main()
{
    // Ambient
    vec3 ambient = light.ambient * texture(material.diffuse, uv0).rgb;

    // Diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - gl_Position);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * texture(material.diffuse, uv0).rgb;

    // Specular
    vec3 viewDir = normalize(viewPos - gl_Position);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * texture(material.specular, uv0).rgb;

    vec3 result = ambient + diffuse + specular;
    color = vec4(result, 1.0);
}