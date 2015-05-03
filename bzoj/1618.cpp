/* BZOJ-1618: [Usaco2008 Nov]Buying Hay 购买干草
 *   背包  */
#include <cstdio>
#include <cstring>
#include <algorithm>

const int MaxW = 55010;
unsigned f[MaxW];
int main()
{
	int n, h;
	std::scanf("%d %d", &n, &h);
	std::memset(f, 100, sizeof(f));
	f[0] = 0;
	for(int i = 0; i != n; ++i)
	{
		int p, c;
		std::scanf("%d %d", &p, &c);
		for(int w = p; w <= h + 5000; ++w)
			f[w] = std::min(f[w - p] + c, f[w]);
	}

	unsigned ans = ~0u;
	for(int i = h; i <= h + 5000; ++i)
		ans = std::min(ans, f[i]);
	std::printf("%u", ans);
	return 0;
}
