#version 400 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexCoords;
layout(location = 2) in vec3 aNormal;    // 추가

out vec2 TexCoords;
out vec3 Normal;     // 추가
out vec3 FragPos;    // 추가

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    FragPos = vec3(model * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(model))) * aNormal;
    
    gl_Position = projection * view * vec4(FragPos, 1.0);
    TexCoords = aTexCoords;
}