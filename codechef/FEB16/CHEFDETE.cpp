#include <cstdio>

const int MaxN = 100001;
bool mark[MaxN];

int main()
{
	int n;
	std::scanf("%d", &n);
	for(int i = 0; i != n; ++i)
	{
		int fa;
		std::scanf("%d", &fa);
		mark[fa] = true;
	}

	for(int i = 1; i <= n; ++i)
		if(!mark[i]) std::printf("%d ", i);
	return 0;
}
