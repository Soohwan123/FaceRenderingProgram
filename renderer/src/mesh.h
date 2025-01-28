#ifndef MESH_H
#define MESH_H

#include "../../external/glew/include/GL/glew.h"
#include <vector>
#include "../../external/glm/glm/glm.hpp"
#include "model.h"  // Vertex 구조체 정의를 위해 추가


class Mesh {
public:
    Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices);
    ~Mesh();

    //렌더링 함수
    void Draw();

    // 버텍스 데이터 업데이트 함수 추가
    void updateVertexPositions(const std::vector<Vertex>& newVertices) {
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferSubData(GL_ARRAY_BUFFER, 0, newVertices.size() * sizeof(Vertex), newVertices.data());
    }

private:
    unsigned int vao, vbo, ebo;

    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    //버퍼 초기화 함수
    void setupMesh();
};

#endif