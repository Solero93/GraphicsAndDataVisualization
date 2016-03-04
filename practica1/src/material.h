#ifndef MATERIAL_H
#define MATERIAL_H

#include <QObject>
#include <vector>
#include <Common.h>
#include <cara.h>
#include <QGLShaderProgram>
#include <QOpenGLTexture>

// Classe que representa les propietats optiques d'un objecte
class Material {

typedef Common::vec4  point4;

public:
    Material();
    ~Material();
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
    struct {
        GLuint ambient;
        GLuint diffuse;
        GLuint specular;
        GLuint shininess;
    } gl_buffer;

    virtual void toGPU(QGLShaderProgram *p);
};

#endif // MATERIAL_H
