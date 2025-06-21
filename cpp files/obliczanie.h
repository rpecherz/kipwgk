#pragma GCC optimize("O3,inline,unroll-loops")
#pragma float_control(precise, on)
#pragma GCC target("avx,avx2,fma,f16c,bmi,bmi2,lzcnt,popcnt")
#pragma GCC target("sse3,ssse3,sse4.1,sse4.2")
#pragma GCC target("tune=native")
#pragma optimize("gt", on)
#ifndef OBLICZANIE_H
#define OBLICZANIE_H

#include <vector>
#include <algorithm>
#include <cmath>
#include <cstdint>
#include <utility>
#include <QVector2D>
#include <QLineF>

using namespace std;

// Fast logarithm approximation
inline float fastlogf(const float& x)
{
    union { float f; uint32_t i; } vx = { x };
    float y = vx.i;
    y *= 8.2629582881927490e-8f;
    return (y - 87.989971088f);
}

// Secant method for root finding
inline double metoda_siecznych(double (*f)(double), double x0, double x1, int iterations, double EPSILON) {
    int iter = 0;
    double x_prev = x0;
    double x_curr = x1;
    double x_next;

    while(fabs(f(x_curr)) > EPSILON && iter < iterations) {
        double f0 = f(x_prev);
        double f1 = f(x_curr);
        x_next = x_curr - f1 * (x_curr - x_prev) / (f1 - f0);
        x_prev = x_curr;
        x_curr = x_next;
        iter++;
    }
    return x_curr;
}
//todo
inline double Clenshaw() {
    return 0;
}

using Point = pair<double, double>;
//todo
vector<Point> decrease_bezier(const vector<Point>& V);

inline double cross(const QVector2D& O, const QVector2D& A, const QVector2D& B) {
    return (A.x() - O.x()) * (B.y() - O.y()) - (A.y() - O.y()) * (B.x() - O.x());
}

vector<QVector2D> monotone_chain(vector<QVector2D> pts);

int next_tangent(const vector<QVector2D>& hull, const QVector2D& p);

vector<QVector2D> fast_convex_hull(vector<QVector2D>& pts);


inline vector<Point> decrease_bezier(const vector<Point>& V)
{
    int n = V.size() - 1;
    vector<Point> W_I(n), W_II(n);

    W_I[0] = V[0];
    for (int k = 1; k <= n / 2; ++k) {
        double alpha = static_cast<double>(k) / (n - k);
        W_I[k].first  = (1 + alpha) * V[k].first  - alpha * W_I[k - 1].first;
        W_I[k].second = (1 + alpha) * V[k].second - alpha * W_I[k - 1].second;
    }

    W_II[n - 1] = V[n];
    for (int k = n - 1 - 1; k > n / 2; --k) {
        double alpha = static_cast<double>(n) / (k + 1);
        double beta = 1.0 - alpha;
        W_II[k].first  = (alpha * V[k + 1].first + beta * W_II[k + 1].first);
        W_II[k].second = (alpha * V[k + 1].second + beta * W_II[k + 1].second);
    }

    vector<Point> W(n);
    for (int k = 0; k <= n / 2; ++k) W[k] = W_I[k];
    for (int k = n / 2 + 1; k < n; ++k) W[k] = W_II[k];
    return W;
}

inline vector<QVector2D> monotone_chain(vector<QVector2D> pts) {
    if (pts.size() <= 1) return pts;
    sort(pts.begin(), pts.end(), [](const QVector2D& a, const QVector2D& b) {
        return a.x() < b.x() || (a.x() == b.x() && a.y() < b.y());
    });

    vector<QVector2D> lower, upper;

    for(const auto& p : pts) {
        while (lower.size() >= 2 && cross(lower[lower.size()-2], lower.back(), p) <= 0)
            lower.pop_back();
        lower.push_back(p);
    }

    for(int i = pts.size() - 1; i >= 0; --i)
    {
        const auto& p = pts[i];
        while (upper.size() >= 2 && cross(upper[upper.size()-2], upper.back(), p) <= 0)
            upper.pop_back();
        upper.push_back(p);
    }

    lower.pop_back();
    upper.pop_back();
    lower.insert(lower.end(), upper.begin(), upper.end());
    return lower;
}



inline int next_tangent(const vector<QVector2D>& hull, const QVector2D& p)
{
    int best = 0;
    for(int i = 1; i < hull.size(); ++i)
    {
        if (cross(p, hull[best], hull[i]) <= 0 && p.distanceToPoint(hull[i]) > p.distanceToPoint(hull[best]))
        {
            best = i;
        }
    }
    return best;
}

inline vector<QVector2D> enforce_ccw(const vector<QVector2D>& pts)
{
    vector<QVector2D> hull;
    for(const auto& p : pts) {
        while(hull.size() >= 2 &&
               cross(hull[hull.size() - 2], hull[hull.size() - 1], p) <= 0)
            hull.pop_back();
        hull.push_back(p);
    }

    if(hull.size() >= 3 && hull.front() != hull.back())
        hull.push_back(hull.front());

    return hull;
}

inline vector<QVector2D> fast_convex_hull(vector<QVector2D>& pts)
{
    int n = pts.size();
    if(n <= 1)
        return pts;

    int max_m = 1;
    while(max_m * max_m < n)
        max_m *= 2;

    for(int m = 1; m <= max_m; m *= 2)
    {
        int t = (n + m - 1) / m;
        vector<vector<QVector2D>> hulls(t);

        for(int i = 0; i < t; ++i)
        {
            int l = i * m, r = min(n, (i + 1) * m);
            vector<QVector2D> subset(pts.begin() + l, pts.begin() + r);
            hulls[i] = monotone_chain(subset);
        }

        vector<QVector2D> hull;
        QVector2D start = *min_element(pts.begin(), pts.end(), [](const QVector2D& a, const QVector2D& b) {
            return a.x() < b.x() || (a.x() == b.x() && a.y() < b.y());
        });

        hull.push_back(start);
        for(int step = 0;; ++step)
        {
            QVector2D last = hull.back();
            QVector2D best;
            bool found = false;

            for (int i = 0; i < t; ++i) {
                if (hulls[i].empty()) continue;
                int j = next_tangent(hulls[i], last);
                const QVector2D& candidate = hulls[i][j];

                if (!found || cross(last, best, candidate) < 0 || (cross(last, best, candidate) == 0 && last.distanceToPoint(candidate) > last.distanceToPoint(best)))
                {
                    best = candidate;
                    found = true;
                }
            }

            if (!found || best == hull.front())
                return hull;
            hull.push_back(best);

        }
    }

    return monotone_chain(pts);
}



#endif // OBLICZANIE_H
