#include "llum.h"

Llum::Llum(Lights t) {
    this->setTipusLlum(t);
}

void Llum::setTipusLlum(Lights t) {
    switch(t) {
    case Puntual:
        this->position = vec3(0.0,0.0,9.0);
        this->direction = NO_VECTOR;
        this->ambient = vec3(0.2,0.2,0.2);
        this->diffuse = vec3(0.8,0.8,0.8);
        this->specular = vec3(1.0,1.0,1.0);
        this->angle = 0.0;
        this->atenuate = vec3(1.0,0.0,0.0);
        break;
    case Direccional:
        this->position = NO_VECTOR;
        this->direction = -vec3(2.0,2.0,2.0);
        this->ambient = vec3(0.2,0.2,0.2);
        this->diffuse = vec3(0.8,0.8,0.8);
        this->specular = vec3(1.0,1.0,1.0);
        this->angle = 0.0;
        this->atenuate = vec3(1.0,0.0,0.0);
        break;
    case SpotLight:
        this->position = vec3(0.0, -1.0, 0.0);
        this->direction = vec3(0.0, 1.0, 0.0);
        this->ambient = vec3(0.2,0.2,0.2);
        this->diffuse = vec3(0.5,0.5,0.5);
        this->specular = vec3(0.5,0.5,0.5);
        this->angle = 1.0f;
        this->atenuate = vec3(1.0,0.0,0.0);
        break;
    }
    this->isActive = true;
}

vec3 Llum::getPosition() {
    return this->position;
}
vec3 Llum::getDirection(){
    return this->direction;
}
vec3 Llum::getAmbient() {
    return isActive ? this->ambient: NO_VECTOR;
}
vec3 Llum::getDiffuse() {
    return isActive ? this->diffuse : NO_VECTOR;
}
vec3 Llum::getSpecular() {
    return isActive ? this->specular : NO_VECTOR;
}
float Llum::getAngle() {
    return this->angle;
}
vec3 Llum::getAtenuate(){
    return this->atenuate;
}

void Llum::setPosition(vec3 v) {
    this->position = v;
}
void Llum::setDirection(vec3 v) {
    this->direction = v;
}
void Llum::setAmbient(vec3 i){
    this->ambient = i;
}
void Llum::setDiffuse(vec3 i) {
    this->diffuse = i;
}
void Llum::setSpecular(vec3 i){
    this->specular = i;
}
void Llum::setAngle(float a){
    this->angle = a;
}
void Llum::setAtenuate(vec3 at){
    this->atenuate = at;
}

void Llum::switchOnOff() {
    this->isActive = !this->isActive;
}
