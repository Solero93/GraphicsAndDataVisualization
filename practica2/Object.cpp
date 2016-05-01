#include "Object.h"

Material::Material():
    ambient(1.0f),
    diffuse(1.0f),
    specular(1.0f),
    shininess(10.0f)
  {}

Object::Object(const glm::mat4 &transform, const Material &material):
    transform(transform),
    material(material)
  {}

Sphere::Sphere(glm::vec3 center, float radius, const glm::mat4 &transform, const Material &material)
    : Object(transform, material){
    this->center = center;
    this->radius = radius;
}

Plane::Plane(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, const glm::mat4 &transform, const Material &material)
    : Object(transform, material){
    this->p1 = p1;
    this->p2 = p2;
    this->p3 = p3;
    this->normal = glm::normalize(glm::cross((p3-p1),(p2-p1)));
}

Triangle::Triangle(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3,const glm::mat4 &transform, const Material &material)
    : Plane(p1,p2,p3,transform, material){}

/* TODO: Implementar en el punt 2 */
bool Sphere::Intersect(const Ray &ray, IntersectInfo &info) const {
    //https://en.wikipedia.org/wiki/Line%E2%80%93sphere_intersection
    glm::vec3 o = ray.origin;
    glm::vec3 u = ray.direction;
    glm::vec3 c = this->center;
    float r = this->radius;
    // If there's no intersection, return false;
    float discriminant = (glm::dot(u,(o-c))*glm::dot(u,(o-c))) - glm::length((o-c)) + r*r;
    if (discriminant < 0){
        return false;
    }
    float a1 = -(glm::dot(u,(o-c))) + sqrt(discriminant);
    float a2 = -(glm::dot(u,(o-c))) - sqrt(discriminant);
    glm::vec3 result1 = o + a1*u;
    glm::vec3 result2 = o + a2*u;
      //We return the intersection that's closest to point
    /**
      TODO hace falta saber si devolver la intersección de detrás del observador
      */
    if (glm::length(result1-o) < glm::length(result2-o)){
        info.time = a1;
        info.hitPoint = result1;
        info.normal = glm::normalize(result1-c);
        info.material = &this->material;
    } else {
        info.time = a2;
        info.hitPoint = result2;
        info.normal = glm::normalize(result2-c);
        info.material = &this->material;
    }
    return true;
}

/* TODO: Implementar en el punt 2 */
bool Plane::Intersect(const Ray &ray, IntersectInfo &info) const {
    //https://en.wikipedia.org/wiki/Line%E2%80%93plane_intersection
    glm::vec3 p0 = this->p1;
    glm::vec3 n = this->normal;

    glm::vec3 l0 = ray.origin;
    glm::vec3 l = ray.direction;

    if (glm::dot(l,n) == 0){
        return false;
    } else {
        info.time = (glm::dot((p0-l0),n))/(glm::dot(l,n));
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
