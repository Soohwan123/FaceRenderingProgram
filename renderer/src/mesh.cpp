#include "mesh.h"
#include <iostream>

Mesh::Mesh(const std::vector<glm::vec3>& vertices, const std::vector<unsigned int>& indices)
    : vertices(vertices), indices(indices)
{
    setupMesh();
}

Mesh::~Mesh(){
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);
}

void Mesh::Draw(){
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    
    // OpenGL 에러 체크
    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR) {
        std::cout << "OpenGL error: " << err << std::endl;
    }
    
    glBindVertexArray(0);
}

void Mesh::setupMesh(){
    // 버퍼 생성
     std::cout << "Setting up mesh with " << vertices.size() << " vertices" << std::endl;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    // VAO 바인딩
    glBindVertexArray(vao);

      // 버텍스 데이터 전송
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW);

    // 인덱스 데이터 전송
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);


    // 버텍스 어트리븃(속성) 설정
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0 );
    glEnableVertexAttribArray(0);

    //vao 언바인드
    std::cout << "VAO: " << vao << ", VBO: " << vbo << ", EBO: " << ebo << std::endl;
    glBindVertexArray(0);
    
    std::cout << "Mesh setup complete. VAO: " << vao << std::endl;
}