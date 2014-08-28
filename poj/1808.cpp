/* POJ-1808 Quadratic Residues
 *  计算 Legendre 符号的值 */
#include <cstdio>

int legendre(int a, int p)
{
	if(a <= 1)
		return 1;
	if(a == 2)
		return (p % 8 == 1 || p % 8 == 7) ? 1 : -1;

	int t = 0;
	while(a % 2 == 0)
		++t, a /= 2;
	if(t % 2)
		return legendre(2, p) * legendre(a, p);

	if(a % 4 == 1 || p % 4 == 1)
		return legendre(p % a, a);
	return -legendre(p % a, a);
}

int main()
{
	int N;
	std::scanf("%d", &N);
	for(int i = 1; i <= N; ++i)
	{
		int a, p;
		std::scanf("%d %d", &a, &p);
		a %= p;
		if(a < 0) a += p;
		std::printf("Scenario #%d:\n%d\n\n", i, legendre(a, p));
	}
	return 0;
}
