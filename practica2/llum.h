#ifndef LLUM_H
#define LLUM_H

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

using namespace glm;

#define NO_VECTOR vec4(0.0,0.0,0.0,0.0)
#define NO_COLOR vec3(0.0,0.0,0.0)

// Tipus de llums
enum Lights {Puntual, Direccional, SpotLight};

// Classe que representa els atributs d'una llum
class Llum {
    public:
        vec4 position;
        vec4 direction;
        vec3 ambient;
        vec3 diffuse;
        vec3 specular;
        float angle;
        vec3 atenuate;
        bool isActive;

        Llum(Lights l);
        void setTipusLlum(Lights t);
        vec4 getPosition();
        vec4 getDirection();
        vec3 getAmbient();
        vec3 getDiffuse();
        vec3 getSpecular();
        float getAngle();
        vec3 getAtenuate();

        void setPosition(vec4 v);
        void setDirection(vec4 v);
        void setAmbient(vec3 i);
        void setDiffuse(vec3 i);
        void setSpecular(vec3 i);
        void setAngle(float a);
        void setAtenuate(vec3 at);
        void switchOnOff();
};

#endif // LLUM_H
