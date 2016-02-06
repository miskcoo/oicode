/* Codeforces 621B. Wet Shark and Bishops
 *  combinatorics */
#include <cstdio>
#include <iostream>

int A[10000], B[10000];

int main()
{
	int n;
	long long sum = 0;
	std::scanf("%d", &n);
	for(int i = 0; i != n; ++i)
	{
		int x, y;
		std::scanf("%d %d", &x, &y);

		sum += B[x - y + 5000] + A[x + y];
		++B[x - y + 5000]; ++A[x + y];
	}

	std::cout << sum << std::endl;
	return 0;
}
