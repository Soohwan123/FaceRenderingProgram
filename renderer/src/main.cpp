#ifdef _WIN32
#include <Windows.h>
#include <direct.h>
#define GetCurrentDir _getcwd
#else
#include <unistd.h>
#define GetCurrentDir getcwd
#endif
#include "../../external/glew/include/GL/glew.h"
#include "../../external/glfw/include/GLFW/glfw3.h"
#include <iostream>
#include "../../external/glm/glm/glm.hpp"
#include "../../external/glm/glm/gtc/matrix_transform.hpp"
#include "../../external/glm/glm/gtc/type_ptr.hpp"

#include "shader.h"
#include "model.h"
#include "mesh.h"
#include "camera.h"

// 윈도우 크기
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// 카메라
Camera* g_camera = nullptr;
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// 타이밍
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// 콜백 함수들
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);

int main() {
    /**************************초기 설정********************************* */
    // 콘솔 한글 출력을 위한 설정
    #ifdef _WIN32
        SetConsoleOutputCP(CP_UTF8);
    #endif
 // GLFW 초기화
    if (!glfwInit()) {
        std::cout << "GLFW 초기화 실패" << std::endl;
        return -1;
    }
    std::cout << "GLFW 초기화 성공" << std::endl;

    // OpenGL 버전 설정
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    std::cout << "OpenGL 버전 설정: 4.0" << std::endl;

    // 윈도우 생성
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Face Renderer", NULL, NULL);
    if (window == NULL) {
        std::cout << "윈도우 생성 실패" << std::endl;
        glfwTerminate();
        return -1;
    }
    std::cout << "윈도우 생성 성공" << std::endl;

    glfwMakeContextCurrent(window);
    std::cout << "OpenGL 컨텍스트 설정 완료" << std::endl;

    // GLEW 초기화
    if (glewInit() != GLEW_OK) {
        std::cout << "GLEW 초기화 실패" << std::endl;
        return -1;
    }
    std::cout << "GLEW 초기화 성공" << std::endl;

    // OpenGL 버전 체크
    const GLubyte* version = glGetString(GL_VERSION);
    const GLubyte* glslVersion = glGetString(GL_SHADING_LANGUAGE_VERSION);
    std::cout << "OpenGL 버전: " << version << std::endl;
    std::cout << "GLSL 버전: " << glslVersion << std::endl;

    // GLEW 초기화 후에 추가
    // 깊이 테스트
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS); 

    // 콜백 함수 설정
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // 마우스 커서 설정
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);


    /**************************초기 설정********************************* */


    /**************************렌더링 설정******************************** */

    // 셰이더 로드 
    Shader shader("shaders/vertex.glsl", 
                 "shaders/fragment.glsl");
    shader.use();
    //shader.setInt("texture1", 0);  // 텍스처 유닛 0 사용

    // 모델 로드
    Model model("models/face.obj");

    // 텍스쳐 바인딩
    unsigned int textureID = model.loadTexture("models/skin_texture.jpg");
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureID);


    // 코의 중심점 계산
    // 모델의 vertices를 변환하여 적절한 위치와 크기로 조정
    float scale = 0.02f;
    glm::mat4 initialTransform = glm::mat4(1.0f);
    initialTransform = glm::scale(initialTransform, glm::vec3(scale));
    // roll 180도 회전
    initialTransform = glm::rotate(initialTransform, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    
    // vertices 변환
    std::vector<Vertex> transformedVertices = model.vertices;
    for(auto& vertex : transformedVertices) {
        glm::vec4 transformed = initialTransform * glm::vec4(vertex.Position, 1.0f);
        vertex.Position = glm::vec3(transformed);
    }
    
    // 변환된 vertices로 새로운 코의 중심점 계산
    glm::vec3 noseCenter = glm::vec3(0.0f);
    for(unsigned int idx : model.noseIndices) {
        noseCenter += transformedVertices[idx].Position;
    }
    noseCenter /= model.noseIndices.size();
    
    // 카메라를 변환된 코의 중심점 앞에 위치시킴
    glm::vec3 cameraPos = noseCenter + glm::vec3(0.0f, 0.0f, -10.0f);  // z축으로 2단위 뒤에 위치
    g_camera = new Camera(cameraPos);
    g_camera->ProcessMouseMovement(0, 0, false);  // 0,0으로 움직여서 view matrix 초기화
    
    // 변환된 vertices로 메시 생성
    Mesh mesh(transformedVertices, model.indices);

    // 렌더링 루프
    while (!glfwWindowShouldClose(window)) {
        // 프레임 시간 계산
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        model.loadObj("models/face.obj");

            unsigned int textureID = model.loadTexture("models/skin_texture.jpg");
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureID);


    // 코의 중심점 계산
    // 모델의 vertices를 변환하여 적절한 위치와 크기로 조정
    float scale = 0.02f;
    glm::mat4 initialTransform = glm::mat4(1.0f);
    initialTransform = glm::scale(initialTransform, glm::vec3(scale));
    // roll 180도 회전
    initialTransform = glm::rotate(initialTransform, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    
    // vertices 변환
    std::vector<Vertex> transformedVertices = model.vertices;
    for(auto& vertex : transformedVertices) {
        glm::vec4 transformed = initialTransform * glm::vec4(vertex.Position, 1.0f);
        vertex.Position = glm::vec3(transformed);
    }
    
    // 변환된 vertices로 새로운 코의 중심점 계산
    glm::vec3 noseCenter = glm::vec3(0.0f);
    for(unsigned int idx : model.noseIndices) {
        noseCenter += transformedVertices[idx].Position;
    }
    noseCenter /= model.noseIndices.size();

        // 입력 처리
        processInput(window);

        // 화면 클리어
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // 셰이더 사용
        shader.use();
        shader.setInt("texture1", 0);

        // 변환 행렬
        glm::mat4 model = glm::mat4(1.0f);
    
        shader.setMat4("model", glm::value_ptr(model));
        shader.setMat4("view", glm::value_ptr(g_camera->GetViewMatrix()));
        shader.setMat4("projection", glm::value_ptr(g_camera->GetProjectionMatrix((float)SCR_WIDTH / (float)SCR_HEIGHT)));

        // 조명 설정
        glm::vec3 lightPos(2.0f, 2.0f, 2.0f);
        glm::vec3 lightColor(1.0f, 1.0f, 1.0f); // 흰색

        shader.setVec3("lightPos", glm::value_ptr(lightPos));
        shader.setVec3("lightColor", glm::value_ptr(lightColor));
        shader.setVec3("viewPos", glm::value_ptr(g_camera->GetProjectionMatrix((float)SCR_WIDTH / (float)SCR_HEIGHT)));

        // 렌더링 루프 안에서, mesh.Draw() 전에
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        // 메시 그리기
        mesh.Draw();

        // 버퍼 스왑
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // 종료
    glfwTerminate();
    delete g_camera;
    return 0;
}

// 뷰포트 크기 조정 콜백
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

// 마우스 이동 콜백
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn) {
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;

    lastX = xpos;
    lastY = ypos;

    g_camera->ProcessMouseMovement(xoffset, yoffset, false);
}

// 마우스 스크롤 콜백
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    g_camera->ProcessMouseScroll(static_cast<float>(yoffset));
}

// 키보드 입력 처리
void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        g_camera->ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        g_camera->ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        g_camera->ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        g_camera->ProcessKeyboard(RIGHT, deltaTime);
}

