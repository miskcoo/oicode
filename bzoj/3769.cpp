/* BZOJ-3769: spoj 8549 BST again 
 *  递推 */
#include <cstdio>
#include <algorithm>
#include <cstring>
 
const int MaxN = 601, MaxH = 601, MaxT = 10;
const long long mod_v = 1000000007;
long long f[MaxH][MaxN];
int ask[MaxT][2];
 
void solve(int n, int h)
{
    f[0][1] = 1;
    for(int i = 0; i <= h; ++i)
        f[i][0] = 1;
    for(int j = 1; j <= h; ++j)
    {
        for(int i = 1; i <= n; ++i)
        {
            long long v = 0;
            for(int k = 0; k != i; ++k)
                v += f[j - 1][k] * f[j - 1][i - k - 1] % mod_v;
            f[j][i] = v % mod_v;
        }
    }
}
 
int main()
{
    int t, max_n = 0, max_h = 0;
    std::scanf("%d", &t);
    for(int i = 0; i != t; ++i)
    {
        int n, h;
        std::scanf("%d %d", &n, &h);
        ask[i][0] = n, ask[i][1] = h;
        if(max_n < n) max_n = n;
        if(max_h < h) max_h = h;
    }
 
    solve(max_n, max_h);
 
    for(int i = 0; i != t; ++i)
    {
        int n = ask[i][0], h = ask[i][1];
        long long ans = (f[h][n] - f[h - 1][n] + mod_v) % mod_v;
        std::printf("%lld\n", ans);
    }
}
