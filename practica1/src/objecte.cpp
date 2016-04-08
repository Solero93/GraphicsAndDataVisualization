#include <objecte.h>
#include <readfile.h>

Objecte::Objecte(int npoints, QObject *parent) : numPoints(npoints) ,QObject(parent){
    points = new point4[numPoints];
    colors = new point4[numPoints];
    normals = new vec4[numPoints];
    vertexsTextura = new vec2[numPoints];
    temporal = vec3(0.0,0.0,0.0);
}

Objecte::Objecte(int npoints, QString n) : numPoints(npoints){
    points = new point4[numPoints];
    colors = new point4[numPoints];
    normals = new vec4[numPoints];
    vertexsTextura = new vec2[numPoints];
    readObj(n);
    material = new Material();
    make();
}

Objecte::Objecte(int npoints, QString n, QString m) : numPoints(npoints){
    points = new point4[numPoints];
    colors = new point4[numPoints];
    normals = new vec4[numPoints];
    readObj(n);
    material = new Material();
    qDebug() << "abc" << n << "def";
    readMat(m);
    make();
}


Objecte::~Objecte(){
    delete points;
    delete colors;
    delete normals;
}

/**
 * @brief Objecte::toGPU
 * @param pr
 */
void Objecte::toGPU(QGLShaderProgram *pr) {
    program = pr;
    glGenBuffers( 1, &buffer );
    glBindBuffer( GL_ARRAY_BUFFER, buffer );

    glBufferData( GL_ARRAY_BUFFER, sizeof(point4)*Index * 3 + sizeof(vec2) * Index , NULL, GL_STATIC_DRAW );
    glEnable( GL_DEPTH_TEST );
//    glEnable( GL_TEXTURE_2D );

}

void Objecte::initTextura()
 {
     qDebug() << "Initializing textures...";
     // Carregar la textura
     glActiveTexture(GL_TEXTURE0);
     texture = new QOpenGLTexture(QImage("://resources/textures/earth1.png"));
     texture->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
     texture->setMagnificationFilter(QOpenGLTexture::Linear);
     texture->bind(0);
     glEnable(GL_DEPTH_TEST ); //-> viene de cubGPUTextures
     glEnable(GL_TEXTURE_2D); //-> ídem
     //program->bind(); //-> viene de cubGPUTextures
}


/**
 * Pintat en la GPU.
 * @brief Objecte::draw
 */
void Objecte::draw(){

    material->toGPU(program);

    // S'activa la textura i es passa a la GPU
    texture->bind(0);
    program->setUniformValue("texMap", 0);

    // Aqui es torna a repetir el pas de dades a la GPU per si hi ha més d'un objecte
    glBindBuffer( GL_ARRAY_BUFFER, buffer );

    glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(point4)*Index, &points[0] );
    glBufferSubData( GL_ARRAY_BUFFER, sizeof(point4)*Index, sizeof(point4)*Index, &colors[0] );
    glBufferSubData( GL_ARRAY_BUFFER, sizeof(point4)*Index*2, sizeof(point4)*Index, &normals[0] );
    glBufferSubData( GL_ARRAY_BUFFER, sizeof(point4)*Index*3, sizeof(vec2)*Index, &vertexsTextura[0] );

    int vertexLocation = program->attributeLocation("vPosition");
    int colorLocation = program->attributeLocation("vColor");
    int normalLocation = program->attributeLocation("vNormal");
    int coordTextureLocation = program->attributeLocation("vCoordTextura");

    program->enableAttributeArray(vertexLocation);
    program->setAttributeBuffer("vPosition", GL_FLOAT, 0, 4);

    program->enableAttributeArray(colorLocation);
    program->setAttributeBuffer("vColor", GL_FLOAT, sizeof(point4)*Index, 4);

    program->enableAttributeArray(normalLocation);
    program->setAttributeBuffer("vNormal", GL_FLOAT, sizeof(point4)*Index*2, 4);

    program->enableAttributeArray(coordTextureLocation);
    program->setAttributeBuffer("vCoordTextura", GL_FLOAT, sizeof(point4)*Index*3, 2);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glDrawArrays( GL_TRIANGLES, 0, Index );
}

