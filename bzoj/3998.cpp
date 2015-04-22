/* BZOJ-3998: [TJOI2015]弦论
 *   后缀自动机 */
#include <cstdio>
#include <algorithm>

const int MaxN = 1000010, MaxAlpha = 26;
struct node_t
{
	int len;
	long long w, w2;
	node_t *fa, *ch[MaxAlpha];
} node[MaxN];

char str[MaxN];
int used, cnt[MaxN], sn[MaxN];
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

void sam_sort(int len)
{
	for(int i = 1; i != used; ++i)
		++cnt[node[i].len];
	for(int i = 1; i <= len + 2; ++i)
		cnt[i] += cnt[i - 1];
	for(int i = 1; i != used; ++i)
		sn[--cnt[node[i].len]] = i;
}

void sam_prework(int t)
{
	if(t)
	{
		node_t *now = sam_head;
		for(int i = 0; str[i]; ++i)
		{
			now = now->ch[str[i] - 'a'];
			++now->w2;
		}

		for(int i = used - 1; i >= 0; --i)
		{
			node_t *n = node + sn[i];
			if(n->fa) n->fa->w2 += n->w2;
		}
	}

	sam_head->w2 = 1;
	for(int i = used - 1; i >= 0; --i)
	{
		node_t *n = node + sn[i];
		n->w = t ? n->w2 : 1;
		for(int j = 0; j != MaxAlpha; ++j)
			if(n->ch[j]) n->w += n->ch[j]->w;
	}
}

int main()
{
	int t, k, n = 0;
	std::scanf("%s", str);
	std::scanf("%d %d", &t, &k);
	for(; str[n]; ++n) sam_extend(str[n] - 'a');
	sam_sort(n);
	sam_prework(t);

	node_t *now = sam_head;
	long long sum = 0;
	for(int i = 0; i != MaxAlpha; ++i)
		if(now->ch[i]) sum += now->ch[i]->w;
	if(sum < k) std::puts("-1");
	else while(k > 0)
	{
		int x;
		k -= t ? now->w2 : 1;
		for(x = 0; x != MaxAlpha; ++x)
		{
			if(now->ch[x])
			{
				if(now->ch[x]->w >= k)
					break;
				k -= now->ch[x]->w;
			}
		}
		
		if(x == MaxAlpha) break;
		std::putchar(x + 'a');
		now = now->ch[x];
	}

	return 0;
}
