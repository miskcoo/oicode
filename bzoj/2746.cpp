/* BZOJ-2746: [HEOI2012]旅行问题 
 *  AC自动机+倍增LCA */
#include <cstdio>
#include <cstring>
#include <vector>
#include <string>
#include <algorithm>

const int mod_v = 1000000007;
const int MaxN = 1000010, MaxAlpha = 26, MaxL = 20;
typedef std::vector<int> describe_t;
struct node_t
{
	int hash;
	int fail, ch[MaxAlpha];
} node[MaxN];

int used = 1;
int dist[MaxL][MaxN], depth[MaxN];
char str[MaxN];
describe_t *describe;

void add_string(const char* str, describe_t* desc)
{
	int now = 1;
	for(int i = 0; str[i]; ++i)
	{
		int x = str[i] - 'a';
		if(!node[now].ch[x])
		{
			node[now].ch[x] = ++used;
			node[used].hash = (1ll * node[now].hash * MaxAlpha + x) % mod_v;
		}

		now = node[now].ch[x];
		desc->push_back(now);
	}
}

void make_fail()
{
	int *que = new int[used];
	int qhead = 0, qtail = 0;
	que[qtail++] = 1;
	while(qhead != qtail)
	{
		int now = que[qhead++];
		dist[0][now] = node[now].fail;
		depth[now] = depth[node[now].fail] + 1;
		for(int l = 1; l != MaxL; ++l)
			dist[l][now] = dist[l - 1][dist[l - 1][now]];
		for(int i = 0; i != MaxAlpha; ++i)
		{
			int u = node[now].ch[i];
			if(!u) continue;
			int z = node[now].fail;
			for(; z && !node[z].ch[i]; z = node[z].fail);
			if(!z) node[u].fail = 1;
			else node[u].fail = node[z].ch[i];
			que[qtail++] = u;
		}
	}

	delete[] que;
}

int get_lca(int u, int v)
{
	if(depth[u] < depth[v])
		std::swap(u, v);
	int diff = depth[u] - depth[v];
	for(int i = 0; diff; ++i, diff >>= 1)
		if(diff & 1) u = dist[i][u];
	for(int p = MaxL - 1; u != v; p ? --p : 0)
	{
		if(!p || dist[p][u] != dist[p][v])
		{
			u = dist[p][u];
			v = dist[p][v];
		}
	}

	return u;
}

int main()
{
	int n, m;
	std::scanf("%d", &n);
	describe = new describe_t[n];
	for(int i = 0; i != n; ++i)
	{
		std::scanf("%s", str);
		add_string(str, describe + i);
	}

	make_fail();

	std::scanf("%d", &m);
	for(int i = 0; i != m; ++i)
	{
		int a, b, c, d;
		std::scanf("%d %d %d %d", &a, &b, &c, &d);
		int u = describe[a - 1][b - 1];
		int v = describe[c - 1][d - 1];
		int lca = get_lca(u, v);
		std::printf("%d\n", node[lca].hash);
	}

	delete[] describe;
	return 0;
}
