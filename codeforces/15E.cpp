/* Codeforces 15E. Triangles
 *   组合计数
 *   combinatorics, dp */
#include <cstdio>

const long long mod_v = 1000000009;

int main()
{
	int n;
	std::scanf("%d", &n);
	long long pw = 2, g = 4, s = 0;
	while(n -= 2)
	{
		pw = (pw << 1) % mod_v;
		g = g * (pw - 3) % mod_v;
		s = (s + g) % mod_v;
	}

	int ans = (2 * s * s + 8 * s + 10) % mod_v;
	if(ans < 0) ans += mod_v;
	std::printf("%d\n", ans);
	return 0;
}
