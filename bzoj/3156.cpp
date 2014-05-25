/*
 * BZOJ-3156 防御准备
 *  DP-斜率优化
 */
#include <cstdio>
#include <algorithm>

long long a[1000000];
long long f[1000000];
long long g[1000000];
int seq[1000000];  
    
inline double get_slope(long long i, long long j)  
{  
	return ((2 * f[i] + i * i) - (2 * f[j] + j * j))
		/ static_cast<double>(j - i);
}  
    
int main()
{
	int N;
	std::scanf("%d", &N);
	for(int i = 0; i != N; ++i)
		std::scanf("%lld", a + i);
	f[N - 1] = a[N - 1];
	g[N - 1] = ~0U >> 1;
	seq[0] = N - 1;
	int head = 0, tail = 0;
	for(int i = N - 2; i >= 0; --i)
	{
		g[i] = ~0LLU >> 1;
		f[i] = std::min(f[i + 1], g[i + 1]) + a[i];

		double right = 1 - 2 * i;  
		while(head < tail && get_slope(seq[head + 1], seq[head]) < right)  
			++head;  

		long long k = seq[head];  
		g[i] = f[k] + (k - i) * (k - i + 1) / 2;  

		while(head < tail && get_slope(seq[tail - 1], seq[tail]) 
			> get_slope(seq[tail], i)) --tail;  
		seq[++tail] = i;  
	}
	std::printf("%lld\n", std::min(g[0], f[0]));
	return 0;
}
