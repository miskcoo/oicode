/* BZOJ-3435: [Wc2014]紫荆花之恋
 *  替罪羊树套Treap+动态点分治 */
#include <cstdio>
#include <vector>
#include <cstdlib>
#include <algorithm>
#include <stack>

const int MaxL = 17, MaxN = 100010, MaxS = MaxN * 60;
const int inf = ~0u >> 1;
double alpha = 0.9;

struct graph_t
{
	int total;
	int head[MaxN], point[MaxN << 1];
	int next[MaxN << 1], weight[MaxN << 1];
	int fa[MaxL][MaxN], dist[MaxN], depth[MaxN];

	void add_edge(int u, int v, int w)
	{
		weight[++total] = w;
		point[total] = v;
		next[total] = head[u];
		head[u] = total;
	}

	void add_child(int u, int f, int w)
	{
		add_edge(u, f, w);
		add_edge(f, u, w);
		fa[0][u] = f;
		dist[u] = dist[f] + w;
		depth[u] = depth[f] + 1;

		for(int i = 1; i != MaxL; ++i)
			fa[i][u] = fa[i - 1][fa[i - 1][u]];
	}

	int get_lca(int u, int v)
	{
		if(depth[u] < depth[v]) 
			std::swap(u, v);
		int diff = depth[u] - depth[v];
		for(int i = 0; diff; ++i, diff >>= 1)
			if(diff & 1) u = fa[i][u];

		for(int p = MaxL - 1; u != v; p ? --p : 0)
		{
			if(!p || fa[p][u] != fa[p][v])
			{
				u = fa[p][u];
				v = fa[p][v];
			}
		}

		return v;
	}

	int get_dist(int u, int v)
	{
		int lca = get_lca(u, v);
		return dist[u] + dist[v] - (dist[lca] << 1);
	}
} gp;

struct treap_t
{
	int val, size;
	treap_t *l, *r;
} treap_node[MaxS];
int treap_used;
treap_t *treap_nil;
std::stack<treap_t*> treap_mem;

treap_t *treap_newnode()
{
	if(!treap_mem.empty())
	{
		treap_t *n = treap_mem.top();
		treap_mem.pop();
		if(n->l != treap_nil)
 			treap_mem.push(n->l);
		if(n->r != treap_nil)
			treap_mem.push(n->r);
		return n;
	} else return treap_node + treap_used++;
}

void treap_insert(treap_t*& n, int v)
{
	if(n == treap_nil)
	{
		n = treap_newnode();
		*n = *treap_nil;
		n->size = 1; n->val = v;
	} else {
		++n->size;
		if(v < n->val)
		{
			treap_insert(n->l, v);
			if(n->l->size > n->r->size + 3)
			{
				// left rotate
				treap_t *q = n->l;
				n->l = q->r; q->r = n;
				q->size = n->size;
				n->size = n->l->size + n->r->size + 1;
				n = q;
			}
		} else {
			treap_insert(n->r, v);
			if(n->r->size > n->l->size + 3)
			{
				// right rotate
				treap_t *q = n->r;
				n->r = q->l; q->l = n;
				q->size = n->size;
				n->size = n->l->size + n->r->size + 1;
				n = q;
			}
		}
	}
}

int treap_find(treap_t* n, int v)
{
	int ans = 0;
	while(n != treap_nil)
	{
		if(v < n->val)
		{
			n = n->l;
		} else {
			ans += n->l->size + 1;
			n = n->r;
		}
	}
	return ans;
}

struct scape_t
{
	int size, top;
	scape_t *fa;
	treap_t *info, *parent_info;
} scape_node[MaxN];
scape_t *scape_nil;
int node_num, R[MaxN], depth[MaxN];
int mark_v, mark[MaxN], que[MaxN], fa[MaxN];
int size[MaxN], max_size[MaxN], dist[MaxN];
int record_num, record[MaxN];
long long lastans;

int bfs_sort(int u)
{
	int qhead = 0, qtail = 0;
	que[qtail++] = u;
	fa[u] = 0;
	while(qhead != qtail)
	{
		int u = que[qhead++];
		size[u] = max_size[u] = 0;
		for(int k = gp.head[u]; k; k = gp.next[k])
		{
			int v = gp.point[k];
			if(mark[v] == mark_v && fa[u] != v)
			{
				fa[v] = u;
				que[qtail++] = v;
			}
		}
	}

	return qtail;
}

