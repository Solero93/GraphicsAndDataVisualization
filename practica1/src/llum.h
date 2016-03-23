#ifndef LLUM_H
#define LLUM_H

#define NONE vec4(0.0,0.0,0.0,0.0)

#include <Common.h>

// Tipus de llums
enum Lights {Puntual, Direccional, SpotLight};

// Classe que representa els atributs d'una llum
class Llum {
    public:
        GLuint id;
        vec4 position;
        vec4 direction;
        vec3 ambient;
        vec3 diffuse;
        vec3 specular;
        float angle;
        bool isActive;

        Llum(Lights l);
        void setTipusLlum(Lights t);
        vec3 getDiffuseIntensity();
        vec4 getLightPosition();
        void setDiffuseIntensity(vec3 i);
        void setLightPosition(vec4 v);
        void switchOnOff();
};

#endif // LLUM_H
