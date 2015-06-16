/* Codeforces 98E. Help Shrek and Donkey
 *   博弈论，纳什均衡，概率
 *   dp, games, math, probabilities */
#include <cstdio>
#include <algorithm>

const double eps = 1.0e-8;
const int MaxN = 1024;
double dp[MaxN][MaxN];

double f(int m, int n)
{
	if(m == 0) return 1.0 / (n + 1.0);
	if(n == 0) return 1.0;
	if(dp[m][n] > -eps) return dp[m][n];
	double p = 1 / (1 + 1 / (f(n, m - 1) * (n + 1)));
	return dp[m][n] = p * n / (n + 1) * (1 - f(n - 1, m)) + (1 - p);
}

int main()
{
	int n, m;
	std::scanf("%d %d", &m, &n);
	for(int i = 0; i <= std::max(n, m); ++i)
		for(int j = 0; j <= std::max(n, m); ++j)
			dp[i][j] = -1;
	std::printf("%.9lf %.9lf\n", f(m, n), 1 - f(m, n));
	return 0;
}
