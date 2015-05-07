/* BZOJ-1001: [BeiJing2006]狼抓兔子
 *   对偶图+Dijkstra */
#include <cstdio>
#include <queue>
#include <cstring>

const int MaxN = 1010, MaxM = 6 * MaxN * MaxN + 4 * MaxN;
int w1[MaxN][MaxN], w2[MaxN][MaxN], w3[MaxN][MaxN];
int total, n, m;
int head[MaxM], point[MaxM], next[MaxM], weight[MaxM];
int dist[MaxM], mark[MaxM];
struct dijkstra_t
{
	int u;
	int dist;
	dijkstra_t(int a, int b) : u(a), dist(b) {}
	bool operator < (const dijkstra_t& r) const
	{
		return dist > r.dist;
	}
};

void add_edge0(int u, int v, int w)
{
	point[++total] = v;
	weight[total] = w;
	next[total] = head[u];
	head[u] = total;
}

void add_edge(int u, int v, int w)
{
	add_edge0(u, v, w);
	add_edge0(v, u, w);
}

int map(int i, int j, int k)
{
	return (i * m + j) * 2 + k + 1;
}

int dijkstra(int s, int t)
{
	std::priority_queue<dijkstra_t> heap;
	for(int i = 0; i <= t; ++i)
		dist[i] = ~0u >> 1;
	dist[s] = 0;
	heap.push(dijkstra_t(s, 0));
	while(!heap.empty())
	{
		int u = heap.top().u;
		heap.pop(); 
		if(mark[u]) continue;
		mark[u] = 1;
		if(u == t) return dist[t];
		for(int k = head[u]; k; k = next[k])
		{
			int v = point[k];
			int d = dist[u] + weight[k];
			if(d < dist[v])
			{
				dist[v] = d;
				heap.push(dijkstra_t(v, d));
			}
		}
	}

	return dist[t];
}

void special_solve(int n, int m)
{
	int t = n == 1 ? m : n;
	int ans = ~0u >> 1;
	for(int i = 1; i != t; ++i)
	{
		int v;
		std::scanf("%d", &v);
		if(v < ans) ans = v;
	}
	std::printf("%d\n", (ans == (~0u >> 1)) ? 0 : ans);
}

int main()
{
	std::scanf("%d %d", &n, &m);
	if(n == 1 || m == 1)
	{
		special_solve(n, m);
		return 0;
	}

	--n, --m;

	for(int i = 0; i <= n; ++i)
		for(int j = 0; j != m; ++j)
			std::scanf("%d", w1[i] + j);

	for(int i = 0; i != n; ++i)
		for(int j = 0; j <= m; ++j)
			std::scanf("%d", w2[i] + j);

	for(int i = 0; i != n; ++i)
		for(int j = 0; j != m; ++j)
			std::scanf("%d", w3[i] + j);

	int s = (n + 1) * m * 2 + 3, t = s + 1;
	for(int i = 0; i != n; ++i)
	{
		add_edge(s, map(i, 0, 0), w2[i][0]);
		add_edge(map(i, m - 1, 1), t, w2[i][m]);
	}

	for(int i = 0; i != m; ++i)
	{
		add_edge(s, map(n - 1, i, 0), w1[n][i]);
		add_edge(map(0, i, 1), t, w1[0][i]);
	}

	for(int i = 0; i != n; ++i)
		for(int j = 0; j != m; ++j)
			add_edge(map(i, j, 0), map(i, j, 1), w3[i][j]);

	for(int i = 0; i != n - 1; ++i)
		for(int j = 0; j != m; ++j)
			add_edge(map(i + 1, j, 1), map(i, j, 0), w1[i + 1][j]);

	for(int i = 0; i != n; ++i)
		for(int j = 0; j != m - 1; ++j)
			add_edge(map(i, j, 1), map(i, j + 1, 0), w2[i][j + 1]);

	std::printf("%d\n", dijkstra(s, t));
	return 0;
}
