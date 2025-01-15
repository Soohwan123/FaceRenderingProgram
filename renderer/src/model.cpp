#include "model.h"
#include <fstream>
#include <sstream>
#include <iostream>


Model::Model(const char* path){
    if(!loadObj(path)){
        std::cerr << "모델 로드 실패: " << path << std::endl;
    }
}

Model::~Model(){

}

bool Model::loadObj(const char* path){
    std::ifstream file(path);
    if(!file){
        std::cerr << "파일 열기 실패: " << path << std::endl;
        return false;
    }

    std::string line;
    while(std::getline(file, line)){
        std::istringstream iss(line);
        std::string type;
        iss >> type;

        // 버텍스 위치
        if(type == "v"){
            glm::vec3 vertex;
            iss >> vertex.x >> vertex.y >> vertex.z;
            vertices.push_back(vertex);
        }
        else if(type == "f"){  // 삼각형 정의
            std::string v1, v2, v3;
            iss >> v1 >> v2 >> v3;

            //obj 인덱스는 1부터 시작하므로 0부터 시작하도록 1을 뺀다
            // 여기서는 위치인덱스만 처리(텍스처, 노말라이즈 안함)
            indices.push_back(std::stoi(v1) - 1);
            indices.push_back(std::stoi(v3) - 1);
            indices.push_back(std::stoi(v2) - 1);
        }

        // 다른 OBJ 요소들 (vt, vn 등)은 현재 무시
    }
    

    std::cout << "Model loaded: " << vertices.size() << " vertices, " 
              << indices.size() / 3 << " triangles" << std::endl;
    
    file.close();
    return true;
}

