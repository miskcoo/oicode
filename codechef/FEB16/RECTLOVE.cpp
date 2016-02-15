#include <cstdio>

int main()
{
	int t;
	std::scanf("%d", &t);
	while(t --> 0)
	{
		int n, m, k;
		std::scanf("%d %d %d", &n, &m, &k);
		double ans = 0.0;
		while(k --> 0)
		{
			long long id;
			std::scanf("%lld", &id);
			double i = (id - 1) / m + 1;
			double j = id - (i - 1ll) * m;
			ans += 4 * (i / m) * (j / n) 
				* ((n - i + 1) / (m + 1))
				* ((m - j + 1) / (n + 1));
		}

		std::printf("%.9lf\n", ans);
	}
	return 0;
}
