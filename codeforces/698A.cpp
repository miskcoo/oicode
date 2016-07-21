/* Codeforces 698A. Vacations
 *  dp */
#include <cstdio>
#include <algorithm>
#include <cstring>
#include <cstdlib>

int n, A[101], f[101][3];

int main()
{
	std::scanf("%d",  &n);
	for(int i = 1; i <= n; ++i)
		std::scanf("%d", A + i);

	for(int i = 1; i <= n; ++i)
	{
		f[i][1] = f[i][2] = ~0u >> 1;
		f[i][0] = std::min(std::min(f[i - 1][0], f[i - 1][1]), f[i - 1][2]) + 1;
		if(A[i] & 2) 
			f[i][2] = std::min(f[i - 1][0], f[i - 1][1]);

		if(A[i] & 1) 
			f[i][1] = std::min(f[i - 1][0], f[i - 1][2]);
	}

	int ans = std::min(f[n][0], f[n][1]);
	std::printf("%d\n", std::min(ans, f[n][2]));
	return 0;
}
