#ifndef NIFS3CURVE_H
#define NIFS3CURVE_H

#include "curve.h"

class nifs3curve : public Curve
{
public:
    nifs3curve();
    void addPoint(float x, float y);
    void addControlPoint(const QVector2D& cp);
    void removeLastPoint();
    void clearPoints();
    void setPoints(const std::vector<QVector2D>& newPoints);

    const std::vector<QVector2D>& getPoints() const;
    const std::vector<QVector2D>& getControlPoints() const;

    void moveControlPoint(int index, const QVector2D& newPos);
};

#endif
