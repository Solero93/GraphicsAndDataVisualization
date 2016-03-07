#include "material.h"

Material::Material() {
    // yellow rubber
    ambient = vec3(0.05, 0.05, 0.0);
    diffuse = vec3(0.05, 0.05, 0.04);
    specular = vec3(0.07, 0.07, 0.04);
    shininess = 0.078125;
}

Material::~Material(){

}

void Material::toGPU(QGLShaderProgram *pr) {
    gl_buffer.ambient = pr->uniformLocation("bufferMat.ambient");
    gl_buffer.diffuse = pr->uniformLocation("bufferMat.diffuse");
    gl_buffer.shininess = pr->uniformLocation("bufferMat.shininess");
    gl_buffer.specular = pr->uniformLocation("bufferMat.specular");
   // glGenBuffers( 1, &gl_buffer );
   // glBindBuffer( GL_ARRAY_BUFFER, gl_buffer );

    //glBufferData( GL_ARRAY_BUFFER, sizeof(point4)*Index + sizeof(point4)*Index, NULL, GL_STATIC_DRAW );
   // glEnable( GL_DEPTH_TEST );
//    glEnable( GL_TEXTURE_2D );
    glUniform3fv(gl_buffer.ambient,1,ambient);
    glUniform3fv(gl_buffer.diffuse,1,diffuse);
    glUniform3fv(gl_buffer.specular,1,specular);
    glUniform1f(gl_buffer.shininess,shininess);
}
