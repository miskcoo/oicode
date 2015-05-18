/* UOJ-#20. 【NOIP2014】解方程 */
#include <cstdio>
#include <iostream>
#include <ctime>
#include <cstdlib>

const int MaxN = 1000001;
char not_prime[MaxN];
int prime[MaxN], prim_num = 0;
long long value[10000];
char cr[110][10010];
void linear_sieve(int n)
{
	for(int i = 2; i <= n; ++i)
	{
		if(!not_prime[i])
			prime[prim_num++] = i;
		for(int j = 0; j != prim_num; ++j)
		{
			long long t = (long long)i * prime[j];
			if(t > n) break;
			not_prime[t] = 1;
			if(i % prime[j] == 0)
				break;
		}
	}
}

int make_value(int n)
{
	int r = prim_num - 1;
	int l = 0;
	while(prime[l] < 1500) ++l;
	for(int i = 0; i != n; )
	{
		long long t = (long long)prime[l] * prime[r];
		if(t >= 2000000000) 
		{
			--r;
		} else {
			value[i++] = t;
			--r, ++l;
			if(r <= l) return i;
		}
	}
	
	return n;
}

char mark[10007];
char candidate[1000010];
long long k[110];
void generate_coeff(int n, long long mod_v)
{
	for(int i = 0; i <= n; ++i)
	{
		k[i] = 0;
		int sign = 1;
		int p = 0;
		if(cr[i][p] == '-')
		{
			++p;
			sign = -1;
		}
		
		for(; cr[i][p]; ++p)
		{
			k[i] = k[i] * 10 + (cr[i][p] - '0');
			if(k[i] >= 100000000000000ll)
				k[i] %= mod_v;
		}
		
		k[i] %= mod_v;
		if(sign == -1) k[i] = mod_v - k[i];
	}
}

int subsititude(int n, long long x, long long mod_v)
{
	long long v = 0;
	for(int i = n; i >= 0; --i)
		v = (v * x + k[i]) % mod_v;
	if(v != 0) return 0;
	return 1;
}

const int base_mod = 10007;
void print_ans(int m)
{
	int count = 0;
	for(int i = 1; i <= m; ++i)
		if(candidate[i] && mark[i % base_mod]) ++count;
	std::printf("%d\n", count);
	for(int i = 1; i <= m; ++i)
		if(candidate[i] && mark[i % base_mod])
			std::printf("%d\n", i);
}

void solve(int n, int m)
{
	long long mod_v = base_mod;
	generate_coeff(n, mod_v);
	for(int i = 0; i != mod_v; ++i)
		mark[i] = subsititude(n, i, mod_v);
	mod_v = value[100];
	generate_coeff(n, mod_v);
	for(int j = 1; j <= m; ++j)
	{
		if(!candidate[j] || !mark[j % base_mod])
			continue;
		candidate[j] = subsititude(n, j, mod_v);
	}
}

int main()
{
	linear_sieve(MaxN);
	make_value(1000);
	int n, m;
	std::scanf("%d %d", &n, &m);
	for(int i = 0; i <= n; ++i)
		std::scanf("%s", cr[i]);
	for(int i = 0; i <= m; ++i)
		candidate[i] = 1;
	
	solve(n, m);
	print_ans(m);
	return 0;
}