void Objecte::make(){
    static vec3  base_colors[] = {
        vec3( 1.0, 0.0, 0.0 ),
        vec3( 0.0, 1.0, 0.0 ),
        vec3( 0.0, 0.0, 1.0 ),
        vec3( 1.0, 1.0, 0.0 )
    };

    vector<point4> tmpNormals = this->calcularNormalVertexs();
    vector<vec2> tmpTextures = this->calcularCoordTextures(tmpNormals);

    Index = 0;
    for(unsigned int i=0; i<cares.size(); i++){
        for(unsigned int j=0; j<cares[i].idxVertices.size(); j++){
            points[Index] = vertexs[cares[i].idxVertices[j]];
            normals[Index] = tmpNormals[cares[i].idxVertices[j]];
            colors[Index] = vec4(base_colors[j%4], 1.0);
            vertexsTextura[Index] = tmpTextures[cares[i].idxVertices[j]];
            Index++;
        }
    }
    this->initTextura();
}

// Llegeix un fitxer .obj
//  Si el fitxer referencia fitxers de materials (.mtl), encara no es llegeixen
//  Tots els elements del fitxer es llegeixen com a un unic objecte.
void Objecte::readObj(QString filename){

    FILE *fp = fopen(filename.toLocal8Bit(),"rb");
    if (!fp)
    {
        cout << "No puc obrir el fitxer " << endl;
    }
    else {

        while (true)
        {
            char *comment_ptr = ReadFile::fetch_line (fp);

            if (comment_ptr == (char *) -1)  /* end-of-file */
                break;

            /* did we get a comment? */
            if (comment_ptr) {
                //make_comment (comment_ptr);
                continue;
            }

            /* if we get here, the line was not a comment */
            int nwords = ReadFile::fetch_words();

            /* skip empty lines */
            if (nwords == 0)
                continue;

            char *first_word = ReadFile::words[0];

            if (!strcmp (first_word, "v"))
            {
                if (nwords < 4) {
                    fprintf (stderr, "Too few coordinates: '%s'", ReadFile::str_orig);
                    exit (-1);
                }

                QString sx(ReadFile::words[1]);
                QString sy(ReadFile::words[2]);
                QString sz(ReadFile::words[3]);
                double x = sx.toDouble();
                double y = sy.toDouble();
                double z = sz.toDouble();

                if (nwords == 5) {
                    QString sw(ReadFile::words[4]);
                    double w = sw.toDouble();
                    x/=w;
                    y/=w;
                    z/=w;
                }
                // S'afegeix el vertex a l'objecte
                vertexs.push_back(point4(x, y, z, 1));

            }
            else if (!strcmp (first_word, "vn")) {
            }
            else if (!strcmp (first_word, "vt")) {
            }
            else if (!strcmp (first_word, "f")) {
                // S'afegeix la cara a l'objecte
                // A modificar si es vol carregar mes de un objecte
                construeix_cara (&ReadFile::words[1], nwords-1);
            }
            // added
            else if (!strcmp (first_word, "mtllib")) {
                //read_mtllib (&words[1], nwords-1, matlib, filename);
            }
            else if (!strcmp (first_word, "usemtl")) {
                //int size = strlen(words[1])-1;
                //while (size && (words[1][size]=='\n' || words[1][size]=='\r') ) words[1][size--]=0;
                //currentMaterial = matlib.index(words[1]);
            }
            // fadded
            else {
                //fprintf (stderr, "Do not recognize: '%s'\n", str_orig);
            }
        }
    }

}

void Objecte::construeix_cara ( char **words, int nwords) {
    Cara face;

    for (int i = 0; i < nwords; i++) {
        int vindex;
        int nindex;
        int tindex;

        if ((words[i][0]>='0')&&(words[i][0]<='9')) {
            ReadFile::get_indices (words[i], &vindex, &tindex, &nindex);

#if 0
            printf ("vtn: %d %d %d\n", vindex, tindex, nindex);
#endif

            /* store the vertex index */

            if (vindex > 0)       /* indices are from one, not zero */
                face.idxVertices.push_back(vindex - 1);
            else if (vindex < 0)  /* negative indices mean count backwards */
                face.idxVertices.push_back(this->vertexs.size() + vindex);
            else {
                fprintf (stderr, "Zero indices not allowed: '%s'\n", ReadFile::str_orig);
                exit (-1);
            }
        }
    }
    face.color = vec4(1.0, 0.0, 0.0, 1.0);
    this->cares.push_back(face);
}

