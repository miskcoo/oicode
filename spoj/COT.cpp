/* SPOJ-COT Count on a tree
 *   主席树+倍增LCA */
#include <cstdio>
#include <algorithm>

const int MaxN = 100010, MaxL = 17, MaxV = ~0u >> 1;
int n, m, total;
int head[MaxN], next[MaxN << 1], point[MaxN << 1], weight[MaxN];
int que[MaxN], fa[MaxN], depth[MaxN], dist[MaxL][MaxN];

template<typename Type>
class memory_pool
{
	Type* data[1 << 16];
	int used, remain;
public:
	memory_pool() : used(0), remain(1 << 16) {}
	~memory_pool() 
	{
		for(int i = 0; i != used; ++i)
			delete[] data[i];
	}

	Type* fetch() 
	{
		if(remain + 1 < 1 << 16) 
			return data[used - 1] + ++remain;
		data[used++] = new Type[1 << 16];
		return data[used - 1] + (remain = 0);
	}
};

struct node_t
{
	int w;
	node_t *l, *r;
};

node_t *nil, *root[MaxN], *tmp[4];
memory_pool<node_t> mem;

void add_edge(int u, int v)
{
	point[++total] = v;
	next[total] = head[u];
	head[u] = total;
}

node_t* modify(node_t* now, unsigned head, unsigned tail, unsigned pos)
{
	node_t* n = mem.fetch();
	*n = *now; ++n->w;
	if(head == tail)
		return n;

	unsigned m = (head + tail) >> 1;
	if(pos <= m) n->l = modify(n->l, head, m, pos);
	else n->r = modify(n->r, m + 1, tail, pos);
	return n;
}

void solve_father()
{
	int qhead = 0, qtail = 0;
	que[qtail++] = 1;
	depth[1] = 1, fa[1] = 0;
	root[0] = nil;
	while(qhead != qtail)
	{
		int u = que[qhead++];
		root[u] = modify(root[fa[u]], 0, MaxV, weight[u]);
		for(int k = head[u]; k; k = next[k])
		{
			int v = point[k];
			if(v == fa[u]) continue;
			fa[v] = u;
			depth[v] = depth[u] + 1;
			que[qtail++] = v;
		}
	}
}

void init_lca()
{
	for(int i = 1; i <= n; ++i)
		dist[0][i] = fa[i];
	for(int l = 1; l != MaxL; ++l)
		for(int i = 1; i <= n; ++i)
			dist[l][i] = dist[l - 1][dist[l - 1][i]];
}

int get_lca(int u, int v)
{
	if(depth[u] > depth[v])
		std::swap(u, v);

	int diff = depth[v] - depth[u];
	for(int i = 0; diff; diff >>= 1, ++i)
		if(diff & 1) v = dist[i][v];

	if(u == v) return u;

	for(int p = MaxL - 1; u != v; p ? --p : 0)
	{
		if(dist[p][u] != dist[p][v] || p == 0)
		{
			v = dist[p][v];
			u = dist[p][u];
		}
	}

	return u;
}

int ask(unsigned head, unsigned tail, int k)
{
	if(head == tail) return head;

	unsigned m = (head + tail) >> 1;
	int num = tmp[0]->l->w + tmp[1]->l->w 
		- tmp[2]->l->w - tmp[3]->l->w;
	if(k <= num)
	{
		for(int i = 0; i != 4; ++i)
			tmp[i] = tmp[i]->l;
		return ask(head, m, k);
	} else {
		for(int i = 0; i != 4; ++i)
			tmp[i] = tmp[i]->r;
		return ask(m + 1, tail, k - num);
	}
}

int main()
{
	std::scanf("%d %d", &n, &m);
	for(int i = 1; i <= n; ++i)
		std::scanf("%d", weight + i);
	for(int i = 1; i != n; ++i)
	{
		int u, v;
		std::scanf("%d %d", &u, &v);
		add_edge(u, v);
		add_edge(v, u);
	}

	nil = mem.fetch();
	nil->l = nil->r = nil, nil->w = 0;
	solve_father();
	init_lca();

	for(int i = 0; i != m; ++i)
	{
		int x, y, k, lca;
		std::scanf("%d %d %d", &x, &y, &k);
		lca = get_lca(x, y);
		tmp[0] = root[x], tmp[1] = root[y];
		tmp[2] = root[lca], tmp[3] = root[fa[lca]];
		std::printf("%d\n", ask(0, MaxV, k));
	}
	return 0;
}
