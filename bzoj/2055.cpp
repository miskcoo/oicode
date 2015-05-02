/* BZOJ-2055: 80人环游世界
 *   上下界费用流 */
#include <cstdio>
#include <cstring>

const int MaxN = 210, MaxM = MaxN * MaxN, inf = ~0u >> 1;
int total = 1, s, t;
int head[MaxN], next[MaxM], point[MaxM], from[MaxM]; 
int cap[MaxM], cost[MaxM];
int pre[MaxM], que[MaxN], mark[MaxN], dist[MaxN];

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
	int qhead = 0, qtail = 0;
	std::memset(dist, 0x33, sizeof(dist));
	dist[s] = 0, que[qtail++] = s;
	while(qhead != qtail)
	{
		int u = que[qhead++];
		if(qhead == MaxN) qhead = 0;
		for(int k = head[u]; k; k = next[k])
		{
			int v = point[k];
			int d = dist[u] + cost[k];
			if(cap[k] && d < dist[v])
			{
				dist[v] = d;
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

	return dist[t] != 0x33333333;
}

int costflow()
{
	int ans = 0;
	while(spfa())
	{
		int flow = inf, cnt = 0;
		for(int u = t; u != s; u = from[pre[u]])
			if(flow > cap[pre[u]]) flow = cap[pre[u]];
		for(int u = t; u != s; u = from[pre[u]])
		{
			int k = pre[u];
			cap[k] -= flow;
			cap[k ^ 1] += flow;
			cnt += cost[k];
		}

		ans += cnt * flow;
	}

	return ans;
}

int main()
{
	int n, m;
	std::scanf("%d %d", &n, &m);
	s = n * 2 + 1, t = s + 1;
	int s0 = s + 2, t0 = t + 2;
	for(int i = 1; i <= n; ++i)
	{
		int v;
		std::scanf("%d", &v);
		add_edge(i, t, v, 0);
		add_edge(s, i + n, v, 0);
		add_edge(s0, i, inf, 0);
		add_edge(i + n, t0, inf, 0);
	}

	add_edge(t0, s0, m, 0);
	for(int i = 1; i <= n; ++i)
	{
		for(int j = i + 1; j <= n; ++j)
		{
			int v;
			std::scanf("%d", &v);
			if(v != -1) add_edge(i + n, j, inf, v);
		}
	}

	std::printf("%d", costflow());
	return 0;
}
