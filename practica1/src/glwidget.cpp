#include <math.h>
#include <glwidget.h>
#include <QtDebug>
#include <QString>


GLWidget::GLWidget(QWidget *parent) : QGLWidget(QGLFormat(QGL::SampleBuffers), parent) {
    setFocusPolicy( Qt::StrongFocus );

    xRot = 0;
    yRot = 0;
    zRot = 0;

    mon = new Mon();
}

GLWidget::~GLWidget() {
    makeCurrent();
    delete mon;
}

//metodes extensio rotacio amb teclat

static void qNormalizeAngle(int &angle)
{
    while (angle < 0)
        angle += 360 * 16;
    while (angle > 360 * 16)
        angle -= 360 * 16;
}


void GLWidget::setXRotation(int angle)
{
    qNormalizeAngle(angle);
    if (angle != xRot) {
        xRot = angle;
        emit xRotationChanged(angle);
        updateGL();
    }
}


void GLWidget::setYRotation(int angle)
{
    qNormalizeAngle(angle);
    if (angle != yRot) {
        yRot = angle;
        emit yRotationChanged(angle);
        updateGL();
    }
}

void GLWidget::setZRotation(int angle)
{
    qNormalizeAngle(angle);
    if (angle != zRot) {
        zRot = angle;
        emit zRotationChanged(angle);
        updateGL();
    }
}


 void GLWidget:: keyPressEvent(QKeyEvent *event)
 {
     if (event->key() == Qt::Key_Up)
     {
     setYRotation(yRot + 8);
     }
     else if(event->key() == Qt::Key_Down)
     {
     setYRotation(yRot - 8);
     }
     else if(event->key() == Qt::Key_Left)
     {
     setYRotation(xRot + 8);
     }
     else if(event->key() == Qt::Key_Right)
     {
     setYRotation(xRot - 8);
     }
     else
     {
     }

 }

// Metodes que es criden des dels menús
void GLWidget::activaToonShader() {
    program = allPrograms[5];
    program->link();
    program->bind();
}

void GLWidget::activaPhongShader() {
    program = allPrograms[3];
    program->link();
    program->bind();
}

void GLWidget::activaGouraudShader() {
    program = allPrograms[1];
    program->link();
    program->bind();
}

void GLWidget::activaPhongTex() {
    program = allPrograms[4];
    program->link();
    program->bind();
}

void GLWidget::activaGouraudTex() {
    program = allPrograms[2];
    program->link();
    program->bind();
}

void GLWidget::ensenyaMenuLlum0() {
    // Agafa la darrera llum creada. A CANVIAR si es vol tenir control de totes les llums.
    // De fet el metode showAuxWindowPuntualLight té com a parametre la llum a mostrar en el widget.
    // Es podria cridar diverese vegades.
    if (mon->llums.size()>0)
        showAuxWindowPuntualLight(mon->getLlumActual());
}

// Per a les extensions OPCIONALS
void GLWidget::changePositionLight() {
    // tipus rep el tipus de llum que es vol afegir. Des d'aqui s'afegeix la llum al mon.
    mon->getLlumActual()->setTipusLlum(Puntual);
    mon->llumsToGPU(program);
    updateGL();

}
void GLWidget::changeDirectionalLight() {
    // tipus rep el tipus de llum que es vol afegir. Des d'aqui s'afegeix la llum al mon.
   mon->getLlumActual()->setTipusLlum(Direccional);
   mon->llumsToGPU(program);
   updateGL();
}
void GLWidget::changeSpotLight() {
    // tipus rep el tipus de llum que es vol afegir. Des d'aqui s'afegeix la llum al mon.
    mon->getLlumActual()->setTipusLlum(SpotLight);
    mon->llumsToGPU(program);
    updateGL();
}
void GLWidget::updateXPositionLight(int xposition) {
    // S'ha de modificar la posicio x de la llum activa
    vec4 v = mon->getLlumActual()->getPosition();
    v[0] = (float)xposition;
    mon->getLlumActual()->setPosition(v);
    mon->llumsToGPU(program);
    updateGL();
}

void GLWidget::updateYPositionLight(int yposition) {
    // S'ha de modificar la posicio y de la llum activa
    vec4 v = mon->getLlumActual()->getPosition();
    v[1] = (float)yposition;
    mon->getLlumActual()->setPosition(v);
    mon->llumsToGPU(program);
    updateGL();
}

void GLWidget::updateZPositionLight(int zposition) {
    // S'ha de modificar la posicio z de la llum activa
    vec4 v = mon->getLlumActual()->getPosition();
    v[2] = (float)zposition;
    mon->getLlumActual()->setPosition(v);
    mon->llumsToGPU(program);
    updateGL();
}

