/* BZOJ-1492: [NOI2007]货币兑换Cash
 *   CDQ分治
 *   合并凸包注意重点 */
#include <cstdio>
#include <algorithm>
 
const double eps = 1.0e-8;
const int MaxN = 100001;
double f[MaxN], A[MaxN], B[MaxN], R[MaxN];
 
bool cmp(double x) { return -eps < x && x < eps; }
struct point_t
{
    int id, size;
    double x, y;
    bool operator < (const point_t& r) const
    {
        if(cmp(r.x - x)) return y < r.y;
        return x < r.x;
    }
 
    point_t operator - (const point_t& r) const
    {
        point_t z;
        z.x = x - r.x;
        z.y = y - r.y;
        return z;
    }
} pt[MaxN], st[MaxN];
 
double cross(const point_t& p, point_t a, point_t b)
{
    a = a - p, b = b - p;
    return a.x * b.y - a.y * b.x;
}
 
double slope(const point_t& a, const point_t& b)
{
    return (a.y - b.y) / (a.x - b.x);
}
 
typedef std::pair<double, int> data_t;
data_t rate[MaxN], tmp[MaxN];
 
void solve(int l, int r)
{
    if(l > r) return;
    if(l == r)
    {
        if(l > 1) f[l] = std::max(f[l - 1], f[l]);
        double v = f[l] * R[l] / (A[l] * R[l] + B[l]);
        pt[l].x = v, pt[l].y = v / R[l];
        pt[l].id = l, pt[l].size = 1;
        return;
    }
 
    int m = (l + r) >> 1;
    solve(l, m);
     
    std::copy(rate + m + 1, rate + r + 1, tmp + m + 1);
    std::sort(tmp + m + 1, tmp + r + 1);
    int pc = l + pt[l].size - 1;
    for(int i = m + 1; i <= r; ++i)
    {
        while(pc != l && slope(pt[pc], pt[pc - 1]) < tmp[i].first) --pc;
        int x = tmp[i].second;
        f[x] = std::max(f[x], pt[pc].x * A[x] + pt[pc].y * B[x]);
    }
 
    solve(m + 1, r);
//  std::merge(rate + l, rate + m + 1, rate + m + 1, rate + r + 1, tmp);
//  std::copy(tmp, tmp + (r - l + 1), rate + l);
 
    int stop = 0, a = l, b = m + 1;
    int sa = l + pt[a].size - 1, sb = m + pt[b].size;
    while(a <= sa || b <= sb)
    {
        point_t now;
        if(b > sb) now = pt[a++];
        else if(a > sa) now = pt[b++];
        else now = pt[a].x < pt[b].x ? pt[a++] : pt[b++];
        while(stop >= 2 && cross(st[stop - 1], now, st[stop - 2]) > -eps) --stop;
        st[stop++] = now;
    }
 
    st[0].size = stop;
    std::copy(st, st + stop, pt + l);
}
 
int main()
{
    int n;
    std::scanf("%d %lf", &n, f + 1);
    for(int i = 1; i <= n; ++i)
    {
        std::scanf("%lf %lf %lf", A + i, B + i, R + i);
        rate[i] = data_t(-A[i] / B[i], i);
    }
 
    solve(1, n);
    std::printf("%.3lf\n", f[n]);
    return 0;
}
