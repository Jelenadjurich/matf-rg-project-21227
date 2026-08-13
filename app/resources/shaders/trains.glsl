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

in vec3 Normal;
in vec3 FragPos;

uniform vec3 baseColor = vec3(0.65, 0.65, 0.68);

void main() {
    vec3 lightDir = normalize(vec3(0.5, 1.0, 0.8));
    vec3 norm = normalize(Normal);

    float ambient = 0.25;
    float diff = max(dot(norm, lightDir), 0.0);

    vec3 color = baseColor * (ambient + diff * 0.75);
    FragColor = vec4(color, 1.0);
}