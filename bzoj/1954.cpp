/* BZOJ-1954: Pku3764 The xor-longest Path
 *   Trie */
#include <cstdio>
#include <algorithm>

const int MaxN = 100010, MaxL = 30;
struct node_t
{
	int size;
	node_t *ch[2];
} node[MaxN * (MaxL + 1)], *nil, *root;
int total, used, dist[MaxN];
int head[MaxN], next[MaxN << 1], point[MaxN << 1], weight[MaxN << 1];

node_t* add_value(node_t *now, int val, int dep = MaxL)
{
	node_t *n = node + used++;
	*n = *now;
	++n->size;
	if(dep < 0) return n;
	int x = (val >> dep) & 1;
	n->ch[x] = add_value(n->ch[x], val, dep - 1);
	return n;
}

int ask(node_t *n, int val)
{
	int ans = 0;
	for(int k = MaxL; k >= 0; --k)
	{
		int x = (val >> k) & 1;
		if(n->ch[!x]->size)
		{
			ans |= 1 << k;
			n = n->ch[!x];
		} else {
			n = n->ch[x];
		}
	}

	return ans;
}

void add_edge(int u, int v, int w)
{
	point[++total] = v;
	weight[total] = w;
	next[total] = head[u];
	head[u] = total;
}

void dfs(int u, int fa)
{
	root = add_value(root, dist[u]);
	for(int k = head[u]; k; k = next[k])
	{
		int v = point[k];
		if(v != fa)
		{
			dist[v] = dist[u] ^ weight[k];
			dfs(v, u);
		}
	}
}

int main()
{
	static node_t nil_base;
	nil = &nil_base;
	nil->ch[0] = nil->ch[1] = nil;
	nil->size = 0;
	root = nil;

	int n;
	std::scanf("%d", &n);
	for(int i = 1; i != n; ++i)
	{
		int u, v, w;
		std::scanf("%d %d %d", &u, &v, &w);
		add_edge(u, v, w);
		add_edge(v, u, w);
	}

	dfs(1, 0);
	int ans = 0;
	for(int i = 1; i <= n; ++i)
		ans = std::max(ans, ask(root, dist[i]));
	std::printf("%d\n", ans);
	return 0;
}
