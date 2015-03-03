/* BZOJ-1937: [Shoi2004]Mst 最小生成树 
 *  费用流，KM算法，最小顶标和 */
#include <cstdio>
#include <algorithm>
#include <cstring>

template<int VSize, int ESize>
class costflow
{
	int total;
	int head[VSize + 1];
	int next[ESize + 1];
	int point[ESize + 1];
	int from[ESize + 1];
	int cap[ESize + 1];
	int cost[ESize + 1];

	int s, t;

	int dist[VSize + 1];
	int mark[VSize + 1];
	int que[VSize + 1];
	int pre[VSize + 1];
	int mark_value;
public:
	void init()
	{
		total = 2;
		mark_value = 0;
		std::memset(mark, 0, sizeof(mark));
		std::memset(head, 0, sizeof(head));
	}

	void add_edge(int u, int v, int flow, int cost)
	{
		point[total] = v;
		from[total] = u;
		next[total] = head[u];
		cap[total] = flow;
		this->cost[total] = cost;
		head[u] = total++;

		point[total] = u;
		from[total] = v;
		next[total] = head[v];
		cap[total] = 0;
		this->cost[total] = -cost;
		head[v] = total++;
	}

	bool spfa()
	{
		++mark_value;
		for(int i = 0; i <= VSize; ++i)
			dist[i] = -10000000;
		int qhead = 0, qtail = 0;
		mark[s] = mark_value;
		que[qtail++] = s;
		dist[s] = 0;

		while(qhead != qtail)
		{
			int u = que[qhead++];
			if(qhead == VSize + 1)
				qhead = 0;
			for(int k = head[u]; k; k = next[k])
			{
				int v = point[k];
				int t = dist[u] + cost[k];
				if(cap[k] && t > dist[v])
				{
					dist[v] = t;
					pre[v] = k;
					if(mark[v] != mark_value)
					{
						mark[v] = mark_value;
						que[qtail++] = v;
						if(qtail == VSize + 1)
							qtail = 0;
					}
				}
			}

			mark[u] = 0;
		}

		return dist[t] > 0;
	}

	int get_cost(int s, int t)
	{
		int answer = 0;
		this->s = s, this->t = t;
		while(spfa())
		{
			int count = 0;
			int min_cap = ~0U >> 1;
			for(int u = t; u != s; u = from[pre[u]])
				min_cap = std::min(min_cap, cap[pre[u]]);
			for(int u = t; u != s; u = from[pre[u]])
			{
				int k = pre[u];
				cap[k] -= min_cap;
				cap[k ^ 1] += min_cap;
				count += cost[k];
			}

			answer += min_cap * count;
		}

		return answer;
	}
};

const int MaxN = 100, MaxE = 1550;
costflow<MaxE + 3, MaxE * MaxE> cf;

struct edge_t
{
	int u, v, w;
	char mark;
} edge[MaxE];

int total, n, m;
int head[MaxN], point[MaxE], next[MaxE], fa[MaxN], depth[MaxN], ind[MaxE], path[MaxN];

void add_edge(int u, int v, int id)
{
	point[++total] = v;
	next[total] = head[u];
	ind[total] = id;
	head[u] = total;
}

void dfs(int u, int f, int d)
{
	fa[u] = f;
	depth[u] = d;
	for(int k = head[u]; k; k = next[k])
	{
		int v = point[k];
		if(v == f) continue;
		path[v] = ind[k];
		dfs(v, u, d + 1);
	}
}

int main()
{
	std::scanf("%d %d", &n, &m);
	for(int i = 0; i != m; ++i)
		std::scanf("%d %d %d", &edge[i].u, &edge[i].v, &edge[i].w);
	for(int i = 1; i != n; ++i)
	{
		int u, v;
		std::scanf("%d %d", &u, &v);
		for(int j = 0; j != m; ++j)
		{
			if(edge[j].u == u && edge[j].v == v || edge[j].u == v && edge[j].v == u)
			{
				edge[j].mark = 1;
				break;
			}
		}
	}

	cf.init();
	int s = MaxE + 1, t = MaxE + 2;
	for(int i = 0; i != m; ++i)
	{
		int u = edge[i].u, v = edge[i].v;
		if(edge[i].mark)
		{
			add_edge(u, v, i);
			add_edge(v, u, i);
			cf.add_edge(s, i + 1, 1, 0);
		} else {
			cf.add_edge(i + 1, t, 1, 0);
		}
	}

	dfs(1, 0, 0);

	for(int i = 0; i != m; ++i)
	{
		if(edge[i].mark) continue;
		int u = edge[i].u, v = edge[i].v;
		if(depth[u] < depth[v])
			std::swap(u, v);
		while(depth[u] != depth[v])
		{
			int e = path[u];
			if(edge[i].w < edge[e].w)
				cf.add_edge(e + 1, i + 1, 1, edge[e].w - edge[i].w);
			u = fa[u];
		}

		while(u != v)
		{
			int e = path[u];
			if(edge[i].w < edge[e].w)
				cf.add_edge(e + 1, i + 1, 1, edge[e].w - edge[i].w);
			u = fa[u];

			e = path[v];
			if(edge[i].w < edge[e].w)
				cf.add_edge(e + 1, i + 1, 1, edge[e].w - edge[i].w);
			v = fa[v];
		}
	}

	std::printf("%d", cf.get_cost(s, t));
	return 0;
}
