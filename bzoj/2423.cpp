/*
 * BZOJ-2423 [HAOI2010]最长公共子序列
 *  DP
 */
#include <cstdio>
#include <cstring>

int main()
{
	const int mod_const = 100000000;
	char a[5001], b[5001];
	int f[5001], g[5001];
	std::memset(f, 0, sizeof(f));
	std::memset(g, 0, sizeof(g));
	std::scanf("%s", a);
	std::scanf("%s", b);
	for(int i = 0; a[i] != '.'; ++i)
	{
		int ans = 0;
		int count = 1;
		for(int j = 0; b[j] != '.'; ++j)
		{
			int pf = f[j];
			int pg = g[j];
			if(a[i] == b[j])
			{
				if(ans + 1 > f[j])
				{
					f[j] = ans + 1;
					g[j] = count;
				} else if(ans + 1 == f[j]) {
					g[j] = (g[j] + count) % mod_const;
				}
			}

			if(ans == pf)
				count = (count + pg) % mod_const;

			if(pf > ans)
			{
				ans = pf;
				count = pg;
			}
		}
	}

	int ans = 0;
	int count = 0;
	for(int i = 0; b[i] != '.'; ++i)
	{
		if(f[i] > ans)
		{
			ans = f[i];
			count = g[i];
		} else if(f[i] == ans) {
			count = (count + g[i]) % mod_const;
		}
	}

	std::printf("%d\n%d\n", ans, count);
	return 0;
}
