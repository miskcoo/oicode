/* BZOJ-1441: Min
 *  裴蜀定理 */
#include <cstdio>

int gcd(int a, int b)
{
	return b ? gcd(b, a % b) : a;
}

int main()
{
	int n, ans;
	std::scanf("%d", &n);
	std::scanf("%d", &ans);
	for(int i = 1; i != n; ++i)
	{
		int v;
		std::scanf("%d", &v);
		ans = gcd(ans, v);
	}

	if(ans < 0) ans = -ans;
	std::printf("%d", ans);
	return 0;
}
