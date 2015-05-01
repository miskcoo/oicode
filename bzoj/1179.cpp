/* BZOJ-1179: [Apio2009]Atm
 *   tarjan+spfa */
#include <cstdio>
#include <cstring>

const int MaxN = 500010, MaxM = MaxN << 1;
int dfn_index, scc_index, stop, tmp;
int dfn[MaxN], low[MaxN], scc[MaxN], st[MaxN];
int value[MaxN], weight[MaxN], mark[MaxN], vis[MaxN];

struct graph_t
{
	int total;
	int head[MaxN], point[MaxM], next[MaxM];

	void add_edge(int u, int v)
	{
		point[++total] = v;
		next[total] = head[u];
		head[u] = total;
	}
} gp, g;

void tarjan(int u)
{
	dfn[u] = low[u] = ++dfn_index;
	mark[u] = 1; st[stop++] = u;
	for(int k = gp.head[u]; k; k = gp.next[k])
	{
		if(!dfn[gp.point[k]])
		{
			tarjan(gp.point[k]);
			if(low[u] > low[gp.point[k]])
				low[u] = low[gp.point[k]];
		} else if(mark[gp.point[k]] && dfn[gp.point[k]] < low[u]) {
			low[u] = dfn[gp.point[k]];
		}
	}

	if(low[u] == dfn[u])
	{
		++scc_index, tmp = 0;
		while(tmp != u)
		{
			tmp = st[--stop];
			scc[tmp] = scc_index;
			mark[tmp] = 0;
			value[scc_index] += weight[tmp];
		}
	}
}

int spfa(int s)
{
	int ans = 0;
	int qhead = 0, qtail = 0;
	st[qtail++] = s; vis[s] = 1;
	std::memset(weight, 0, sizeof(weight));
	weight[s] = value[s];
	while(qhead != qtail)
	{
		int u = st[qhead++];
		if(qhead == MaxN) qhead = 0;
		if(mark[u] && weight[u] > ans) 
			ans = weight[u];
		for(int k = g.head[u]; k; k = g.next[k])
		{
			int v = g.point[k];
			int d = weight[u] + value[v];
			if(d > weight[v]) 
			{
				weight[v] = d;
				if(!vis[v])
				{
					vis[v] = 1;
					st[qtail++] = v;
					if(qtail == MaxN) qtail = 0;
				}
			}
		}

		vis[u] = 0;
	}

	return ans;
}

int main()
{
	int n, m;
	std::scanf("%d %d", &n, &m);
	for(int i = 0; i != m; ++i)
	{
		int u, v;
		std::scanf("%d %d", &u, &v);
		gp.add_edge(u, v);
	}

	for(int i = 1; i <= n; ++i)
		std::scanf("%d", weight + i);

	for(int i = 1; i <= n; ++i)
		if(!dfn[i]) tarjan(i);
	
	int s, p;
	std::scanf("%d %d", &s, &p);
	for(int i = 0; i != p; ++i)
	{
		int u;
		std::scanf("%d", &u);
		mark[scc[u]] = 1;
	}

	for(int i = 1; i <= n; ++i)
	{
		for(int k = gp.head[i]; k; k = gp.next[k])
		{
			int v = gp.point[k];
			if(scc[i] != scc[v])
				g.add_edge(scc[i], scc[v]);
		}
	}

	int ans = spfa(scc[s]);
	std::printf("%d", ans);
	return 0;
}
