/* SPOJ-MUL Fast Multiplication
 *   快速傅立叶变换 */
#include <cstdio>
#include <cstring>
#include <algorithm>

typedef long long value_t;
const int MaxN = 10010, MaxK = 1 << 15, MaxL = 27;
const value_t mod_v = 15ll * (1 << MaxL) + 1;
value_t epsilon[MaxL + 1], arti_epsilon[MaxL + 1];
char str[MaxN];
value_t A[MaxK], B[MaxK], ans[MaxK], tmp[MaxK];

value_t power(value_t x, value_t p)
{
	value_t v = 1;
	while(p)
	{
		if(p & 1) v = v * x % mod_v;
		x = x * x % mod_v;
		p >>= 1;
	}

	return v;
}

void init_epsilon()
{
	epsilon[MaxL] = power(31, 15);
	arti_epsilon[MaxL] = power(64944062, 15);

	for(int i = MaxL - 1; i >= 0; --i)
	{
		epsilon[i] = epsilon[i + 1] * epsilon[i + 1] % mod_v;
		arti_epsilon[i] = arti_epsilon[i + 1] * arti_epsilon[i + 1] % mod_v;
	}
}

void shuffle(int n, value_t *x)
{
	for(int i = 0, j = 0; i != n; ++i)
	{
		if(i > j) std::swap(x[i], x[j]);
		for(int l = n >> 1; (j ^= l) < l; l >>= 1);
	}
}

void transform(int n, value_t *x, value_t *w)
{
	shuffle(n, x);
	for(int i = 2; i <= n; i <<= 1, ++w)
	{
		int m = i >> 1;
		tmp[0] = 1;
		for(int l = 1; l != m; ++l)
			tmp[l] = tmp[l - 1] * *w % mod_v;
		for(int p = 0; p < n; p += i)
		{
			for(int l = 0; l != m; ++l)
			{
				value_t t = x[p + m + l] * tmp[l] % mod_v;
				x[p + m + l] = x[p + l] - t;
				if(x[p + m + l] < 0) x[p + m + l] += mod_v;
				x[p + l] += t;
				if(x[p + l] >= mod_v) x[p + l] -= mod_v;
			}
		}
	}
}

value_t conv(char* str, int zip)
{
	value_t v = 0;
	for(int i = 0; i != zip; ++i)
		v = v * 10 + (str[i] - '0');
	return v;
}

int parse(char* str, value_t* buf, int zip = 3)
{
	int slen = std::strlen(str);
	int len = slen / zip, remain = slen - len * zip;
	if(remain) buf[len] = conv(str, remain);
	for(int i = len - 1; i >= 0; --i, str += zip)
		buf[i] = conv(str + remain, zip);
	return len + (remain != 0);
}

void output(int len, value_t* buf)
{
	std::printf("%lld", buf[len - 1]);
	for(int i = len - 2; i >= 0; --i)
		std::printf("%03lld", buf[i]);
	std::printf("\n");
}

void solve()
{
	std::memset(A, 0, sizeof(A));
	std::memset(B, 0, sizeof(B));

	std::scanf("%s", str);
	int la = parse(str, A);
	std::scanf("%s", str);
	int lb = parse(str, B);
	int n = la + lb, p = 1;
	while(p < n) p <<= 1;

	transform(p, A, epsilon + 1);
	transform(p, B, epsilon + 1);
	for(int i = 0; i != p; ++i)
		A[i] = A[i] * B[i] % mod_v;

	transform(p, A, arti_epsilon + 1);
	value_t inv = power(p, mod_v - 2);
	for(int i = 0; i != p; ++i)
		ans[i] = (A[i] * inv % mod_v + mod_v) % mod_v;

	ans[p] = 0;
	for(int i = 0; i != p; ++i)
	{
		ans[i + 1] += ans[i] / 1000;
		ans[i] %= 1000;
	}

	int len = p;
	while(!ans[len] && len) --len;
	output(len + 1, ans);
}

int main()
{
	init_epsilon();
	int t;
	std::scanf("%d", &t);
	while(t --> 0)
		solve();
}
