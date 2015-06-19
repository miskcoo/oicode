/* BZOJ-2876: [Noi2012]骑行川藏
 *   拉格朗日乘数+二分 */
#include <cstdio>

const int MaxN = 10010;
int n;
double Eu, E, s[MaxN], k[MaxN], v[MaxN];

double calc_v(int i, double lambda)
{
	double c = 0.5 / (lambda * k[i]);
	double l = 0 < v[i] ? v[i] : 0, r = 1.0e9;
	while(r - l > 1.0e-10)
	{
		double x = 0.5 * (l + r);
		if(x * x * (x - v[i]) < c) l = x;
		else r = x;
	}

	return (l + r) * 0.5;
}

double calc(double lambda)
{
	E = 0;
	double time = 0;
	for(int i = 0; i != n; ++i)
	{
		double v0 = calc_v(i, lambda);
		E += k[i] * (v0 - v[i]) * (v0 - v[i]) * s[i];
		time += s[i] / v0;
	}

	return time;
}

int main()
{
	std::scanf("%d %lf", &n, &Eu);
	for(int i = 0; i != n; ++i)
		std::scanf("%lf %lf %lf", s + i, k + i, v + i);

	double ans = 1.0e100;
	double l = -1.0e10, r = 1.0e10;
	while(r - l > 1.0e-12)
	{
		double lambda = 0.5 * (l + r);
		double now = calc(lambda);
		if(E < Eu) 
		{
			if(ans > now) ans = now;
			r = lambda;
		} else {
			l = lambda;
		}
	}

	std::printf("%.6lf\n", ans);

	return 0;
}
