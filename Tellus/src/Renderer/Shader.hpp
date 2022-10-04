#pragma once
#include <glm/glm.hpp>

namespace ts {

class Shader {
   public:
    Shader(const char *vertexPath, const char *fragmentShader, const char *geometryFile = nullptr);
    // TEMP -> Not implemented properly
    Shader(const std::string &vertexCode, const std::string &fragmentCode);
    ~Shader();

    // TEMP -> Move to a utility file
    static std::string ReadFile(const char *filepath);

    void Bind() const;
    void Unbind() const;

    void set1i(const char *name, int value) const;
    void set1f(const char *name, float value) const;
    void setVec2fv(const char *name, const glm::vec2 &value) const;
    void setVec3fv(const char *name, const glm::vec3 &value) const;
    void setVec4fv(const char *name, const glm::vec4 &value) const;
    void setMat4fv(const char *name, const glm::mat4 &value, bool transpose = false) const;

   private:
    int getLoc(const char *name) const;
    unsigned int loadShader(unsigned int type, const char *filename);
    void linkProgram(unsigned int vShader, unsigned int fShader, unsigned int gShader);

   private:
    unsigned int m_ID;
};

};  // namespace ts
