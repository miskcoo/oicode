/* BZOJ-2894: 世界线
 *   广义后缀自动机  */
#include <cstdio>
#include <algorithm>

const int MaxN = 500010, MaxAlpha = 26;
struct trie_t
{
	trie_t *ch[MaxAlpha];
} trie_node[MaxN >> 1];
int trie_used;

struct sam_t
{
	int len;
	long long w;
	sam_t *fa, *ch[MaxAlpha];
} sam_node[MaxN];
int sam_used, cnt[MaxN], sn[MaxN];
sam_t *sam_head = sam_node + sam_used++;

sam_t* sam_extend(sam_t *tail, int x)
{
	sam_t *p = tail, *sam_tail;
	if(p->ch[x])
	{
		if(p->len + 1 == p->ch[x]->len)
		{
			sam_tail = p->ch[x];
		} else {
			sam_t *q = p->ch[x], *r = sam_node + sam_used++;
			*r = *q, r->len = p->len + 1;
			q->fa = r, sam_tail = r;
			for(; p && p->ch[x] == q; p = p->fa)
				p->ch[x] = r;
		}
	} else {
		sam_t *n = sam_node + sam_used++;
		n->len = p->len + 1, sam_tail = n;
		for(; p && !p->ch[x]; p = p->fa)
			p->ch[x] = n;
		if(!p)
		{
			n->fa = sam_head;
		} else {
			if(p->len + 1 == p->ch[x]->len)
			{
				n->fa = p->ch[x];
			} else {
				sam_t *q = p->ch[x], *r = sam_node + sam_used++;
				*r = *q, r->len = p->len + 1;
				n->fa = q->fa = r;
				for(; p && p->ch[x] == q; p = p->fa)
					p->ch[x] = r;
			}
		}
	}

	return sam_tail;
}

struct graph_t
{
	int total;
	int head[MaxN >> 1], point[MaxN], next[MaxN];

	void add_edge(int u, int v)
	{
		point[++total] = v;
		next[total] = head[u];
		head[u] = total;
	}
} gp;

char str[MaxN];
int fa[MaxN], que[MaxN];
trie_t *que_trie[MaxN];
sam_t *que_sam[MaxN];

trie_t* build_trie()
{
	trie_t *root = trie_node + trie_used++;
	int qhead = 0, qtail = 0;
	que[qtail] = 1;
	que_trie[qtail++] = root;
	while(qhead != qtail)
	{
		int u = que[qhead], x = str[u] - 'a';
		trie_t *now = que_trie[qhead++];
		if(!now->ch[x]) now->ch[x] = trie_node + trie_used++;
		now = now->ch[x];
		for(int k = gp.head[u]; k; k = gp.next[k])
		{
			int v = gp.point[k];
			if(v != fa[u])
			{
				fa[v] = u;
				que[qtail] = v;
				que_trie[qtail++] = now;
			}
		}
	}

	return root;
}

void build_sam(trie_t *trie)
{
	int qhead = 0, qtail = 0;
	que_sam[qtail] = sam_head;
	que_trie[qtail++] = trie;
	while(qhead != qtail)
	{
		trie_t *trie = que_trie[qhead];
		sam_t *now = que_sam[qhead++];
		for(int i = 0; i != MaxAlpha; ++i)
		{
			if(trie->ch[i])
			{
				que_sam[qtail] = sam_extend(now, i);
				que_trie[qtail++] = trie->ch[i];
			}
		}
	}
}

void sam_dp(int n)
{
	for(int i = 0; i != sam_used; ++i)
		++cnt[sam_node[i].len];
	for(int i = 1; i <= n; ++i)
		cnt[i] += cnt[i - 1];
	for(int i = 0; i != sam_used; ++i)
		sn[--cnt[sam_node[i].len]] = i;
	for(int i = sam_used - 1; i >= 0; --i)
	{
		sam_t *n = sam_node + sn[i];
		for(int j = 0; j != MaxAlpha; ++j)
			if(n->ch[j]) n->w += n->ch[j]->w;
		++n->w;
	}
}

void solve_kth(long long k)
{
	if(k == 1)
	{
		std::puts("");
	} else if(k > sam_head->w) {
		std::puts("-1");
	} else {
		sam_t *now = sam_head;
		while(k > 0)
		{
			if(--k == 0) break;
			long long s = 0;
			int i;
			for(i = 0; i != MaxAlpha; ++i)
			{
				if(now->ch[i]) s += now->ch[i]->w;
				if(s >= k) break;
			}

			std::putchar('a' + i);
			k -= s - now->ch[i]->w;
			now = now->ch[i];
		}
		std::puts("");
	}
}

int main()
{
	int n, q;
	std::scanf("%d %d", &n, &q);
	std::scanf("%s", str + 1);
	for(int i = 1; i != n; ++i)
	{
		int u, v;
		std::scanf("%d %d", &u, &v);
		gp.add_edge(u, v);
		gp.add_edge(v, u);
	}

	trie_t *trie = build_trie();
	build_sam(trie);
	sam_dp(n);
	std::printf("%lld\n", sam_head->w);
	while(q --> 0)
	{
		long long k;
		std::scanf("%lld", &k);
		solve_kth(k);
	}
	return 0;
}
