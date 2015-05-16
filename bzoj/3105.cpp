/* BZOJ-3105: [cqoi2013]新Nim游戏
 *   高斯消元线性基 */
#include <cstdio>
#include <algorithm>

const int MaxN = 110;
int val[MaxN], w[MaxN];

int main()
{
	int n;
	std::scanf("%d", &n);
	for(int i = 0; i != n; ++i) 
		std::scanf("%d", val + i);
	for(int i = 0; i <= 30; ++i)
		w[i] = -1;
	std::sort(val, val + n);

	long long ans = 0;
	for(int i = n - 1; i >= 0; --i)
	{
		int v = val[i];
		for(int j = 30; j >= 0; --j)
		{
			if(val[i] & (1 << j))
			{
				if(w[j] == -1)
				{
					w[j] = val[i];
					break;
				} else {
					val[i] ^= w[j];
				}
			}
		}

		if(!val[i]) ans += v;
	}

	std::printf("%lld", ans ? ans : -1ll);
	return 0;
}
