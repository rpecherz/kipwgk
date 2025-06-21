#ifndef BEZIERCURVE_H
#define BEZIERCURVE_H

#include "curve.h"

class BezierCurve : public Curve {
public:
    BezierCurve();

    void addPoint(float x, float y);
    void addControlPoint(const QVector2D& cp);
    void removeLastPoint();
    void clearPoints();
    void setPoints(const std::vector<QVector2D>& newPoints);
    void setWeight(int index,double weight);
    void translateBezier(float dx, float dy);
    void rotateBezier(float degrees);
    void scaleBezier(float factor);



    float getWeight(int index)
    {
        //todo posible wyjebka pamiec
        return waga[index];
    }
    const std::vector<double>& getWeights() const
    {
        return waga;
    }

    const std::vector<QVector2D>& getPoints() const;
    const std::vector<QVector2D>& getControlPoints() const;

protected:
    std::vector<double> waga;
    void c0continuity();
    void c1continuity();
    void c2continuity();
};

#endif
