#include "model.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"


Model::Model(const char* path){
    if(!loadObj(path)){
        std::cerr << "모델 로드 실패: " << path << std::endl;
    }
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

