#include <cstdio>
#include <bitset>

int C[500], cnt[501];
std::bitset<510> f[501];

int main()
{
	int n, k, sum = 0;
	std::scanf("%d %d", &n, &k);
	f[0][0] = 1;
	for(int i = 0; i != n; ++i)
	{
		int c;
		std::scanf("%d", &c);
		sum += c;
		for(int j = k; j - c >= 0; --j)
		{
			f[j] |= f[j - c];
			f[j] |= f[j - c] << c;
		}
	}

	int cnt = 0;
	for(int i = 0; i <= 500; ++i)
		cnt += f[k][i];
	std::printf("%d\n", cnt);
	for(int i = 0; i <= 500; ++i)
		if(f[k][i]) std::printf("%d ", i);
	return 0;
}
