#ifndef LLUM_H
#define LLUM_H

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

using namespace glm;

#define NO_VECTOR vec3(0.0,0.0,0.0)

// Tipus de llums

// Classe que representa els atributs d'una llum
class Llum {
    public:
        vec3 position;
        vec3 ambient;
        vec3 diffuse;
        vec3 specular;
        vec3 atenuate;

        Llum(vec3 pos, vec3 amb=vec3(0.2,0.2,0.2), vec3 dif=vec3(0.8,0.8,0.8),
             vec3 spec=vec3(1.0,1.0,1.0), vec3 atenuate=vec3(1.0,0.0,0.0));
};

#endif // LLUM_H
