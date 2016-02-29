#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <math.h>
#include <mon.h>
#include <QtWidgets>
#include <QGLWidget>
#include <QTime>

#define PROGRAM_VERTEX_ATTRIBUTE 0
#define PROGRAM_COLOR_ATTRIBUTE 1

class QGLShaderProgram;

class GLWidget : public QGLWidget
{
    Q_OBJECT

public:
    GLWidget(QWidget *parent = 0);
    ~GLWidget();

    QSize minimumSizeHint() const;
    QSize sizeHint() const;

    void newObj(QString fichero);
public slots:

    void activaToonShader();
    void activaPhongShader();
    void activaGouraudShader();
    void activaPhongTex();
    void activaGouraudTex();
    // Per extensions
    void activaBumpMapping();
    void activaEnvMapping();

    void ensenyaMenuLlum0();
    void changePositionLight();
    void changeDirectionalLight();
    void changeSpotLight();
    void activateLight();
    void updateXPositionLight(int xposition);
    void updateYPositionLight(int yposition);
    void updateZPositionLight(int zposition);
    void updateLightIntensity(int intensity);
    void showAuxWindowPuntualLight(Llum *light);

protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);

private:
    QWidget* auxWidget; // window for parameters

    Mon *mon;

    QGLShaderProgram *program;

    void InitShader( const char* vertexShaderFile, const char* fragmentShaderFile );
    void initShadersGPU();
};

#endif // GLWIDGET_H
