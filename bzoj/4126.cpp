/* BZOJ-4126: 国王奇遇记加强版之再加强版 */
#include <cstdio>

const int MaxM = 500010;
const long long mod_v = 1000000007;

int n, m, ptot;
long long P[MaxM], A[MaxM], B[MaxM], L[MaxM], R[MaxM];
long long inv[MaxM], fac[MaxM], inv_fac[MaxM], pw[MaxM];
int prime[MaxM], not_prime[MaxM];

long long pow(long long x, long long p)
{
	long long v = 1;
	for(; p; p >>= 1, x = x * x % mod_v)
		if(p & 1) v = x * v % mod_v;
	return v;
}

void linear_sieve(int n)
{
	pw[1] = 1;
	for(int i = 2; i <= n; ++i)
	{
		if(!not_prime[i])
		{
			prime[ptot++] = i;
			pw[i] = pow(i, m);
		}

		for(int j = 0; j != ptot; ++j)
		{
			int t = prime[j] * i;
			if(t > n) break;
			not_prime[t] = 1;
			pw[t] = pw[i] * pw[prime[j]] % mod_v;
			if(i % prime[j] == 0) 
				break;
		}
	}
}

long long comb(int r, int k)
{
	return fac[r] * inv_fac[k] % mod_v * inv_fac[r - k] % mod_v;
}

void prework()
{
	inv[1] = 1;
	for(int i = 2; i <= m + 1; ++i)
		inv[i] = mod_v - mod_v / i * inv[mod_v % i] % mod_v;

	fac[0] = inv_fac[0] = 1;
	for(int i = 1; i <= m + 1; ++i)
	{
		fac[i] = fac[i - 1] * i % mod_v;
		inv_fac[i] = inv_fac[i - 1] * inv[i] % mod_v;
	}

	A[0] = 1, B[0] = 0;
	for(int i = 0; i <= m; ++i)
	{
		A[i + 1] = A[i] * inv[m] % mod_v;
		B[i + 1] = (B[i] * inv[m] + pw[i + 1]) % mod_v;
	}

	// R*P_m(0) + K = 0
	long long R = 0, K = 0;
	for(int i = 0; i <= m + 1; ++i)
	{
		long long coeffi = comb(m + 1, i) % mod_v;
		if(i & 1) coeffi = -coeffi;
		R = (R + coeffi * A[i]) % mod_v;
		K = (K + coeffi * B[i]) % mod_v;
	}

	P[0] = -K * pow(R, mod_v - 2) % mod_v;

	for(int i = 1; i <= m; ++i)
		P[i] = (A[i] * P[0] + B[i]) % mod_v;
}

long long solve()
{
	L[0] = n - m, R[0] = n;
	for(int i = 1; i <= m; ++i)
	{
		R[i] = R[i - 1] * (n - i) % mod_v;
		L[i] = L[i - 1] * (n - m + i) % mod_v;
	}

	long long p = 0;
	for(int i = 0; i <= m; ++i)
	{
		long long coeffi = inv_fac[i] * inv_fac[m - i] % mod_v;
		if(i != m) coeffi = coeffi * L[m - i - 1] % mod_v;
		if(i) coeffi = coeffi * R[i - 1] % mod_v;
		if((m ^ i) & 1) coeffi = -coeffi;
		p = (p + coeffi * P[i]) % mod_v;
	}

	long long ans = (pow(m, n) * p - P[0]) % mod_v;
	return (ans + mod_v) % mod_v;
}

int main()
{
	long long ans;
	std::scanf("%d %d", &n, &m);
	linear_sieve(m + 1);
	if(m == 1) 
	{
		ans = n * (n + 1ll) % mod_v * pow(2, mod_v - 2) % mod_v;
	} else if(n <= m) {
		ans = 0;
		long long exp = m;
		for(int i = 1; i <= n; ++i, exp = exp * m % mod_v)
			ans = (ans + exp * pw[i]) % mod_v;
	} else {
		prework();
		ans = solve();
	}

	std::printf("%lld\n", ans);
	return 0;
}
