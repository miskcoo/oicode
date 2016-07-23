/* Codeforces 700A. As Fast As Possible
 *   binary search, math, ternary search */
#include <cstdio>

int n, l, v1, v2, k;

bool check(long double t)
{
	long double t1 = (l - v2 * t) / (v1 - v2);
	long double t2 = t - t1;
	long double tr = (v2 - v1) * t2 / (v1 + v2);

	int tot = (n + k - 1) / k;
	long double s = tot * t2 + (tot - 1) * tr;
	return s <= t;
}

int main()
{
	std::scanf("%d %d %d %d %d", &n, &l, &v1, &v2, &k);
	long double L = l / double(v2), R = l / double(v1);
	while(R - L > 1.0e-8)
	{
		long double m = (L + R) * 0.5;
		if(check(m)) R = m;
		else L = m;
	}
	std::printf("%.9lf", (double)L);
	return 0;
}
