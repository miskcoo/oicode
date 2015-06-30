/* BZOJ-2879: [Noi2012]美食节
 *   动态加边费用流 */
#include <cstdio>
#include <algorithm>
#include <cstring>

const int inf = ~0u >> 1;
const int MaxN = 41, MaxM = 101, MaxV = 1200, MaxE = 100000;
int n, m, S, T, total = 1, node_tot, dishes_tot;
int head[MaxV], from[MaxE], point[MaxE], next[MaxE], cost[MaxE];
int dist[MaxV], que[MaxV], mark[MaxV], cap[MaxE], pre[MaxV];
int time[MaxN][MaxM], cnt[MaxM], dishes[MaxN];

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
	std::memset(dist, 0x33, sizeof(int) * (node_tot + 2));
	int qhead = 0, qtail = 0;
	mark[S] = 1, que[qtail++] = S, dist[S] = 0;
	while(qhead != qtail)
	{
		int u = que[qhead++];
		if(qhead == MaxV) qhead = 0;
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
					if(qtail == MaxV) qtail = 0;
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
		int flow = inf, cost_sum = 0, cook = 0;
		for(int u = T; u != S; u = from[pre[u]])
			flow = std::min(flow, cap[pre[u]]);
		for(int u = T; u != S; u = from[pre[u]])
		{
			cap[pre[u]] -= flow;
			cap[pre[u] ^ 1] += flow;
			cost_sum += cost[pre[u]];
			if(T < u && u <= T + m) cook = u - T;
		}

		ans += flow * cost_sum;

		++cnt[cook];
		add_edge(cook + T, ++node_tot, 1, 0);
		for(int i = 1; i <= n; ++i)
			add_edge(node_tot, T + m + i, 1, (cnt[cook] + 1) * time[i][cook]);
	}

	return ans;
}

int main()
{
	std::scanf("%d %d", &n, &m);
	for(int i = 1; i <= n; ++i)
	{
		std::scanf("%d", dishes + i);
		dishes_tot += dishes[i];
	}

	for(int i = 1; i <= n; ++i)
		for(int j = 1; j <= m; ++j)
			std::scanf("%d", time[i] + j);

	S = ++node_tot; T = ++node_tot;
	for(int i = 1; i <= m; ++i)
		add_edge(S, ++node_tot, inf, 0);
	for(int i = 1; i <= n; ++i)
		add_edge(++node_tot, T, dishes[i], 0);

	for(int i = 1; i <= m; ++i)
	{
		add_edge(T + i, ++node_tot, 1, 0);
		for(int j = 1; j <= n; ++j)
			add_edge(node_tot, T + m + j, 1, time[j][i]);
	}

	std::printf("%d\n", costflow());
	return 0;
}
