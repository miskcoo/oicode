/*
 * BZOJ-2179 FFT快速傅立叶
 *  FFT/FNT
 */
#include <cmath>
#include <cstdio>
#include <cstring>
#include <algorithm>
 
typedef long long value_t;
const value_t mod_const = 15ll * (1 << 27) + 1;
const int MaxN = 1 << 17;
value_t temp[MaxN];
 
void dft(int n, value_t* buffer,
    int offset, int step, value_t epsilon)
{
    if(n == 1) return;
    int m = n >> 1;
    value_t epsilon_sqr = epsilon * epsilon % mod_const;
    dft(m, buffer, offset, step << 1, epsilon_sqr);
    dft(m, buffer, offset + step, step << 1, epsilon_sqr);
    value_t base = epsilon;
    epsilon = 1;
    for(int k = 0; k != m; ++k)
    {
        int pos = 2 * step * k;
		value_t ep = epsilon * buffer[pos + offset + step];
        temp[k] = (buffer[pos + offset] + ep) % mod_const;
 
        temp[k + m] = (buffer[pos + offset] - ep) % mod_const;
        if(temp[k + m] < 0) temp[k + m] += mod_const;
 
        epsilon = epsilon * base % mod_const;
    }
 
    for(int i = 0; i != n; ++i)
        buffer[i * step + offset] = temp[i];
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
 
value_t num_a[MaxN], num_b[MaxN];
value_t ans[MaxN + 2];
int main()
{
    int N;
    std::scanf("%d", &N);
    read_number(N, num_a);
    read_number(N, num_b);
 
    N = (N + 1) / 2;
    int p = 1;
    while(p < N) p <<= 1;
    N = p << 1;
    dft(N, num_a, 0, 1, power(31, (mod_const - 1) / N));
    dft(N, num_b, 0, 1, power(31, (mod_const - 1) / N));
    for(int i = 0; i != N; ++i)
    {
        num_a[i] *= num_b[i];
        num_a[i] %= mod_const;
    }
 
    dft(N, num_a, 0, 1, power(64944062, (mod_const - 1) / N));
    value_t invert_n = power(N, mod_const - 2);
    for(int i = 0; i != N; ++i)
        ans[i] = num_a[i] * invert_n % mod_const;
 
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
