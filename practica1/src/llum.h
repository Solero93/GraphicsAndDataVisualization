#ifndef LLUM_H
#define LLUM_H

#include <Common.h>

// Tipus de llums
enum Lights {Puntual, Direccional, SpotLight};

// Classe que representa els atributs d'una llum
class Llum {
    public:
        Llum(Lights l);
        void setTipusLlum(Lights t);
        vec3 getDiffuseIntensity();
        vec4 getLightPosition();
        void setDiffuseIntensity(vec3 i);
        void setLightPosition(vec4 v);
        void switchOnOff();
};

#endif // LLUM_H
