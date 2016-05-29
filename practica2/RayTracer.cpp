#include "RayTracer.h"

// Metode per netejar la memòria

void cleanup() {
    delete scene;
}


// Metode Render

// Aquest metode pinta pixels en la finestra GL usant GL_POINT. Es crida cada vegada que cal refrescar la pantalla

// Aquesta funcio ha de transformar cada pixel a coordenades de mon i
// envia un raig des de la camera en aquella direccio cridant al metode CastRay
// i pel pintar la imatge final:
// 1) Neteja la pantalla per a visualitzar el nou frame
// 2) Envia un raig a l'escena per a cada pixel de la pantalla i utilitza el color retornat per a pintar el pixel
// 3) Actualitza la finestra GL fent flush de les intruccions GL


void Render()
{

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);// Netejar la finestra OpenGL

    // TODO: Cridar a IniViewProjMatrices de la classe camera (punt 2 de l'enunciat)
    scene->cam->IniViewProjMatrices(1.0 + 1.0/3.0, 1.0);

    glBegin(GL_POINTS);	//S'activa el mode GL_POINTS. En aquest mode
                        // cada vertex especificat és un punt.
                        //	Reference https://en.wikibooks.org/wiki/OpenGL_Programming/GLStart/Tut3 si us interessa.

    std::cout<<"observador:"<<scene->cam->obs.x<<","<<scene->cam->obs.y<<","<<scene->cam->obs.z<<std::endl;
    // Recorregut de cada pixel de la imatge final

    mat4 changeOfCoordinates = inverse(scene->cam->viewMatrix) * inverse(scene->cam->projMatrix);

    for(int x = 0; x < scene->cam->viewportX; ++x)
        for(int y = 0; y < scene->cam->viewportY; ++y){
            // Pasar de coordenadas viewport a coordenadas window normalizados
            float pixelX =  2*((x+0.5f)/scene->cam->viewportX)-1;
            float pixelY = -2*((y+0.5f)/scene->cam->viewportY)+1;


            // TODO: A canviar en el punt 3 de la pràctica. Ara s'esta suposant que l'observador està situat al punt
            // 0,0,10 i s'està considerant que el angle d'obertura es de arcos (1/10) i la relació de l'amplada i l'alçada
            // es 1. Aixi, sota aquests suposits, les coordenades de window es poden calcular amb el calcul
            // de pixelX, pixelY i les coordenades mon i de camera es poden considerar les mateixes.

            // TODO: Cal canviar aquestes 2 linies per a fer la transformacio de pixel a coordenades de mon de forma correcta
            // en qualsevol transformacio perspectiva

            /*
             * 1. Coger dos puntos p1, p2 tal que z_p1 = -1 y z_p2 = 1 (el z_p2 tiene que ser z_near)
             * 2. formar recta de ellos, interseccionar y encontrar lo primero que choque
             */

            vec4 pix1 = changeOfCoordinates * vec4(pixelX, pixelY, -1.0f, 1.0f);
            pix1 /= pix1.w;
            vec4 pix2 = changeOfCoordinates * vec4(pixelX, pixelY, 1.0f, 1.0f);
            pix2 /= pix2.w;
            vec3 direction = normalize(vec3(pix2 - pix1));

            Payload payload;
            // Creacio del raig
            // HELP: Ray(const vec3 &origin, const vec3 &direction)
            Ray ray(vec3(pix1), direction);

            if(scene->CastRay(ray,payload) > 0.0f){
				glColor3f(payload.color.x,payload.color.y,payload.color.z);
			} 
            else {
                glColor3f(scene->llumAmbient.x,scene->llumAmbient.y,scene->llumAmbient.z);
			}

            glVertex3f(pixelX,pixelY,0.0f);
		}
  
	glEnd();
	glFlush();
}

// Metode principal del programa

int main(int argc, char **argv) {

    // inicialitza l'escena: el constructor de l'escena s'encarrega de
    // crear la camera, els objectes i les llums
    scene = new Scene();

    //inicialitza OpenGL
	glutInit(&argc, argv);
    //Define la mida de la window GL especificada per la mida del viewport
    glutInitWindowSize(scene->cam->viewportX, scene->cam->viewportY);

    //Creacio de la window per pintar amb GL
	glutCreateWindow("RayTracer");
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);	

    //Posa la funcio Render per a que la window GL cridi quan es refresca
	glutDisplayFunc(Render);
  
    //	TODO: Afegir els objectes a l'escena
    //  TODO: Afegir les llums a l'escena

	atexit(cleanup);

    // Inicia el main loop per a poder refrescar l'escena
	glutMainLoop();
}
