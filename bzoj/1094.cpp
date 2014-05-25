/*
 * BZOJ-1094 粒子运动
 *  计算几何
 */
#include <cstdio>
#include <cmath>

struct point
{
	double x, y;
	
	point(double x = 0.0, double y = 0.0)
		: x(x), y(y) {}

	point operator - (const point& v) const 
	{
		return point(x - v.x, y - v.y);
	}

	point operator + (const point& v) const
	{
		return point(x + v.x, y + v.y);
	}

	point operator * (double t) const
	{
		return point(x * t, y * t);
	}
};

struct insection
{
	point s, v;
	double time;
	double start;
} crash[101][103];

inline double distance(const point& a, const point& b)
{
	return std::sqrt((a.x - b.x) * (a.x - b.x) 
		+ (a.y - b.y) * (a.y - b.y));
}

inline double cross(const point& a, const point& b)
{
	return a.x * b.y - a.y * b.x;
}

inline double find_min(double time,
	const point& sa, const point& sb, 
	const point& va, const point& vb)
{
	point v = vb - va;
	point s = sb - sa;

	double A = v.x * v.x + v.y * v.y;
	double B = 2 * (s.x * v.x + s.y * v.y);
	double C = s.x * s.x + s.y * s.y;

	double mt = -0.5 * B / A;
	if(mt < 0) return C;
	else if(mt > time) 
		return A * time * time + B * time + C;
	return C - 0.25 * B * B / A;
}

void make_crash(
	insection* crash, 
	point p, point v,
	point origin, 
	double r, int k)
{
	double total_time = 0.0;
	for(int i = 0; i <= k; ++i)
	{
		point u = p - origin;
		double A = v.x * v.x + v.y * v.y;
		double B = 2.0 * (u.x * v.x + u.y * v.y);
		double C = u.x * u.x + u.y * u.y - r * r;

		double t = 0.5 * (std::sqrt(B * B - 4.0 * A * C) - B) / A;
		crash[i].start = total_time;
		crash[i].time = total_time + t;
		total_time += t;
		crash[i].v = v;
		crash[i].s = p;

		point g = p + v * t;
		double sinx = cross(p - g, origin - g)
			/ (distance(g, p) * r);
		double cosx = std::sqrt(1.0 - sinx * sinx);
		double sin2x = 2 * sinx * cosx;
		double cos2x = cosx * cosx - sinx * sinx;
		v = point(-v.x * cos2x + v.y * sin2x, 
				-v.x * sin2x - v.y * cos2x);
		p = g;
	}
}

inline point next_step(const insection& crash, double t)
{
	return crash.s + crash.v * (t - crash.start);
}

double solve(int i, int j, int k)
{
	double ans = 1.0e200;
	int di = 0, dj = 0;
	double time = 0.0;
	while(di <= k && dj <= k)
	{
		double t1 = crash[i][di].time;
		double t2 = crash[j][dj].time;
		if(t1 < t2) {
			double temp = find_min(t1 - time, 
				next_step(crash[i][di], time),
				next_step(crash[j][dj], time), 
				crash[i][di].v, crash[j][dj].v);
			if(temp < ans) ans = temp;
			time = crash[i][di].time;
			++di;
		} else {
			double temp = find_min(t2 - time, 
				next_step(crash[i][di], time),
				next_step(crash[j][dj], time), 
				crash[i][di].v, crash[j][dj].v);
			if(temp < ans) ans = temp;
			time = crash[j][dj].time;
			++dj;
		}
	}
	return ans;
}

int main()
{
	point origin;
	double r;
	std::scanf("%lf %lf %lf", 
		&origin.x, &origin.y, &r);
	int N, K;
	std::scanf("%d %d", &N, &K);
	for(int i = 0; i != N; ++i)
	{
		point s, v;
		std::scanf("%lf %lf %lf %lf",
			&s.x, &s.y, &v.x, &v.y);
		make_crash(crash[i], s, v, origin, r, K + 1);
	}
	double ans = 1.0e200;
	for(int i = 0; i != N; ++i)
	{
		for(int j = 0; j != N; ++j)
		{
			if(i == j) continue;
			double temp = solve(i, j, K);
			if(temp < ans) ans = temp;
		}
	}
	std::printf("%.3lf\n", std::sqrt(ans + 1.0e-8));
	return 0;
}
