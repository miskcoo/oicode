/* BZOJ-1621: [Usaco2008 Open]Roads Around The Farm分岔路口
 *   模拟 */
#include <cstdio>

int solve(int x, int k)
{
	if((x ^ k) & 1 || x <= k) return 1;
	return solve((x - k) >> 1, k) + solve((x + k) >> 1, k);
}

int main()
{
	int x, k;
	std::scanf("%d %d", &x, &k);
	std::printf("%d", solve(x, k));
	return 0;
}
