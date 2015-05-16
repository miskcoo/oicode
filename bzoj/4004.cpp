/* BZOJ-4004: [JLOI2015]装备购买
 *   高斯消元+线性基 */
#include <cstdio>
#include <algorithm>

const int MaxN = 510;
const long long mod_v = 1000000007;
long long eqn[MaxN][MaxN];
int ind[MaxN], cost[MaxN], w[MaxN];

bool cmp(int a, int b)
{
	return cost[a] < cost[b];
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

void gauss(int n, int m)
{
	for(int i = 0; i != n; ++i)
	{
		for(int j = 0; j != m; ++j)
		{
			if(!eqn[ind[i]][j]) continue;
			if(w[j] == -1)
			{ 
				w[j] = ind[i];
				break;
			} else {
				long long z = eqn[ind[i]][j] * power(eqn[w[j]][j], mod_v - 2) % mod_v;
				for(int k = j; k != m; ++k)
					eqn[ind[i]][k] = (eqn[ind[i]][k] - eqn[w[j]][k] * z) % mod_v;
			}
		}
	}
}

int main()
{
	int n, m;
	std::scanf("%d %d", &n, &m);
	for(int i = 0; i != n; ++i)
	{
		ind[i] = i;
		for(int j = 0; j != m; ++j)
			std::scanf("%lld", eqn[i] + j);
	}

	for(int i = 0; i != n; ++i)
		std::scanf("%d", cost + i);
	for(int i = 0; i != m; ++i) w[i] = -1;

	std::sort(ind, ind + n, cmp);
	gauss(n, m);

	int cnt = 0, ans = 0;
	for(int i = 0; i != m; ++i)
		if(w[i] != -1) ++cnt, ans += cost[w[i]];

	std::printf("%d %d", cnt, ans);
	return 0;
}
