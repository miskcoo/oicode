/* BZOJ-1070: [SCOI2007]修车
 *   费用流 */
#include <cstdio>
#include <algorithm>
#include <cstring>

const int MaxN = 650, MaxM = 216000;
int S, T, total = 1;
int head[MaxN], from[MaxM], point[MaxM], next[MaxM], cost[MaxM];
int dist[MaxN], que[MaxN], mark[MaxN], cap[MaxM], pre[MaxN];

void add_edge0(int u, int v, int f, int c)
{
	point[++total] = v;
	from[total] = u;
	cost[total] = c;
	cap[total] = f;
	next[total] = head[u];
	head[u] = total;
}

void add_edge(int u, int v, int f, int c)
{
	add_edge0(u, v, f, c);
	add_edge0(v, u, 0, -c);
}

bool spfa()
{
	std::memset(dist, 0x33, sizeof(dist));
	int qhead = 0, qtail = 0;
	mark[S] = 1, que[qtail++] = S, dist[S] = 0;
	while(qhead != qtail)
	{
		int u = que[qhead++];
		if(qhead == MaxN) qhead = 0;
		for(int k = head[u]; k; k = next[k])
		{
			int v = point[k];
			if(cap[k] && dist[u] + cost[k] < dist[v])
			{
				dist[v] = dist[u] + cost[k];
				pre[v] = k;
				if(!mark[v]) 
				{
					mark[v] = 1;
					que[qtail++] = v;
					if(qtail == MaxN) qtail = 0;
				}
			}
		}

		mark[u] = 0;
	}

	return dist[T] != 0x33333333;
}

int costflow()
{
	int ans = 0;
	while(spfa())
	{
		int flow = ~0u >> 1, cnt = 0;
		for(int u = T; u != S; u = from[pre[u]])
			flow = std::min(flow, cap[pre[u]]);
		for(int u = T; u != S; u = from[pre[u]])
		{
			cap[pre[u]] -= flow;
			cap[pre[u] ^ 1] += flow;
			cnt += cost[pre[u]];
		}

		ans += flow * cnt;
	}

	return ans;
}

int time[61][10];
int main()
{
	int n, m;
	std::scanf("%d %d", &m, &n);
	S = n * (m + 1) + 2, T = S + 1;
	for(int i = 1; i <= n; ++i)
		for(int j = 1; j <= m; ++j)
			std::scanf("%d", time[i] + j);
	for(int i = 1; i <= n; ++i)
		add_edge(S, i, 1, 0);
	for(int i = 1; i <= n * m; ++i)
		add_edge(n + i, T, 1, 0);
	for(int i = 1; i <= n; ++i)
		for(int j = 1; j <= m; ++j)
			for(int k = 1; k <= n; ++k)
				add_edge(i, j * n + k, 1, time[i][j] * k);
	std::printf("%.2lf\n", costflow() / double(n));
	return 0;
}
