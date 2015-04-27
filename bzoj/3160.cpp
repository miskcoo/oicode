/* BZOJ-3160: 万径人踪灭
 *   回文自动机+FFT */
#include <cstdio>
#include <complex>
#include <cmath>
#include <cstring>
#include <algorithm>

const long long mod_v = 1000000007;
const int MaxN = 1 << 18, MaxAlpha = 2;
struct node_t
{
	int len;
	long long w;
	node_t *fail, *ch[MaxAlpha];
} node[MaxN];
int used;
char str[MaxN];
node_t *pam_odd, *pam_even, *pam_tail;

void pam_init()
{
	pam_even = node + used++;
	pam_odd = node + used++;
	pam_tail = pam_even;
	pam_odd->fail = pam_even->fail = pam_odd;
	pam_odd->len = -1, pam_even->len = 0;
}

void pam_extend(int n)
{
	int x = str[n] - 'a';
	node_t *p = pam_tail;
	while(str[n - p->len - 1] != str[n])
		p = p->fail;
	if(!p->ch[x])
	{
		node_t *z = node + used++;
		z->len = p->len + 2;
		node_t *q = p->fail;
		while(str[n - q->len - 1] != str[n])
			q = q->fail;
		z->fail = q->ch[x] ? q->ch[x] : pam_even;
		p->ch[x] = z;
	}

	pam_tail = p->ch[x];
	++pam_tail->w;
}

typedef std::complex<double> complex_t;
complex_t eps[MaxN], inv_eps[MaxN];
complex_t A[MaxN], B[MaxN];

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

void init_eps(int n)
{
	double angle = 2.0 * acos(-1) / n;
	for(int i = 0; i < n; ++i)
	{
		eps[i] = complex_t(cos(angle * i), sin(angle * i));
		inv_eps[i] = conj(eps[i]);
	}
}


void transform(int n, complex_t *x, complex_t *w)
{
	for(int i = 0, j = 0; i != n; ++i)
	{
		if(i > j) std::swap(x[i], x[j]);
		for(int l = n >> 1; (j ^= l) < l; l >>= 1);
	}

	for(int i = 2; i <= n; i <<= 1)
	{
		int m = i >> 1;
		for(int j = 0; j < n; j += i)
		{
			for(int k = 0; k != m; ++k)
			{
				complex_t z = x[j + k + m] * w[n / i * k];
				x[j + k + m] = x[j + k] - z;
				x[j + k] += z;
			}
		}
	}
}

int main()
{
	pam_init();
	std::scanf("%s", str);
	int len = std::strlen(str);
	for(int i = 0; str[i]; ++i)
		pam_extend(i);

	long long ans = 0;
	for(int i = used - 1; i; --i)
	{
		node_t *n = node + i;
		n->fail->w += n->w;
		ans -= n->w;
	}

	for(int i = 0; i != len; ++i)
	{
		if(str[i] == 'a') A[i] += 1.0;
		else B[i] += 1.0;
	}

	int p = 1;
	while(p < len) p <<= 1;
	p <<= 1;
	init_eps(p);
	transform(p, A, eps);
	transform(p, B, eps);
	for(int i = 0; i != p; ++i)
		A[i] = A[i] * A[i] + B[i] * B[i];
	transform(p, A, inv_eps);
	for(int i = 0; i != p; ++i)
	{
		long long v = A[i].real() / p + 0.5;
		v = (v >> 1) + (v & 1);
		ans = (ans + power(2, v) - 1) % mod_v;
	}

	ans = (ans + mod_v) % mod_v;
	std::printf("%lld", ans);
	return 0;
}
