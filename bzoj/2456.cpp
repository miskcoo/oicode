/* BZOJ-2456: mode */
#include <cstdio>

int main()
{
	int n, v, cnt = 0;
	std::scanf("%d", &n);
	for(int i = 0; i != n; ++i)
	{
		int z;
		std::scanf("%d", &z);
		if(z == v) ++cnt;
		else if(!cnt) v = z, cnt = 0;
		else --cnt;
	}

	std::printf("%d", v);
	return 0;
}
