#include "Scene.h"
#include <iostream>

using namespace std;

Scene::Scene()
{
    // Afegeix la camera a l'escena
    cam = new Camera();
    // TODO: Cal crear els objectes de l'escena (punt 2 de l'enunciat)
    this->objects.push_back(new Sphere(glm::vec3(0.0,0.0,0.0),0.25));
    // TODO: Cal afegir llums a l'escena (punt 4 de l'enunciat)

}

Scene::~Scene()
{
    delete cam;
    for(unsigned int i = 0; i < objects.size(); ++i){
        if(objects[i]){
            delete objects[i];
        }
    }
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
    for(unsigned int i = 0; i < objects.size(); ++i){
        if(objects[i]->Intersect(ray, info)){
            return true;
        }
    }
    return false;
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

        payload.color = glm::vec3(fabs(ray.direction.x),fabs(ray.direction.y),fabs(ray.direction.z)) ;

        return info.time;
    }
    else{
        payload.color = glm::vec3(0.0f);
        // Si el ray des de la camera no intersecta amb cap objecte
        // no s'ha de veure res, encara que també es podria posar el color de la Intensita ambien global
        return -1.0f;
    }
}

