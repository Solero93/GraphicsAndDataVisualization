#include "material.h"

Material::Material() {
    //yellow rubber
    ambient = vec3(0.05,0.05,0.0);
    diffuse = vec3(0.5,0.5,0.4);
    specular = vec3(0.7,0.7,0.04);
    shininess = 0.078125;
}

void Material::toGPU(QGLShaderProgram *pr) {
    struct {
        GLuint ambient;
        GLuint diffuse;
        GLuint specular;
        GLuint shininess;
    } gl_buffer;

    gl_buffer.ambient = pr->uniformLocation("bufferMat.ambient");
    gl_buffer.diffuse = pr->uniformLocation("bufferMat.diffuse");
    gl_buffer.shininess = pr->uniformLocation("bufferMat.shininess");
    gl_buffer.specular = pr->uniformLocation("bufferMat.specular");

    glUniform3fv(gl_buffer.ambient, 1, ambient);
    glUniform3fv(gl_buffer.diffuse, 1, diffuse);
    glUniform3fv(gl_buffer.specular, 1, specular);
    glUniform1f(gl_buffer.shininess, shininess);
}
