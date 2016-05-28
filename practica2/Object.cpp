#include "Object.h"
#include <iostream>

using namespace std;

Material::Material(){
    //Default material
    ambient = vec3(0.2,0.2,0.2);
    diffuse = vec3(0.8,0.0,0.0);
    specular = vec3(0.3,0.3,0.3);
    shininess = 20.0;
}

Material::Material(vec3 amb, vec3 dif, vec3 spec, float shin){
    this->ambient = amb;
    this->diffuse = dif;
    this->specular = spec;
    this->shininess = shin;
}

Object::Object(const mat4 &transform, const Material &material):
    transform(transform),
    material(material)
  {}

Sphere::Sphere(vec3 center, float radius, const mat4 &transform, const Material &material)
    : Object(transform, material){
    this->center = center;
    this->radius = radius;
}

Plane::Plane(vec3 p1, vec3 p2, vec3 p3, const mat4 &transform, const Material &material)
    : Object(transform, material){
    this->p1 = p1;
    this->p2 = p2;
    this->p3 = p3;
    this->normal = normalize(cross((p2-p1),(p3-p1)));
}

Plane::Plane(vec3 p1, vec3 normal, const mat4 &transform, const Material &material)
    : Object(transform, material){
    this->p1 = p1;
    this->normal = normal;
}

Triangle::Triangle(vec3 p1, vec3 p2, vec3 p3,const mat4 &transform, const Material &material)
    : Plane(p1,p2,p3,transform, material){}

/* TODO: Implementar en el punt 2 */
bool Sphere::Intersect(const Ray &ray, IntersectInfo &info) const {
    //https://en.wikipedia.org/wiki/Line%E2%80%93sphere_intersection
    vec3 o = ray.origin;
    vec3 u = ray.direction;
    vec3 c = this->center;
    float r = this->radius;
    float discriminant = (dot(u,(o-c))*dot(u,(o-c))) - (dot(u,u))*((dot(o-c,o-c)) - (r*r));

    // If there's no intersection, return false;
    if (discriminant < 0.0) {
        return false;
    }

    float a1 = -(dot(u,(o-c))) + sqrt(discriminant);
    float a2 = -(dot(u,(o-c))) - sqrt(discriminant);

    float time = (a1 < a2) ? a1 : a2;

    time/=dot(u,u);

    vec3 result = o + time*u;
    info.time = time;
    info.hitPoint = result;
    info.normal = normalize(result - c);
    info.material = &this->material;

    return true;
}

bool Plane::Intersect(const Ray &ray, IntersectInfo &info) const {
    //https://en.wikipedia.org/wiki/Line%E2%80%93plane_intersection
    vec3 p0 = this->p1;
    vec3 n = this->normal;

    vec3 l0 = ray.origin;
    vec3 l = ray.direction;

    if (dot(l,n) == 0.0){
        return false;
    }
    float time = (dot((p0-l0),n))/(dot(l,n));
    /*if (time < 0.0) {
        return false;
    }*/

    info.time = time;
    info.hitPoint = time * l + l0;
    info.normal = this->normal;
    info.material = &this->material;
    return true;
}

/* TODO: Implementar com a extensio */
bool Triangle::Intersect(const Ray &ray, IntersectInfo &info) const {
    /*bool t = Plane::Intersect(&ray, &info);
    return (!t || this->isPointOfTriangle(info.hitPoint));*/
}
