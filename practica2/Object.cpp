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
    : Plane(p1,p2,p3,transform, material){
    // Cambiamos de base para tomar referencia en
        // {p1; p1p2, p1p3, normal}
    // Con eso conseguimos proyectar sobre el plano y la pertenencia al triángulo
        // Es simplemente aplicar la fórmula para pertenencia a triángulo en un plano
    vec3 tmp;
    tmp = this->p2 - this->p1;
    vec4 base1 = vec4(tmp.x, tmp.y, tmp.z, 0.0);
    tmp = this->p3 - this->p1;
    vec4 base2 = vec4(tmp.x, tmp.y, tmp.z, 0.0);
    vec4 base3 = vec4(this->normal.x, this->normal.y, this->normal.z, 0.0);
    vec4 origin = vec4(this->p1.x, this->p1.y, this->p1.z, 1.0);
    this->changeOfBasis = mat4(base1, base2, base3, origin);

    this->p1_ = vec2(changeOfBasis * vec4(this->p1.x, this->p1.y, this->p1.z, 0.0));
    this->p2_ = vec2(changeOfBasis * vec4(this->p2.x, this->p2.y, this->p2.z, 0.0));
    this->p3_ = vec2(changeOfBasis * vec4(this->p3.x, this->p3.y, this->p3.z, 0.0));
}

bool Sphere::Intersect(const Ray &ray, IntersectInfo &info) const {
    //https://en.wikipedia.org/wiki/Line%E2%80%93sphere_intersection
    vec3 o = ray.origin;
    vec3 u = ray.direction;
    vec3 c = this->center;
    float r = this->radius;
    float discriminant = (dot(u,(o-c))*dot(u,(o-c))) - (dot(u,u))*((dot(o-c,o-c)) - (r*r));

    // If there's no intersection, return false;
    if (discriminant < FLOAT_EPSILON) {
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

    if (fabs(dot(l,n)) < FLOAT_EPSILON){
        return false;
    }

    float time = (dot((p0-l0),n))/(dot(l,n));

    info.time = time;
    info.hitPoint = time * l + l0;
    info.normal = this->normal;
    info.material = &this->material;
    return true;
}

// EXTENSIÓ
bool Triangle::Intersect(const Ray &ray, IntersectInfo &info) const {
    return (Plane::Intersect(ray, info) && this->isPointOfTriangle(info.hitPoint));
}

float Triangle::sign (vec2 p1, vec2 p2, vec2 p3) const {
    return (p1.x - p3.x) * (p2.y - p3.y) - (p2.x - p3.x) * (p1.y - p3.y);
}

bool Triangle::isPointOfTriangle(vec3 point) const {
    // Proyectamos el punto sobre el plano y miramos si pertenece al triángulo
    vec2 p_ = vec2(this->changeOfBasis * vec4(point.x, point.y, point.z, 0.0));

    bool b1, b2, b3;

    b1 = this->sign(p_, p1_, p2_) < FLOAT_EPSILON;
    b2 = this->sign(p_, p2_, p3_) < FLOAT_EPSILON;
    b3 = this->sign(p_, p3_, p1_) < FLOAT_EPSILON;

    return ((b1 == b2) && (b2 == b3));
}
