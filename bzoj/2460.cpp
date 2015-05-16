/* BZOJ-2460: [BeiJing2011]元素
 *   高斯消元线性基 */
#include <cstdio>
#include <algorithm>

const int MaxN = 1000, MaxL = 62;
long long val[MaxN], base[MaxL + 1];
int cost[MaxN], ind[MaxN];

bool cmp(int a, int b)
{
	if(cost[a] == cost[b])
		return val[a] > val[b];
	return cost[a] > cost[b];
}

int main()
{
	int n;
	std::scanf("%d", &n);
	for(int i = 0; i != n; ++i)
	{
		ind[i] = i;
		std::scanf("%lld %d", val + i, cost + i);
	}

	std::sort(ind, ind + n, cmp);
	int ans = 0;
	for(int i = 0; i != n; ++i)
	{
		for(int j = MaxL; j >= 0; --j)
		{
			if(val[ind[i]] & (1ll << j))
			{
				if(!base[j])
				{
					base[j] = val[ind[i]];
					break;
				} else val[ind[i]] ^= base[j];
			}
		}

		if(val[ind[i]]) ans += cost[ind[i]];
	}

	std::printf("%d", ans);
	return 0;
}
