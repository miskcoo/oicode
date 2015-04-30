/* BZOJ-3926: [Zjoi20150]诸神眷顾的幻想乡
 *   广义后缀自动机  */
#include <cstdio>

const int MaxN = 100010, MaxAlpha = 10;
struct node_t
{
	int len;
	long long w;
	node_t *fa, *ch[MaxAlpha];
} node[MaxN * 40];
struct trie_t
{
	trie_t *ch[MaxAlpha];
} trie_node[MaxN * 20];
int used, trie_used, total, alpha;
int head[MaxN], point[MaxN << 1], next[MaxN << 1];
int deg[MaxN], color[MaxN], mark[MaxN];
int cnt[MaxN * 40], sn[MaxN * 40];
node_t *sam_head;

void add_edge(int u, int v)
{
	++deg[v];
	point[++total] = v;
	next[total] = head[u];
	head[u] = total;
}

node_t* sam_extend(node_t *tail, int x)
{
	node_t *n = tail->ch[x];
	if(n)
	{
		if(tail->len + 1 == n->len)
			return n;
		node_t *r = node + used++;
		*r = *n;
		r->len = tail->len + 1;
		n->fa = r;
		for(; tail && tail->ch[x] == n; tail = tail->fa)
			tail->ch[x] = r;
		return r;
	} else {
		n = node + used++;
		n->len = tail->len + 1;
		for(; tail && !tail->ch[x]; tail = tail->fa)
			tail->ch[x] = n;
		if(!tail) 
		{
			n->fa = sam_head;
		} else {
			if(tail->len + 1 == tail->ch[x]->len)
			{
				n->fa = tail->ch[x];
			} else {
				node_t *q = tail->ch[x], *r = node + used++;
				*r = *q;
				r->len = tail->len + 1;
				n->fa = q->fa = r;
				for(; tail && tail->ch[x] == q; tail = tail->fa)
					tail->ch[x] = r;
			}
		}
		return n;
	}
}

void build_sam(node_t *now, trie_t *trie)
{
	for(int i = 0; i != alpha; ++i)
	{
		if(trie->ch[i])
		{
			node_t *n = sam_extend(now, i);
			build_sam(n, trie->ch[i]);
		}
	}
}

void build_trie(trie_t *now, int u)
{
	if(!now->ch[color[u]])
		now->ch[color[u]] = trie_node + trie_used++;
	now = now->ch[color[u]];
	mark[u] = 1;
	for(int k = head[u]; k; k = next[k])
		if(!mark[point[k]]) 
			build_trie(now, point[k]);
	mark[u] = 0;
}

void calc(int tot)
{
	for(int i = 0; i != used; ++i) 
		++cnt[node[i].len];
	for(int i = 1; i <= tot; ++i)
		cnt[i] += cnt[i - 1];
	for(int i = used - 1; i >= 0; --i)
		sn[--cnt[node[i].len]] = i;
	for(int i = used - 1; i >= 0; --i)
	{
		node_t *n = node + sn[i];
		for(int j = 0; j != alpha; ++j)
			if(n->ch[j]) n->w += n->ch[j]->w;
		++n->w;
	}
}

int main()
{
	int n;
	std::scanf("%d %d", &n, &alpha);
	for(int i = 1; i <= n; ++i)
		std::scanf("%d", color + i);
	for(int i = 1; i != n; ++i)
	{
		int u, v;
		std::scanf("%d %d", &u, &v);
		add_edge(u, v);
		add_edge(v, u);
	}

	trie_t *trie_head = trie_node + trie_used++;
	for(int i = 1; i <= n; ++i)
		if(deg[i] == 1) build_trie(trie_head, i);
	sam_head = node + used++;
	build_sam(sam_head, trie_head);
	calc(n);
	std::printf("%lld", sam_head->w - 1);
	return 0;
}
