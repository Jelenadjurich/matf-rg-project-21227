//#shader vertex
#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec2 TexCoords;
out vec3 Normal;
out vec3 FragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;


void main()
{
    FragPos = vec3(model * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(model))) * aNormal;
    TexCoords = aTexCoords;
    gl_Position = projection * view * vec4(FragPos, 1.0);
}





//#shader fragment
#version 330 core

out vec4 FragColor;

in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;

uniform sampler2D texture_diffuse1;

uniform vec3 viewPos;

// Usmerena (directional) svetlost
uniform vec3 dirLight_direction;
uniform vec3 dirLight_color;

// Tačkasta (point) svetlost
uniform vec3 pointLight_position;
uniform vec3 pointLight_color;

vec3 calcDirLight(vec3 normal, vec3 viewDir, vec3 baseColor) {
    vec3 lightDir = normalize(-dirLight_direction);

    // ambient
    vec3 ambient = 0.15 * dirLight_color * baseColor;

    // diffuse
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = diff * dirLight_color * baseColor;

    // specular
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);
    vec3 specular = spec * dirLight_color * 0.5;

    return ambient + diffuse + specular;
}



//point
vec3 calcPointLight(vec3 normal, vec3 fragPos, vec3 viewDir, vec3 baseColor) {
    vec3 lightDir = normalize(pointLight_position - fragPos);

    // ambient
    vec3 ambient = 0.05 * pointLight_color * baseColor;

    // diffuse
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = diff * pointLight_color * baseColor;

    // specular
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);
    vec3 specular = spec * pointLight_color * 0.5;

    // attenuation (slabljenje sa udaljenošću)
    float distance = length(pointLight_position - fragPos);
    float attenuation = 1.0 / (1.0 + 0.09 * distance + 0.032 * distance * distance);

    return (ambient + diffuse + specular) * attenuation;
}


void main() {
    vec3 baseColor = texture(texture_diffuse1, TexCoords).rgb;
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    vec3 result = calcDirLight(norm, viewDir, baseColor);
    result += calcPointLight(norm, FragPos, viewDir, baseColor);

    FragColor = vec4(result, 1.0);
}