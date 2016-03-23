#include "material.h"

vec3 GOLD_AMBIENT = (0.24725, 0.1995, 0.0745);
vec3 GOLD_DIFFUSE = (0.7516, 0.6064, 0.2264);
vec3 GOLD_SPECULAR = (0.628281, 0.555802, 0.366065);
float GOLD_SHININESS = 0.4;

vec3 EMERALD_AMBIENT = (0.0215, 0.1745, 0.0215);
vec3 EMERALD_DIFFUSE = (0.07568, 0.61424, 0.07568);
vec3 EMERALD_SPECULAR = (0.633, 0.727811, 0.633);
float EMERALD_SHININESS = 0.6;


Material::Material() {
    ambient = vec3(0.0215, 0.1745, 0.0215);
    diffuse = vec3(0.07568, 0.61424, 0.07568);
    specular = vec3(0.633, 0.727811, 0.633);
    shininess = 0.6;
}

/*Material::~Material(){
    delete ambient;
    delete diffuse;
    delete specular;
    delete shininess;
}*/

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

    glUniform3fv(gl_buffer.ambient,1,ambient);
    glUniform3fv(gl_buffer.diffuse,1,diffuse);
    glUniform3fv(gl_buffer.specular,1,specular);
    glUniform1f(gl_buffer.shininess,shininess);
}
