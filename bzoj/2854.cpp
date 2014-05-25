/* 
 * BZOJ-2854 civilization
 *  高斯消元+中国剩余定理
 */
#include <cstdio>
#include <cstring>
#include <algorithm>

int N;
char right[200][200];
long long matrix[200][200];
long long conv_value(char* str, long long mod_v)
{
	long long v = 0;
	while(*str)
		v = (v * 10 + (*str++ - '0')) % mod_v;
	return v;
}

long long power(long long a, long long b, long long p)
{
	long long v = 1;
	while(b)
	{
		if(b & 1) v = v * a % p;
		a = a * a % p;
		b >>= 1;
	}
	return v;
}

void exgcd(long long a, long long b, 
	long long& x, long long & y)
{
	if(b == 0)
	{
		x = 1, y = 0;
		if(a < 0) x = -1;
		return;
	}

	exgcd(b, a % b, x, y);
	long long t = x;
	x = y;
	y = t - a / b * y;
}

void gauss(long long p, long long* ans)
{
	int index[200];
	long long f[200];
	long long m[200][200];
	std::memcpy(m, matrix, sizeof(matrix));
	for(int i = 0; i != N; ++i)
	{
		index[i] = i;
		f[i] = conv_value(right[i], p);
	}

	for(int i = 0; i != N; ++i)
	{
		int pos = i;
		for(; pos != N; ++pos)
		{
			if(m[index[pos]][i] != 0)
				break;
		}

		if(pos == N) continue;
		std::swap(index[i], index[pos]);
		for(int j = i + 1; j < N; ++j)
		{
			int u = index[j];
			if(m[u][i] == 0) continue;
			f[u] = (f[u] * m[i][i] - f[i] * m[u][i]) % p;
			for(int k = N - 1; k >= i; --k)
				m[u][k] = (m[u][k] * m[i][i] - m[i][k] * m[u][i]) % p;
		}
	}

	for(int i = N - 1; i >= 0; --i)
	{
		for(int j = i + 1; j < N; ++j)
			f[i] = (f[i] - m[index[i]][j] * ans[j]) % p;
		ans[i] = f[i] * power(m[index[i]][i], p - 2, p) % p;
	}
}

int main()
{
	std::scanf("%d", &N);
	for(int i = 0; i != N; ++i)
	{
		for(int j = 0; j != N; ++j)
			std::scanf("%lld", matrix[i] + j);
		std::scanf("%s", right[i]);
	}

	long long p1 = 1000000007;
	long long p2 = 1000000009;
	long long ans1[200], ans2[200];
	gauss(p1, ans1);
	gauss(p2, ans2);

	for(int i = 0; i != N; ++i)
	{
		long long k, r;
		long long a = ans1[i];
		long long b = ans2[i];
		exgcd(p1, p2, k, r);
		k = (k % p2 + p2) % p2;
		long long t = (k * (b - a) % p2 + p2) % p2;
		std::printf("%lld\n", a + t * p1);
	}
	return 0;
}
