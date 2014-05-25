/*
 * BZOJ-2178 圆的面积并
 *  辛普森积分
 */
#include <algorithm>
#include <cstdio>
#include <cmath>

int N;
const double epsilon = 1.0e-8;
struct circle
{
	double x, y, r;
	bool operator < (const circle& v) const
	{
		return x + r < v.x + v.r;
	}
} circ[1000];
struct info { 
	int w;
	double y0;
	bool operator < (const info& v) const
	{
		return y0 > v.y0;
	}
} data[2000];

double get_length(double x0)
{
	int count = 0;
	for(int i = N - 1; i >= 0; --i)
	{
		if(circ[i].r + circ[i].x < x0)
			break;
		if(circ[i].x - circ[i].r < x0)
		{
			double delta = std::sqrt(circ[i].r * circ[i].r
				- (x0 - circ[i].x) * (x0 - circ[i].x));
			data[count].w = 1;
			data[count++].y0 = delta + circ[i].y;
			data[count].w = -1;
			data[count++].y0 = -delta + circ[i].y;
		}
	}

	double length = 0.0;
	int weight = 0;
	std::sort(data, data + count);
	for(int i = 0; i != count; ++i)
	{
		if(weight) length += data[i - 1].y0 - data[i].y0;
		weight += data[i].w;
	}
	return length;
}

double calc(double l, double r, double lv,
	double rv, double mv, double Sc)
{
	double m = (l + r) / 2.0;
	double lm = (m + l) / 2.0;
	double rm = (m + r) / 2.0;
	double lmv = get_length(lm);
	double rmv = get_length(rm);
	double Sl = (m - l) * (lv + mv + 4.0 * lmv) / 6.0;
	double Sr = (r - m) * (rv + mv + 4.0 * rmv) / 6.0;

	if(r - l >= 2.3333 || std::fabs(Sl + Sr - Sc) >= epsilon)
		return calc(l, m, lv, mv, lmv, Sl)
			+ calc(m, r, mv, rv, rmv, Sr);
	return Sc;
}

int main()
{
	std::scanf("%d", &N);
	for(int i = 0; i != N; ++i)
		std::scanf("%lf %lf %lf", &circ[i].x, &circ[i].y, &circ[i].r);
	std::sort(circ, circ + N);
	double l = 1.0e100, r = circ[N - 1].x + circ[N - 1].r;
	for(int i = 0; i != N; ++i)
	{
		if(circ[i].x - circ[i].r < l)
			l = circ[i].x - circ[i].r;
	}
	double lv = get_length(l);
	double rv = get_length(r);
	double mv = get_length((l + r) / 2.0);
	std::printf("%.3lf", calc(l, r, lv, rv, mv, 
		(r - l) * (lv + rv + 4.0 * mv) / 6.0));
	return 0;
}
