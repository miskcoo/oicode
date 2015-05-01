/* BZOJ-3511: 土地划分
 *   二元关系网络流 */
#include <cstdio>
#include <algorithm>
#include <cstring>

const int MaxN = 10010, MaxM = 200010;
int total = 1, s, t, inf = ~0u >> 1;
int head[MaxN], point[MaxM], next[MaxM], cap[MaxM];
int cur[MaxN], flag[MaxN], que[MaxN], count[MaxN][2];

void add_edge(int u, int v, int f, int fr = 0)
{
	point[++total] = v;
	next[total] = head[u];
	cap[total] = f;
	head[u] = total;

	point[++total] = u;
	next[total] = head[v];
	cap[total] = fr;
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
	int n, m, ans = 0;
	std::scanf("%d %d", &n, &m);
	for(int i = 2; i != n; ++i)
	{
		int v;
		std::scanf("%d", &v);
		count[i][0] += v << 1;
		ans += v << 1;
	}

	for(int i = 2; i != n; ++i)
	{
		int v;
		std::scanf("%d", &v);
		count[i][1] += v << 1;
		ans += v << 1;
	}

	for(int i = 0; i != m; ++i)
	{
		int x, y, a, b, c;
		std::scanf("%d %d %d %d %d", &x, &y, &a, &b, &c);
		ans += (a + b) << 1;
		count[x][0] += a; count[y][0] += a;
		count[x][1] += b; count[y][1] += b;
		int f = c * 2 + a + b;
		add_edge(x, y, f, f);
	}

	count[1][0] = count[n][1] = inf;
	s = n + 1, t = n + 2;
	for(int i = 1; i <= n; ++i)
	{
		add_edge(s, i, count[i][0]);
		add_edge(i, t, count[i][1]);
	}

	std::printf("%d", (ans - dinic()) >> 1);
	return 0;
}
