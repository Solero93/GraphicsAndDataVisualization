#include "llum.h"

Llum::Llum(Lights t) {
    this->setTipusLlum(t);
}

void Llum::setTipusLlum(Lights t) {
    switch(t) {
    case Puntual:
        this->position = vec4(2.0,2.0,2.0,1.0);
        this->direction = NONE;
        this->ambient = vec3(0.5,0.5,0.5);
        this->diffuse = vec3(0.5,0.5,0.5);
        this->specular = vec3(0.5,0.5,0.5);
        this->angle = 0.0;
        break;
    case Direccional:
        this->position = NONE;
        this->direction = vec4(2.0,2.0,2.0,0.0);
        this->ambient = vec3(0.5,0.5,0.5);
        this->diffuse = vec3(0.5,0.5,0.5);
        this->specular = vec3(0.5,0.5,0.5);
        this->angle = 0.0;
        break;
    case SpotLight:
        this->position = vec4(0.0, -5.0, 0.0, 1.0);
        this->direction = vec4(2, 2, 2, 0);
        this->ambient = vec3(0.5,0.5,0.5);
        this->diffuse = vec3(0.5,0.5,0.5);
        this->specular = vec3(0.5,0.5,0.5);
        this->angle = 1.0f;
        break;
    }
    this->isActive = true;
}

vec4 Llum::getPosition() {
    return this->position;
}
vec4 Llum::getDirection(){
    return this->direction;
}
vec3 Llum::getAmbient() {
    return isActive ? this->ambient: vec3(0.0,0.0,0.0);
}
vec3 Llum::getDiffuse() {
    return isActive ? this->diffuse : vec3(0.0,0.0,0.0);
}
vec3 Llum::getSpecular() {
    return isActive ? this->specular : vec3(0.0,0.0,0.0);
}
float Llum::getAngle() {
    return this->angle;
}

void Llum::setPosition(vec4 v) {
    this->position = v;
}
void Llum::setDirection(vec4 v) {
    this->direction = v;
}
void Llum::setAmbient(vec3 i){
    this->ambient = i;
}
void Llum::setDiffuse(vec3 i) {
    this->diffuse = i;
// el float que es reb ha de multiplicar els tres valors de la intensitat digusa de la llum
}
void Llum::setSpecular(vec3 i){
    this->specular = i;
}
void Llum::setAngle(float a){
    this->angle = a;
}

void Llum::switchOnOff() {
    this->isActive = !this->isActive;
    // Si esta en on la posa a off i a l'inreves
}
