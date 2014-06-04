/*
 * Codeforces 439A. Devu, the Singer and Churu, the Joker
 */
#include <cstdio>
#include <cstring>

int song[100];
int main()
{
	int n, d;
	std::scanf("%d %d", &n, &d);
	int sum = 0;
	for(int i = 0; i != n; ++i)
	{
		std::scanf("%d", song + i);
		sum += song[i];
	}
	if((n - 1) * 10 + sum > d)
	{
		std::printf("-1");
	} else {
		std::printf("%d", (d - sum) / 5);
	}
	return 0;
}
