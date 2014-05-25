/* 
 * BZOJ-2705 [SDOI2012]Longge的问题
 *  数论-分解质因数，欧拉函数
 */
#include <cstdio>
#include <cmath>

int main()
{
	long long N;
	std::scanf("%lld", &N);
	long long t = N;
	long long sum = N;
	int sqrtn = (int)std::sqrt(double(N + 1));
	for(int i = 2; N != 1 && i <= sqrtn; ++i)
	{
		if(N % i == 0)
		{
			int p = i;
			int a = 0;
			while(N % p == 0)
			{
				N /= p;
				++a;
			}

			sum = sum / p * (p + a * p - a);
		}
	}

	if(N != 1) sum = sum / N * (2 * N - 1);
	std::printf("%lld", sum);
	return 0;
}
