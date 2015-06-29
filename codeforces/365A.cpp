/* Codeforces 365A. Good Number
 *   implementation */
#include <cstdio>

int main()
{
	int n, k, cnt = 0;
	std::scanf("%d %d", &n, &k);
	for(int i = 0; i != n; ++i)
	{
		int x, v = 0;
		std::scanf("%d", &x);
		while(x)
		{
			int digit = x % 10;
			if(digit <= k) v |= 1 << digit;
			x /= 10;
		}

		if(v + 1 == 2 << k) ++cnt;
	}
	std::printf("%d", cnt);
	return 0;
}
