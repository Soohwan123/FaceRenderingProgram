#include "camera.h"
#include <iostream>

// 생성자: 카메라의 초기 위치와 속성을 설정
// position: 카메라의 시작 위치 (기본값: (0,0,3))
Camera::Camera(glm::vec3 position)
: front(glm::vec3(0.0f, 0.0f, -5.0f)),  // 카메라가 -z 방향을 바라봄(정면임)
movementSpeed(2.0f),                      // 이동 속도
mouseSensitivity(0.1f),                   // 마우스 감도
zoom(45.0f) {                             // 시야각(FOV)
    this->position = position;
    worldUp = glm::vec3(0.0f, -1.0f, 0.0f);  // 월드 공간의 상향 벡터
    yaw = -18.0f;    // z축 회전 (좌우 회전)
    pitch = 70.0f;    // x축 회전 (상하 회전)
    roll = -100.0f;
    updateCameraVectors();
}

Camera::~Camera(){

}

// 뷰 mat 반환: 월드 공간을 카메라 공간으로 변환하는 mat
glm::mat4 Camera::GetViewMatrix(){
    return glm::lookAt(position,      // 카메라 위치
                      position + front,// 보는 지점
                      up);            // 카메라의 상향 벡터
}

// 투영 mat 반환: 카메라 공간을 클립 공간으로 변환하는 mat
// aspectRatio: 화면의 가로/세로 비율
glm::mat4 Camera::GetProjectionMatrix(float aspectRatio) {
    // FOV, aspect ratio, near plane, far plane 값 출력
    std::cout << "Projection settings:" << std::endl;
    std::cout << "FOV: " << zoom << " degrees" << std::endl;
    std::cout << "Aspect Ratio: " << aspectRatio << std::endl;
    std::cout << "Near plane: " << 0.1f << std::endl;
    std::cout << "Far plane: " << 100.0f << std::endl;

    return glm::perspective(glm::radians(zoom),  // FOV (기본값은 보통 45도)
                          aspectRatio,           // 화면 비율
                          0.1f,                  // near plane
                          100.0f);              // far plane
}

// 키보드 입력 처리: 카메라 위치 이동
// direction: 이동 방향 (FORWARD, BACKWARD, LEFT, RIGHT)
// deltaTime: 프레임 간의 시간 간격
void Camera::ProcessKeyboard(int direction, float deltaTime) {
    float velocity = movementSpeed * deltaTime;  // 이동 속도 계산
    
    // 방향에 따라 카메라 위치 업데이트
    if (direction == FORWARD)
        position += front * velocity;
    if (direction == BACKWARD)
        position -= front * velocity;
    if (direction == LEFT)
        position -= right * velocity;
    if (direction == RIGHT)
        position += right * velocity;
}

// 마우스 이동 처리: 카메라 방향 변경
// xoffset: 마우스의 x축 이동량
// yoffset: 마우스의 y축 이동량
// constrainPitch: 수직 회전 제한 여부
void Camera::ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch) {
    xoffset *= mouseSensitivity;  // 마우스 감도 적용
    yoffset *= mouseSensitivity;

    yaw   += xoffset;  // 좌우 회전 각도 업데이트
    pitch += yoffset;  // 상하 회전 각도 업데이트

    // 수직 회전 제한 (-89도 ~ +89도)
    if (constrainPitch) {
        if (pitch > 89.0f)
            pitch = 89.0f;
        if (pitch < -89.0f)
            pitch = -89.0f;
    }

    updateCameraVectors();  // 카메라 방향 벡터 업데이트
}

// 마우스 스크롤 처리: 줌 인/아웃
// yoffset: 스크롤 이동량
void Camera::ProcessMouseScroll(float yoffset) {
    zoom -= yoffset;  // 시야각 조정
    
    // 시야각 제한 (1도 ~ 45도)
    if (zoom < 1.0f)
        zoom = 1.0f;
    if (zoom > 45.0f)
        zoom = 45.0f;
}

// 카메라의 방향 벡터들을 업데이트
// front, right, up 벡터를 현재 회전 각도에 맞게 계산
void Camera::updateCameraVectors() {
    // 구면 좌표계를 이용해 front 벡터 계산
    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    this->front = glm::normalize(front);
    
    // front 벡터와 worldUp을 이용해 right, up 벡터 계산
    right = glm::normalize(glm::cross(this->front, worldUp));
    up    = glm::normalize(glm::cross(right, this->front));
    
    // Roll 회전 적용
    glm::mat4 rollMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(roll), this->front);
    up    = glm::vec3(rollMatrix * glm::vec4(up, 0.0f));
    right = glm::vec3(rollMatrix * glm::vec4(right, 0.0f));
}

