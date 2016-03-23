#include "llum.h"

Llum::Llum(Lights t) {
    this->setTipusLlum(t);
}

void Llum::setTipusLlum(Lights t) {
    switch(t) {
    case Puntual:
        this->position = vec4(1.0,0.0,0.0,1.0);
        this->direction = NULL;
        this->ambient = vec3(0.5,0.0,0.0);
        this->diffuse = vec3(0.5,0.0,0.0);
        this->specular = vec3(0.5,0.0,0.0);
        this->angle = NULL;
        break;
    case Direccional:
        this->position = NULL;
        this->direction = vec4(0.0,0.5,0.0);
        this->ambient = vec3(0.0,0.5,0.0);
        this->diffuse = vec3(0.0,0.5,0.0);
        this->specular = vec3(0.0,0.5,0.0);
        this->angle = NULL;
        break;
    case SpotLight:
        this->position = vec4(1.0,0.0,0.0,1.0);
        this->direction = NULL;
        this->ambient = vec3(0.0,0.0,0.5);
        this->diffuse = vec3(0.0,0.0,0.5);
        this->specular = vec3(0.0,0.0,0.5);
        this->angle = 45.0;
        break;
    }
    this->isActive = true;
}

vec3 Llum::getDiffuseIntensity() {
// A canviar
    if (isActive){
        return (this->diffuse);
    } else {
        return (NULL);
    }
}

vec4 Llum::getLightPosition() {
    // A canviar
    if (isActive){
        return (this->position);
    } else {
        return (NULL);
    }
}

void Llum::setDiffuseIntensity(vec3 i) {
    this->diffuse = i;
// el float que es reb ha de multiplicar els tres valors de la intensitat digusa de la llum
}

void Llum::setLightPosition(vec4 v) {
    this->position = v;
}

void Llum::switchOnOff() {
    this->isActive = !this->isActive;
    // Si esta en on la posa a off i a l'inreves

}
