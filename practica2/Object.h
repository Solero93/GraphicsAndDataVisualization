#pragma once

#include "Ray.h"

using namespace glm;

class Material {
  public:
    Material();
    Material(vec3,vec3,vec3,float);
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;

};

// Es la classe pare de tots els objectes que s'han de visualitzar.
class Object {
  public:
    Object(const mat4 &transform = mat4(1.0f), const Material &material = Material());

    virtual bool Intersect(const Ray &ray, IntersectInfo &info) const { return true; }
    vec3 Position() const { return vec3(transform[3][0], transform[3][1], transform[3][2]); }
    const Material *MaterialPtr() const { return &material; }

  protected:
    mat4 transform;  // Matriu de transformacio de coordenades locals a globals
    Material material;
};

// TODO: Cal definir els diferents tipus d'objectes de l'escena com a fills d'Object.
// Cal definir-lis els atributs propis i implementar la funció
// tots els objectes de l'escena, cal definir el metode Intersect()

/* TODO: Implementar en el punt 2 de l'enunciat*/
class Sphere : public Object {
  public:
    virtual bool Intersect(const Ray &ray, IntersectInfo &info) const;  //  To figure out if the Ray hit this object.
    Sphere(vec3,float,
           const mat4 &transform = mat4(1.0f), const Material &material = Material());
    vec3 center;
    float radius;
};

/* TODO: Implementar en el punt 2 de l'enunciat*/
class Plane : public Object {
  public:
    virtual bool Intersect(const Ray &ray, IntersectInfo &info) const;
    Plane(vec3,vec3,vec3,
          const mat4 &transform = mat4(1.0f), const Material &material = Material());
    Plane(vec3, vec3,
          const mat4 &transform = mat4(1.0f), const Material &material = Material());
    vec3 p1;
    vec3 p2;
    vec3 p3;
    vec3 normal;
};

/* TODO: Implementar com a extensio*/
class Triangle : public Plane {
  public:
    virtual bool Intersect(const Ray &ray, IntersectInfo &info) const;
    bool isPointOfTriangle(vec3) const;
    float sign (vec2, vec2, vec2) const;
    Triangle(vec3,vec3,vec3,
             const mat4 &transform = mat4(1.0f), const Material &material = Material());
    mat4 changeOfBasis;
    vec2 p1_;
    vec2 p2_;
    vec2 p3_;
};
