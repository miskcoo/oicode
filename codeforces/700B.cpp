/* Codeforces 700B. Connecting Universities
 *   dfs and similar, dp, geometry, graphs, trees */
#include <cstdio>
#include <queue>

const int MaxN = 200010;
int n, k, total, root;
int head[MaxN], next[MaxN * 2], point[MaxN * 2];
int mark[MaxN], depth[MaxN], cnt[MaxN];
long long ans = 0;

typedef std::priority_queue<std::pair<int, int> > heap_t;

void add_edge(int u, int v)
{
	point[++total] = v;
	next[total] = head[u];
	head[u] = total;
}

void dfs0(int u, int fa)
{
	for(int k = head[u]; k; k = next[k])
	{
		int v = point[k];
		if(v != fa)
		{
			depth[v] = depth[u] + 1;
			dfs0(v, u);
			cnt[u] += cnt[v];
		}
	}
}

void dfs(int u, int fa, int z)
{
	heap_t heap;
	for(int k = head[u]; k; k = next[k])
	{
		int v = point[k];
		if(v != fa && cnt[v])
			heap.push(std::make_pair(cnt[v], v));
	}

	if(mark[u]) heap.push(std::make_pair(1, u));

	int count = 0;
	while(count < z && heap.size() > 1)
	{
		auto A = heap.top(); heap.pop();
		auto B = heap.top(); heap.pop();
		++count; 
		if(--A.first) heap.push(A); 
		if(--B.first) heap.push(B);
	}

	ans -= 2ll * count * depth[u];

	if(count != z)
	{
		auto A = heap.top(); heap.pop();
		while(A.second == u)
		{
			A = heap.top();
			heap.pop();
		}

		dfs(A.second, u, z - count);
	}
}

int main()
{
	std::scanf("%d %d", &n, &k);
	for(int i = 0; i != k * 2; ++i)
	{
		int u;
		std::scanf("%d", &u);
		cnt[u] = mark[u] = 1;
	}
	
	for(int i = 1; i != n; ++i)
	{
		int u, v;
		std::scanf("%d %d", &u, &v);
		add_edge(u, v);
		add_edge(v, u);
	}

	dfs0(1, 0);

	for(int i = 1; i <= n; ++i)
	{
		if(mark[i])
			ans += depth[i];
	}

	dfs(1, 0, k);
	std::printf("%I64d\n", ans);

	return 0;
}
