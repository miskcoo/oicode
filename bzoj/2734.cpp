/* BZOJ-2734: [HNOI2012]集合选数
 *   插头DP */
#include <cstdio>
#include <cstring>
#include <algorithm>

int n, mark_v;
int data[17][11];
int mark[100001];
int dp[2][4096];
int *f = dp[0], *g = dp[1];
const int mod_const = 1000000001;

void update(int& v, int t)
{
	v += t;
	if(v >= mod_const)
		v -= mod_const;
}

int solve_matrix(int row, int cell)
{
	std::memset(g, 0, sizeof(dp[0]));
	g[0] = 1;
	for(int i = 0; i != row; ++i)
	{
		for(int j = 0; j != cell; ++j)
		{
			std::memset(f, 0, sizeof(dp[0]));
			for(int s = 0; s != 1 << cell; ++s)
			{
				if(s & (1 << j))
					update(f[s ^ (1 << j)], g[s]);
				else update(f[s], g[s]);

				if(data[i][j] != mark_v)
					continue;

				if(j == 0)
				{
					if(!(s & 1)) 
						update(f[s | 1], g[s]);
				} else {
					if(!(s & (1 << j)) && !(s & (1 << (j - 1))))
						update(f[s | (1 << j)], g[s]);
				}
			}

			std::swap(f, g);
		}
	}

	int ans = 0;
	for(int i = 0; i != 1 << cell; ++i)
		update(ans, g[i]);
	return ans;
}

int make_matrix(int v)
{
	++mark_v;
	int row = 0, cell = 0;
	for(int i = v; i <= n; i *= 2, ++row)
	{
		int c = 0;
		for(int j = i; j <= n; j *= 3, ++c)
		{
			mark[j] = 1;
			data[row][c] = mark_v;
		}

		cell = std::max(cell, c);
	}

	return solve_matrix(row, cell);
}

int main()
{
	long long ans = 1;
	std::scanf("%d", &n);
	for(int i = 1; i <= n; ++i)
	{
		if(mark[i]) continue;
		ans = ans * make_matrix(i) % mod_const;
	}

	std::printf("%lld", ans);
	return 0;
}
