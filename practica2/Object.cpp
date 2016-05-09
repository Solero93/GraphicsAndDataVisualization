#include "Object.h"
#include <iostream>

using namespace std;

Material::Material(){
    ambient = glm::vec3(0.2,0.2,0.2);
    diffuse = glm::vec3(0.8,0.0,0.0);
    specular = glm::vec3(1.0,1.0,1.0);
    shininess = 20.0;
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
    this->normal = normalize(cross((p3-p1),(p2-p1)));
}

Triangle::Triangle(vec3 p1, vec3 p2, vec3 p3,const mat4 &transform, const Material &material)
    : Plane(p1,p2,p3,transform, material){}

/* TODO: Implementar en el punt 2 */
bool Sphere::Intersect(const Ray &ray, IntersectInfo &info) const {
    //https://en.wikipedia.org/wiki/Line%E2%80%93sphere_intersection
    vec3 o = ray.origin;
    vec3 u = normalize(ray.direction);
    vec3 c = this->center;
    float r = this->radius;
    // If there's no intersection, return false;
    float discriminant = (dot(u,(o-c))*dot(u,(o-c))) - length((o-c))*length((o-c)) + r*r;
    if (discriminant < 0.0) {
        return false;
    }
    float a1 = -(dot(u,(o-c))) + sqrt(discriminant);
    float a2 = -(dot(u,(o-c))) - sqrt(discriminant);

    float time;

    if (a1 < 0.0){
        return false;
    } else if (a2 < 0.0) {
        time = a1;
    } else {
        time = a2;
    }

    vec3 result = o + time*u;
    info.time = time;
    info.hitPoint = result;
    info.normal = normalize(result - c);
    info.material = &this->material;

    return true;
}

/* TODO: Implementar en el punt 2 */
bool Plane::Intersect(const Ray &ray, IntersectInfo &info) const {

    //https://en.wikipedia.org/wiki/Line%E2%80%93plane_intersection
    vec3 p0 = this->p1;
    vec3 n = this->normal;

    vec3 l0 = ray.origin;
    vec3 l = ray.direction;

    if (dot(l,n) == 0.0){
        return false;
    } else {
        info.time = (dot((p0-l0),n))/(dot(l,n));
        info.hitPoint = info.time * l + l0;
        info.normal = this->normal;
        info.material = &this->material;
        return true;
    }
}

/* TODO: Implementar com a extensio */
bool Triangle::Intersect(const Ray &ray, IntersectInfo &info) const {
    return -1.0f;
}
