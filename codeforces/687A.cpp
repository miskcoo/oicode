#include <cstdio>
#include <algorithm>

const int MaxN = 100010, MaxM = MaxN << 1;
int total, flag = 0;
int head[MaxN], next[MaxM], point[MaxM];
int part[MaxN];

void add_edge(int u, int v)
{
	point[++total] = v;
	next[total] = head[u];
	head[u] = total;
}

void dfs(int u, int now)
{
	part[u] = now;
	for(int k = head[u]; k && !flag; k = next[k])
	{
		int v = point[k];
		if(part[v] == now)
		{
			flag = 1;
			return;
		}

		if(part[v] == -1)
			dfs(v, now ^ 1);
	}
}

int main()
{
	int n, m;
	std::scanf("%d %d", &n, &m);
	for(int i = 1; i <= n; ++i) part[i] = -1;
	for(int i = 0; i != m; ++i)
	{
		int u, v;
		std::scanf("%d %d", &u, &v);
		add_edge(u, v);
		add_edge(v, u);
	}

	for(int i = 1; i <= n && !flag; ++i)
	{
		if(part[i] == -1)
			dfs(i, 0);
	}

	if(flag) 
	{
		std::puts("-1");
	} else {
		int cnt = std::count(part + 1, part + n + 1, 0);
		std::printf("%d\n", cnt);
		for(int i = 1; i <= n; ++i)
			if(part[i] == 0) std::printf("%d ", i);
		std::printf("\n%d\n", n - cnt);
		for(int i = 1; i <= n; ++i)
			if(part[i] == 1) std::printf("%d ", i);
	}
	return 0;
}
