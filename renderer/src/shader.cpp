#include "shader.h"
#include <cstddef>
#include <fstream>
#include <sstream>
#include <iostream>

Shader::Shader(const char* vertexPath, const char* fragmentPath) {
    std::cout << "Shader 생성 시작..." << std::endl;
    std::cout << "Vertex shader path: " << vertexPath << std::endl;
    std::cout << "Fragment shader path: " << fragmentPath << std::endl;

    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;

    // 파일 읽기 실패 처리를 위한 예외 설정
    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try {
        // 셰이더 파일 열기
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);

        // 파일 내용 읽기
        std::stringstream vShaderStream, fShaderStream;
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();

        // 파일 닫기
        vShaderFile.close();
        fShaderFile.close();

        // 스트림을 string으로 변환
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();

        std::cout << "Vertex Shader Code:\n" << vertexCode << std::endl;
        std::cout << "Fragment Shader Code:\n" << fragmentCode << std::endl;
    }
    catch(std::ifstream::failure e){
        std::cout << "셰이더 파일 읽기 실패: " << e.what() << std::endl;
    }
    //C 스타일 string const char* 변환
    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();

    //셰이더 컴파일
    unsigned int vertex, fragment;
    int success;
    char infoLog[512];


    // 버텍스 셰이더
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    
    // 컴파일 에러 체크
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(vertex, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    } else {
        std::cout << "Vertex shader 컴파일 성공" << std::endl;
    }

    // 프래그먼트 셰이더
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    
    // 컴파일 에러 체크
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(fragment, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    } else {
        std::cout << "Fragment shader 컴파일 성공" << std::endl;
    }

    // 셰이더 프로그램
    ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);
    
    // 링킹 에러 체크
    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(ID, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    } else {
        std::cout << "Shader program 링크 성공 (ID: " << ID << ")" << std::endl;
    }

    // 셰이더 삭제
    glDeleteShader(vertex);
    glDeleteShader(fragment);

    std::cout << "Shader 초기화 완료" << std::endl;
}

void Shader::use() {
    // 셰이더 프로그램 활성화
    glUseProgram(ID);
}

void Shader::setBool(const std::string &name, bool value) const{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void Shader::setInt(const std::string &name, int value) const {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setFloat(const std::string &name, float value) const{
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setMat4(const std::string &name, const float* value) const {
    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, value);
}

void Shader::setVec3(const std::string &name, const float* value) const {
    glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, value);
}

Shader::~Shader(){
    glDeleteProgram(ID);
}

