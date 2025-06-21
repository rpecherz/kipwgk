#pragma once

#include <QOpenGLWidget>
#include <QOpenGLFunctions_4_5_Core>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QVector2D>
#include <QVector>
#include <QInputDialog>
#include <QDoubleSpinBox>
#include <QMatrix4x4>
#include <QWheelEvent>
#include <QMouseEvent>
#include <QMutex>
#include <QOpenGLTexture>
#include <QTimer>
#include "beziercurve.h"
#include "nifs3curve.h"
#include "oifs3curve.h"

#include "compactinfowidget.h"


enum class CurveType
{
    Bezier,
    NIFS3,
    OIFS3
};

enum ContinuityType { C0, C1, C2 };


struct CurveEntry
{
    CurveType type;
    //todo QString name;
    bool wasChanged = false;
    std::variant<BezierCurve, nifs3curve, oifs3curve> curve;

    CurveEntry(CurveType t, BezierCurve* b)
        : type(t), curve(*b) {}

    CurveEntry(CurveType t, nifs3curve* n)
        : type(t), curve(*n) {}

    CurveEntry(CurveType t, oifs3curve* o)
        : type(t), curve(*o) {}

};

enum class CurveMode { Bezier, NIFS3, OIFS3 };
extern CurveMode currentCurveMode;

enum class drawMode { Curve, Surface, Animation };
extern drawMode currentDrawMode;

struct BezierSegment {
    QVector2D p0, p1, p2, p3;
};



class OpenGLCanvasWidget : public QOpenGLWidget, protected QOpenGLFunctions {
    Q_OBJECT

public:
    OpenGLCanvasWidget(QWidget *parent = nullptr);
    ~OpenGLCanvasWidget();

    QSize minimumSizeHint() const override;
    QSize sizeHint() const override;
    void clearPoints();
    void drawnifs3();
    void drawBezier();
    void addNewCurve(CurveMode& sitek);
    void addPointToCurrentCurve(float x, float y);
    void removeLastPointFromCurrentCurve();
    void deleteCurrentCurve();
    int getCurveCount() const;
    void setCurrentCurveIndex(int index);
    int getCurrentCurveIndex();
    std::vector<CurveEntry>* get_list();
    void removeLastPoint();
    void setbezierColour(GLfloat r,GLfloat g, GLfloat b);
    void deletecontrolPointbyclick(int index);
    void draw_otoczka_wypukla();
    void setBackgroundImage(const QImage& image);
    void clearBackground();
    void otoczka_if(),kratka_if();
    void set_bezier_algo(int x);
    void cord(double x1,double y1,double x2,double y2);
    void setTimerSpeed(int speed)
    {
        timer = speed;
    }
    void change_bezier_eval(int index)
    {
        bezier_algo_type = index;
    }
protected:
    int animationProgress = 0;
    QTimer animationTimer;
    int timer;
    int akt_index_curve = -1;
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void wheelEvent(QWheelEvent *event) override;
private:
    QMutex renderMutex;
    bool rysuj_kratke = true;
    std::vector<BezierSegment> generateBezierSegments(const std::vector<QVector2D>& points, float tension = 0.5f);

    QVector2D windowToWorld(const QVector2D& windowPos) const;
    QOpenGLShaderProgram shader;
    QOpenGLVertexArrayObject vaoAxes;
    QOpenGLBuffer vboAxes;
    QOpenGLVertexArrayObject vaoGrid;
    QOpenGLBuffer vboGrid;
    QOpenGLVertexArrayObject vaoCurve;
    QOpenGLBuffer vboCurve;

    GLuint textureID = 0;
    QMenu* wspolrzedneMenu;

    // pomocnicze do setbackgroun d dla okna opengl nie apki
    QOpenGLTexture* backgroundTexture = nullptr;
    QOpenGLVertexArrayObject vaoBackground;
    QOpenGLBuffer vboBackground{QOpenGLBuffer::VertexBuffer};
    QOpenGLShaderProgram backgroundShader;


    QImage backgroundImage;
    bool hasBackground = false;
    float backgroundZoom = 1.0f;
    QVector2D backgroundPan{0.0f, 0.0f};

    void initializeBackground();
    void cleanupBackground();
    void zoomBackground(float factor, QPointF center),setBackgroundPan(QVector2D pan),setBackgroundZoom(float zoom);

    int bezier_algo_type = 0,selectedCurveIndex = -1,selectedControlPointIndex = -1;
    bool draw_or_not = false, isDragging = false, isPanning = false, isPrzesuwac = false;
    QVector2D lastPanPos,lastpoint;;
    void loadTexture(),drawBackground();

    float hitboxRadius = 6.0f;

    QMatrix4x4 projection;
    QMatrix4x4 view;
    float zoom = 1.0f;
    QPoint lastMousePos;
    QVector2D panOffset = {0.0f, 0.0f};

    std::vector<QVector2D> points;
    std::vector<CurveEntry> curves;
    QVector2D dragStart,bezierTranslation = {0.0f, 0.0f};
    float bezierRotation = 0.0f;
    float bezierScale = 1.0f;
    std::vector<QVector2D> otoczka;
    /* todo void enforceC0(BezierCurve& A, BezierCurve& B) {
        B.getcontrolPoints[0] = A.controlPoints.back();
    }
    void enforceC1(BezierCurve& A, BezierCurve& B)
    {
        QVector2D p_n = controlPoints[A.controlPoints.size() - 1];
        QVector2D p_n_minus1 = A.controlPoints[A.controlPoints.size() - 2];

        QVector2D tangent = p_n - p_n_minus1;
        B.controlPoints[0] = p_n;
        B.controlPoints[1] = p_n + tangent;
    }
    void enforceC2(BezierCurve& A, BezierCurve& B)
    {
        QVector2D p_n     = A.controlPoints[A.controlPoints.size() - 1];
        QVector2D p_nm1   = A.controlPoints[A.controlPoints.size() - 2];
        QVector2D p_nm2   = A.controlPoints[A.controlPoints.size() - 3];

        QVector2D d1 = p_n - p_nm1;
        QVector2D d2 = p_nm1 - p_nm2;

        QVector2D curvature = d1 - d2;
        B.controlPoints[0] = p_n;
        B.controlPoints[1] = p_n + d1;
        B.controlPoints[2] = p_n + 2 * d1 - d2; // Match curvature
    }*/
    void setupAxes();
    void setupGrid();
signals:
    void PointAdded(const QVector2D& lastpoint);
    void AktPozycja(const QVector2D& lastpoint);
    void sizchange();
};




