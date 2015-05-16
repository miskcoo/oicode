/* BZOJ-3687: 简单题
 *   压位背包  */
#include <cstdio>
#include <bitset>

std::bitset<2000001> bit;

int main()
{
	int n, tot = 0;
	std::scanf("%d", &n);
	bit |= 1;
	for(int i = 0; i != n; ++i)
	{
		int v;
		std::scanf("%d", &v);
		bit ^= bit << v;
		tot += v;
	}

	int ans = 0;
	for(int i = 1; i <= tot; ++i)
		if(bit[i]) ans ^= i;
	std::printf("%d", ans);
	return 0;
}
