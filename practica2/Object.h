#pragma once

#include "Ray.h"

class Material {
  public:
    Material();
    glm::vec3 ambient;
    glm::vec3 diffuse;   
    glm::vec3 specular;	
    float shininess;

};

// Es la classe pare de tots els objectes que s'han de visualitzar.
class Object {
  public:
    Object(const glm::mat4 &transform = glm::mat4(1.0f), const Material &material = Material());

    virtual bool Intersect(const Ray &ray, IntersectInfo &info) const { return true; }
    glm::vec3 Position() const { return glm::vec3(transform[3][0], transform[3][1], transform[3][2]); }
    const Material *MaterialPtr() const { return &material; }

  protected:
    glm::mat4 transform;  // Matriu de transformacio de coordenades locals a globals
    Material material;
};

// TODO: Cal definir els diferents tipus d'objectes de l'escena com a fills d'Object.
// Cal definir-lis els atributs propis i implementar la funció
// tots els objectes de l'escena, cal definir el metode Intersect()

/* TODO: Implementar en el punt 2 de l'enunciat*/
class Sphere : public Object {
  public:
    virtual bool Intersect(const Ray &ray, IntersectInfo &info) const;  //  To figure out if the Ray hit this object.
    Sphere(glm::vec3,float,
           const glm::mat4 &transform = glm::mat4(1.0f), const Material &material = Material());
    glm::vec3 center;
    float radius;
};

/* TODO: Implementar en el punt 2 de l'enunciat*/
class Plane : public Object {
  public:
    virtual bool Intersect(const Ray &ray, IntersectInfo &info) const;
    Plane(glm::vec3,glm::vec3,glm::vec3,
          const glm::mat4 &transform = glm::mat4(1.0f), const Material &material = Material());
    glm::vec3 p1;
    glm::vec3 p2;
    glm::vec3 p3;
    glm::vec3 normal;
};

/* TODO: Implementar com a extensio*/
class Triangle : public Plane {
  public:
    virtual bool Intersect(const Ray &ray, IntersectInfo &info) const;
    Triangle(glm::vec3,glm::vec3,glm::vec3,
             const glm::mat4 &transform = glm::mat4(1.0f), const Material &material = Material());
    bool isPointOfTriangle(glm::vec3);
};
