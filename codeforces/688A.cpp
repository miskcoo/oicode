#include <cstdio>
#include <algorithm>
#include <cstring>
#include <cstdlib>

char A[110];
int x[100];

int main()
{
	int n, m;
	std::scanf("%d %d", &n, &m);
	int now = 0, pre = 0;
	for(int i = 0; i != m; ++i)
	{
		std::scanf("%s", A);
		int cnt = std::count(A, A + std::strlen(A), '0');
		if(cnt) ++now;
		else { pre = std::max(pre, now); now = 0; }
	}

	std::printf("%d\n", std::max(pre, now));
	return 0;
}
