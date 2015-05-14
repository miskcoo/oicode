/* BZOJ-1072: [SCOI2007]排列perm
 *   状压DP */
#include <cstdio>
#include <algorithm>
#include <cstring>

int n, d, cnt[11], fac[11];
char str[11];
int f[1 << 10][1001];

int main()
{
	int t;
	std::scanf("%d", &t);
	fac[0] = 1;
	for(int i = 1; i <= 10; ++i)
		fac[i] = fac[i - 1] * i;
	while(t --> 0)
	{
		std::scanf("%s %d", str, &d);
		n = std::strlen(str);
		for(int s = 0; s != 1 << n; ++s)
			std::memset(f[s], 0, sizeof(int) * d);
		f[0][0] = 1;
		for(int s = 0; s != 1 << n; ++s)
		{
			for(int i = 0; i != n; ++i)
			{
				if(!(s & (1 << i)))
				{
					for(int j = 0; j != d; ++j)
						f[s | (1 << i)][(j * 10 + (str[i] - '0')) % d] += f[s][j];
				}
			}
		}

		std::memset(cnt, 0, sizeof(cnt));
		for(int i = 0; i != n; ++i)
			++cnt[str[i] - '0'];
		int ans = f[(1 << n) - 1][0];
		for(int i = 0; i != 10; ++i)
			ans /= fac[cnt[i]];
		std::printf("%d\n", ans);
	}
	return 0;
}
