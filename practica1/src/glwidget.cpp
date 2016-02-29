#include <math.h>
#include <glwidget.h>
#include <QtDebug>
#include <QString>


GLWidget::GLWidget(QWidget *parent) : QGLWidget(QGLFormat(QGL::SampleBuffers), parent) {
    setFocusPolicy( Qt::StrongFocus );
    mon = new Mon();
}

GLWidget::~GLWidget() {
    makeCurrent();
    delete mon;
}

// Metodes que es criden des dels menús

void GLWidget::activaToonShader() {
    //A implementar a la practica 1
}

void GLWidget::activaPhongShader() {
    //A implementar a la practica 1
}

void GLWidget::activaGouraudShader() {
    //A implementar a la practica 1
}

void GLWidget::activaPhongTex() {
    //A implementar a la practica 1
}

void GLWidget::activaGouraudTex() {
    //A implementar a la practica 1
    cout<<"Estic a Gouraud Tex"<<endl;

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

}
void GLWidget::changeDirectionalLight() {
    // tipus rep el tipus de llum que es vol afegir. Des d'aqui s'afegeix la llum al mon.
   mon->getLlumActual()->setTipusLlum(Direccional);

}
void GLWidget::changeSpotLight() {
    // tipus rep el tipus de llum que es vol afegir. Des d'aqui s'afegeix la llum al mon.
    mon->getLlumActual()->setTipusLlum(SpotLight);

}
void GLWidget::updateXPositionLight(int xposition) {
    // S'ha de modificar la posicio x de la llum activa
    vec4 v = mon->getLlumActual()->getLightPosition();
    v[0] = (float)xposition;
    mon->getLlumActual()->setLightPosition(v);
}

void GLWidget::updateYPositionLight(int yposition) {
    // S'ha de modificar la posicio y de la llum activa
    vec4 v = mon->getLlumActual()->getLightPosition();
    v[1] = (float)yposition;
    mon->getLlumActual()->setLightPosition(v);
}

void GLWidget::updateZPositionLight(int zposition) {
    // S'ha de modificar la posicio z de la llum activa
    vec4 v = mon->getLlumActual()->getLightPosition();
    v[2] = (float)zposition;
    mon->getLlumActual()->setLightPosition(v);
}

void GLWidget::updateLightIntensity(int intens) {
    // S'ha de modificar la intensitat de la llum 0. es podria canviar per la llum actual
    vec3 intensitat;
    intensitat =  mon->getLlumActual()->getDiffuseIntensity();
    intensitat[0] = intens/200.0;
    intensitat[1] = intens/200.0;
    intensitat[2] = intens/200.0; // el 200 es l'escala del scrollbar

     mon->getLlumActual()->setDiffuseIntensity(intensitat);
}

void GLWidget::activateLight(){
    mon->getLlumActual()->switchOnOff();
}

void GLWidget::activaBumpMapping() {
    //OPICIONAL: a implementar a la practica 1
}

void GLWidget::activaEnvMapping() {
    //OPICIONAL: a implementar a la practica 1
}

void GLWidget::InitShader(const char* vShaderFile, const char* fShaderFile){
    QGLShader *vshader = new QGLShader(QGLShader::Vertex, this);
    QGLShader *fshader = new QGLShader(QGLShader::Fragment, this);

    vshader->compileSourceFile(vShaderFile);
    fshader->compileSourceFile(fShaderFile);

    program = new QGLShaderProgram(this);
    program->addShader(vshader);
    program->addShader(fshader);
    program->link();
    program->bind();
}

/**
 * @brief GLWidget::initShadersGPU
 */
void GLWidget::initShadersGPU(){
    InitShader("://resources/vshader1.glsl", "://resources/fshader1.glsl");
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

    initShadersGPU();

    // Creacio d'una llum per apoder modificar el seus valors amb la interficie
    Llum *l = new Llum(Puntual);
    mon->addLlum(l);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void GLWidget::paintGL() {
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
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
        lightSlider->setSliderPosition(100*light->getDiffuseIntensity()[0]);
    connect(lightSlider,SIGNAL(valueChanged(int)),this,SLOT(updateLightIntensity(int)));
    QLabel* lightLabel = new QLabel("Light intensity = ");
    QLabel* lightLabelValue = new QLabel();
    lightLabelValue->setNum(100 * light->getDiffuseIntensity()[0]);
    connect(lightSlider,SIGNAL(valueChanged(int)),lightLabelValue,SLOT(setNum(int)));
    QHBoxLayout *hboxLight = new QHBoxLayout;
    hboxLight->addWidget(lightLabel);
    hboxLight->addWidget(lightLabelValue);
    outer->addLayout(hboxLight);
    outer->addWidget(lightSlider);

    // X slider
    QSlider* XSlider = new QSlider(Qt::Horizontal);
    XSlider->setTickPosition(QSlider::TicksBelow);
    XSlider->setMinimum(0);
    XSlider->setMaximum(200);
    if (light == NULL)
        XSlider->setSliderPosition(100);
    else
        XSlider->setSliderPosition(light->getLightPosition()[0]);
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
    YSlider->setMinimum(0);
    YSlider->setMaximum(200);
    if (light == NULL)
        YSlider->setSliderPosition(100);
    else
        YSlider->setSliderPosition(light->getLightPosition()[1]);

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
    ZSlider->setMinimum(0);
    ZSlider->setMaximum(200);
    if (light == NULL)
        ZSlider->setSliderPosition(100);
    else
        ZSlider->setSliderPosition(light->getLightPosition()[2]);
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

