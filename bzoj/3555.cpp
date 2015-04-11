/* BZOJ-3555: [Ctsc2014]企鹅QQ
 *  哈希 */
#include <cstring>
#include <cstdio>
#include <algorithm>

typedef unsigned long long value_t;
const int MaxN = 30010, MaxL = 210;
char str[MaxL];
value_t left[MaxN][MaxL], right[MaxN][MaxL], value[MaxN];

int solve(int n, int p)
{
	for(int i = 0; i != n; ++i)
		value[i] = left[i][p - 1] * 233 + right[i][p + 1] * 431;

	std::sort(value, value + n);
	int count = 1, ans = 0;
	value_t prev = value[0];
	for(int i = 1; i != n; ++i)
	{
		if(value[i] != prev)
		{
			prev = value[i];
			ans += count * (count - 1) / 2;
			count = 1;
		} else ++count;
	}

	return ans + count * (count - 1) / 2;
}

int main()
{
	int n, l, s;
	std::scanf("%d %d %d", &n, &l, &s);
	for(int i = 0; i != n; ++i)
	{
		std::scanf("%s", str);
		for(int j = 1; j <= l; ++j)
			left[i][j] = left[i][j - 1] * 97 + str[j - 1];
		for(int j = l; j; --j)
			right[i][j] = right[i][j + 1] * 131 + str[j - 1];
	}

	int ans = 0;
	for(int i = 1; i <= l; ++i)
		ans += solve(n, i);
	std::printf("%d", ans);
	return 0;
}
