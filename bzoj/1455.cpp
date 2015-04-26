/* BZOJ-1455: 罗马游戏
 *  左偏树+并查集 */
#include <cstdio>
#include <algorithm>

const int MaxN = 1000010;
struct node_t
{
	int v, id, len;
	node_t *l, *r;
} node[MaxN];
node_t *nil = node, *root[MaxN];

node_t* merge(node_t *a, node_t *b)
{
	if(a == nil) return b;
	if(b == nil) return a;
	if(a->v > b->v) std::swap(a, b);
	a->r = merge(a->r, b);
	a->len = a->r->len + 1;
	if(a->r->len > a->l->len) std::swap(a->l, a->r);
	return a;
}

int fa[MaxN], dead[MaxN];
int find(int v)
{
	if(fa[v] == v) return v;
	return fa[v] = find(fa[v]);
}

int main()
{
	int n;
	std::scanf("%d", &n);
	nil->l = nil->r = nil;
	for(int i = 1; i <= n; ++i)
	{
		int v;
		std::scanf("%d", &v);
		node[i] = *nil;
		node[i].v = v;
		node[i].id = i;
		node[i].len = 1;
		root[i] = node + i;
		fa[i] = i;
	}

	int m;
	std::scanf("%d", &m);
	for(int i = 0; i != m; ++i)
	{
		char op[2];
		std::scanf("%s", op);
		if(*op == 'K')
		{
			int w;
			std::scanf("%d", &w);
			if(!dead[w])
			{
				int g = find(w);
				std::printf("%d\n", root[g]->v);
				dead[root[g]->id] = 1;
				root[g] = merge(root[g]->l, root[g]->r);
			} else std::puts("0");
		} else {
			int i, j;
			std::scanf("%d %d", &i, &j);
			if(!dead[i] && !dead[j])
			{
				int u = find(i), v = find(j);
				if(u != v)
				{
					fa[u] = v;
					root[v] = merge(root[u], root[v]);
				}
			}
		}
	}
	return 0;
}
