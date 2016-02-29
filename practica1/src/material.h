#ifndef MATERIAL_H
#define MATERIAL_H

#include <vector>

// Classe que representa les propietats optiques d'un objecte
class Material {

public:
    Material();
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

#endif // MATERIAL_H
