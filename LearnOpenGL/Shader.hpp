//
//  Shader.hpp
//  LearnOpenGL
//
//  Created by Loyio Hex on 9/5/22.
//

#ifndef Shader_hpp
#define Shader_hpp

#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{
public:
    // 程序的ID
    unsigned int ID;
    
    // 构造器读取构建着色器
    Shader(const char* vertexPath, const char* fragmentPath);
    
    // 激活程序
    void use();
    
    // uniform 工具函数
    void setBool(const std::string &name, bool value) const;
    void setInt(const std::string &name, int value) const;
    void setFloat(const std::string &name, float value) const;
};

#endif /* Shader_hpp */
