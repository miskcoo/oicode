/*
 * POJ-2234 Matches Game
 *  博弈论裸题,Nim游戏
 */
#include <cstdio>

int main()
{
	int n;
	while(std::scanf("%d", &n) != EOF)
	{
		int ans = 0;
		for(int i = 0; i != n; ++i)
		{
			int v;
			std::scanf("%d", &v);
			ans ^= v;
		}

		if(ans == 0) std::puts("No");
		else std::puts("Yes");
	}
	return 0;
}