void GLWidget::updateLightIntensity(int intens) {
    // S'ha de modificar la intensitat de la llum 0. es podria canviar per la llum actual
    vec3 intensitat;
    intensitat =  mon->getLlumActual()->getDiffuse();
    intensitat[0] = intens/200.0;
    intensitat[1] = intens/200.0;
    intensitat[2] = intens/200.0; // el 200 es l'escala del scrollbar
    mon->getLlumActual()->setDiffuse(intensitat);
    mon->llumsToGPU(program);
    updateGL();

}

void GLWidget::activateLight(){
    mon->getLlumActual()->switchOnOff();
    mon->llumsToGPU(program);
    updateGL();
}

void GLWidget::activaBumpMapping() {
    //OPICIONAL: a implementar a la practica 1
}

void GLWidget::activaEnvMapping() {
    //OPICIONAL: a implementar a la practica 1
}

QGLShaderProgram* GLWidget::InitShader(const char* vShaderFile, const char* fShaderFile){
    QGLShader *vshader = new QGLShader(QGLShader::Vertex, this);
    QGLShader *fshader = new QGLShader(QGLShader::Fragment, this);

    vshader->compileSourceFile(vShaderFile);
    fshader->compileSourceFile(fShaderFile);

    QGLShaderProgram* pr = new QGLShaderProgram(this);
    pr->addShader(vshader);
    pr->addShader(fshader);
    return pr;
}

/**
 * @brief GLWidget::initShadersGPU
 */
void GLWidget::initShadersGPU(){
    allPrograms[0] = InitShader("://resources/vshader1.glsl", "://resources/fshader1.glsl");
    allPrograms[1] = InitShader("://resources/vshaderGouraud.glsl", "://resources/fshaderGouraud.glsl");
    allPrograms[2] = InitShader("://resources/vshaderGouraudTextura.glsl", "://resources/fshaderGouraudTextura.glsl");
    allPrograms[3] = InitShader("://resources/vshaderPhong.glsl", "://resources/fshaderPhong.glsl");
    allPrograms[4] = InitShader("://resources/vshaderPhongTextura.glsl", "://resources/fshaderPhongTextura.glsl");
    allPrograms[5] = InitShader("://resources/vshaderToon.glsl", "://resources/fshaderToon.glsl");

    program = allPrograms[0];
    program->link();
    program->bind();
}

QSize GLWidget::minimumSizeHint() const {
    return QSize(50, 50);
}

QSize GLWidget::sizeHint() const {
    return QSize(400, 400);
}

void GLWidget::initializeGL() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_RGBA);
    glEnable(GL_DOUBLE);
    //Added GL_NMORMALIZE
    //glEnable(GL_NORMALIZE);

    initShadersGPU();

    // Creacio d'una llum per a poder modificar el seus valors amb la interficie
    mon->addLlum(new Llum(Puntual));

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void GLWidget::paintGL() {
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    mon->llumsToGPU(program);
    mon->setAmbientToGPU(program);
    for (int i=0; i<mon->elements.size(); i++){
        mon->elements[i]->toGPU(program);
    }
    mon->draw();
}


void GLWidget::resizeGL(int width, int height) {
    int side = qMin(width, height);
    glViewport((width - side) / 2, (height - side) / 2, side, side);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
#ifdef QT_OPENGL_ES_1
    glOrthof(-1.5, +1.5, -1.5, +1.5, 0.0, 15.0);
#else
    glOrtho(-1.5, +1.5, -1.5, +1.5, 0.0, 15.0);
#endif
    glMatrixMode(GL_MODELVIEW);
}

void GLWidget::newObj(QString fichero){
    qDebug() << fichero;
    Objecte * obj = new Objecte(100000, fichero);
    obj->toGPU(program);
    mon->addObjecte(obj);

    updateGL();
}

void GLWidget::newObjMat(QString fichero,QString fichero2){
    qDebug() << fichero;
    Objecte * obj = new Objecte(100000, fichero, fichero2);
    obj->toGPU(program);
    mon->addObjecte(obj);
    updateGL();
}

