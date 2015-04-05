/* BZOJ-1069: [SCOI2007]最大土地面积
 *  凸包+旋转卡壳
 *  注意重点 */
#include <cstdio>
#include <cmath>
#include <algorithm>

const double eps = 1.0e-7;
const int MaxN = 4010;

struct point_t
{
	double x, y;
	point_t(double x = 0.0, double y = 0.0) : x(x), y(y) {}
} pt[MaxN], hull[MaxN];

int cmp(double x)
{
	if(x > eps) return 1;
	else if(x < -eps) return -1;
	return 0;
}

point_t operator - (point_t a, point_t b)
{
	return point_t(a.x - b.x, a.y - b.y);
}

double cross(point_t a, point_t b)
{
	return a.x * b.y - a.y * b.x;
}

double cross(point_t a, point_t b, point_t c)
{
	return cross(b - a, c - a);
}

bool sort_cmp(const point_t& a, const point_t& b)
{
	if(cmp(a.x - b.x) == 0)
		return a.y < b.y;
	return a.x < b.x;
}

bool unique_cmp(const point_t& a, const point_t& b)
{
	return cmp(a.x - b.x) == 0 && cmp(a.y - b.y) == 0;
}

int convex_hull(int n)
{
	std::sort(pt, pt + n, sort_cmp);
	n = std::unique(pt, pt + n, unique_cmp) - pt;
	int st = 0;
	for(int i = 0; i != n; ++i)
	{
		while(st >= 2 && cross(hull[st - 2], hull[st - 1], pt[i]) < -eps) --st;
		hull[st++] = pt[i];
	}

	int st2 = st + 1;
	for(int i = n - 2; i >= 0; --i)
	{
		while(st >= st2 && cross(hull[st - 2], hull[st - 1], pt[i]) < -eps) --st;
		hull[st++] = pt[i];
	}

	return st;
}

double get_ans(int n)
{
	double ans = 0.0;
	for(int i = 0; i != n; ++i)
		hull[n + i] = hull[i];
	for(int i = 0; i != n; ++i)
	{
		int p1 = i + 1;
		int m = p1 + 1;
		int p2 = m + 1;
		for(; m < n + i - 1; ++m)
		{
			while(cross(hull[m] - hull[i], hull[p1 + 1] - hull[p1]) < -eps) ++p1;
			while(cross(hull[m] - hull[i], hull[p2 + 1] - hull[p2]) > eps) ++p2;
			double s1 = cross(hull[i], hull[p1], hull[m]);
			double s2 = cross(hull[i], hull[p2], hull[m]);
			double s = std::fabs(s1) + std::fabs(s2);
			if(ans < s) ans = s;
		}
	}

	return ans * 0.5;
}

int main()
{
	int n;
	std::scanf("%d", &n);
	for(int i = 0; i != n; ++i)
		std::scanf("%lf %lf", &pt[i].x, &pt[i].y);
	int st = convex_hull(n) - 1;
	std::printf("%.3lf\n", get_ans(st));
	return 0;
}
