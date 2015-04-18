/* SPOJ-8222 Substrings
 *  后缀自动机 */
#include <cstdio>
#include <algorithm>

const int MaxN = 250001, MaxAlpha = 26;
struct node_t
{
	int len, size;
	node_t *fa, *ch[MaxAlpha];
} node[MaxN << 1];
char str[MaxN];
int used, cnt[MaxN], sn[MaxN << 1], dp[MaxN];
node_t *sam_head = node + used++, *sam_tail = sam_head;

void sam_extend(int x)
{
	node_t *p = sam_tail, *n = node + used++;
	sam_tail = n;
	n->len = p->len + 1;
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
			node_t *q = p->ch[x], *r = node + used++;
			*r = *q;
			r->len = p->len + 1;
			n->fa = q->fa = r;
			for(; p && p->ch[x] == q; p = p->fa)
				p->ch[x] = r;
		}
	}
}

int sort_node()
{
	int max = 0;
	for(int i = 0; i != used; ++i)
	{
		++cnt[node[i].len];
		if(max < node[i].len)
			max = node[i].len;
	}

	for(int i = 1; i <= max; ++i)
		cnt[i] += cnt[i - 1];
	
	int tot = cnt[max];
	for(int i = 0; i != tot; ++i)
		sn[--cnt[node[i].len]] = i;
	return tot;
}

int main()
{
	std::scanf("%s", str);
	int len = 0;
	for(; str[len]; ++len)
		sam_extend(str[len] - 'a');
	node_t *now = sam_head;
	for(int i = 0; str[i]; ++i)
	{
		now = now->ch[str[i] - 'a'];
		++now->size;
	}

	int tot = sort_node();
	for(int i = tot - 1; i >= 0; --i)
	{
		node_t *p = node + sn[i];
		if(p->fa) p->fa->size += p->size;
	}

	for(int i = 0; i != tot; ++i)
		dp[node[i].len] = std::max(dp[node[i].len], node[i].size);
	for(int i = len - 1; i; --i)
		dp[i] = std::max(dp[i], dp[i + 1]);
	for(int i = 1; i <= len; ++i)
		std::printf("%d\n", dp[i]);
	return 0;
}
