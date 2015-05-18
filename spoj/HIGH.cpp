/* SPOJ-HIGH Highways
 *   Matrix-Tree 定理 */
#include <cstdio>
#include <algorithm>
#include <cstring>

const int MaxN = 12;
long long eqn[MaxN][MaxN];

long long det(int n)
{
	long long ans = 1;
	for(int i = 0; i != n; ++i)
	{
		for(int j = i; j != n; ++j)
		{
			if(eqn[j][i])
			{
				for(int k = i; k != n; ++k)
					std::swap(eqn[j][k], eqn[i][k]);
				break;
			}
		}

		for(int j = i + 1; j != n; ++j)
		{
			while(eqn[j][i])
			{
				long long t = eqn[j][i] / eqn[i][i];
				for(int k = i; k != n; ++k)
					eqn[j][k] -= t * eqn[i][k];
				if(!eqn[j][i]) break;
				for(int k = i; k != n; ++k)
					std::swap(eqn[j][k], eqn[i][k]);
			}
		}

		ans *= eqn[i][i];
	}

	return ans < 0 ? -ans : ans;
}

int main()
{
	int t;
	std::scanf("%d", &t);
	while(t --> 0)
	{
		int n, m;
		std::scanf("%d %d", &n, &m);
		std::memset(eqn, 0, sizeof(eqn));
		for(int i = 0; i != m; ++i)
		{
			int u, v;
			std::scanf("%d %d", &u, &v);
			--u, --v;
			++eqn[u][v], ++eqn[v][u];
			--eqn[v][v], --eqn[u][u];
		}

		std::printf("%lld\n", det(n - 1));
	}
	return 0;
}
