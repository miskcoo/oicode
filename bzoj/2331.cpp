/*
 * BZOJ-2331 地板
 *  插头DP
 */
#include <cstdio>
#include <cstring>

int dp[2][200000];
int *pre = dp[0];
int *f = dp[1];
int power[12];
char map[10][101];
const int mod_const = 20110520;

void solve(int N, int M)
{
	int memset_size = sizeof(int) * power[N + 1];
	pre[0] = 1;
	for(int j = 0; j != M; ++j)
	{
		for(int i = 0; i != power[N]; ++i)
			f[i * 3] = pre[i];
		int* te = f;
		f = pre;
		pre = te;
		std::memset(f, 0, memset_size);
		for(int i = 0; i != N; ++i)
		{
			for(int s = 0; s != power[N + 1]; ++s)
			{
				if(pre[s] == 0) continue;
				int sa = s / power[i] % 3;
				int sb = s / power[i + 1] % 3;

				int rz = s - sa * power[i]
						- sb * power[i + 1];
				int pz = power[i];
				pre[s] %= mod_const;
				if(map[i][j] == '*')
				{
					if(sa == 0 && sb == 0)
						f[s] = pre[s];
				} else if(sa == 0) {
					if(sb == 0)
					{
						// (0, 0)
						f[rz + 8 * pz] += pre[s];
						f[rz + 3 * pz] += pre[s];
						f[rz + pz] += pre[s];
					} else if(sb == 1) {
						// (0, 1)
						f[rz + 6 * pz] += pre[s];
						f[rz + pz] += pre[s];
					} else if(sb == 2) {
						// (0, 2)
						f[rz + 2 * pz] += pre[s];
						f[rz] += pre[s];
					}
				} else if(sa == 1) {
					if(sb == 0)
					{
						// (1, 0)
						f[rz + 3 * pz] += pre[s];
						f[rz + 2 * pz] += pre[s];
					} else if(sb == 1) {
						// (1, 1)
						f[rz] += pre[s];
					}
				} else if(sa == 2) {
					if(sb == 0)
					{
						// (2, 0)
						f[rz + 6 * pz] += pre[s];
						f[rz] += pre[s];
					}
				}
			}
			int* t = f;
			f = pre;
			pre = t;
			std::memset(f, 0, memset_size);
		}
	}
}

int main()
{
	power[0] = 1;
	for(int i = 1; i != 12; ++i)
		power[i] = power[i - 1] * 3;
	int N, M;
	std::scanf("%d %d", &N, &M);
	for(int i = 0; i != N; ++i)
	{
		char c[101];
		if(N < M)
		{
			std::scanf("%s", map[i]);
		} else {
			std::scanf("%s", c);
			for(int j = 0; j != M; ++j)
				map[j][i] = c[j];
		}
	}
	if(N < M) solve(N, M);
	else solve(M, N);
	std::printf("%d\n", pre[0] % mod_const);
	return 0;
}
