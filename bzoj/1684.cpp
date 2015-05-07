/* BZOJ-1684: [Usaco2005 Oct]Close Encounter */
#include <cstdio>

int main()
{
	int n, d, a, b;
	std::scanf("%d %d", &n, &d);
	double v = double(n) / double(d);
	double diff = 1.0e100;
	for(int i = 1; i <= 32767; ++i)
	{
		if(i % d == 0) continue;
		int x = int(v * i + 0.5);
		double d = double(x) / double(i) - v;
		if(d < 0) d = -d;
		if(d < diff)
		{
			a = x, b = i;
			diff = d;
		}
	}

	std::printf("%d %d", a, b);
	return 0;
}
