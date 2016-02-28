#include <tetraedre.h>

tetraedre::tetraedre()
{
    std::cout<<"Estic en el constructor del tetraedre\n";
    // tetraedre centrat al 0,0,0 amb dimensió 1 a totes les seves arestes
    xorig = 0.0; yorig = 0.0; zorig = 1.0;
    a = 5;
    h = 5;
    p = 5;

    // Vertices of a unit cube centered at origin, sides aligned with axes
    vertices[0] = point4( 0.0, 0.0, 1.0, 1.0 );
    vertices[1] = point4( 0.0,  2.0*sqrt(2.0)/3.0,  -1.0/3.0, 1.0 );
    vertices[2] = point4( -sqrt(6.0)/3.0, -sqrt(2.0)/3.0, -1.0/3.0, 1.0 );
    vertices[3] = point4( sqrt(6.0)/3.0, -sqrt(2.0)/3.0, -1.0/3.0, 1.0 );

    // RGBA colors
    vertex_colors[0] = color4( 0.0, 0.0, 0.0, 1.0 );  // black
    vertex_colors[1] = color4( 1.0, 0.0, 0.0, 1.0 );  // red
    vertex_colors[2] = color4( 1.0, 1.0, 0.0, 1.0 );  // yellow
    vertex_colors[3] = color4( 0.0, 1.0, 0.0, 1.0 );  // green

}

// Constructora amb tots els parametres
tetraedre::tetraedre(int an, int al, int profu, const GLfloat x0, GLfloat y0, GLfloat z0)
{
    std::cout<<"Estic en el constructor parametritzat del tetraedre\n";
    a = an;
    h = al;
    p = profu;
    xorig = x0;
    yorig = y0;
    zorig = z0;
}

// Destructora
tetraedre::~tetraedre()
{
}

// quad generates two triangles for each face and assigns colors
//    to the vertices

void tetraedre::tri( int a, int b, int c)
{
    // Vectors en vertexTextura estan en referencia {a; ab, ac}
    colors[Index] = vertex_colors[a]; points[Index] = vertices[a];
    vertexsTextura[Index] = vec2(0.0, 0.0); Index++;
    colors[Index] = vertex_colors[b]; points[Index] = vertices[b];
    vertexsTextura[Index] = vec2(1.0, 0.0); Index++;
    colors[Index] = vertex_colors[c]; points[Index] = vertices[c];
    vertexsTextura[Index] = vec2(0.0, 1.0); Index++;
}


// Realitzacio de la geometria del tetraedre en una genList o en el vertex array, segons el que visualitzem

void tetraedre::make()
{
    std::cout<<"Estic en el make del tetraedre\n";
    // generacio de la geometria dels triangles per a visualitzar-lo
    Index = 0;
    // Vigilar amb orientació a l'hora de pintar
        // Fer servir regla del cargol per determinar orientació
    tri( 0, 1, 2);
    tri( 0, 3, 1);
    tri( 0, 2, 3);
    tri( 1, 3, 2);
    initTextura();

}

void tetraedre::initTextura()
 {
     qDebug() << "Initializing textures...";


     // Carregar la textura
     glActiveTexture(GL_TEXTURE0);
     texture = new QOpenGLTexture(QImage("://resources/mosaic.png"));
     texture->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
     texture->setMagnificationFilter(QOpenGLTexture::Linear);

     texture->bind(0);

 }

// Carrega a la GPU del cub

void tetraedre::toGPU(QGLShaderProgram *program){

    std::cout<<"Pas de les dades del cub a la GPU\n";

    // S'activa la textura i es passa a la GPU
    texture->bind(0);
    program->setUniformValue("texMap", 0);

    // Creacio i inicialitzacio d'un vertex buffer object (VBO)
    GLuint buffer;
    glGenBuffers( 1, &buffer );

    // Activació a GL del Vertex Buffer Object
    glBindBuffer( GL_ARRAY_BUFFER, buffer );

    // Transferència dels punts, colors i coordenades de textura al vertex buffer object
    glBufferData( GL_ARRAY_BUFFER, sizeof(points) + sizeof(colors)+sizeof(vertexsTextura),
                  NULL, GL_STATIC_DRAW );
    glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(points), points );
    glBufferSubData( GL_ARRAY_BUFFER, sizeof(points), sizeof(colors), colors );
    glBufferSubData( GL_ARRAY_BUFFER, sizeof(points)+sizeof(colors), sizeof(vertexsTextura), vertexsTextura );


    // Definició de la correspondència de les variables del shader vPosition i vColor

    int vertexLocation = program->attributeLocation("vPosition");
    int colorLocation = program->attributeLocation("vColor");
    int coordTextureLocation = program->attributeLocation("vCoordTexture");
    program->enableAttributeArray(vertexLocation);
    program->setAttributeBuffer("vPosition", GL_FLOAT, 0, 4);

    program->enableAttributeArray(colorLocation);
    program->setAttributeBuffer("vColor", GL_FLOAT, sizeof(points), 4);

    program->enableAttributeArray(coordTextureLocation);
    program->setAttributeBuffer("vCoordTexture", GL_FLOAT, sizeof(points)+sizeof(colors), 2);

    // Activació de la correspondencia entre les variables
    program->bindAttributeLocation("vPosition", vertexLocation);
    program->bindAttributeLocation("vColor", colorLocation);
    program->bindAttributeLocation("vCoordTexture", coordTextureLocation);


    glEnable( GL_DEPTH_TEST );
    glEnable(GL_TEXTURE_2D);
    program->bind();

}


void tetraedre::draw()
{
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glDrawArrays( GL_TRIANGLES, 0, NumVertices );
}

void tetraedre::aplicaTG(mat4 m)
{
    point4  transformed_points[NumVertices];

    for ( int i = 0; i < NumVertices; ++i ) {
        transformed_points[i] = m * points[i];
    }

    // Actualitzacio del vertex array per a preparar per pintar
    glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(transformed_points),
                     transformed_points );

}
