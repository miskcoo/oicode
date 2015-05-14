/* BZOJ-4027: [HEOI2015]兔子与樱花
 *   树形DP+贪心  */
#include <cstdio>
#include <algorithm>

const int MaxN = 2000010;
int total, n, m, ans;
int head[MaxN], point[MaxN], size[MaxN];
long long f[MaxN];

bool cmp(int a, int b)
{
	return f[a] < f[b];
}

void dfs(int u)
{
	for(int i = 0; i != size[u]; ++i)
		dfs(point[head[u] + i]);
	std::sort(point + head[u], point + head[u] + size[u], cmp);
	f[u] += size[u];
	for(int i = 0; i != size[u]; ++i)
	{
		long long v = f[point[head[u] + i]] - 1;
		if(f[u] + v <= m)
			f[u] += v, ++ans;
		else break;
	}
}

int main()
{
	std::scanf("%d %d", &n, &m);
	for(int i = 0; i != n; ++i)
		std::scanf("%lld", f + i);
	for(int i = 0, cnt = 0; i != n; ++i)
	{
		std::scanf("%d", size + i);
		head[i] = cnt;
		for(int j = 0; j != size[i]; ++j)
			std::scanf("%d", point + cnt++);
	}

	dfs(0);
	std::printf("%d", ans);
	return 0;
}
