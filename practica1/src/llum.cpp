#include "llum.h"

Llum::Llum(Lights t) {
}

void Llum::setTipusLlum(Lights t) {
}

vec3 Llum::getDiffuseIntensity() {
// A canviar
    return(vec3(0.0, 0.0, 0.0));
}

vec4 Llum::getLightPosition() {
    // A canviar
    return(vec4(0.0, 0.0, 0.0, 0.0));
}

void Llum::setDiffuseIntensity(vec3 i) {
// el float que es reb ha de multiplicar els tres valors de la intensitat digusa de la llum
}

void Llum::setLightPosition(vec4 v) {

}

void Llum::switchOnOff() {
    // Si esta en on la posa a off i a l'inreves

}
