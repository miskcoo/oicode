/* BZOJ-3680: 吊打XXX
 *   模拟退火  */
#include <cstdio>
#include <cstdlib>
#include <cmath>

const int MaxN = 10010;
struct point_t
{
	double x, y, w;
} ans_pt, pt[MaxN];
int n;
double ans = 1.0e100;

double dist(point_t a, point_t b)
{
	return std::sqrt((a.x - b.x) * (a.x - b.x)
			+ (a.y - b.y) * (a.y - b.y));
}

double calc(point_t p)
{
	double v = 0;
	for(int i = 0; i != n; ++i)
		v += dist(p, pt[i]) * (double)pt[i].w;
	if(v < ans) ans = v, ans_pt = p;
	return v;
}

double rand_real()
{
	return rand() % 1000 / 1000.0;
}

void simulated_annealing()
{
	double T = 1.0e5, eps = 1.0e-3;
	point_t p, np;
	p.x = p.y = 0;
	for(int i = 0; i != n; ++i)
		p.x += pt[i].x, p.y += pt[i].y;
	p.x /= n, p.y /= n;
	calc(p);
	double now_ans = ans;
	while(T > eps)
	{
		double theta = 2.0 * acos(-1) * rand_real();
		np.x = p.x + T * cos(theta);
		np.y = p.y + T * sin(theta);
		double next_ans = calc(np);
		double dE = now_ans - next_ans;
		if(dE >= 0 || exp(dE / T) >= rand_real())
			p = np, now_ans = next_ans;
		T *= 0.98;
	}

	T = eps;
	for(int i = 0; i != 200; ++i)
	{
		double theta = 2.0 * acos(-1) * rand_real();
		np.x = ans_pt.x + T * cos(theta) * rand_real();
		np.y = ans_pt.y + T * sin(theta) * rand_real();
		calc(np);
	}
}

int main()
{
	std::scanf("%d", &n);
	for(int i = 0; i != n; ++i)
		std::scanf("%lf %lf %lf", &pt[i].x, &pt[i].y, &pt[i].w);
	simulated_annealing();
	std::printf("%.3lf %.3lf\n", ans_pt.x, ans_pt.y);
	return 0;
}
