#include "camera.h"
#include <iostream>

// 생성자: 카메라의 초기 위치와 속성을 설정
// position: 카메라의 시작 위치 (기본값: (0,0,3))
Camera::Camera(glm::vec3 position)
: front(glm::vec3(0.0f, 0.0f, -1.0f)),  // 카메라가 -z 방향을 바라봄
  movementSpeed(2.0f),
  mouseSensitivity(0.1f),
  zoom(45.0f) {
    this->position = position;
    up = glm::vec3(0.0f, 1.0f, 0.0f);  // Y축이 위쪽을 향하도록 수정
    yaw = 90.0f;   // 초기에 -z 방향을 바라보도록
    pitch = 0.0f;   // 수평을 바라보도록
    roll = 0.0f;
}

Camera::~Camera(){

}

// 뷰 mat 반환: 월드 공간을 카메라 공간으로 변환하는 mat
glm::mat4 Camera::GetViewMatrix() {
    return glm::lookAt(position, position + front, up);
}


// 투영 mat 반환: 카메라 공간을 클립 공간으로 변환하는 mat
// aspectRatio: 화면의 가로/세로 비율
glm::mat4 Camera::GetProjectionMatrix(float aspectRatio) {
    // FOV, aspect ratio, near plane, far plane 값 출력
    // std::cout << "Projection settings:" << std::endl;
    // std::cout << "FOV: " << zoom << " degrees" << std::endl;
    // std::cout << "Aspect Ratio: " << aspectRatio << std::endl;
    // std::cout << "Near plane: " << 0.1f << std::endl;
    // std::cout << "Far plane: " << 100.0f << std::endl;

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
    xoffset *= mouseSensitivity;
    yoffset *= mouseSensitivity;

    yaw += xoffset;
    pitch += yoffset;

    // pitch 제한
    if (constrainPitch) {
        if (pitch > 89.0f)
            pitch = 89.0f;
        if (pitch < -89.0f)
            pitch = -89.0f;
    }

    // 카메라 벡터 업데이트
    updateCameraVectors();
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

void Camera::updateCameraVectors() {
    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    this->front = glm::normalize(front);

    // Right 벡터는 Front와 월드 업 벡터의 외적
    right = glm::normalize(glm::cross(this->front, glm::vec3(0.0f, 1.0f, 0.0f)));

    // Up 벡터는 Right와 Front의 외적
    up = glm::normalize(glm::cross(right, this->front));
}

