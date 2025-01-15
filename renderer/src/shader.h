#ifndef SHADER_H
#define SHADER_H

#include "../external/glew/include/GL/glew.h"
#include <string>


//각종 쉐이더 기능등을 제공하는 클래스
class Shader {
public:
    unsigned int ID;

    Shader(const char* vertexPath, const char* fragmentPath);
    ~Shader();

    //셰이더 사용
    void use();

    //uniform 변수 설정
    void setBool(const std::string &name, bool value) const;
    void setInt(const std::string &name, int value) const;
    void setFloat(const std::string &name, float value) const;
    void setMat4(const std::string &name, const float* value) const;
};

#endif
