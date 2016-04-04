#include "mon.h"

Mon::Mon() {
    this->llumAmbient = vec3(0.05,0.05,0.05);
}

Mon::~Mon() {
    elements.clear();
    llums.clear();
}

void Mon::addObjecte(Objecte *obj) {
    elements.push_back(obj);
}

void Mon::draw() {
    for(unsigned int i=0; i < elements.size(); i++){
        elements.at(i)->draw();
    }
}


Llum* Mon::getLlumActual() {
    // Ara per ara dona com a llum actual la darrera que s'ha inserit, però es podria canviar segons el
    // que es vulgui fer en la GUI
    return (llums[llums.size()-1]);
}

void Mon::llumsToGPU(QGLShaderProgram *pr){
    struct gl_idBuffer{
        GLuint position;
        GLuint direction;
        GLuint ambient;
        GLuint diffuse;
        GLuint specular;
        GLuint angle;
        GLuint atenuate;
    };

    gl_idBuffer gl_buffer[llums.size()];

    for (int i=0; i<this->llums.size(); i++){
        gl_buffer[i].position = pr->uniformLocation(QString("bufferLights[%1].position").arg(i));
        gl_buffer[i].direction = pr->uniformLocation(QString("bufferLights[%1].direction").arg(i));
        gl_buffer[i].ambient = pr->uniformLocation(QString("bufferLights[%1].ambient").arg(i));
        gl_buffer[i].diffuse = pr->uniformLocation(QString("bufferLights[%1].diffuse").arg(i));
        gl_buffer[i].specular = pr->uniformLocation(QString("bufferLights[%1].specular").arg(i));
        gl_buffer[i].angle = pr->uniformLocation(QString("bufferLights[%1].angle").arg(i));
        gl_buffer[i].atenuate = pr->uniformLocation(QString("bufferLights[%1].atenuate").arg(i));

        glUniform4fv(gl_buffer[i].position,1,this->llums[i]->getPosition());
        glUniform4fv(gl_buffer[i].direction,1,this->llums[i]->getDirection());
        glUniform3fv(gl_buffer[i].ambient,1,this->llums[i]->getAmbient());
        glUniform3fv(gl_buffer[i].diffuse,1,this->llums[i]->getDiffuse());
        glUniform3fv(gl_buffer[i].specular,1,this->llums[i]->getSpecular());
        glUniform1f(gl_buffer[i].angle,this->llums[i]->getAngle());
        glUniform3fv(gl_buffer[i].atenuate,1,this->llums[i]->getAtenuate());

        this->llums[i]->id = i;
    }
    GLuint numLlums = pr->uniformLocation("numLlums");
    glUniform1i(numLlums, this->llums.size());
}

void Mon::setAmbientToGPU(QGLShaderProgram *program){
    GLuint llumAmbient = program->uniformLocation("llumAmbient");
    glUniform3fv(llumAmbient,1,this->llumAmbient);
}

void Mon::addLlum(Llum *l) {
    llums.push_back(l);
}
