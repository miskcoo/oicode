/* Codeforces 365B. The Fibonacci Segment
 *   implementation */
#include <cstdio>

int main()
{
	int n;
	std::scanf("%d", &n);

	int ans = 1, now = 1;
	int p1, p2 = -1000000001;
	std::scanf("%d", &p1);
	for(int i = 1; i != n; ++i)
	{
		int v;
		std::scanf("%d", &v);
		if(v == p1 + p2 || now == 1)
		{
			++now;
		} else {
			now = 2;
		}

		p2 = p1, p1 = v;
		if(ans < now) ans = now;
	}

	std::printf("%d", ans);
	return 0;
}
