#include "shader.h"
#include <cstddef>
#include <fstream>
#include <sstream>
#include <iostream>

Shader::Shader(const char* vertexPath, const char* fragmentPath) {
    std::cout << "Loading shaders from: " << std::endl;
    std::cout << "Vertex: " << vertexPath << std::endl;
    std::cout << "Fragment: " << fragmentPath << std::endl;
    //셰이더 코드 읽기
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;

    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try{
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);
        std::stringstream vShaderStream, fShaderStream;

        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();

        vShaderFile.close();
        fShaderFile.close();

        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
        
        // 읽은 셰이더 코드 출력
        std::cout << "\nVertex Shader Code:\n" << vertexCode << std::endl;
        std::cout << "\nFragment Shader Code:\n" << fragmentCode << std::endl;
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
    //컴파일 디버그
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if(!success){
        glGetShaderInfoLog(vertex, 512, NULL, infoLog);
        std::cout << "버텍스 셰이더 컴파일 실패: " << infoLog << std::endl;
    }


    // 프래그먼트 셰이더
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    //컴파일 디버그
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if(!success){
        glGetShaderInfoLog(fragment, 512, NULL, infoLog);
        std::cout << "프래그먼트 셰이더 컴파일 실패: " << infoLog << std::endl;
    }

    //셰이더 프로그램
    ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);
    
    //컴파일 디버그
    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if(!success){
        glGetProgramInfoLog(ID, 512, NULL, infoLog);
        std::cout << "셰이더 프로그램 링크 실패: " << infoLog << std::endl;
    }

     // 컴파일 성공 여부 출력
    std::cout << "Shader compilation " << 
        (success ? "successful" : "failed") << std::endl;

    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

void Shader::use(){
    glUseProgram(ID);
}

void Shader::setBool(const std::string &name, bool value) const{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void Shader::setInt(const std::string &name, int value) const{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setFloat(const std::string &name, float value) const{
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setMat4(const std::string &name, const float* value) const {
    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, value);
}

Shader::~Shader(){
    glDeleteProgram(ID);
}