scape_t* divide(int r, scape_t *f, int dep)
{
	int tot = bfs_sort(r);
	for(int i = tot - 1; i >= 0; --i)
	{
		int u = que[i];
		size[u] += 1;
		size[fa[u]] += size[u];
		if(max_size[fa[u]] < size[u])
			max_size[fa[u]] = size[u];
	}

	int root, root_size = ~0u >> 1;
	for(int i = 0; i != tot; ++i)
	{
		int u = que[i];
		if(max_size[u] < size[r] - size[u])
			max_size[u] = size[r] - size[u];
		if(max_size[u] < root_size)
			root = u, root_size = max_size[u];
	}

	dist[r] = 0;
	for(int i = 0; i != tot; ++i)
	{
		int u = que[i];
		for(int k = gp.head[u]; k; k = gp.next[k])
		{
			int v = gp.point[k];
			if(fa[u] != v && mark[v] == mark_v)
				dist[v] = dist[u] + gp.weight[k];
		}
	}

	tot = bfs_sort(root);
	mark[root] = 0; depth[root] = dep;
	scape_t *n = scape_node + root;
	n->fa = f; n->top = r; n->size = tot;

	for(int i = 0; i != tot; ++i)
		treap_insert(n->parent_info, dist[que[i]] - R[que[i]]);
	treap_insert(n->info, -R[root]);

	for(int k = gp.head[root]; k; k = gp.next[k])
	{
		int v = gp.point[k];
		if(mark[v] == mark_v)
		{
			scape_t *son = divide(v, n, dep + 1);
			int qhead = 0, qtail = 0;
			que[qtail++] = son->parent_info - treap_node;
			while(qhead != qtail)
			{
				treap_t *z = treap_node + que[qhead++];
				treap_insert(n->info, z->val + gp.weight[k]);
				if(z->l != treap_nil)
					que[qtail++] = z->l - treap_node;
				if(z->r != treap_nil)
					que[qtail++] = z->r - treap_node;
			}
		}
	}

	return n;
}

void scape_rebuild(scape_t *n)
{
	int qhead = 0, qtail = 0;
	int id = n - scape_node;
	que[qtail++] = id;
	mark[id] = ++mark_v;
	while(qhead != qtail)
	{
		int u = que[qhead++];
		treap_mem.push(scape_node[u].info);
		treap_mem.push(scape_node[u].parent_info);
		scape_node[u].info = treap_nil;
		scape_node[u].parent_info = treap_nil;
		for(int k = gp.head[u]; k; k = gp.next[k])
		{
			int v = gp.point[k];
			if(depth[v] > depth[id] && mark[v] != mark_v)
				que[qtail++] = v, mark[v] = mark_v;
		}
	}

	int dep = depth[n - scape_node];
	divide(n->top, n->fa, dep);
}

void scape_insert(int n, int fa, int dist, int r)
{
	// set tree info
	R[n] = r;
	gp.add_child(n, fa, dist);

	// set split info
	depth[n] = depth[fa] + 1;

	// init node
	scape_t *z = scape_node + n;
	*z = *scape_nil;
	z->size = 1; z->top = n;
	z->fa = scape_node + fa;
	treap_insert(z->info, -r);
	treap_insert(z->parent_info, -r);

	scape_t *unbalanced_node;
	double unbalanced_val = 0.0;

	// update chain
	for(scape_t *prev = z, *now = z->fa; now != scape_nil; prev = now, now = now->fa)
	{
		++now->size;
		// balance size
		if(double(prev->size) / now->size > unbalanced_val)
		{
			unbalanced_node = now;
			unbalanced_val = double(prev->size) / now->size;
		}

		// calc answer
		int d = gp.get_dist(now - scape_node, n);
		treap_insert(now->info, d - r);

		int dz = gp.get_dist(now->top, n);
		treap_insert(now->parent_info, dz - r);

		int id = now - scape_node;
		lastans += treap_find(now->info, r - d);

		int dn = gp.get_dist(prev->top, now - scape_node);
		lastans -= treap_find(prev->parent_info, r - d - dn);
	}

	if(unbalanced_val > alpha)
		scape_rebuild(unbalanced_node);
}

void init()
{
	treap_nil = treap_node + treap_used++;
	treap_nil->l = treap_nil->r = treap_nil;
	treap_nil->size = 0;

	scape_nil = scape_node;
	scape_nil->fa = scape_nil;
	scape_nil->info = treap_nil;
	scape_nil->parent_info = treap_nil;
	scape_nil->size = 0;
}

int main()
{
	int n;
	std::scanf("%*d %d", &n);
	init();
	for(int i = 1; i <= n; ++i)
	{
		int a, b, r;
		std::scanf("%d %d %d", &a, &b, &r);
		a ^= lastans % 1000000000;
		scape_insert(i, a, b, r);
		std::printf("%lld\n", lastans);
	}
	return 0;
}
