/* BZOJ-3931: [CQOI2015]网络吞吐量
 *  Dijkstra+网络流 */
#include <cstdio>
#include <algorithm>
#include <cstring>
#include <queue>

const int MaxN = 1010, MaxM = 2000010;

class network_t
{
	int total;
	int head[MaxN << 1], point[MaxM], next[MaxM];
	int cur[MaxM], cap[MaxM], flag[MaxN], que[MaxN];
public:
	network_t() : total(1) {}
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

	int bfs(int s, int t)
	{
		std::memset(flag, 0, sizeof(flag));
		int qhead = 0, qtail = 0;
		flag[t] = 1;
		que[qtail++] = t;
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
				}
			}
		}

		return flag[s];
	}

	int dfs(int t, int u, int rest)
	{
		if(u == t) return rest;
		int used = 0;
		for(int& k = cur[u]; k; k = next[k])
		{
			int v = point[k];
			if(cap[k] && flag[v] + 1 == flag[u])
			{
				int g = dfs(t, v, std::min(cap[k], rest - used));
				cap[k] -= g;
				cap[k ^ 1] += g;
				used += g;
				if(used == rest) return used;
			}
		}

		return used;
	}

	long long dinic(int s, int t)
	{
		long long ans = 0;
		while(bfs(s, t))
		{
			std::memcpy(cur, head, sizeof(head));
			ans += dfs(t, s, ~0u >> 1);
		}
		return ans;
	}
} net;

class graph_t
{
	struct dijkstra_t
	{
		int id;
		long long dist;
		dijkstra_t(int id, long long d) : id(id), dist(d) {}
		bool operator < (const dijkstra_t& v) const
		{
			return dist > v.dist;
		}
	};

	typedef std::priority_queue<dijkstra_t> heap_t;

	heap_t heap;
	int total, mark[MaxN];
	int head[MaxN], point[MaxM], next[MaxM];
	long long dist[MaxN], weight[MaxM];
public:
	void add_edge(int u, int v, int w)
	{
		point[++total] = v;
		weight[total] = w;
		next[total] = head[u];
		head[u] = total;
	}

	void dijkstra(int n, int s)
	{
		for(int i = 0; i <= n; ++i)
			dist[i] = ~0llu >> 1;
		dist[s] = 0;
		heap.push(dijkstra_t(s, 0));
		while(!heap.empty())
		{
			int u = heap.top().id;
			heap.pop();
			if(mark[u]) continue;
			mark[u] = 1;
			for(int k = head[u]; k; k = next[k])
			{
				int v = point[k];
				long long d = dist[u] + weight[k];
				if(d < dist[v])
				{
					dist[v] = d;
					heap.push(dijkstra_t(v, d));
				}
			}
		}
	}

	void build_network(int n, network_t *net, int *cap)
	{
		for(int u = 1; u <= n; ++u)
		{
			net->add_edge(u * 2, u * 2 + 1, cap[u]);
			for(int k = head[u]; k; k = next[k])
			{
				int v = point[k];
				if(dist[u] + weight[k] == dist[v])
					net->add_edge(u * 2 + 1, v * 2, ~0u >> 1);
			}
		}
	}
} gp;

int cap[MaxN];

int main()
{
	int n, m;
	std::scanf("%d %d", &n, &m);
	for(int i = 0; i != m; ++i)
	{
		int u, v, w;
		std::scanf("%d %d %d", &u, &v, &w);
		gp.add_edge(u, v, w);
		gp.add_edge(v, u, w);
	}

	for(int i = 1; i <= n; ++i)
		std::scanf("%d", cap + i);
	cap[1] = cap[n] = ~0u >> 1;
	gp.dijkstra(n, 1);
	gp.build_network(n, &net, cap);
	std::printf("%lld", net.dinic(3, n * 2));
	return 0;
}