// Llegeix un fitxer .mtl
//  Si el fitxer referencia fitxers de materials (.mtl), encara no es llegeixen
//  Tots els elements del fitxer es llegeixen com a un unic objecte.
void Objecte::readMat(QString filename){

    FILE *fp = fopen(filename.toLocal8Bit(),"rb");
    if (!fp)
    {
        cout << "No puc obrir el fitxer " << endl;
    }
    else {

        while (true)
        {
            char *comment_ptr = ReadFile::fetch_line (fp);

            if (comment_ptr == (char *) -1)  /* end-of-file */
                break;

            /* did we get a comment? */
            if (comment_ptr) {
                //make_comment (comment_ptr);
                continue;
            }

            /* if we get here, the line was not a comment */
            int nwords = ReadFile::fetch_words();

            /* skip empty lines */
            if (nwords == 0)
                continue;

            char *first_word = ReadFile::words[0];

            if (!strcmp (first_word, "Ka"))
            {
                QString sx(ReadFile::words[1]);
                QString sy(ReadFile::words[2]);
                QString sz(ReadFile::words[3]);
                double x = sx.toDouble();
                double y = sy.toDouble();
                double z = sz.toDouble();
                temporal = vec3(x,y,z);
                material->setAmbient(temporal);

            }
            else if (!strcmp (first_word, "Kd")) {
                QString sx(ReadFile::words[1]);
                QString sy(ReadFile::words[2]);
                QString sz(ReadFile::words[3]);
                double x = sx.toDouble();
                double y = sy.toDouble();
                double z = sz.toDouble();
                temporal = vec3(x,y,z);
                material->setDiffuse(temporal);

            }
            else if (!strcmp (first_word, "Ks")) {
                QString sx(ReadFile::words[1]);
                QString sy(ReadFile::words[2]);
                QString sz(ReadFile::words[3]);
                double x = sx.toDouble();
                double y = sy.toDouble();
                double z = sz.toDouble();
                temporal = vec3(x,y,z);
                material->setDiffuse(temporal);
            }

        }
    }

}

vector<vec4> Objecte::calcularNormalVertexs(){
    vector<vec4> normals(numPoints);
    for (int i=0; i < cares.size(); i++){
        cares[i].calculaNormal(vertexs);
        for (int j=0; j < cares[i].idxVertices.size(); j++){
            int id = cares[i].idxVertices[j];
            normals[id] += vec4(
               cares[i].normal[0],
               cares[i].normal[1],
               cares[i].normal[2],
               0.0
            );
        }
    }
    for (int i=0; i < normals.size(); i++){
        normals[i] = normalize(normals[i]); // We normalize the vector
    }
    return normals;
}

vector<vec2> Objecte::calcularCoordTextures(vector<vec4> normals) {
    vector<vec2> coordTextures(this->numPoints);
    for (int i=0; i<vertexs.size(); i++){
        vec4 intersection = this->intersectWithSphere(vertexs[i],normals[i]);
        if (intersection == vec3(0.0,0.0,0.0)){
            qDebug() << "ERROR with spherical coordinates";
        }
        float u = 0.5 + atan2(intersection.z, intersection.x)/(2*M_PI);
        float v = 0.5 - asin(intersection.y)/M_PI;
        coordTextures[i] = vec2(u,v);
    }
    return coordTextures;
}

vec4 Objecte::intersectWithSphere(vec4 point, vec4 normal){
    vec4 p = point;
    vec4 u = normal;
    //Calculated by hand the two intersections of line (defined by normal) with unit sphere
    if ((dot(u,p)*dot(u,p))-length(p)+1 < 0){
        //Technically, should never arrive here
            //It means there's no intersection with sphere
        return vec4(0.0, 0.0, 0.0, 0.0);
    }

    float a1 = -(dot(u,p)) + sqrt((dot(u,p)*dot(u,p))-length(p)+1);
    float a2 = -(dot(u,p)) - sqrt((dot(u,p)*dot(u,p))-length(p)+1);

    vec4 result1 = p + a1*u;
    vec4 result2 = p + a2*u;

    //We return the intersection that's closest to point
    if (length(result1-point) < length(result2-point)){
        return result1;
    } else {
        return result2;
    }
}
