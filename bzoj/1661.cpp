/* BZOJ-1661: [Usaco2006 Nov]Big Square 巨大正方形 */
#include <cstdio>

const int MaxN = 1005;
int n, ans = 0, count = 0;
char data[MaxN][MaxN];

bool check(int x, int y)
{
	return x >= 0 && y >= 0 && x < n && y < n && data[x][y] == 'J';
}

int main()
{
	std::scanf("%d", &n);
	for(int i = 0; i != n; ++i)
		std::scanf("%s", data[i]);
	for(int x0 = 0; x0 != n; ++x0)
	{
		for(int y0 = 0; y0 != n; ++y0)
		{
			if(data[x0][y0] != 'J') continue;
			++count;
			for(int x1 = 0; x1 != n; ++x1)
			{
				for(int y1 = 0; y1 != n; ++y1)
				{
					if(data[x1][y1] == 'B') continue;
					int i = x1 - x0, j = y1 - y0;
					if(i * i + j * j < ans) continue;
					if(check(x0 + j, y0 - i) && check(x1 + j, y1 - i))
						ans = i * i + j * j;
					else if(check(x0 - j, y0 + i) && check(x1 - j, y1 + i))
						ans = i * i + j * j;
				}
			}
		}
	}

	if(count == n * n) ans = 0;
	std::printf("%d", ans);
	return 0;
}
