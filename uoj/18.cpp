/* UOJ-#18. 【NOIP2014】无线网络发射器选址 */
#include <cstdio>
#include <iostream>

long long network[200][200];

long long find_cover(int x, int y, int d)
{
	long long count = 0;
	for(int i = x - d; i <= x + d; ++i)
	{
		for(int j = y - d; j <= y + d; ++j)
		{
			if(i < 0 || j < 0 || i > 128 || j > 128)
				continue;
			count += network[i][j];
		}
	}
	return count;
}

int main()
{
	int d, n;
	std::cin >> d >> n;
	for(int i = 0; i != n; ++i)
	{
		int x, y;
		long long k;
		std::cin >> x >> y >> k;
		network[x][y] = k;
	}
	
	long long comb = 0, ans = 0;
	for(int i = 0; i <= 128; ++i)
	{
		for(int j = 0; j <= 128; ++j)
		{
			long long ret = find_cover(i, j, d);
			if(ret == ans)
			{
				++comb;
			} else if(ret > ans) {
				comb = 1;
				ans = ret;
			}
		}
	}
	
	std::cout << comb << " " << ans;
	return 0;
}


