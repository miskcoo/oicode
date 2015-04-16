/* BZOJ-3675: [Apio2014]序列分割 
 *  DP，斜率优化 */
#include <cstdio>
#include <algorithm>

const int MaxN = 100010, MaxK = 210;
int now, q[MaxN];
long long f[2][MaxN], y[2][MaxN];
long long sum[MaxN];

double slope(int a, int b)
{
	if(sum[a] == sum[b]) return 1.0e100;
	return double(y[now ^ 1][a] - y[now ^ 1][b]) / double(sum[a] - sum[b]);
}

void solve(int n, int k)
{
	now = 0;
	for(int i = 1; i <= n; ++i)
		y[now][i] = sum[i] * sum[i];

	for(int i = 1; i <= k; ++i)
	{
		now ^= 1;
		int h = 0, t = 0;
		q[t] = 0;
		for(int j = 1; j <= n; ++j)
		{
			while(h < t && slope(q[h], q[h + 1]) <= sum[j]) h++;
			f[now][j] = f[now ^ 1][q[h]] + sum[q[h]] * (sum[j] - sum[q[h]]);
			y[now][j] = sum[j] * sum[j] - f[now][j];
			while(h < t && slope(j, q[t]) <= slope(q[t], q[t - 1])) --t;
			q[++t] = j;
		}
	}

	std::printf("%lld", f[now][n]);
}

int main()
{
	int n, k;
	std::scanf("%d %d", &n, &k);
	for(int i = 1; i <= n; ++i)
	{
		int v;
		std::scanf("%d", &v);
		sum[i] = sum[i - 1] + v;
	}

	solve(n, k);
	return 0;
}
