/* BZOJ-3713: [PA2014]Iloczyn */
#include <cstdio>

long long f[50];

bool check(int v)
{
	if(v == 0) return true;
	for(int i = 1; f[i] <= v; ++i)
	{
		if(v / f[i] * f[i] == v)
		{
			int u = v / f[i];
			for(int j = 1; f[j] <= u; ++j)
				if(f[j] == u) return true;
		}
	}

	return false;
}

int main()
{
	f[0] = f[1] = 1;
	for(int i = 2; i != 45; ++i)
		f[i] = f[i - 1] + f[i - 2];

	int t;
	std::scanf("%d", &t);
	while(t --> 0)
	{
		int v;
		std::scanf("%d", &v);
		std::puts(check(v) ? "TAK" : "NIE");
	}

	return 0;
}
