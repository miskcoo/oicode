/* BZOJ-4063: [Cerc2012]Darts */
#include <cstdio>
#include <cmath>

int main()
{
	int t;
	std::scanf("%d", &t);
	while(t --> 0)
	{
		int n;
		std::scanf("%d", &n);
		int ans = 0;
		for(int i = 0; i != n; ++i)
		{
			int x, y;
			std::scanf("%d %d", &x, &y);
			if(x == 0 && y == 0)
			{
				ans += 10;
			} else {
				double r = sqrt(x * x + y * y);
				if(r > 200.0) continue;
				ans += 11 - int(r + 20.0 - 1.0e-5) / 20;
			}
		}

		std::printf("%d\n", ans);
	}
	return 0;
}
