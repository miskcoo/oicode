/* BZOJ-2160: 拉拉队排练
 *   回文自动机 */
#include <cstdio>

const long long mod_v = 19930726;
const int MaxN = 1000010, MaxAlpha = 26;
struct node_t
{
	int len;
	long long w;
	node_t *fail, *ch[MaxAlpha];
} node[MaxN]; 
int used = 0;
char str[MaxN];
long long count[MaxN];
node_t *pam_odd, *pam_even, *pam_tail;
void pam_init()
{
	pam_even = node + used++;
	pam_odd = node + used++;
	pam_tail = pam_even;
	pam_odd->fail = pam_even->fail = pam_odd;
	pam_odd->len = -1, pam_even->len = 0;
}

void pam_extend(const char* str, int n)
{
	int x = str[n] - 'a';
	node_t *p = pam_tail;
	while(str[n - p->len - 1] != str[n]) p = p->fail;
	if(!p->ch[x])
	{
		node_t *z = node + used++;
		z->len = p->len + 2;
		node_t *q = p->fail;
		while(str[n - q->len - 1] != str[n]) q = q->fail;
		z->fail = q->ch[x] ? q->ch[x] : pam_even;
		p->ch[x] = z;
	}

	pam_tail = p->ch[x];
	++pam_tail->w;
}

long long power(long long x, long long p)
{
	long long v = 1;
	while(p)
	{
		if(p & 1) v = x * v % mod_v;
		x = x * x % mod_v;
		p >>= 1;
	}
	return v;
}

int main()
{
	int n;
	long long k;
	std::scanf("%d %lld", &n, &k);
	std::scanf("%s", str);
	pam_init();
	for(int i = 0; str[i]; ++i)
		pam_extend(str, i);
	for(int i = used - 1; i; --i)
	{
		node_t *n = node + i;
		n->fail->w += n->w;
		count[n->len] += n->w;
		for(int j = 0; j != MaxAlpha; ++j)
			if(n->ch[j]) n->w += n->ch[j]->w;
	}

	if(pam_odd->w < k)
	{
		std::puts("-1");
	} else {
		long long ans = 1;
		for(int i = n | 1; i > 0; i -= 2)
		{
			if(count[i] >= k)
			{
				ans = ans * power(i, k) % mod_v;
				break;
			} else ans = ans * power(i, count[i]) % mod_v;
			k -= count[i];
		}
		std::printf("%lld", ans);
	}

	return 0;
}
