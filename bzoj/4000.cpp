/* BZOJ-4000: [TJOI2015]棋盘
 *   状压DP+矩阵乘法 */
#include <cstdio>
#include <cstring>

const int MaxN = 1000010, MaxM = 1 << 6;
int n, m, p, k, mx, A[3];

struct matrix_t
{
	unsigned m[MaxM][MaxM];

	matrix_t operator * (const matrix_t& r) const
	{
		matrix_t z;
		for(int i = 0; i != mx; ++i)
		{
			for(int j = 0; j != mx; ++j)
			{
				unsigned v = 0;
				for(int k = 0; k != mx; ++k)
					v += m[i][k] * r.m[k][j];
				z.m[i][j] = v;
			}
		}

		return z;
	}
};

matrix_t matrix_pow(matrix_t x, int p)
{
	matrix_t z;
	std::memset(z.m, 0, sizeof(z.m));
	for(int i = 0; i != mx; ++i) z.m[i][i] = 1;
	while(p)
	{
		if(p & 1) z = x * z;
		x = x * x;
		p >>= 1;
	}
	return z;
}

int move(int a, int b, int v)
{
	if(b < 0) return (a >> (-b)) & v;
	return (a << b) & v;
}

int check(int a, int b)
{
	for(int i = 0; i != m; ++i)
	{
		if(((a >> i) & 1) && (move(A[2], i - k, b) || move(A[1], i - k, a)))
			return 0;
		if(((b >> i) & 1) && (move(A[0], i - k, a) || move(A[1], i - k, b)))
			return 0;
	}

	return 1;
}

int main()
{
	std::scanf("%d %d", &n, &m);
	std::scanf("%d %d", &p, &k);
	for(int i = 0; i != 3; ++i)
	{
		for(int j = 0; j != p; ++j)
		{
			int v;
			std::scanf("%d", &v);
			if(v) A[i] |= 1 << j;
		}
	}

	A[1] ^= 1 << k;

	mx = 1 << m;
	matrix_t m0;
	for(int i = 0; i != mx; ++i)
		for(int j = 0; j != mx; ++j)
			m0.m[i][j] = check(i, j);
	matrix_t m = matrix_pow(m0, n);
	unsigned ans = 0;
	for(int i = 0; i != mx; ++i)
		ans += m.m[i][0];
	std::printf("%u\n", ans);
	return 0;
}
