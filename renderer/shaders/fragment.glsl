#version 400 core

in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;

out vec4 FragColor;

uniform sampler2D texture1;  //텍스쳐 샘플러
uniform vec3 lightPos; // 조명 위치
uniform vec3 viewPos; // 카메라 위치
uniform vec3 lightColor; // 조명 색상

void main() {
    // ambient
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;

    // diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    // specular
    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
    vec3 specular = specularStrength * spec * lightColor;

    // 최종 색상 계산
    vec3 result = (ambient + diffuse + specular) * vec3(texture(texture1, TexCoords));
    FragColor = vec4(result, 1.0);
}