/* BZOJ-1336: [Balkan2002]Alien最小圆覆盖
 *   随机增量法 */
#include <cstdio>
#include <cmath>
#include <algorithm>
 
struct point_t
{
    double x, y;
    point_t(double x = 0.0, double y = 0.0)
        : x(x), y(y) {}
 
    friend point_t operator + (const point_t& a, const point_t& b)
    {
        return point_t(a.x + b.x, a.y + b.y);
    }
 
    friend point_t operator - (const point_t& a, const point_t& b)
    {
        return point_t(a.x - b.x, a.y - b.y);
    }
 
    friend double operator * (const point_t& a, const point_t& b)
    {
        return a.x * b.y - b.x * a.y;
    }
};
 
point_t middle_point(const point_t& a, const point_t& b)
{
    return point_t((a.x + b.x) * 0.5, (a.y + b.y) * 0.5);
}
 
point_t incenter(const point_t& a, const point_t& b, const point_t& c)
{
    point_t p1 = b - a, p2 = c - a;
    std::swap(p1.x, p1.y);
    std::swap(p2.x, p2.y);
    p1.x = -p1.x, p2.x = -p2.x;
    point_t m1 = middle_point(a, b);
    point_t m2 = middle_point(a, c);
 
    double k = (m2 - m1) * p2 / (p1 * p2);
    return point_t(k * p1.x, k * p1.y) + m1;
}
 
double dist(const point_t& a, const point_t& b)
{
    point_t t = a - b;
    return t.x * t.x + t.y * t.y;
}
 
point_t pt[160000];
const double eps = 1.0e-7;
 
int main()
{
    int N;
    std::scanf("%d", &N);
    for(int i = 0; i != N; ++i)
        std::scanf("%lf %lf", &pt[i].x, &pt[i].y);
    std::random_shuffle(pt, pt + N);
    point_t c = pt[0];
    double r = 0.0;
    for(int i = 1; i != N; ++i)
    {
        if(dist(pt[i], c) <= r + eps)
            continue;
        c = pt[i], r = 0.0;
        for(int j = 0; j != i; ++j)
        {
            if(dist(pt[j], c) <= r + eps)
                continue;
            c = middle_point(pt[i], pt[j]);
            r = dist(c, pt[i]);
            for(int k = 0; k != j; ++k)
            {
                if(dist(pt[k], c) <= r + eps)
                    continue;
                c = incenter(pt[i], pt[j], pt[k]);
                r = dist(c, pt[i]);
            }
        }
    }
 
    std::printf("%lf\n", std::sqrt(r));
    std::printf("%lf %lf", c.x, c.y);
    return 0;
}
