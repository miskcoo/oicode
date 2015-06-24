/* Codeforces 486A. Calculating Function
 *   implementation, math */
#include <cstdio>

int main()
{
	long long n;
	std::scanf("%I64d", &n);
	if(n & 1) std::printf("%I64d", ((n - 1) >> 1) - n);
	else std::printf("%I64d", n >> 1);
	return 0;
}
