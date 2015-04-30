/* BZOJ-3028: 食物
 *   生成函数  */
#include <cstdio>

const int mod_v = 10007, mod_inv = 1668;
char str[1000];

int main()
{
	std::scanf("%s", str);
	int x = 0;
	for(int i = 0; str[i]; ++i)
		x = (x * 10 + str[i] - '0') % mod_v;
	int ans = x * (x + 1) % mod_v * (x + 2) % mod_v * mod_inv % mod_v;
	std::printf("%d", ans);
	return 0;
}
