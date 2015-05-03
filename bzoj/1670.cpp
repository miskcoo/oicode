/* BZOJ-1670: [Usaco2006 Oct]Building the Moat护城河的挖掘
 *   凸包 */
#include <cstdio>
#include <cmath>
#include <algorithm>
 
const int MaxN = 5010;
struct point_t
{
    long long x, y;
    point_t(long long x = 0, long long y = 0) : x(x), y(y) {}
    bool operator < (const point_t& r) const
    {
        if(x == r.x) return y < r.y;
        return x < r.x;
    }
} pt[MaxN], st[MaxN];
 
point_t operator - (const point_t& a, const point_t& b)
{
    return point_t(a.x - b.x, a.y - b.y);
}
 
long long cross(const point_t& a, const point_t& b)
{
    return a.x * b.y - a.y * b.x;
}
 
long long cross(const point_t& a, const point_t& b, const point_t& c)
{
    return cross(b - a, c - a);
}
 
int convex_hull(int n)
{
    int s = 0;
    for(int i = 0; i != n; ++i)
    {
        while(s >= 2 && cross(st[s - 1] - st[s - 2], pt[i] - st[s - 1]) < 0)
            --s;
        st[s++] = pt[i];
    }
 
    int s0 = s;
    for(int i = n - 2; i >= 0; --i)
    {
        while(s >= 1 + s0 && cross(st[s - 1] - st[s - 2], pt[i] - st[s - 1]) < 0)
            --s;
        st[s++] = pt[i];
    }
 
    st[s] = st[0];
    return s;
}
 
double get_dist(const point_t& a, const point_t& b)
{
    return std::sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
}
 
int main()
{
    int n;
    std::scanf("%d", &n);
    for(int i = 0; i != n; ++i)
    {
        int x, y;
        std::scanf("%d %d", &x, &y);
        pt[i].x = x, pt[i].y = y;
    }
 
    std::sort(pt, pt + n);
    int tot = convex_hull(n);
    double ans = 0.0;
    for(int i = 0; i != tot; ++i)
        ans += get_dist(st[i], st[i + 1]);
    std::printf("%.2lf", ans);
    return 0;
}
