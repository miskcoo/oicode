/* BZOJ-2554: Color
 *   概率+DP  */
#include <cstdio>
#include <cstring>

const int MaxN = 10010;
double f[MaxN];
int cnt[26];
char str[MaxN];

int main()
{
	std::scanf("%s", str);
	int n = std::strlen(str);

	f[1] = (n - 1.0) * (n - 1.0) / n;
	for(int i = 1; i != n; ++i)
		f[i + 1] = 2.0 * f[i] - f[i - 1] - (n - 1.0) / (n - i);
	for(int i = 0; i != n; ++i)
		++cnt[str[i] - 'A'];
	double ans = 0.0;
	for(int i = 0; i != 26; ++i)
		ans += f[cnt[i]];
	std::printf("%.1lf\n", ans + 1.0e-8);
	return 0;
}
