#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

using namespace glm;

class Material;

class Ray {
  public:
    vec3 origin;
    vec3 direction;

    Ray(const vec3 &origin, const vec3 &direction):
      origin(origin),
      direction(direction)
    {}

    /* retorna el punt del raig en en temps/lambda t */
    vec3 operator() (const float &t) const {
      return origin + direction*t;
    }
};

class IntersectInfo {
  public:

    IntersectInfo():
      hitPoint(0.0f),
      normal(0.0f),
      time(std::numeric_limits<float>::infinity()),
      material(NULL)
    {}

    
    /* El punt d'interseccio en coordenades 3D */
    vec3 hitPoint;
    /* El vector normal a la superficie en el punt d'interseccio */
    vec3 normal;
    /* El temps/lambda del raig on passa la interseccio */
    float time;
    /* El material de l'objecte intersectat*/
    const Material *material;

    //  "operator =" per la classe  IntersectInfo
    IntersectInfo &operator =(const IntersectInfo &rhs) {
      hitPoint = rhs.hitPoint;
      material = rhs.material;
      normal = rhs.normal;
      time = rhs.time;
      return *this;
    }
};

class Payload {
    // Guarda la informació de la partícula de llum que rebota
    // per l'escena: el color en un cert moment i el nombre de rebots.
    public:
        Payload():
        color(0.0f),
        numBounces(0)
     {}
    
    vec3 color;//  A cada moment de la recursivitat, quan s'intersecta amb algun objecte, es canvia el color del PayLoad
    int numBounces; //  Per a controlar el fi de la recursivitat, amb el nombre de rebots.
};

