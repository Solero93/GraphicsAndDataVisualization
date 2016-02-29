#include "material.h"

Material::Material() {
    // yellow rubber
    ambient = (0.05, 0.05, 0.0, 1.0);
    diffuse = (0.05, 0.05, 0.04, 1.0);
    specular = (0.07, 0.07, 0.04, 1.0);
    shininess = 0.078125;
}
