#include <cstdio>

int main()
{
	int n, t;
	std::scanf("%d", &t);
	while(t --> 0)
	{
		std::scanf("%d", &n);

		if(n <= 6) 
		{
			std::puts("-1");
		} else if(n == 8) {
			std::printf("8\n1 2\n2 3\n3 4\n4 5\n5 6\n1 6\n2 7\n6 8\n1\n");
		} else {
			std::printf("%d\n", n);
			int r = (n - 1) / 2;
			for(int i = 1; i != n - 1; ++i)
				std::printf("%d %d\n", i, i + 1);
			std::printf("1 %d\n", n - 1);
			std::printf("1 %d\n", n);
			std::printf("%d\n", r - 1);
		}
	}

	return 0;
}
