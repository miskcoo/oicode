/* BZOJ-1625: [Usaco2007 Dec]宝石手镯
 *   0-1背包 */
#include <cstdio>
#include <algorithm>

const int MaxW = 13000;
int f[MaxW];

int main()
{
	int n, m;
	std::scanf("%d %d", &n, &m);
	for(int i = 0; i != n; ++i)
	{
		int w, d;
		std::scanf("%d %d", &w, &d);
		for(int j = w; j <= m; ++j)
			f[j - w] = std::max(f[j - w], f[j] + d);
	}

	int ans = 0;
	for(int w = 0; w <= m; ++w)
		ans = std::max(ans, f[w]);
	std::printf("%d", ans);
	return 0;
}
