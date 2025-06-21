#ifndef CURVE_H
#define CURVE_H

#include <vector>
#include <QVector2D>
#include <QVector2D>
#include <QOpenGLFunctions>
#include <QOpenGLBuffer>
struct GLfloat3 {
    GLfloat r, g, b;
};

class Curve {
public:
    Curve();

    virtual ~Curve() = default;

    void setWidth(float w);
    float getWidth() const;

    void setColour(GLfloat r, GLfloat g, GLfloat b);
    GLfloat3 getColour() const;

    void clearPoints();
    const std::vector<QVector2D>& getPoints() const;
    const std::vector<QVector2D>& getControlPoints() const;
    QVector2D getControlPointAtIndex();
    void moveControlPoint(int index, const QVector2D& newPos);
    void updateGpuBuffer();
    mutable QOpenGLBuffer curveVBO = QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    int vertexCount = 0;
protected:
    std::vector<QVector2D> points;
    std::vector<QVector2D> controlPoints;
    float width = 1.0f;
    GLfloat3 colour = {0.5f, 0.5f, 0.5f};
};

#endif
