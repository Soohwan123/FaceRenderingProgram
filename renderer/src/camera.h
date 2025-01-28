#ifndef CAMERA_H
#define CAMERA_H

#include "../../external/glm/glm/glm.hpp"
#include "../../external/glm/glm/gtc/matrix_transform.hpp"


//카메라 이동방향 상수
enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

class Camera {
public:
    //카메라 속성
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 worldUp;

    //오일러 각
    float yaw;
    float pitch;
    float roll;

    //카메라 옵션
    float movementSpeed;
    float mouseSensitivity;
    float zoom;

    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 3.0f));
    ~Camera();

    //행렬 얻기
    glm::mat4 GetViewMatrix();
    glm::mat4 GetProjectionMatrix(float aspect);

    //카메라 이동
    void ProcessKeyboard(int direction, float deltaTime);
    void ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch = true);
    void ProcessMouseScroll(float yoffset);

private:
    //카메라 벡터 업데이트
    void updateCameraVectors();
};

#endif
