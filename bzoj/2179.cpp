/*
 * BZOJ-2179 FFT快速傅立叶
 *  FFT/FNT 迭代版（蝶形变换）
 */
#include <cmath>
#include <cstdio>
#include <cstring>
#include <algorithm>
 
typedef long long value_t;
const value_t mod_const = 15ll * (1 << 27) + 1;
const int MaxN = 1 << 16;
value_t temp[MaxN];
value_t epsilon[28];
value_t arti_epsilon[28];
 
value_t power(value_t x, int p)
{
    value_t v = 1;
    while(p)
    {
        if(p & 1) v = x * v % mod_const;
        x = x * x % mod_const;
        p >>= 1;
    }
 
    return v;
}

void init_epsilon()
{
	epsilon[27] = power(31, 15);
	arti_epsilon[27] = power(64944062, 15);
	for(int i = 26; i >= 0; --i)
	{
		epsilon[i] = epsilon[i + 1] 
			* epsilon[i + 1] % mod_const;
		arti_epsilon[i] = arti_epsilon[i + 1] 
			* arti_epsilon[i + 1] % mod_const;
	}
}
 
void dft(int k, value_t* x, value_t* epsilon)
{
	for(int i = 0, j = 0; i != k; ++i)
	{
		using std::swap;
		if(i > j) swap(x[i], x[j]);
		for(int l = k >> 1; (j ^= l) < l; l >>= 1);
	}

	for(int i = 2; i <= k; i <<= 1, ++epsilon)
	{
		int t = i >> 1;
		for(int p = 0; p < k; p += i)
		{
			value_t e = 1;
			for(int l = 0; l != t; ++l)
			{
				value_t r = e * x[p + l + t] % mod_const;
				if((x[p + l + t] = x[p + l] - r) < 0) 
					x[p + l + t] += mod_const;
				if((x[p + l] += r) >= mod_const)
					x[p + l] -= mod_const;
				e = e * *epsilon % mod_const;
			}
		}
	}
}
 
value_t get_number(int len)
{
    value_t v = 0;
    for(int i = 0; i != len; ++i)
    {
        char ch;
        while((ch = std::getchar()) && (ch < '0' || ch > '9'));
        v = v * 10 + (ch - '0');
    }
 
    return v;
}
 
void read_number(int N, value_t* buffer)
{
    int total = N / 2;
    int remain = N % 2;
    if(remain != 0) buffer[total] = get_number(remain);
    for(int i = total - 1; i >= 0; --i)
        buffer[i] = get_number(2);
}
 
value_t num_a[MaxN], num_b[MaxN];
value_t ans[MaxN + 2];
int main()
{
    int N;
    std::scanf("%d", &N);
    read_number(N, num_a);
    read_number(N, num_b);
	init_epsilon();
    N = (N + 1) / 2;
    int p = 1;
    while(p < N) p <<= 1;
    N = p << 1;
    dft(N, num_a, epsilon + 1);
    dft(N, num_b, epsilon + 1);
    for(int i = 0; i != N; ++i)
    {
        num_a[i] *= num_b[i];
        num_a[i] %= mod_const;
    }
 
    dft(N, num_a, arti_epsilon + 1);
    value_t invert_n = power(N, mod_const - 2);
    for(int i = 0; i != N; ++i)
	{
        ans[i] = num_a[i] * invert_n % mod_const;
		if(ans[i] < 0) ans[i] += mod_const;
	}
 
    for(int i = 0; i <= N; ++i)
    {
        ans[i + 1] += ans[i] / 100;
        ans[i] %= 100;
    }
 
    int len = N + 1;
    while(!ans[len] && len) --len;
    std::printf("%lld", ans[len]);
    for(int i = len - 1; i >= 0; --i)
        std::printf("%02lld", ans[i]);
 
    return 0;
}
