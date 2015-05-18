/* UOJ-#19. 【NOIP2014】寻找道路 */
#include <cstdio>
#include <cstring>

const int MaxN = 20010, MaxE = 400010;
struct edge_t
{
	int u, v;
} edge[MaxE];

int total, mark_v;
int head[MaxN], next[MaxE], point[MaxE];
int mark[MaxN], reached[MaxN], avail[MaxN];
int que[MaxN], dist[MaxN];

void add_edge(int u, int v)
{
	point[++total] = v;
	next[total] = head[u];
	head[u] = total;
}

void dfs(int u)
{
	mark[u] = mark_v;
	reached[u] = 1;
	for(int k = head[u]; k; k = next[k])
	{
		int v = point[k];
		if(mark[v] == mark_v)
			continue;
		dfs(v);
	}
}

int bfs(int s, int t)
{
	int qhead = 0, qtail = 0;
	dist[t] = -1;
	if(avail[s])
	{
		que[qtail++] = s;
		mark[s] = mark_v;
		dist[s] = 0;
	}
	
	while(qhead != qtail)
	{
		int u = que[qhead++];
		for(int k = head[u]; k; k = next[k])
		{
			int v = point[k];
			if(mark[v] == mark_v || !avail[v])
				continue;
			dist[v] = dist[u] + 1;
			que[qtail++] = v;
			mark[v] = mark_v;
		}
	}
	
	return dist[t];
}

int main()
{
	int n, m;
	std::scanf("%d %d", &n, &m);
	for(int i = 0; i != m; ++i)
	{
		int u, v;
		std::scanf("%d %d", &u, &v);
		edge[i].u = u, edge[i].v = v;
		add_edge(v, u);
	}
	
	int s, t;
	std::scanf("%d %d", &s, &t);
	++mark_v;
	dfs(t);
	
	total = 0;
	std::memset(head, 0, sizeof(head));
	for(int i = 0; i != m; ++i)
		add_edge(edge[i].u, edge[i].v);
	++mark_v;
	
	for(int u = 1; u <= n; ++u)
	{
		avail[u] = 1;
		for(int k = head[u]; k; k = next[k])
		{
			if(reached[point[k]] == 0)
			{
				avail[u] = 0;
				break;
			}
		}
	}
	
	int ans = bfs(s, t);
	std::printf("%d", ans);
	
	return 0;
}

