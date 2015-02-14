/* BZOJ-1043: [HAOI2008]下落的圆盘
 *  计算几何，圆求交点 */
#include <cstdio>
#include <algorithm>
#include <cmath>

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
		return a.x * b.x + a.y * b.y;
	}

	friend point_t operator * (double v, const point_t& a)
	{
		return point_t(v * a.x, v * a.y);
	}
};


double cross(const point_t& a, const point_t& b)
{
	return a.x * b.y - a.y * b.x;
}

const int MaxN = 1010;
const point_t origin;
const double pi = 3.141592653589793238463;
struct mark_t
{
	double x;
	int count;

	friend bool operator < (const mark_t& a, const mark_t& b)
	{
		return a.x < b.x;
	}
} mark[MaxN * 2];
point_t point[MaxN];
double radius[MaxN];

point_t rotate(const point_t& p, double rad, const point_t& v = origin)
{
	double sin = std::sin(rad), cos = std::cos(rad);
	point_t pp = p - v;
	return v + point_t(pp.x * cos - pp.y * sin, pp.x * sin + pp.y * cos);
}

double dist(const point_t& p)
{
	return std::sqrt(p.x * p.x + p.y * p.y);
}

double dist(const point_t& a, const point_t& b)
{
	return dist(a - b);
}

double sqr(double v)
{
	return v * v;
}

double solve(int n, int k)
{
	int used = 0;
	for(int i = k + 1; i != n; ++i)
	{
		double d = dist(point[i], point[k]);
		if(d >= radius[i] + radius[k])
			continue;
		if(d <= std::fabs(radius[i] - radius[k]))
		{
			if(radius[i] > radius[k])
				return 0.0;
			else continue;
		}

		double cos = d * d + sqr(radius[k]) - sqr(radius[i]);
		cos /= 2.0 * d * radius[k];
		double sin = std::sqrt(1.0 - cos * cos);

		point_t p1 = point[k] + radius[k] * point_t(cos, -sin);
		point_t p2 = point[k] + radius[k] * point_t(cos, sin);

		point_t ki = point[i] - point[k];
		double rad = std::atan2(ki.y, ki.x);
		p1 = rotate(p1, rad, point[k]);
		p2 = rotate(p2, rad, point[k]);

		double theta1 = std::atan2(p1.y - point[k].y, p1.x - point[k].x);
		double theta2 = std::atan2(p2.y - point[k].y, p2.x - point[k].x);

		if(theta1 < 0) theta1 += 2 * pi;
		if(theta2 < 0) theta2 += 2 * pi;
		if(theta2 < theta1) {
			mark[used].count = 1;
			mark[used++].x = 0;
			mark[used].count = -1;
			mark[used++].x = theta2;

			mark[used].count = 1;
			mark[used++].x = theta1;
			mark[used].count = -1;
			mark[used++].x = 2 * pi;
		} else {

			mark[used].count = 1;
			mark[used++].x = theta1;
			mark[used].count = -1;
			mark[used++].x = theta2;
		}
	}

	std::sort(mark, mark + used);
	int count = 0;
	double prev = 0.0, sum = 0.0;
	for(int i = 0; i != used; ++i)
	{
		count += mark[i].count;
		if(count == 0)
			sum += mark[i].x - prev;
		else if(count == 1 && mark[i].count == 1)
			prev = mark[i].x;
	}

	return (2.0 * pi - sum) * radius[k];
}

int main()
{
	int n;
	std::scanf("%d", &n);
	for(int i = 0; i != n; ++i)
		std::scanf("%lf %lf %lf", radius + i, &point[i].x, &point[i].y);

	double ans = 0.0;
	for(int i = 0; i != n; ++i)
		ans += solve(n, i);

	std::printf("%.3lf\n", ans);
	return 0;
}