// Finestra per a introduir una llum puntual o modificar-ne el seu contingut
void GLWidget::showAuxWindowPuntualLight(Llum *light)
{

    auxWidget = new QWidget;
    auxWidget->move(0,0);

    QVBoxLayout *outer = new QVBoxLayout;
    QHBoxLayout *buttons = new QHBoxLayout;

    QGroupBox *groupBox = new QGroupBox("Light type");
    QRadioButton *radio1 = new QRadioButton("Position light");
    connect(radio1, SIGNAL(clicked()), this, SLOT(changePositionLight()));
    radio1->setChecked(true);

    QRadioButton *radio2 = new QRadioButton("Directional light");
    connect(radio2, SIGNAL(clicked()), this, SLOT(changeDirectionalLight()));
    radio2->setChecked(false); // Quan es clica es modifica el tipus de llum

    QRadioButton *radio3 = new QRadioButton("Spot light");
    connect(radio3, SIGNAL(clicked()), this, SLOT(changeSpotLight()));
    radio3->setChecked(false); // Sols quan es clica es modifica el tipus de  llum

    QVBoxLayout *vbox = new QVBoxLayout;
    vbox->addWidget(radio1);
    vbox->addWidget(radio2);
    vbox->addWidget(radio3);

    groupBox->setLayout(vbox);
    buttons->addWidget(groupBox);

    QGroupBox *groupBox2 = new QGroupBox("Selection");

    QRadioButton *transparent1 = new QRadioButton("&On");
    transparent1->setChecked(true);
    connect(transparent1, SIGNAL(clicked()), this, SLOT(activateLight()));

    QVBoxLayout *vbox2 = new QVBoxLayout;
    vbox2->addWidget(transparent1);

    groupBox2->setLayout(vbox2);
    buttons->addWidget(groupBox2);
    outer->addLayout(buttons);

    // light source intensity
    QSlider* lightSlider = new QSlider(Qt::Horizontal);
    lightSlider->setTickPosition(QSlider::TicksBelow);
    lightSlider->setMinimum(0);
    lightSlider->setMaximum(200);
    if (light == NULL)
        lightSlider->setSliderPosition(100);
    else
        lightSlider->setSliderPosition(100*light->getDiffuse()[0]);
    connect(lightSlider,SIGNAL(valueChanged(int)),this,SLOT(updateLightIntensity(int)));
    QLabel* lightLabel = new QLabel("Light intensity = ");
    QLabel* lightLabelValue = new QLabel();
    lightLabelValue->setNum(100 * light->getDiffuse()[0]);
    connect(lightSlider,SIGNAL(valueChanged(int)),lightLabelValue,SLOT(setNum(int)));
    QHBoxLayout *hboxLight = new QHBoxLayout;
    hboxLight->addWidget(lightLabel);
    hboxLight->addWidget(lightLabelValue);
    outer->addLayout(hboxLight);
    outer->addWidget(lightSlider);

    // X slider
    QSlider* XSlider = new QSlider(Qt::Horizontal);
    XSlider->setTickPosition(QSlider::TicksBelow);
    XSlider->setMinimum(-200);
    XSlider->setMaximum(200);
    if (light == NULL)
        XSlider->setSliderPosition(100);
    else
        XSlider->setSliderPosition(light->getPosition()[0]);
    connect(XSlider,SIGNAL(valueChanged(int)),this,SLOT(updateXPositionLight(int)));
    QLabel* XLabel = new QLabel("X Position = ");
    QLabel* XLabelValue = new QLabel();
    XLabelValue->setNum(0);
    connect(XSlider,SIGNAL(valueChanged(int)),XLabelValue,SLOT(setNum(int)));
    QHBoxLayout *hboxX = new QHBoxLayout;
    hboxX->addWidget(XLabel);
    hboxX->addWidget(XLabelValue);
    outer->addLayout(hboxX);
    outer->addWidget(XSlider);

    // Y slider
    QSlider* YSlider = new QSlider(Qt::Horizontal);
    YSlider->setTickPosition(QSlider::TicksBelow);
    YSlider->setMinimum(-200);
    YSlider->setMaximum(200);
    if (light == NULL)
        YSlider->setSliderPosition(100);
    else
        YSlider->setSliderPosition(light->getPosition()[1]);

    connect(YSlider,SIGNAL(valueChanged(int)),this,SLOT(updateYPositionLight(int)));
    QLabel* YLabel = new QLabel("Y Position = ");
    QLabel* YLabelValue = new QLabel();
    YLabelValue->setNum(0);
    connect(YSlider,SIGNAL(valueChanged(int)),YLabelValue,SLOT(setNum(int)));
    QHBoxLayout *hboxY = new QHBoxLayout;
    hboxY->addWidget(YLabel);
    hboxY->addWidget(YLabelValue);
    outer->addLayout(hboxY);
    outer->addWidget(YSlider);

    // Z Slider
    QSlider* ZSlider = new QSlider(Qt::Horizontal);
    ZSlider->setTickPosition(QSlider::TicksBelow);
    ZSlider->setMinimum(-200);
    ZSlider->setMaximum(200);
    if (light == NULL)
        ZSlider->setSliderPosition(100);
    else
        ZSlider->setSliderPosition(light->getPosition()[2]);
    connect(ZSlider,SIGNAL(valueChanged(int)),this,SLOT(updateZPositionLight(int)));
    QLabel* ZLabel = new QLabel("Z Position = ");
    QLabel* ZLabelValue = new QLabel();
    ZLabelValue->setNum(0);
    connect(ZSlider,SIGNAL(valueChanged(int)),ZLabelValue,SLOT(setNum(int)));
    QHBoxLayout *hboxZ = new QHBoxLayout;
    hboxZ->addWidget(ZLabel);
    hboxZ->addWidget(ZLabelValue);
    outer->addLayout(hboxZ);
    outer->addWidget(ZSlider);

    // Es poden afegir més camps a explorar amb el widget
    // Afegiu-los aqui sota

    auxWidget->setLayout(outer);
    auxWidget->show();
}
