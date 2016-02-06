/* Codeforces 621C. Wet Shark and Flowers
 *   probabilities */
#include <cstdio>

const int MaxN = 100010;
int n, p;
double cnt[MaxN];

int main()
{
	std::scanf("%d %d", &n, &p);
	for(int i = 0; i != n; ++i)
	{
		int l, r;
		std::scanf("%d %d", &l, &r);
		cnt[i] = 1.0 - ((r / p) - (l - 1) / p) / (r - l + 1.0);
	}

	double ans = 0.0;
	for(int i = 0; i != n; ++i)
	{
		int j = i == n - 1 ? 0 : i + 1;
		ans += 1.0 - cnt[i] * cnt[j];
	}

	std::printf("%.6lf", ans * 2000);
	return 0;
}
