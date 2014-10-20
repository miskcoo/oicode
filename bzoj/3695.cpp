/* BZOJ-3695: 滑行
 *  二分、光的最速原理 */
#include <cstdio>
#include <cmath>

int n, x;
int velocity[100], height[100];

double solve(double sin)
{
	double dist = 0.0;
	for(int i = 0; i != n; ++i)
	{
		double tan = sin / std::sqrt(1.0 - sin * sin);
		dist += tan * height[i];
		if(i + 1 != n)
			sin = sin / velocity[i] * velocity[i + 1];
		if(sin > 1.0) return x + 1.0;
	}

	return dist;
}

double calc_time(double sin)
{
	double time = 0.0;
	for(int i = 0; i != n; ++i)
	{
		double cos = std::sqrt(1.0 - sin * sin);
		time += height[i] / (cos * velocity[i]);
		if(i + 1 != n)
			sin = sin / velocity[i] * velocity[i + 1];
	}

	return time;
}

int main()
{
	std::scanf("%d %d", &n, &x);
	for(int i = 0; i != n; ++i)
		std::scanf("%d", height + i);
	for(int i = 0; i != n; ++i)
		std::scanf("%d", velocity + i);
	double l = 0.0, r = 1.0;
	for(int i = 0; i != 100; ++i)
	{
		double m = (l + r) * 0.5;
		double dist = solve(m);
		if(dist > x) r = m;
		else l = m;
	}

	std::printf("%.3lf", calc_time(l));
	return 0;
}
