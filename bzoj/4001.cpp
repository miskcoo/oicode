/* BZOJ-4001: [TJOI2015]概率论
 *   生成函数 */
#include <cstdio>

int main()
{
	double n;
	std::scanf("%lf", &n);
	std::printf("%.9lf", n * (n + 1.0) / (2.0 * (2.0 * n - 1)));
	return 0;
}
