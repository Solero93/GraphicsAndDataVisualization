#include "llum.h"

// Crear llum puntual per aquesta pràctica
Llum::Llum(vec3 pos, vec3 amb, vec3 dif, vec3 spec, vec3 atenuate){
    this->position = pos;
    this->ambient = amb;
    this->diffuse = dif;
    this->specular = spec;
    this->atenuate = atenuate;
}
