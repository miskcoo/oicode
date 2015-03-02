/* BZOJ-1044: [HAOI2008]木棍分割
 *  二分+DP */
#include <cstdio>
#include <algorithm>

const int mod_v = 10007, MaxN = 50001;
int len[MaxN], sum[MaxN];
int dp[2][MaxN];

bool check(int n, int m, int l)
{
	int s = 0, count = 0;
	for(int i = 1; i <= n && count <= m; ++i)
	{
		s += len[i];
		if(s > l)
		{
			s = len[i];
			++count;
		}
	}

	return count <= m;
}

int get_ans(int n, int m)
{
	int l = 0, r = 0;
	for(int i = 1; i <= n; ++i)
	{
		r += len[i];
		l = std::max(l, len[i]);
	}

	while(l < r)
	{
		int d = (l + r) >> 1;
		if(check(n, m, d))
			r = d;
		else l = d + 1;
	}

	return l;
}

int count_num(int n, int m, int d)
{
	int* f = dp[0], *g = dp[1];
	g[0] = 1;

	int ans = 0;
	for(int i = 1; i <= m + 1; ++i)
	{
		int t = 0, k = i - 1;
		for(int j = i; j <= n; ++j)
		{
			t += g[j - 1];
			while(sum[j] - sum[k] > d)
				t -= g[k++];
			f[j] = t % mod_v;
		}

		ans += f[n];
		if(ans >= mod_v) ans -= mod_v;
		std::swap(f, g);
	}

	return ans;
}

int main()
{
	int n, m;
	std::scanf("%d %d", &n, &m);
	for(int i = 1; i <= n; ++i)
	{
		std::scanf("%d", len + i);
		sum[i] = sum[i - 1] + len[i];
	}

	int ans = get_ans(n, m);
	std::printf("%d %d", ans, count_num(n, m, ans));
	return 0;
}
