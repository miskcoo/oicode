/* Codeforces 552C. Vanya and Scales
 *   brute force, dp, greedy, math, meet-in-the-middle */
#include <cstdio>

int val[40], used[101];

int main()
{
	int w, m, t = 0;
	std::scanf("%d %d", &w, &m);
	for(int x = m; x; x /= w, ++t)
		val[t] = x % w;
	for(int i = 0; i != t; ++i)
	{
		if(val[i] == w) ++val[i + 1], val[i] = 0;
		if(val[i] == 1 && used[i] || val[i] && val[i] != 1 && val[i] != w - 1)
		{
			std::puts("NO");
			return 0;
		} else if(w != 2 && val[i] == w - 1) {
			++val[i + 1];
			val[i] = 0;
			used[i] = 1;
		}
	}

	std::puts("YES");
	return 0;
}
