/* BZOJ-3996: [TJOI2015]线性代数
 *   网络流-最小割 */
#include <cstdio>
#include <cstring>
#include <algorithm>

const int MaxV = 500 * 500 + 600, MaxE = 500 * 500 * 9;
int total = 1, s, t;
int head[MaxV], point[MaxE], next[MaxE];
int cur[MaxE], flag[MaxV], cap[MaxE], que[MaxE];

int bfs()
{
	int qhead = 0, qtail = 0;
	std::memset(flag, 0, sizeof(flag));
	flag[t] = 1, que[qtail++] = t;
	while(qhead != qtail)
	{
		int u = que[qhead++];
		for(int k = head[u]; k; k = next[k])
		{
			int v = point[k];
			if(!flag[v] && cap[k ^ 1])
			{
				flag[v] = flag[u] + 1;
				que[qtail++] = v;
			}
		}
	}

	return flag[s];
}

int dfs(int u, int rest)
{
	if(u == t) return rest;
	int used = 0;
	for(int& k = cur[u]; k; k = next[k])
	{
		int v = point[k];
		if(!cap[k] || flag[u] != flag[v] + 1) 
			continue;
		int g = dfs(v, rest - used < cap[k] ? rest - used : cap[k]);
		cap[k] -= g;
		cap[k ^ 1] += g;
		used += g;
		if(used == rest) return used;
	}

	return used;
}

int dinic()
{
	int ans = 0;
	while(bfs())
	{
		std::memcpy(cur, head, sizeof(head));
		ans += dfs(s, ~0u >> 1);
	}

	return ans;
}

void add_edge(int u, int v, int w)
{
	point[++total] = v;
	cap[total] = w;
	next[total] = head[u];
	head[u] = total;

	point[++total] = u;
	cap[total] = 0;
	next[total] = head[v];
	head[v] = total;
}

int main()
{
	int n, sum = 0;
	std::scanf("%d", &n);
	s = n * (n + 1) + 3, t = s + 1;
	for(int i = 1; i <= n; ++i)
	{
		for(int j = 1; j <= n; ++j)
		{
			int v;
			std::scanf("%d", &v);
			sum += v;
			add_edge(s, i * n + j, v);
			add_edge(i * n + j, i, ~0u >> 1);
			add_edge(i * n + j, j, ~0u >> 1);
		}
	}

	for(int i = 1; i <= n; ++i)
	{
		int v;
		std::scanf("%d", &v);
		add_edge(i, t, v);
	}

	std::printf("%d", sum -= dinic());
	return 0;
}
