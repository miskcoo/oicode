/* BZOJ-2829: 信用卡凸包
 *  凸包 */
#include <cstdio>
#include <cstring>
#include <algorithm>
#include <cmath>
 
const double eps = 1.0e-7;
const int MaxN = 40100;
struct point_t
{
    double x, y;
     
    point_t(double x = 0.0, double y = 0.0)
        : x(x), y(y) {}
     
    friend bool operator < (const point_t& a, const point_t& b)
    {
        if(std::fabs(a.x - b.x) < eps)
            return a.y < b.y;
        return a.x < b.x;
    }
     
    friend point_t operator - (const point_t& a, const point_t& b)
    {
        return point_t(a.x - b.x, a.y - b.y);
    }
     
    friend point_t operator + (const point_t& a, const point_t& b)
    {
        return point_t(a.x + b.x, a.y + b.y);
    }
     
    friend double operator * (const point_t& a, const point_t& b)
    {
        return a.x * b.y - a.y * b.x;
    }
};
 
point_t pt[MaxN], conv[MaxN], conv2[MaxN];
 
int convex_hull(int n, const point_t* pt, point_t* st)
{
    int t = 1;
    st[0] = pt[0];
     
    for(int i = 1; i != n; ++i)
    {
        while(t >= 2 && (st[t - 1] - st[t - 2]) * (pt[i] - st[t - 1]) < -eps)
            --t;
        st[t++] = pt[i];
    }
     
    return t;
}
 
point_t get_pt(double x, double y, double sin, double cos)
{
    return point_t(x * cos - y * sin, x * sin + y * cos);
}
 
double get_dist(const point_t& a, const point_t& b)
{
    point_t t = a - b;
    return std::sqrt(t.x * t.x + t.y * t.y);
}
 
bool cmp_eq(const point_t& a, const point_t& b)
{
    return std::fabs(a.x - b.x) < eps && std::fabs(a.y - b.y) < eps;
}
 
int main()
{
    int n;
    std::scanf("%d", &n);
    double w, h, r;
    std::scanf("%lf %lf %lf", &h, &w, &r);
    w -= r * 2.0, h -= r * 2.0;
    for(int i = 0; i != n; ++i)
    {
        double x, y, theta;
        std::scanf("%lf %lf %lf", &x, &y, &theta);
        double sin = std::sin(theta), cos = std::cos(theta);
        point_t p(x, y);
        pt[i * 4] = p + get_pt(-w * 0.5, -h * 0.5, sin, cos);
        pt[i * 4 + 1] = p + get_pt(-w * 0.5, h * 0.5, sin, cos);
        pt[i * 4 + 2] = p + get_pt(w * 0.5, -h * 0.5, sin, cos);
        pt[i * 4 + 3] = p + get_pt(w * 0.5, h * 0.5, sin, cos);
    }
     
    n <<= 2;
    std::sort(pt, pt + n);
    n = std::unique(pt, pt + n, cmp_eq) - pt;
    int t1 = convex_hull(n, pt, conv);
    std::reverse(pt, pt + n);
    int t2 = convex_hull(n, pt, conv2);
    for(int i = 0; i != t2; ++i)
        conv[t1 + i] = conv2[i];
    point_t* conv_end = std::unique(conv, conv + t1 + t2, cmp_eq);
    *conv_end = conv[1];
    double ans = 0.0;
     
    const double pi = 3.1415926535897;
    for(point_t* cc = conv + 1; cc != conv_end; ++cc)
    {
        ans += get_dist(*cc, *(cc - 1));
        point_t p1 = *cc - *(cc - 1);
        point_t p2 = *cc - *(cc + 1);
        double cos = std::fabs(p1.x * p2.x + p1.y * p2.y);
        if(std::fabs(cos) < eps)
        {
            ans += pi * r * 0.5;
        } else {
            cos /= std::sqrt(p1.x * p1.x + p1.y * p1.y);
            cos /= std::sqrt(p2.x * p2.x + p2.y * p2.y);
            double tan = std::sqrt(1.0 - cos * cos) / cos;
            double angle = std::atan(tan);
            while(angle > pi * 0.5 + eps)
                angle -= pi * 0.5;
            ans += angle * r;      
        }
        if(cmp_eq(*cc, *conv))
            break;
    }
     
    std::printf("%.2lf", ans);
    return 0;
}
