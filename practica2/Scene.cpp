#include "Scene.h"
#include <iostream>

#define WALL_FRONT 5.0
#define WALL_BACK 5.0
#define WALL_LEFT -5.0
#define WALL_RIGHT 5.0
#define WALL_UP 5.0
#define WALL_DOWN -5.0

using namespace std;

Scene::Scene()
{
    // Afegeix la camera a l'escena
    cam = new Camera();
    // TODO: Cal crear els objectes de l'escena (punt 2 de l'enunciat)
    this->llumAmbient = vec3(0.5,0.5,0.5);

    //this->objects.push_back(new Sphere(vec3(0.0,0.0,0.0),1.0));
    //this->objects.push_back(new Plane(vec3(1.0,0.0,WALL_FRONT),vec3(0.0,1.0,WALL_FRONT), vec3(0.0,0.0,WALL_FRONT)));
    this->objects.push_back(new Plane(vec3(1.0, 0.0, WALL_BACK),vec3(0.0, 1.0, WALL_BACK), vec3(0.0, 0.0, WALL_BACK)));
    //this->objects.push_back(new Plane(vec3(1.0,WALL_UP,0.0),vec3(0.0,WALL_UP,1.0), vec3(0.0,WALL_UP,0.0)));
    //this->objects.push_back(new Plane(vec3(1.0,WALL_DOWN,0.0),vec3(0.0,WALL_DOWN,1.0), vec3(0.0,WALL_DOWN,0.0)));
    //this->objects.push_back(new Plane(vec3(WALL_LEFT,1.0,0.0),vec3(WALL_LEFT,0.0,1.0), vec3(WALL_LEFT,0.0,0.0)));
    //this->objects.push_back(new Plane(vec3(WALL_RIGHT,1.0,0.0),vec3(WALL_RIGHT,0.0,1.0), vec3(WALL_RIGHT,0.0,0.0)));
    // TODO: Cal afegir llums a l'escena (punt 4 de l'enunciat)
    this->addLlum(new Llum(Puntual));
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
        if (objects[i]->Intersect(ray, tmp) && tmp.time > 0 && tmp.time < info.time){
            info.hitPoint = tmp.hitPoint;
            info.material = tmp.material;
            info.normal = tmp.normal;
            info.time = tmp.time;
            trobat = true;
        }
    }
    return trobat;
    // TODO: Heu de codificar la vostra solucio per aquest metode substituint el 'return true'
    // Una possible solucio es cridar Intersect per a tots els objectes i quedar-se amb la interseccio
    // mes propera a l'observador, en el cas que n'hi hagi més d'una.
    // Cada vegada que s'intersecta un objecte s'ha d'actualitzar el PayLoad del raig,
    // pero no en aquesta funcio. Per això es posa const en el paràmetre ray, per a
    // que no es canvïi aqui.

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
//	La funcio CastRay ha de calcular la il·luminacio,
//  les ombres i les reflexions.

float Scene::CastRay(Ray &ray, Payload &payload) {

    IntersectInfo info;

    if (CheckIntersection(ray,info)) {
        /* TODO: Canviar aquesta assignacio pel color basat amb la il.luminacio basada amb Phong-Blinn segons
         * el material de l'objecte i les llums per l'apartat 4 de la practica
         * I per l'apartat 5, cal fer que tambe es tinguin en compte els rebots de les reflexions.

           Inicialment s'ha posat la direccio del raig per tenir un color diferents per a cada pixel pero
           payload ha d'anar tenint el color actualitzat segons els rebots.
        */

        payload.color = shade(info,ray);

        return info.time;
    }
    else{
        payload.color = vec3(0.2f);
        // Si el ray des de la camera no intersecta amb cap objecte
        // no s'ha de veure res, encara que també es podria posar el color de la Intensita ambien global
        return -1.0f;
    }
}

/*IntersectInfo Scene::closestIntersection(Ray ray){
    IntersectInfo info;
    objects[0]->Intersect(ray, info);
    IntersectInfo nearest = info;
    for(unsigned int i = 1; i < objects.size(); i++){
        objects[i]->Intersect(ray, info);
        if (length(info.hitPoint - cam->obs) < length(nearest.hitPoint - cam->obs)){
            nearest = info;
        }
    }
    return nearest;
}*/

vec3 Scene::shade(IntersectInfo info, Ray ray){
    vec3 c = llumAmbient * info.material->ambient;
    IntersectInfo tmp = info;

    for(int i=0; i<llums.size(); i++){
        vec3 L = llums[i]->position - info.hitPoint;
        Ray r(tmp.hitPoint + EPSILON*L, L);
        if (CheckIntersection(r, info)){
            c += llums[i]->ambient * info.material->ambient;
        } else {
            c += calculatePhong(info, r);
        }
    }
    return c;
}

void Scene::addLlum(Llum *l) {
    llums.push_back(l);
}

vec3 Scene::calculatePhong(IntersectInfo info, Ray &ray)
{
    vec3 c = vec3(0.0, 0.0, 0.0);
    vec3 L, H, N=normalize(info.normal);
    vec3 diffuseTmp, specularTmp, ambientTmp;
    float atenuation;
    for (int j=0; j<llums.size(); j++){
        L = normalize(llums[j]->position - info.hitPoint);
        H = normalize(L + ray.direction);

        diffuseTmp = info.material->diffuse * llums[j]->diffuse * glm::max(dot(L,N),0.0f);
        specularTmp = info.material->specular * llums[j]->specular * pow(glm::max(dot(N,H),0.0f), info.material->shininess);
        ambientTmp = info.material->ambient * llums[j]->ambient;

        atenuation = atenuateFactor(j, llums[j]->atenuate, info.hitPoint);

        vec3 llumAmbient = vec3(0.0,0.0,0.0); // hasta que no sepamos si hay o no
        c += (diffuseTmp + specularTmp + ambientTmp) * atenuation + llumAmbient * info.material->ambient;
    }
    return c;
  }

float Scene::atenuateFactor(int j, vec3 atenuate, vec3 hitPoint){
    vec3 rayDirection = llums[j]->position - hitPoint;
    float a = atenuate[0], b = atenuate[1], c = atenuate[2];
    float d = length(rayDirection);
    return 1.0/(a + b*d + c*d*d);
}















