#include "Scene.h"
#include <iostream>

using namespace std;

Scene::Scene()
{
    cam = new Camera();

    this->llumAmbient = vec3(0.1,0.1,0.1);

    this->objects.push_back(new Sphere(vec3(0.0,0.0,0.0),1.0));
    this->objects.push_back(new Sphere(vec3(0.0,3.0,0.0),1.0));
    this->objects.push_back(new Sphere(vec3(3.0,0.0,0.0),1.0));
    this->objects.push_back(new Sphere(vec3(0.0,0.0,3.0),1.0));

    Material mat1(vec3(0.0,0.0,0.0),vec3(0.55,0.55,0.55),vec3(0.05,0.05,0.05),32.0);
    Material mat2(vec3(0.0,0.0,0.0),vec3(0.0,0.0,0.95),vec3(0.5,0.5,0.5),32.0);
    Material mat3(vec3(0.0,0.0,0.0),vec3(0.0,0.6,0.5),vec3(0.1,0.1,0.1),32.0);

    this->objects.push_back(new Plane(vec3(-10.0,-3.0,-10.0),vec3(0.0,1.0,0.0), mat4(1.0f), mat1));
    this->objects.push_back(new Plane(vec3(-10.0,-10.0,-3.0),vec3(0.0,0.0,1.0), mat4(1.0f), mat2));
    this->objects.push_back(new Plane(vec3(4.0,-10.0,-10.0),vec3(-1.0,0.0,0.0), mat4(1.0f), mat3));
    this->objects.push_back(new Triangle(vec3(-2.0,0.0,0.0),vec3(-2.0,3.0,0.0), vec3(-1.5,0.0,3.0)));

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
** TODO: Metode que testeja la interseccio contra tots els objectes de l'escena
**
** Si un objecte es intersecat pel raig, el parametre  de tipus IntersectInfo conte
** la informació sobre la interesccio.
** El metode retorna true si algun objecte es intersecat o false en cas contrari.
**
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

bool Scene::CheckIntersectionLlum(const Ray &ray) {
    IntersectInfo tmp;
    for(unsigned int i = 0; i < objects.size(); i++) {
        if (objects[i]->Intersect(ray, tmp) && tmp.time > 0.0f && tmp.time <= 1.0f){
            return true;
        }
    }
    return false;
}

/*
** TODO: Funcio recursiva del RayTracing.
** A modificar en el punt 4 de l'enunciat per incloure Blinn-Phong  i ombres
** A modificar en el punt 5 per a tractar reflexions
**
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
        /* TODO: Canviar aquesta assignacio pel color basat amb la il.luminacio basada amb Phong-Blinn segons
         * el material de l'objecte i les llums per l'apartat 4 de la practica
         * I per l'apartat 5, cal fer que tambe es tinguin en compte els rebots de les reflexions.

           Inicialment s'ha posat la direccio del raig per tenir un color diferents per a cada pixel pero
           payload ha d'anar tenint el color actualitzat segons els rebots.
        */
        /*if (length(phongColor-payload.color) < TOL){
            cout << "max_tol" << endl;
            return info.time;
        }*/
        if (payload.numBounces >= MAX_REFLECT) {
            payload.color = shade(info,ray);
            return info.time;
        }
        vec3 R = -(2.0f*dot(info.normal,ray.direction))*info.normal + ray.direction;
        Ray reflected(info.hitPoint + EPSILON*R, R);
        payload.numBounces++;
        CastRay(reflected, payload);

        payload.color = shade(info,ray) + info.material->specular * payload.color;
        return info.time;
    }
    else {
        payload.color = vec3(0.0f);
        // Si el ray des de la camera no intersecta amb cap objecte
        // no s'ha de veure res, encara que també es podria posar el color de la Intensita ambien global
        return -1.0f;
    }
}

vec3 Scene::shade(IntersectInfo info, Ray ray){
    vec3 c = llumAmbient * info.material->ambient;

    for(int i=0; i < llums.size(); i++){
        vec3 puntE = info.hitPoint + EPSILON*(llums[i]->position - info.hitPoint);
        vec3 L = llums[i]->position - puntE;
        Ray r(puntE, L);
        if (CheckIntersectionLlum(r)) {
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

vec3 Scene::calculatePhong(int j, IntersectInfo info, Ray &ray)
{
    vec3 L, H, N=normalize(info.normal);
    vec3 diffuseTmp, specularTmp, ambientTmp;
    float atenuation;
    vec3 puntE = info.hitPoint + EPSILON*(llums[j]->position - info.hitPoint);
    L = normalize(llums[j]->position - puntE);
    H = normalize(L + ray.origin - puntE);

    diffuseTmp = info.material->diffuse * llums[j]->diffuse * glm::max(dot(L,N),0.0f);
    specularTmp = info.material->specular * llums[j]->specular * pow(glm::max(dot(N,H),0.0f), info.material->shininess);
    ambientTmp = info.material->ambient * llums[j]->ambient;

    atenuation = atenuateFactor(j, llums[j]->atenuate, puntE);

    return (diffuseTmp + specularTmp + ambientTmp) * atenuation;
}

float Scene::atenuateFactor(int j, vec3 atenuate, vec3 hitPoint){
    vec3 rayDirection = llums[j]->position - hitPoint;
    float a = atenuate[0], b = atenuate[1], c = atenuate[2];
    float d = length(rayDirection);
    return 1.0/(a + b*d + c*d*d);
}
