#ifndef MESH_H
#define MESH_H

#include "../external/glew/include/GL/glew.h"
#include <vector>
#include "glm/glm/glm.hpp"


class Mesh {
public:
    Mesh(const std::vector<glm::vec3>& vertices, const std::vector<unsigned int>& indices);
    ~Mesh();

    //렌더링 함수
    void Draw();

private:
    unsigned int vao, vbo, ebo;

    std::vector<glm::vec3> vertices;
    std::vector<unsigned int> indices;

    //버퍼 초기화 함수
    void setupMesh();
};

#endif