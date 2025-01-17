#version 330 core

in vec2 TexCoords;
out vec4 FragColor;

uniform sampler2D texture1;  //텍스쳐 샘플러

void main() {
    FragColor = texture(texture1, TexCoords);
}