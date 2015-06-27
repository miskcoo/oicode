/* Codeforces 555A. Case of Matryoshkas 
 *   implementation */
#include <cstdio>

int n, k;

int main()
{
	int ans = 0, cnt = 0;
	std::scanf("%d %d", &n, &k);
	for(int i = 0; i != k; ++i)
	{
		int m;
		std::scanf("%d", &m);
		int prev = -1;
		for(int j = 0; j != m; ++j)
		{
			int v;
			std::scanf("%d", &v);
			if(v == 1) prev = v;
			if(prev + 1 == v) prev = v;
		}

		if(prev != -1)
		{
			ans += m - prev;
			cnt += m - prev + 1;
		} else {
			ans += m - 1;
			cnt += m;
		}
	}

	ans += cnt - 1;
	std::printf("%d\n", ans);
	return 0;
}
