/* BZOJ-1619: [Usaco2008 Nov]Guarding the Farm 保卫牧场
 *   搜索  */
#include <cstdio>

const int MaxN = 1000, MaxM = 1000;
const int dir[][2] = {{-1, -1}, {-1, 0}, {-1, 1}, 
	{0, -1}, {0, 1}, {1, -1}, {1, 0}, {1, 1}};
int data[MaxN][MaxM], mark[MaxN][MaxM];
int n, m, now_val, now_max;

void dfs(int x, int y)
{
	if(mark[x][y] || data[x][y] != now_val)
	{
		if(data[x][y] != now_val && data[x][y] > now_max)
			now_max = data[x][y];
		return;
	}

	mark[x][y] = 1;
	for(int i = 0; i != 8; ++i)
	{
		int nx = x + dir[i][0];
		int ny = y + dir[i][1];
		if(nx < 0 || ny < 0 || nx >= n || ny >= m)
			continue;
		dfs(nx, ny);
	}
}

int main()
{
	std::scanf("%d %d", &n, &m);
	for(int i = 0; i != n; ++i)
	{
		for(int j = 0; j != m; ++j)
			std::scanf("%d", data[i] + j);
	}

	int count = 0;
	for(int i = 0; i != n; ++i)
	{
		for(int j = 0; j != m; ++j)
		{
			if(!mark[i][j])
			{
				now_max = 0;
				now_val = data[i][j];
				dfs(i, j);
				if(now_max < now_val)
					++count;
			}
		}
	}

	std::printf("%d", count);
	return 0;
}
