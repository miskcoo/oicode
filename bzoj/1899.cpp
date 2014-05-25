#include <cstdio>
#include <cstring>
#include <algorithm>

struct info
{
	int a, b;
	bool operator < (const info& v) const
	{
		return b > v.b;
	}
} data[200];

inline void int_max(int& d, int a, int b)
{
	int m = a < b ? b : a;
	if(d == -1 || d > m) d = m;
}

int dp[2][200 * 202];
int* f = dp[0];
int* g = dp[1];

int main()
{
	int N;
	std::scanf("%d", &N);
	for(int i = 0; i != N; ++i)
	{
		std::scanf("%d %d", &data[i].a, &data[i].b);
		f[i] = g[i] = -1;
	}
	f[0] = g[0] = 0;
	std::sort(data, data + N);
	int total = 0;
	for(int i = 0; i != N; ++i)
	{
		int tcost = data[i].a + data[i].b;
		for(int j = 0; j <= total + 200; ++j)
			f[j] = -1;
		for(int j = 0; j <= total; ++j)
		{
			if(g[j] == -1) continue;
			int_max(f[j + data[i].a], g[j], j + tcost);
			int_max(f[j], total - j + tcost, g[j]);
		}
		total += data[i].a;
		std::swap(f, g);
	}
	int ans = 0x7fffffff;
	for(int i = 0; i <= total; ++i)
	{
		if(ans > g[i] && g[i] != -1)
			ans = g[i];
	}
	std::printf("%d", ans);
	return 0;
}
