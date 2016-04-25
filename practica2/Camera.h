#ifndef CAMERA_H
#define CAMERA_H


#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

class Camera {
public:
    Camera();
    Camera(const glm::vec3 &_obs,
                   const glm::vec3 &_vrp,
                   const glm::vec3 &_vup,
                   float _angleObertura,
                   float _zNear,
                   float _zFar,
                   int _viewportX,
                   int _viewportY);

    void IniViewProjMatrices();

// Camera:
// Vectors base del sistema de coordenades de camera
/*
** obs es la posicio de l'observador
*/
glm::vec3 obs;
/*
** vrp es el View Reference Point
*/
glm::vec3 vrp;
/*
 ** vup es el vector de verticalitat
 **/
glm::vec3 vup;

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
glm::mat4 viewMatrix;
glm::mat4 projMatrix;

};

#endif // CAMERA_H

