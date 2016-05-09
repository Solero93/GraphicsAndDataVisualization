#ifndef SCENE_H
#define SCENE_H

#include <llum.h>
#include "Camera.h"
#include "Ray.h"
#include "Object.h"
#include <vector> //Notice that vector in C++ is different from Vector2, Vector3 or similar things in a graphic library.

using namespace glm;

class Scene
{
public:
    Scene();
    ~Scene();

    bool  CheckIntersection(const Ray &ray, IntersectInfo &info);
    float CastRay(Ray &ray, Payload &payload);


    /*
    ** std::vector d'objectes continguts a l'escena
    */
    std::vector<Object*> objects;

    /*
     * camera de l'escena
     */
    Camera *cam;
    /*
     * TODO: Cal afegir el conjunt de llums de l'escena
     */
    std::vector<Llum*> llums;
    void addLlum(Llum *l);

    /*metodes de calcular les llums*/
    float atenuateFactor(int j, vec3 atenuate);
    vec4 calculateH(vec4 L);
    vec4 calculateL(int j);
    vec3 calculatePhong(vec3 /*rellenar*/);
};



#endif // SCENE_H
