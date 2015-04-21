/* BZOJ-1975: [Sdoi2010]魔法猪学院
 *   K短路 */
#include <cstdio>
#include <cmath>
#include <queue>
#include <cstring>
#include <algorithm>

const int MaxN = 5010, MaxM = 200010;
const double eps = 1.0e-7;

template<typename Type>
class memory_pool
{
	int used, remain;
	Type *data[1 << 16];
public:
	memory_pool() : used(0), remain(0) {}
	~memory_pool() 
	{
		for(int i = 0; i != used; ++i)
			delete[] data[i];
	}

	Type* get()
	{
		if(!remain) data[used++] = new Type[remain = 1 << 16];
		return data[used - 1] + --remain;
	}
};

struct node_t
{
	double v;
	int h, to;
	node_t *l, *r;
};

memory_pool<node_t> mem;
node_t *nil, *root[MaxN];

node_t* merge(node_t *a, node_t *b)
{
	if(a == nil) return b;
	if(b == nil) return a;
	if(a->v > b->v) std::swap(a, b);
	a->r = merge(a->r, b);
	a->h = a->r->h + 1;
	if(a->l->h < a->r->h)
		std::swap(a->l, a->r);
	return a;
}

node_t* functional_merge(node_t *a, node_t *b)
{
	if(a == nil) return b;
	if(b == nil) return a;
	if(a->v > b->v) std::swap(a, b);
	node_t *n = mem.get();
	*n = *a;
	n->r = functional_merge(n->r, b);
	n->h = n->r->h + 1;
	if(n->l->h < n->r->h)
		std::swap(n->l, n->r);
	return n;
}

struct graph_t
{
	int tot, head[MaxN], point[MaxM], next[MaxM];
	double weight[MaxM];

	void add_edge(int u, int v, double w)
	{
		point[++tot] = v;
		weight[tot] = w;
		next[tot] = head[u];
		head[u] = tot;
	}
} gp, rgp;

int fa[MaxN], mark[MaxM], mark_v;
double dist[MaxN];

int que[MaxN];

void spfa(graph_t *g, int n, int s)
{
	++mark_v;
	for(int i = 1; i <= n; ++i)
		dist[i] = 1.0e100;

	int qhead = 0, qtail = 0;
	dist[n] = 0.0;
	que[qtail++] = n;
	while(qhead != qtail)
	{
		int u = que[qhead++];
		if(qhead == MaxN) qhead = 0;
		for(int k = g->head[u]; k; k = g->next[k])
		{
			int v = g->point[k];
			double d = dist[u] + g->weight[k];
			if(d < dist[v])
			{
				dist[v] = d;
				if(mark[v] != mark_v)
				{
					mark[v] = mark_v;
					que[qtail++] = v;
					if(qtail == MaxN) qtail = 0;
				}
			}
		}

		mark[u] = 0;
	}
}

bool dcmp(double x)
{
	return x < eps && x > -eps;
}

struct data_t
{
	int to;
	double v;

	bool operator < (const data_t& r) const
	{
		return v < r.v;
	}
};

void prework(int n, int m)
{
	++mark_v;
	for(int u = 1; u <= n; ++u)
	{
		for(int k = gp.head[u]; k; k = gp.next[k])
		{
			int v = gp.point[k];
			if(dcmp(dist[v] + gp.weight[k] - dist[u]))
			{
				fa[u] = v;
				mark[k] = mark_v;
				break;
			}
		}
	}

	data_t *que = new data_t[n];
	for(int i = 1; i <= n; ++i)
	{
		root[i] = nil;
		que[i - 1].to = i;
		que[i - 1].v = dist[i];
	}

	std::sort(que, que + n);
	for(int i = 1; i != n; ++i)
	{
		int u = que[i].to;
		node_t *r = nil;
		for(int k = gp.head[u]; k; k = gp.next[k])
		{
			if(mark[k] != mark_v)
			{
				node_t *n = mem.get();
				n->h = 0, n->l = n->r = nil;
				n->to = gp.point[k];
				n->v = gp.weight[k] + (dist[n->to] - dist[u]);
				r = merge(r, n);
			}
		}

		if(r == nil) root[u] = root[fa[u]];
		else root[u] = functional_merge(root[fa[u]], r);
	}

	delete[] que;
}

struct global_t
{
	double v;
	node_t *n;
	bool operator < (const global_t& r) const
	{
		return v > r.v;
	}
};

double tot;
std::priority_queue<global_t> heap;

void init()
{
	static node_t nil_base;
	nil = &nil_base;
	nil->l = nil->r = nil;
}

void insert(node_t *r, double v)
{
	if(r == nil || r->v > tot) return;
	global_t g;
	g.n = r;
	g.v = v;
	heap.push(g);
}

int main()
{
	int n, m;
	std::scanf("%d %d %lf", &n, &m, &tot);
	for(int i = 0; i != m; ++i)
	{
		int u, v;
		double w;
		std::scanf("%d %d %lf", &u, &v, &w);
		gp.add_edge(u, v, w);
		rgp.add_edge(v, u, w);
	}

	init();
	spfa(&rgp, n, n);
	prework(n, m);
	insert(root[1], root[1]->v);

	int count = 0;
	if(dist[1] <= tot)
	{
		tot -= dist[1];
		++count;
	}

	while(!heap.empty())
	{
		global_t g = heap.top();
		heap.pop();
		node_t *now = g.n;

		if(g.v + dist[1] > tot) break;
		insert(now->l, g.v - now->v + now->l->v);
		insert(now->r, g.v - now->v + now->r->v);
		++count;
		tot -= dist[1] + g.v;

		node_t *t = root[now->to];
		insert(t, g.v + t->v);
	}

	std::printf("%d", count);
	return 0;
}
