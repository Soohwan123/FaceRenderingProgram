#include "mesh.h"
#include "model.h"
#include <iostream>

Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices)
    : vertices(vertices), indices(indices)
{
    std::cout << "\n[Mesh 초기화 시작]" << std::endl;
    std::cout << "Vertex 구조체 크기: " << sizeof(Vertex) << " bytes" << std::endl;
    std::cout << "Position offset: " << offsetof(Vertex, Position) << std::endl;
    std::cout << "TexCoords offset: " << offsetof(Vertex, TexCoords) << std::endl;
    std::cout << "Normal offset: " << offsetof(Vertex, Normal) << std::endl;
    std::cout << "총 vertex 수: " << vertices.size() << std::endl;
    std::cout << "총 index 수: " << indices.size() << std::endl;

    setupMesh();

    // 버퍼 생성 후 ID 확인
    std::cout << "VAO ID: " << vao << std::endl;
    std::cout << "VBO ID: " << vbo << std::endl;
    std::cout << "EBO ID: " << ebo << std::endl;
    
    // OpenGL 에러 체크
    GLenum err;
    while((err = glGetError()) != GL_NO_ERROR) {
        std::cout << "OpenGL error: " << err << std::endl;
    }
    
    std::cout << "[Mesh 초기화 완료]\n" << std::endl;
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
    // GLenum err;
    // while ((err = glGetError()) != GL_NO_ERROR) {
    //     std::cout << "OpenGL error: " << err << std::endl;
    // }
    
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
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

    // 인덱스 데이터 전송
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    // Position 속성 설정 (location = 0)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Position));
    glEnableVertexAttribArray(0);

    // TexCoords 속성 설정 (location = 1)
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
    glEnableVertexAttribArray(1);

    // Normal 속성 설정 (location = 2)
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
    glEnableVertexAttribArray(2);

    // VAO 언바인드
    std::cout << "VAO: " << vao << ", VBO: " << vbo << ", EBO: " << ebo << std::endl;
    glBindVertexArray(0);
    
    std::cout << "Mesh setup complete. VAO: " << vao << std::endl;
}