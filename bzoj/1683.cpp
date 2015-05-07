/* BZOJ-1683: [Usaco2005 Nov]City skyline 城市地平线
 *   单调栈  */
#include <cstdio>

const int MaxN = 50010;
int st[MaxN];

int main()
{
	int n, ans = 0, stop = 0;
	std::scanf("%d %*d", &n);
	for(int i = 0; i != n; ++i)
	{
		int h;
		std::scanf("%*d %d", &h);
		while(stop && st[stop - 1] > h) 
			--stop, ++ans;
		if((!stop || st[stop - 1] != h) && h)
			st[stop++] = h;
	}

	ans += stop;
	std::printf("%d", ans);
	return 0;
}
