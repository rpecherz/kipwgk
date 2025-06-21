#ifndef OIFS3CURVE_H
#define OIFS3CURVE_H

#include "curve.h"

class oifs3curve : public Curve
{
public:
    oifs3curve();
    void addPoint(float x, float y);
    void addControlPoint(const QVector2D& cp);
    void removeLastPoint();
    void clearPoints();
    void setPoints(const std::vector<QVector2D>& newPoints);

    const std::vector<QVector2D>& getPoints() const;
    const std::vector<QVector2D>& getControlPoints() const;

    void moveControlPoint(int index, const QVector2D& newPos);
};

#endif // OIFS3CURVE_H
