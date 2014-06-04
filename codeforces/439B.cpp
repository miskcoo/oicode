/*
 * Codeforces 439B. Devu, the Dumb Guy
 */
#include <cstdio>
#include <algorithm>

long long data[100000];
int main()
{
	long long n, d;
	std::scanf("%I64d %I64d", &n, &d);
	for(int i = 0; i != n; ++i)
		std::scanf("%I64d", data + i);
	std::sort(data, data + n);
	long long ans = 0;
	for(int i = 0; i != n; ++i)
	{
		ans += data[i] * d;
		if(d != 1) --d;
	}
	std::printf("%I64d", ans);
	return 0;
}
