#include "Camera.h"

Camera::Camera():
    //obs(glm::vec3(4.9f,4.9f,4.9f)),
    obs(glm::vec3(0.0f, -2.0f, 10.0f)),
    vrp(glm::vec3(0.0f, 0.0f, 0.0f)),
    vup(glm::vec3(0.0f, 1.0f, 0.0f)),
    angleObertura(45.0f), zNear(1.0f), zFar(1000.0f),
    viewportX(640), viewportY(480)
{
}

Camera::Camera(const glm::vec3 &_obs,
               const glm::vec3 &_vrp,
               const glm::vec3 &_vup,
               float _angleObertura,
               float _zNear,
               float _zFar,
               int _viewportX,
               int _viewportY):
    obs(_obs), vrp(_vrp), vup(_vup),
    angleObertura(_angleObertura), zNear(_zNear), zFar(_zFar),
    viewportX(_viewportX), viewportY(_viewportY){
}

void Camera::IniViewProjMatrices(float width, float height)
{
    this->viewMatrix = lookAt(obs,vrp,vup);
    this->projMatrix = perspective(angleObertura,width/height,zNear,zFar);
    // Inicialització de les matrius viewMatrix i projMatrix
    //	Utilitzeu els metodes de lookAt i perspective
    //  que teniu a  glm.h
}
