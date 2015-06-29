/* Codeforces 469C. 24 Game
 *   constructive algorithms, implementation */
#include <cstdio>

int main()
{
	int n;
	std::scanf("%d", &n);
	if(n <= 3) 
	{
		std::puts("NO");
	} else {
		std::puts("YES");

		if(n & 1)
		{
			for(int i = n; i > 5; i -= 2)
			{
				std::printf("%d - %d = 1\n", i, i - 1);
				std::puts("1 * 1 = 1");
			}

			std::puts("1 * 2 = 2");
			std::puts("3 + 5 = 8");
			std::puts("8 + 4 = 12");
			std::puts("2 * 12 = 24");
		} else {
			for(int i = n; i > 4; i -= 2)
			{
				std::printf("%d - %d = 1\n", i, i - 1);
				std::puts("1 * 1 = 1");
			}

			std::puts("1 * 2 = 2");
			std::puts("2 * 3 = 6");
			std::puts("4 * 6 = 24");
		}
	}
	return 0;
}
