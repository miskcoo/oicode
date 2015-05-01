/* BZOJ-2502: 清理雪道
 *   上下界最小流  */
#include <cstdio>
#include <algorithm>
#include <cstring>

const int MaxN = 120, MaxM = MaxN * MaxN * 10;
int total = 1, s, t, inf = ~0u >> 1;
int head[MaxN], point[MaxM], next[MaxM], cap[MaxM];
int cur[MaxN], flag[MaxN], que[MaxN], count[MaxN];

void add_edge(int u, int v, int f)
{
	point[++total] = v;
	next[total] = head[u];
	cap[total] = f;
	head[u] = total;

	point[++total] = u;
	next[total] = head[v];
	cap[total] = 0;
	head[v] = total;
}

bool bfs()
{
	int qhead = 0, qtail = 0;
	std::memset(flag, 0, sizeof(flag));
	que[qtail++] = t; flag[t] = 1;
	while(qhead != qtail)
	{
		int u = que[qhead++];
		for(int k = head[u]; k; k = next[k])
		{
			int v = point[k];
			if(cap[k ^ 1] && !flag[v])
			{
				flag[v] = flag[u] + 1;
				que[qtail++] = v;
				if(v == s) return true;
			}
		}
	}

	return flag[s];
}

int dfs(int u, int rest)
{
	if(u == t) return rest;
	int used = 0;
	for(int &k = cur[u]; k; k = next[k])
	{
		int v = point[k];
		if(cap[k] && flag[v] + 1 == flag[u])
		{
			int g = std::min(rest - used, cap[k]);
			g = dfs(v, g);
			cap[k] -= g;
			cap[k ^ 1] += g;
			used += g;
			if(used == rest) return used;
		}
	}

	return used;
}

int dinic()
{
	int ans = 0;
	while(bfs())
	{
		std::memcpy(cur, head, sizeof(cur));
		ans += dfs(s, inf);
	}

	return ans;
}

int main()
{
	int n;
	std::scanf("%d", &n);
	s = n + 1, t = n + 2;
	int s0 = n + 3, t0 = n + 4;
	for(int i = 1; i <= n; ++i)
	{
		int m;
		std::scanf("%d", &m);
		add_edge(s0, i, inf);
		add_edge(i, t0, inf);
		count[i] -= m;
		while(m --> 0)
		{
			int v;
			std::scanf("%d", &v);
			++count[v];
			add_edge(i, v, inf);
		}
	}

	for(int i = 1; i <= n; ++i)
	{
		if(count[i] > 0) add_edge(s, i, count[i]);
		else if(count[i] < 0) add_edge(i, t, -count[i]);
	}

	dinic();
	add_edge(t0, s0, inf);
	dinic();
	std::printf("%d", cap[total]);
	return 0;
}
