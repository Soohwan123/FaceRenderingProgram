#ifndef MODEL_H
#define MODEL_H

#include <vector>
#include <string>
#include "glm/glm/glm.hpp"
#include "mesh.h"


/*
// 버텍스 데이터와 인덱스 데이터를 저장하고 렌더링하는 기능을 제공한다.
// OBJ 파일에서 모델을 로드하고 OpenGL을 통해 화면에 그리는 역할을 한다.
// 셰이더 프로그램과 연동하여 모델의 변환(이동, 회전, 크기 조절)을 처리한다.
*/
class Model{
public:
    // 생성자 : obj 경로 로드
    Model(const char* path);
    ~Model();
    

    //모델 데이터
    std::vector<glm::vec3> vertices; // 얼굴 정점 위치
    std::vector<unsigned int> indices; // 인덱스 버퍼용
    std::vector<unsigned int> noseIndices;  // 코 vertices의 인덱스를 저장할 벡터
    
    glm::vec3 getNoseCenter() const;

private:
    // obj 로드 함수
    bool loadObj(const char* path);
};

#endif