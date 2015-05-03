/* BZOJ-1717: [Usaco2006 Dec]Milk Patterns 产奶的模式
 *   二分+哈希  */
#include <cstdio>
#include <map>
#include <cstring>
 
const int MaxN = 20010;
const long long mod_v = 13 * (1 << 27) + 1, g = 31;
int data[MaxN];
long long pw[MaxN];
 
bool check(int n, int len, int k)
{
    if(k == 1) return true;
    std::map<long long, int> m;
    long long hash = 0;
    for(int i = 0; i != len; ++i)
        hash = (hash * g + data[i]) % mod_v;
    ++m[hash];
    for(int i = len; i != n; ++i)
    {
        hash -= pw[len - 1] * data[i - len] % mod_v;
        hash = (hash * g + data[i]) % mod_v;
        if(++m[hash] >= k) return true;
    }
    return false;
}
 
int main()
{
    int n, k;
    std::scanf("%d %d", &n, &k);
    for(int i = 0; i != n; ++i)
        std::scanf("%d", data + i);
 
    pw[0] = 1;
    for(int i = 1; i <= n; ++i)
        pw[i] = pw[i - 1] * g % mod_v;
 
    int l = 1, r = n;
    while(l < r)
    {
        int m = (l + r) >> 1;
        if(check(n, m, k)) l = m + 1;
        else r = m;
    }
 
    std::printf("%d", l - 1);
 
    return 0;
}
