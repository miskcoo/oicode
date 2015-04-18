/* SPOJ-SUBLEX Lexicographical Substring Search
 *  后缀自动机 */
#include <cstdio>
#include <cstring>

const int MaxN = 900001, MaxAlpha = 26;
struct node_t
{
	int len;
	long long w, chw[MaxAlpha];
	node_t *fa, *ch[MaxAlpha];
} node[MaxN << 1];

char str[MaxN];
int used, cnt[MaxN], sn[MaxN << 1];
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

void presolve(int len)
{
	for(int i = 0; i != used; ++i)
		++cnt[node[i].len];
	for(int i = 1; i <= len; ++i)
		cnt[i] += cnt[i - 1];
	for(int i = 0; i != used; ++i)
		sn[--cnt[node[i].len]] = i;

	for(int i = used - 1; i >= 0; --i)
	{
		node_t *p = node + sn[i];
		p->w = 1;
		for(int j = 0; j != MaxAlpha; ++j)
		{
			if(p->ch[j]) 
			{
				p->w += p->ch[j]->w;
				p->chw[j] = p->ch[j]->w;
			}
		}

		for(int j = 1; j != MaxAlpha; ++j)
			p->chw[j] += p->chw[j - 1];
	}
}

int main()
{
	int len = 0;
	std::scanf("%s", str);
	for(; str[len]; ++len)
		sam_extend(str[len] - 'a');
	presolve(len);

	int q;
	std::scanf("%d", &q);
	while(q --> 0)
	{
		int k;
		std::scanf("%d", &k);
		node_t *now = sam_head;
		k %= now->w - 1;
		while(k)
		{
			if(k-- == now->w) break;
			int l = 0, r = MaxAlpha - 1;
			while(l < r)
			{
				int m = (l + r) >> 1;
				if(now->chw[m] > k) r = m;
				else l = m + 1;
			}

			if(l) k -= now->chw[l - 1];
			now = now->ch[l];
			std::putchar(l + 'a');
		}

		std::puts("");
	}
	return 0;
}
