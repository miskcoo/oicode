/* Codeforces 621E. Wet Shark and Blocks
 *  dp, matrices, divide and conquer */
#include <cstdio>
#include <cstring>

const long long mod_v = 1000000007;
int n[10], rem[110], occ[110][110], b, k, x, loop, factor;

struct M {
	int x[110];
};

void mod_add(int& x, long long y)
{
	x = (x + y) % mod_v;
}

M operator * (const M& a, const M& b)
{
	M z;
	std::memset(z.x, 0, sizeof(z.x));
	for(int i = 0; i != x; ++i)
		for(int j = 0; j < x; ++j)
			mod_add(z.x[(i + factor * j) % x], 1ll * a.x[i] * b.x[j]);
	return z;
}

M conv(int p)
{
	M m0, mx;
	for(int i = 0; i != x; ++i)
	{
		mx.x[i] = occ[loop][i];
		m0.x[i] = 0;
	}

	m0.x[0] = 1;
	while(p)
	{
		if(p & 1) m0 = mx * m0;
		if(p >> 1) mx = mx * mx;
		p >>= 1;
	}

	return m0;
}

int main()
{
	int num;
	std::scanf("%d %d %d %d", &num, &b, &k, &x);
	for(int i = 0; i != num; ++i)
	{
		int x;
		std::scanf("%d", &x);
		++n[x];
	}

	occ[0][0] = 1;
	for(int i = 1, r = 1; i < x; ++i, r = r * 10 % x)
		for(int j = 0; j != x; ++j)
			for(int k = 0; k != 10; ++k)
				mod_add(occ[i][(r * k + j) % x], (long long)occ[i - 1][j] * n[k]);

	rem[0] = 1;
	int start = 0;
	for(int i = 1; i <= x; ++i)
	{
		rem[i] = rem[i - 1] * 10 % x;
		for(int j = 0; j != i; ++j)
		{
			if(rem[i] == rem[j])
			{
				start = j;
				factor = rem[i];
				loop = i - j;
				break;
			}
		}

		if(loop) break;
	}
	
	M m0, m1;
	for(int i = 0; i != x; ++i)
	{
		m0.x[i] = occ[start][i];
		m1.x[i] = occ[(b - start) % loop][i];
	}
	m0 = m0 * conv((b - start) / loop);
	m0 = m0 * m1;
	std::printf("%d\n", m0.x[k]);

	return 0;
}
