#include "model.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"


Model::Model(const char* path){
    if(!loadObj(path)){
        std::cerr << "모델 로드 실패: " << path << std::endl;
    }
    
    // 법선 벡터 계산 추가
    calculateNormals();
}

Model::~Model(){

}


bool Model::loadObj(const char* path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cout << "OBJ 파일을 열 수 없습니다: " << path << std::endl;
        return false;
    }

    std::vector<glm::vec3> temp_positions;
    std::vector<glm::vec2> temp_texcoords;
    std::string mtl_file;
    
    vertices.clear();
    indices.clear();
    noseIndices.clear();

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string type;
        iss >> type;

        if (type == "v") {  // 정점 위치
            glm::vec3 pos;
            iss >> pos.x >> pos.y >> pos.z;
            temp_positions.push_back(pos);
            if (temp_positions.size() % 100 == 0) {
                std::cout << "정점 " << temp_positions.size() << "개 로드됨" << std::endl;
            }
        }
        else if (type == "vt") {  // 텍스처 좌표
            glm::vec2 tex;
            iss >> tex.x >> tex.y;
            temp_texcoords.push_back(tex);
            if (temp_texcoords.size() % 100 == 0) {
                std::cout << "UV 좌표 " << temp_texcoords.size() << "개 로드됨" << std::endl;
            }
        }
        else if (type == "f") {  // 면 정보
            std::string vertex1, vertex2, vertex3;
            iss >> vertex1 >> vertex2 >> vertex3;

            // 면의 각 정점에 대한 인덱스 처리
            std::vector<std::string> v1 = split(vertex1, '/');
            std::vector<std::string> v2 = split(vertex2, '/');
            std::vector<std::string> v3 = split(vertex3, '/');

            // OBJ는 1-based indexing이므로 1을 빼줌
            unsigned int posIndex1 = std::stoi(v1[0]) - 1;
            unsigned int posIndex2 = std::stoi(v2[0]) - 1;
            unsigned int posIndex3 = std::stoi(v3[0]) - 1;

            indices.push_back(posIndex1);
            indices.push_back(posIndex2);
            indices.push_back(posIndex3);
        }
        else if (type == "mtllib") {
            iss >> mtl_file;
            std::cout << "MTL 파일 발견: " << mtl_file << std::endl;
        }
        else if (line.substr(0, 13) == "# nose_vertex") {
            std::istringstream niss(line);
            std::string dummy1, dummy2;
            unsigned int index;
            niss >> dummy1 >> dummy2 >> index;
            noseIndices.push_back(index - 1);  // 1-based to 0-based
        }
    }

    // vertices 배열 생성
    vertices.resize(temp_positions.size());
    for (size_t i = 0; i < temp_positions.size(); i++) {
        vertices[i].Position = temp_positions[i];
        if (i < temp_texcoords.size()) {
            vertices[i].TexCoords = temp_texcoords[i];
        }
    }

    std::cout << "\n로드 완료:" << std::endl;
    std::cout << "- 정점 수: " << temp_positions.size() << std::endl;
    std::cout << "- UV 좌표 수: " << temp_texcoords.size() << std::endl;
    std::cout << "- vertices 배열 크기: " << vertices.size() << std::endl;
    std::cout << "- indices 배열 크기: " << indices.size() << std::endl;
    std::cout << "- 코 정점 수: " << noseIndices.size() << std::endl;

    file.close();
    return true;
}

glm::vec3 Model::getNoseCenter() const {
    glm::vec3 center(0.0f);
    for (unsigned int idx : noseIndices) {
        center += vertices[idx].Position;
    }
    return center / static_cast<float>(noseIndices.size());
}

//법선 벡터 계산
void Model::calculateNormals() {
    // 초기화 전 vertices 크기 확인
    std::cout << "\n[Normal 계산 시작]" << std::endl;
    std::cout << "vertices 크기: " << vertices.size() << std::endl;
    
    // 각 정점의 법선 벡터를 0으로 초기화
    for(auto& v: vertices){
        v.Normal = glm::vec3(0.0f);
    }

    // 삼각형 개수 출력
    std::cout << "삼각형 개수: " << indices.size() / 3 << std::endl;

    //각 삼각형의 법선벡터 구하기
    for(size_t i = 0; i < indices.size(); i += 3){
        unsigned int idx1 = indices[i];
        unsigned int idx2 = indices[i+1];
        unsigned int idx3 = indices[i+2];

        // 인덱스 범위 체크
        if(idx1 >= vertices.size() || idx2 >= vertices.size() || idx3 >= vertices.size()) {
            std::cout << "잘못된 인덱스 발견: " << idx1 << ", " << idx2 << ", " << idx3 << std::endl;
            continue;
        }

        //삼각형의 두 엣지의 벡터
        glm::vec3 edge1 = vertices[idx2].Position - vertices[idx1].Position;
        glm::vec3 edge2 = vertices[idx3].Position - vertices[idx1].Position;

        // 법선 벡터 계산 (외적)
        glm::vec3 normal = glm::normalize(glm::cross(edge1, edge2));

        // 계산된 법선 벡터 출력 (100개 단위로)
        if(i % 300 == 0) {
            std::cout << "삼각형 " << i/3 << " 법선: " 
                      << normal.x << ", " << normal.y << ", " << normal.z << std::endl;
        }

        vertices[idx1].Normal += normal;
        vertices[idx2].Normal += normal;
        vertices[idx3].Normal += normal;
    }

    // 정규화 후 몇 개의 법선 벡터 출력
    std::cout << "\n정규화된 법선 벡터 샘플:" << std::endl;
    for(size_t i = 0; i < vertices.size(); i += 1000) {
        glm::vec3& normal = vertices[i].Normal;
        normal = glm::normalize(normal);
        std::cout << "정점 " << i << " 법선: " 
                  << normal.x << ", " << normal.y << ", " << normal.z << std::endl;
    }
    
    std::cout << "[Normal 계산 완료]\n" << std::endl;
}

std::string Model::getDirectory(const std::string& filepath) {
    size_t found = filepath.find_last_of("/\\");
    return (found != std::string::npos) ? filepath.substr(0, found + 1) : "";
}

unsigned int Model::loadTexture(const char* path) {
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
    
    if (data) {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else {
        std::cout << "텍스처 로드 실패: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}


std::vector<std::string> Model::split(const std::string& s, char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(s);
    
    while (std::getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }
    
    return tokens;
}

