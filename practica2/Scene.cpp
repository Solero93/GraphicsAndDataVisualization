#include "Scene.h"
#include <iostream>

using namespace std;

Scene::Scene()
{
    cam = new Camera();

    this->llumAmbient = vec3(0.1,0.1,0.1);

    // 4 spheres and a triangle with default Material
    this->objects.push_back(new Sphere(vec3(0.0,0.0,0.0),1.0));
    this->objects.push_back(new Sphere(vec3(0.0,3.0,0.0),1.0));
    this->objects.push_back(new Sphere(vec3(3.0,0.0,0.0),1.0));
    this->objects.push_back(new Sphere(vec3(0.0,0.0,3.0),1.0));
    this->objects.push_back(new Triangle(vec3(-2.0,0.0,0.0),vec3(-2.0,3.0,0.0), vec3(-1.5,0.0,3.0)));

    // Material of planes
    Material matPlane1(vec3(0.0,0.0,0.0),vec3(0.55,0.55,0.55),vec3(0.05,0.05,0.05),32.0);
    Material matPlane2(vec3(0.0,0.0,0.0),vec3(0.0,0.0,0.95),vec3(0.5,0.5,0.5),32.0);
    Material matPlane3(vec3(0.0,0.0,0.0),vec3(0.0,0.6,0.5),vec3(0.1,0.1,0.1),32.0);

    // 3 planes with that material
    this->objects.push_back(new Plane(vec3(-10.0,-3.0,-10.0),vec3(0.0,1.0,0.0), mat4(1.0f), matPlane1));
    this->objects.push_back(new Plane(vec3(-10.0,-10.0,-3.0),vec3(0.0,0.0,1.0), mat4(1.0f), matPlane2));
    this->objects.push_back(new Plane(vec3(4.0,-10.0,-10.0),vec3(-1.0,0.0,0.0), mat4(1.0f), matPlane3));

    // Frontal and up lights
    this->addLlum(new Llum(vec3(2.0,2.0,2.0)));
    this->addLlum(new Llum(vec3(0.0,5.0,0.0)));
}

Scene::~Scene()
{
    delete cam;
    for(unsigned int i = 0; i < objects.size(); ++i){
        if(objects[i]){
            delete objects[i];
        }
    }
    llums.clear();
}


/*
** Metode que testeja la interseccio contra tots els objectes de l'escena - troba el més proper
*/

bool Scene::CheckIntersection(const Ray &ray, IntersectInfo &info) {
    IntersectInfo tmp;
    bool trobat = false;
    for(unsigned int i = 0; i < objects.size(); i++) {
        if (objects[i]->Intersect(ray, tmp) && tmp.time > 0.0f && tmp.time < info.time){
            info.hitPoint = tmp.hitPoint;
            info.material = tmp.material;
            info.normal = tmp.normal;
            info.time = tmp.time;
            trobat = true;
        }
    }
    return trobat;
}

/*
 * Metode d'interseccio que mira si entre la llum i l'objecte hi ha algun altre objecte
 */
bool Scene::CheckIntersectionShadow(const Ray &ray) {
    IntersectInfo tmp;
    for(unsigned int i = 0; i < objects.size(); i++) {
        if (objects[i]->Intersect(ray, tmp) && tmp.time > 0.0f && tmp.time < 1.0f){
            return true;
        }
    }
    return false;
}

/*
** Aquest metode es crida per a cada pixel, i cada cop que es calculen ombres o reflexions
** El paràmetre PayLoad es pot usar per a guardar informacio de sobre el quin es color actual del raig i el nombre de
** rebots que s'han fet.
** Aquest metode retorna el delta del raig (o t) en el cas que hi hagi interseccio o
** -1 si no hi ha interseccio amb cap objecte.
*/
float Scene::CastRay(Ray &ray, Payload &payload) {

    IntersectInfo info;

    bool intersect = CheckIntersection(ray,info);
    if (intersect) {
        // Si el color calculat nou no aporta gaire o ens hem passat del nombre màxim de rebots, parem.
        if (payload.numBounces >= MAX_REFLECT) {
            payload.color = shade(info,ray);
            return info.time;
        }
        // En cas contrari, calculem raig reflectit i seguim amb la recursió.
        vec3 R = -(2.0f*dot(info.normal,ray.direction))*info.normal + ray.direction;
        Ray reflected(info.hitPoint + EPSILON*R, R);
        payload.numBounces++;
        CastRay(reflected, payload);

        payload.color = shade(info,ray) + info.material->specular * payload.color;
        return info.time;
    }
    else {
        // Si no interseca, posem el color de fons (negre) i parem.
        payload.color = vec3(0.0f);
        return -1.0f;
    }
}

/*
 * Métode que retorna el color a posar per un punt en concret (ombra o Blinn-Phong)
 */
vec3 Scene::shade(IntersectInfo info, Ray ray){
    vec3 c = llumAmbient * info.material->ambient;

    for(int i=0; i < llums.size(); i++){
        // Pertorbem el punt d'intersecció, fem el raig i veïem si fer ombra o calcular Blinn-Phong
        vec3 puntE = info.hitPoint + EPSILON*(llums[i]->position - info.hitPoint);
        vec3 L = llums[i]->position - puntE;
        Ray r(puntE, L);
        // Si el raig interseca amb algun objecte fins arribar a la llum, fem ombra, si no, Blinn-Phong
        if (CheckIntersectionShadow(r)) {
            c += llums[i]->ambient * info.material->ambient;
        } else {
            c += calculatePhong(i, info, r);
        }
    }
    return c;
}

void Scene::addLlum(Llum *l) {
    llums.push_back(l);
}

/*
 * Aplicar el model Blinn-Phong amb atenuació de la llum
 */
vec3 Scene::calculatePhong(int j, IntersectInfo info, Ray &ray)
{
    vec3 L, H, N=normalize(info.normal);
    vec3 diffuseTmp, specularTmp, ambientTmp;

    L = normalize(llums[j]->position - ray.origin);
    H = normalize(L + normalize(cam->obs - ray.origin));

    diffuseTmp = info.material->diffuse * llums[j]->diffuse * glm::max(dot(L,N),0.0f);
    specularTmp = info.material->specular * llums[j]->specular * pow(glm::max(dot(N,H),0.0f), info.material->shininess);
    ambientTmp = info.material->ambient * llums[j]->ambient;

    float atenuation = atenuateFactor(j, llums[j]->atenuate, ray.origin);

    return (diffuseTmp + specularTmp + ambientTmp) * atenuation;
}

/*
 * Calcula la atenuació de la llum actual
 */
float Scene::atenuateFactor(int j, vec3 atenuate, vec3 hitPoint){
    vec3 rayDirection = llums[j]->position - hitPoint;
    float a = atenuate[0], b = atenuate[1], c = atenuate[2];
    float d = length(rayDirection);
    return 1.0f/(a + b*d + c*d*d);
}
