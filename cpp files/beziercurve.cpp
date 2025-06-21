#include "beziercurve.h"
#include <QDebug>
#include <QtMath>

BezierCurve::BezierCurve() = default;

void BezierCurve::addPoint(float x, float y)
{
    points.push_back(QVector2D(x, y));
}

void BezierCurve::addControlPoint(const QVector2D& cp)
{
    controlPoints.push_back(cp);
    waga.push_back(1.0f);
}

void BezierCurve::removeLastPoint()
{
    if (!controlPoints.empty())
    {
        controlPoints.pop_back();
        waga.pop_back();
    }
}

void BezierCurve::clearPoints()
{
    points.clear();
}

void BezierCurve::setPoints(const std::vector<QVector2D>& newPoints)
{
    points = newPoints;
}

const std::vector<QVector2D>& BezierCurve::getPoints() const
{
    return points;
}

const std::vector<QVector2D>& BezierCurve::getControlPoints() const
{
    return controlPoints;
}


void BezierCurve::translateBezier(float dx, float dy)
{
    for(auto& XD : controlPoints)
        XD += QVector2D(dx, dy);
}

void BezierCurve::rotateBezier(float degrees)
{
    const float radians = qDegreesToRadians(degrees);
    const float cosTheta = qCos(radians);
    const float sinTheta = qSin(radians);

    for(auto& point : controlPoints)
    {
        const float x = point.x();
        const float y = point.y();
        point.setX(x * cosTheta - y * sinTheta);
        point.setY(x * sinTheta + y * cosTheta);
    }
}

void BezierCurve::scaleBezier(float factor)
{
    for(auto& XD : controlPoints)
        XD *= factor;
}



void BezierCurve::setWeight(int index, double weight)
{
    if (index >= 0 && index < static_cast<int>(waga.size()))
    {
        waga[index] = weight;
    }
    return;
    /*else if (index >= 0 && index >= static_cast<int>(waga.size())) {
        waga.resize(index + 1, 1.0f);
        waga[index] = weight;
    }*/
}
