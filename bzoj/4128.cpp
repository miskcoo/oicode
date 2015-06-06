/* BZOJ-4128: Matrix
 *   矩阵大步小步 */
#include <cstdio>
#include <cstring>
#include <cmath>

const int hash_mod = 7340033;
const int MaxN = 70, MaxSqrtP = 150;
int matrix_size, mod_v;
struct matrix_t
{
	int m[MaxN][MaxN];

	matrix_t operator * (const matrix_t& r) const
	{
		matrix_t z;
		for(int i = 0; i != matrix_size; ++i)
		{
			for(int j = 0; j != matrix_size; ++j)
			{
				long long v = 0;
				for(int k = 0; k != matrix_size; ++k)
					v += m[i][k] * r.m[k][j];
				z.m[i][j] = v % mod_v;
			}
		}

		return z;
	}

	bool operator == (const matrix_t& r) const
	{
		for(int i = 0; i != matrix_size; ++i)
		{
			for(int j = 0; j != matrix_size; ++j)
			{
				if(m[i][j] != r.m[i][j]) 
					return false;
			}
		}

		return true;
	}

	int get_hash() const
	{
		unsigned long long hash = 0;
		for(int i = 0; i != matrix_size; ++i)
		{
			for(int j = 0; j != matrix_size; ++j)
				hash = hash * 1004535809 + m[i][j];
		}

		return hash % hash_mod;
	}
} A, B, one;

struct hash_t
{
	int total;
	int head[hash_mod], next[MaxSqrtP << 1], value[MaxSqrtP << 1];
	matrix_t key[MaxSqrtP << 1];

	int& operator [] (const matrix_t& x)
	{
		int pos = x.get_hash();
		for(int k = head[pos]; k; k = next[k])
			if(key[k] == x) return value[k];
		next[++total] = head[pos];
		head[pos] = total;
		value[total] = -1;
		key[total] = x;
		return value[total];
	}
} hash;

matrix_t matrix_pow(matrix_t x, int p)
{
	matrix_t v = one;
	while(p)
	{
		if(p & 1) v = x * v;
		if(p >> 1) x = x * x;
		p >>= 1;
	}
	return v;
}

int baby_step_giant_step(int p)
{
	if(B == one) return 0;

	int m = int(sqrt(1.0 * p) + 1);
	matrix_t base = B;
	for(int i = 0; i != m; ++i)
	{
		hash[base] = i;
		base = base * A;
	}

	base = matrix_pow(A, m);
	matrix_t now = one;
	for(int i = 1; i <= m + 1; ++i)
	{
		now = now * base;
		if(hash[now] >= 0)
			return i * m - hash[now];
	}

	return -1;
}

int main()
{
	std::scanf("%d %d", &matrix_size, &mod_v);
	std::memset(one.m, 0, sizeof(one.m));
	for(int i = 0; i != matrix_size; ++i)
		one.m[i][i] = 1;
	for(int i = 0; i != matrix_size; ++i)
		for(int j = 0; j != matrix_size; ++j)
			std::scanf("%d", A.m[i] + j);
	for(int i = 0; i != matrix_size; ++i)
		for(int j = 0; j != matrix_size; ++j)
			std::scanf("%d", B.m[i] + j);
	std::printf("%d\n", baby_step_giant_step(mod_v));
	return 0;
}
