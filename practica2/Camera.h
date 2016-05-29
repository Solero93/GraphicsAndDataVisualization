#ifndef CAMERA_H
#define CAMERA_H


#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

using namespace glm;

class Camera {
public:
    Camera();
    Camera(const vec3 &_obs,
                   const vec3 &_vrp,
                   const vec3 &_vup,
                   float _zNear,
                   float _zFar,
                   int _viewportX,
                   int _viewportY);

    void IniViewProjMatrices(float, float);

// Camera:
// Vectors base del sistema de coordenades de camera
/*
** obs es la posicio de l'observador
*/
vec3 obs;
/*
** vrp es el View Reference Point
*/
vec3 vrp;
/*
 ** vup es el vector de verticalitat
 **/
vec3 vup;

// Tipus de projeccio: Perspectiva
/*
 *  Angle obertura de la camera
 */
float angleObertura;
/*
 *  Pla de retallat anterior: zNear
 */
float zNear;
/*
 * Pla de retallat posterior: zFar
 */
float zFar;

// Viewport
int viewportX;
int viewportY;

// Matrius de visualitzacio
mat4 viewMatrix;
mat4 projMatrix;

};

#endif // CAMERA_H

