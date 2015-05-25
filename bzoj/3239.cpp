/* BZOJ-3239: Discrete Logging
 *   大步小步+Hash */
#include <cstdio>
#include <cmath>
#include <cstring>
 
const int mod_v = 65537;
class hash_t
{
    int head[mod_v], next[mod_v];
    int total, val[mod_v], f[mod_v];
public:
    int& operator [](int v) 
    {
        int z = v % mod_v;
        for(int k = head[z]; k; k = next[k])
            if(val[k] == v) return f[k];
        val[++total] = v;
        f[total] = 0;
        next[total] = head[z];
        head[z] = total;
        return f[total];
    }
 
    int find(int v)
    {
        for(int k = head[v % mod_v]; k; k = next[k])
            if(val[k] == v) return f[k];
        return -1;
    }
 
    void reset()
    {
		total = 0;
        std::memset(head, 0, sizeof(head));
    }
} hash;
 
int power(int x, int p, int m)
{
    int v = 1;
    while(p)
    {
        if(p & 1) v = 1ll * x * v % m;
        x = 1ll * x * x % m;
        p >>= 1;
    }
    return v;
}
 
int solve(int B, int N, int P)
{
    if(N == 1) return 0;
    hash.reset();
    int m = int(std::sqrt(1.0 * P) + 1);
    int val = N;
    for(int i = 0; i != m; ++i)
    {
        hash[val] = i;
        val = 1ll * val * B % P;
    }
 
    int base = power(B, m, P);
    val = 1;
    for(int i = 1; i <= m + 1; ++i)
    {
        val = 1ll * val * base % P;
        int z = hash.find(val);
        if(z >= 0) return i * m - z;
    }
 
    return -1;
}
 
int main()
{
    int P, B, N;
    while(std::scanf("%d %d %d", &P, &B, &N) != EOF)
    {
        int ans = solve(B, N, P);
        if(ans == -1) std::puts("no solution");
        else std::printf("%d\n", ans);
    }
    return 0;
}
