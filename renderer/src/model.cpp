#include "model.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>



Model::Model(const char* path){
    if(!loadObj(path)){
        std::cerr << "모델 로드 실패: " << path << std::endl;
    }
}

Model::~Model(){

}

bool Model::loadObj(const char* path){
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "Error: Cannot open file " << path << std::endl;
        return false;
    }

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;

        std::istringstream iss(line);
        std::string prefix;
        iss >> prefix;

        if (prefix == "v") {
            glm::vec3 vertex;
            iss >> vertex.x >> vertex.y >> vertex.z;
            vertices.push_back(vertex);
        }
        else if (prefix == "f") {
            unsigned int v1, v2, v3;
            iss >> v1 >> v2 >> v3;
            // OBJ는 1-based indexing이므로 1을 빼줌
            indices.push_back(v1 - 1);
            indices.push_back(v2 - 1);
            indices.push_back(v3 - 1);
        }
        else if (prefix == "#") {
            // 코 vertex 인덱스 읽기
            std::string comment;
            std::getline(iss, comment);
            if (comment.find("nose_vertex") != std::string::npos) {
                unsigned int noseIdx;
                sscanf(comment.c_str(), " nose_vertex %u", &noseIdx);
                noseIndices.push_back(noseIdx - 1);  // 1-based to 0-based
            }
        }
    }

    if (!noseIndices.empty()) {
        std::cout << "Loaded " << noseIndices.size() << " nose vertices" << std::endl;
        
        // 코의 중심점 계산
        glm::vec3 noseCenter = getNoseCenter();
        std::cout << "Nose center: (" << noseCenter.x << ", " 
                                    << noseCenter.y << ", " 
                                    << noseCenter.z << ")" << std::endl;
    }

    return true;
}

glm::vec3 Model::getNoseCenter() const {
    glm::vec3 center(0.0f);
    for (unsigned int idx : noseIndices) {
        center += vertices[idx];
    }
    return center / static_cast<float>(noseIndices.size());
}

