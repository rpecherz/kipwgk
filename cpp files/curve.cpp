#include "curve.h"
#include <QDebug>

Curve::Curve() {}

void Curve::setWidth(float w)
{
    if(w >= 0.0f && w <= 10.0f)
        width = w;
}


void Curve::updateGpuBuffer()
{
    if (!curveVBO.isCreated())
        curveVBO.create();

    curveVBO.bind();
    const auto& pts = getPoints();
    curveVBO.allocate(pts.data(), pts.size() * sizeof(QVector2D));
    vertexCount = pts.size();
    curveVBO.release();
}

float Curve::getWidth() const
{
    return width;
}

void Curve::setColour(GLfloat r, GLfloat g, GLfloat b)
{
    if (r < 0.0f || g < 0.0f || b < 0.0f || r > 255.0f || g > 255.0f || b > 255.0f) {
        qDebug() << "zle wartosic";
        return;
    }
    colour = { r / 255.0f, g / 255.0f, b / 255.0f };
}

void Curve::moveControlPoint(int index, const QVector2D& newPos)
{
    if (index >= 0 && index < static_cast<int>(controlPoints.size()))
    {
        controlPoints[index] = newPos;
    }
}


GLfloat3 Curve::getColour() const
{
    return colour;
}

void Curve::clearPoints() {
    points.clear();
    controlPoints.clear();
}

const std::vector<QVector2D>& Curve::getPoints() const
{
    return points;
}

const std::vector<QVector2D>& Curve::getControlPoints() const
{
    return controlPoints;
}

/*std::optional<QVector2D> Curve::getControlPointAtIndex(int index)
{
    if(index < 0 || index >= Curve::controlPoints.size())
        return std::nullopt;
    return controlPoints[index];
}*/
