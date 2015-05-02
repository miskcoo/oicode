/* BZOJ-4031: [HEOI2015]小Z的房间
 *   生成树计数（Matrix-Tree 定理）*/
#include <cstdio>
#include <algorithm>
#include <cstring>

const int MaxN = 100;
const int mod_v = 1000000000;
long long eqn[MaxN][MaxN];
char str[MaxN][MaxN];
int id[MaxN][MaxN];

long long det(int n)
{
	int sign = 0;
	long long ans = 1;
	for(int i = 0; i != n; ++i)
	{
		if(!eqn[i][i])
		{
			int j;
			for(j = i + 1; j != n; ++j)
				if(eqn[j][i]) break;
			if(j == n) return 0;
			for(int k = 0; k != n; ++k)
				std::swap(eqn[i][k], eqn[j][k]);
			++sign;
		}

		for(int j = i + 1; j != n; ++j)
		{
			while(eqn[j][i])
			{
				long long t = eqn[j][i] / eqn[i][i];
				for(int k = i; k != n; ++k)
					eqn[j][k] = ((eqn[j][k] - t * eqn[i][k]) % mod_v + mod_v) % mod_v;
				if(!eqn[j][i]) break;
				++sign;
				for(int k = i; k != n; ++k)
					std::swap(eqn[i][k], eqn[j][k]);
			}
		}

		ans = ans * eqn[i][i] % mod_v;
	}

	if(ans < 0) ans += mod_v;
	if(sign & 1) return mod_v - ans;
	return ans;
}

void inc(int x, int y)
{
	eqn[x][y] -= 1;
	eqn[y][x] -= 1;
}

int main()
{
	int n, m;
	std::scanf("%d %d", &n, &m);
	for(int i = 0; i != n; ++i)
		std::scanf("%s", str[i]);
	int cnt = 0;
	for(int i = 0; i != n; ++i)
	{
		for(int j = 0; j != m; ++j)
		{
			if(str[i][j] != '*')
				id[i][j] = cnt++;
		}
	}

	for(int i = 0; i != n; ++i)
	{
		for(int j = 0; j != m; ++j)
		{
			if(str[i][j] == '*') continue;
			int deg = 0;
			if(i != 0 && str[i - 1][j] == '.')
				++deg;
			if(j != 0 && str[i][j - 1] == '.')
				++deg;
			if(i + 1 != n && str[i + 1][j] == '.')
				inc(id[i][j], id[i + 1][j]), ++deg;
			if(j + 1 != m && str[i][j + 1] == '.')
				inc(id[i][j], id[i][j + 1]), ++deg;
			eqn[id[i][j]][id[i][j]] = deg;
		}
	}

	for(int i = 0; i != cnt; ++i)
		for(int j = 0; j != cnt; ++j)
			if(eqn[i][j] < 0) eqn[i][j] += mod_v;

	std::printf("%lld", det(cnt - 1));
	return 0;
}
