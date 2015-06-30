/* Codeforces 553C. Love Triangles
 *   dfs and similar, dsu */
#include <cstdio>

const int mod_v = 1000000007;
const int MaxN = 100001, MaxM = 200001;
int total, odd_circ;
int head[MaxN], point[MaxM], next[MaxM], type[MaxM];
int mark[MaxN], depth[MaxN], gf[MaxN];

void add_edge(int u, int v, int t)
{
	point[++total] = v;
	type[total] = t;
	next[total] = head[u];
	head[u] = total;
}

void dfs(int u)
{
	mark[u] = 1;
	for(int k = head[u]; k; k = next[k])
	{
		int v = point[k];
		if(mark[v])
		{
			if(!type[k] && (depth[u] - depth[v]) % 2 == 0)
				odd_circ = 1;
		} else {
			depth[v] = depth[u] + (1 - type[k]);
			dfs(v);
		}
	}
}

int find(int v)
{
	if(gf[v] == v) return v;
	return gf[v] = find(gf[v]);
}

int pow(long long x, int p)
{
	long long v = 1;
	for(; p; p >>= 1, x = x * x % mod_v)
		if(p & 1) v = x * v % mod_v;
	return v;
}

int main()
{
	int n, m;
	std::scanf("%d %d", &n, &m);
	for(int i = 0; i != m; ++i)
	{
		int u, v, t;
		std::scanf("%d %d %d", &u, &v, &t);
		add_edge(u, v, t);
		add_edge(v, u, t);
	}

	for(int i = 1; i <= n; ++i)
		if(!mark[i]) dfs(i);
	if(odd_circ)
	{
		std::puts("0");
		return 0;
	}

	for(int i = 1; i <= n; ++i) gf[i] = i;

	for(int u = 1; u <= n; ++u)
	{
		for(int k = head[u]; k; k = next[k])
		{
			int v = point[k];
			if(type[k] && u < v)
				gf[find(u)] = find(v);
		}
	}

	for(int u = 1; u <= n; ++u)
	{
		for(int k = head[u]; k; k = next[k])
		{
			int v = point[k];
			if(!type[k] && find(u) == find(v))
			{
				std::puts("0");
				return 0;
			}
		}
	}

	for(int u = 1; u <= n; ++u)
	{
		for(int k = head[u]; k; k = next[k])
		{
			int v = point[k];
			gf[find(u)] = find(v);
		}
	}

	int count = 0;
	for(int i = 1; i <= n; ++i)
		if(find(i) == i) ++count;

	std::printf("%d\n", pow(2, count - 1));
	return 0;
}
