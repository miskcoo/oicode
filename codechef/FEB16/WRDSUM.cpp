#include <cstdio>
#include <algorithm>
#include <cmath>
#include <cstring>

typedef int val_t;
typedef long long calc_t;
const val_t mod_v = 998244353;
const int MaxN = 7000, MaxL = 1000, MaxR = 6000;
const int bit = 7, limit = 10000000;
char input_str[MaxN];

inline int cmp(int a, int b)
{
	if(a == b) return 0;
	return a < b ? -1 : 1;
}

struct num_t
{
	int n;
	val_t x[MaxL];

	friend inline int cmp(const num_t& A, const num_t& B)
	{
		if(A.n != B.n) return cmp(A.n, B.n);
		for(int i = A.n - 1; i >= 0; --i)
			if(A.x[i] != B.x[i])
				return cmp(A.x[i], B.x[i]);
		return 0;
	}

	int get_bit() const
	{
		int b = (n - 1) * bit;
		if(x[n - 1] < 10) return b + 1;
		else if(x[n - 1] < 100) return b + 2;
		else if(x[n - 1] < 1000) return b + 3;
		else if(x[n - 1] < 10000) return b + 4;
		else if(x[n - 1] < 100000) return b + 5;
		else if(x[n - 1] < 1000000) return b + 6;
		return b + 7;
	}

	val_t normalize() const
	{
		calc_t v = 0;
		for(int i = n - 1; i >= 0; --i)
			v = (v * limit + x[i]) % mod_v;
		return v;
	}

	long double get_long() const
	{
		long double v = 0;
		for(int i = n - 1; i >= 0; --i)
			v = v * limit + x[i];
		return v;
	}

	long double get_long2() const
	{
		long double v = 0;
		for(int i = n - 1; i >= 0; --i)
			v = v * limit + x[i] + 2;
		return v;
	}

	void parse(const char* str)
	{
		int len = std::strlen(str);
		n = (len + bit - 1) / bit;
		for(int i = 0, z = len / bit; i != z; ++i)
		{
			int v = 0;
			for(int j = 0; j != bit; ++j)
				v = v * 10 + (str[len - bit + j] - '0');
			len -= bit;
			x[i] = v;
		}

		if(len)
		{
			int v = 0;
			for(int i = 0; i != len; ++i)
				v = v * 10 + (str[i] - '0');
			x[n - 1] = v;
		}
	}
} one, record[MaxR];


val_t fpow(calc_t x, calc_t p)
{
	calc_t v = 1;
	for(; p; p >>= 1, x = x * x % mod_v)
		if(p & 1) v = x * v % mod_v;
	return v;
}

class sieve_t
{
	int pnum;
	char not_prime[MaxN];
	int prime[MaxN];
	val_t miu[MaxN], fac[MaxN];
public:
	sieve_t() : pnum(0) { 
		std::memset(not_prime, 0, sizeof(not_prime)); 
	}

	void init(int n)
	{
		pnum = 0;
		miu[1] = 1;
		for(int i = 2; i <= n; ++i)
		{
			if(!not_prime[i]) {
				miu[i] = -1;
				fac[i] = i;
				prime[pnum++] = i;
			}

			for(int j = 0; j != pnum; ++j)
			{
				int t = prime[j] * i;
				if(t > n) break;
				not_prime[t] = 1;
				fac[t] = prime[j];
				if(i % prime[j] == 0) {
					miu[t] = 0;
					break; 
				} else {
					miu[t] = -miu[i];
				}
			}
		}
	}

	val_t operator() (val_t n) const {
		return miu[n];
	}

	val_t get_factor(val_t n) const {
		return fac[n];
	}
} miu;

void multiply(num_t& A, const num_t& B)
{
	calc_t T[MaxN];
	int size = A.n + B.n + 1;
	std::fill(T, T + size, 0);
	for(int i = 0; i != A.n; ++i)
		for(int j = 0; j != B.n; ++j)
			T[i + j] += (calc_t)A.x[i] * B.x[j];

	for(int i = 0; i != size; ++i)
	{
		if(T[i] >= limit)
		{
			T[i + 1] += T[i] / limit;
			T[i] %= limit;
		}
	}

	while(size - 1 && !T[size - 1]) --size;
	std::copy(T, T + size, A.x);
	A.n = size;
}

void get_sum(num_t& S, const num_t& A, const num_t& B)
{
	int len_a = A.n, len_b = B.n;
	S.n = std::min(A.n, B.n);
	int carry = 0;

	auto make_carry = [&carry, &S](int i) {
		if(S.x[i] >= limit) 
		{
			S.x[i] -= limit;
			carry = 1;
		} else carry = 0;
	};

	for(int i = 0; i != S.n; ++i) {
		S.x[i] = A.x[i] + B.x[i] + carry;
		make_carry(i);
	}

	for(int& i = S.n; i < len_a; ++i) {
		S.x[i] = A.x[i] + carry;
		make_carry(i);
	}

	for(int& i = S.n; i < len_b; ++i) {
		S.x[i] = B.x[i] + carry;
		make_carry(i);
	}

	if(carry) S.x[S.n++] = carry;
}

void get_sub(num_t& S, const num_t& A, const num_t& B)
{
	int len = A.n;
	S.n = B.n;
	int carry = 0;

	for(int i = 0; i != S.n; ++i) {
		S.x[i] = A.x[i] - B.x[i] - carry;
		if(S.x[i] < 0) 
		{
			carry = 1;
			S.x[i] += limit;
		} else carry = 0;
	}

	for(int& i = S.n; i < len; ++i) {
		S.x[i] = A.x[i] - carry;
		if(S.x[i] < 0) 
		{
			carry = 1;
			S.x[i] += limit;
		} else carry = 0;
	}
}

void get_mean(num_t& M, const num_t& L, const num_t& R)
{
	get_sum(M, L, R);
	bool divide = 0;
	for(int i = M.n - 1; i >= 0; --i)
	{
		if(divide) M.x[i] += limit;
		divide = M.x[i] & 1;
		M.x[i] >>= 1;
	}

	while(M.n - 1 && !M.x[M.n - 1]) --M.n;
}

void get_pow(num_t& X, int p)
{
	num_t V = one;
	while(p)
	{
		if(p & 1) multiply(V, X);
		if(p >> 1) multiply(X, X);
		p >>= 1;
	}

	X = V;
}

void get_root(num_t& L, num_t& R, const num_t& X, int p)
{
	while(cmp(L, R) < 0)
	{
		num_t M, P;
		get_mean(M, L, R);
		get_pow(P = M, p);
		if(cmp(P, X) <= 0) 
			get_sum(L, M, one);
		else R = M;
	}
}

int get_p(int x)
{
	if(x == 0) return limit / 10;
	return fpow(10, x - 1);
}

void get_root(num_t& D, const num_t& X, int p)
{
	num_t& L = D, R;

	int factor = miu.get_factor(p);
	if(factor != p)
	{
		L = record[p / factor];
		get_root(L, record[p / factor], factor);

		get_sub(R, L, one);
		get_pow(R, p);
		if(cmp(R, X) > 0) get_sub(L, L, one);
	} else {
		int bit_r = (X.get_bit() + p - 1) / p + 1;
		R.n = (bit_r + bit - 1) / bit;
		std::fill(R.x, R.x + R.n, 0);
		R.x[R.n - 1] = get_p(bit_r % bit);
		if(cmp(D, R) < 0) R = D;

		int bit_l = X.get_bit() / p;
		L.n = (bit_l + bit - 1) / bit;
		if(L.n != 0)
		{
			std::fill(L.x, L.x + L.n, 0);
			L.x[L.n - 1] = get_p(bit_l % bit);
		} else L = one;

		num_t L2;
		long double apl = std::pow(X.get_long(), 1.0 / p);
		int high;
		L2.n = 0;
		while(apl > 1.0)
		{
			L2.x[L2.n++] = 0;
			high = apl;
			apl /= limit;
		}

		if(L2.n && high > 1) L2.x[L2.n - 1] = high - 1;
		else L2 = one;
		if(cmp(L2, L) > 0) L = L2;

		get_root(L, R, X, p);
	}
}

int get_root2(const num_t& n, int L, int p)
{
	// R < log(n) / log(p)
	int R = (n.n * bit + 1) / log10(p) + 1;
	num_t Z, P;
	Z.n = 1, Z.x[0] = p;
	while(L < R)
	{
		int M = (L + R) >> 1;
		get_pow(P = Z, M);
		if(cmp(P, n) <= 0) 
			L = M + 1;
		else R = M;
	}

	return L;
}

class root_t
{
	int max_index;
	calc_t root[MaxN];
	num_t num, tmp;
public:
	void init(calc_t n)
	{
		root[0] = 0; root[1] = n % mod_v;
		max_index = log2(n + 1.0e-8) + 1;
		for(int i = 2; i <= max_index; ++i)
			root[i] = (calc_t)pow(n + 1.0e-8, 1.0 / i) % mod_v; 
	}

	void init(const char* str)
	{
		num.parse(str);

		if(num.get_long() < 1.0e18)
		{
			calc_t n = 0;
			for(int i = num.n - 1; i >= 0; --i)
				n = n * limit + num.x[i];
			init(n);
			return;
		}

		root[0] = 0, root[1] = num.normalize();
		int &i = max_index;

		for(i = 2, tmp = num; tmp.n != 1 || tmp.x[0] != 2; ++i)
		{
			get_root(tmp, num, i);
			record[i] = tmp;
			root[i] = (tmp.normalize() - 1 + mod_v) % mod_v;
			if(root[i] == root[i - 1]) break;
		}

		do {
			int p = root[i];
			int r = get_root2(num, i, p);
			while(i != r) root[i++] = p;
			root[i] = p - 1;
		} while(root[i] > 1);   
	}

	val_t operator() (val_t n) const {
		return root[n];
	}

	int len() const {
		return max_index;
	}
} root;

class pow_sum_t
{
	calc_t f[MaxN], inv[MaxN];
	calc_t L[MaxN], R[MaxN], T[MaxN];
public:
	void init(int n) {
		inv[1] = 1;
		for(int i = 2; i <= n; ++i)
			inv[i] = -(mod_v / i) * inv[mod_v % i] % mod_v;
	}

	val_t operator() (calc_t r, calc_t d)
	{
		if(r == 1) return 0;

		calc_t ans = 0;
		if(d <= 5 || 3 * d + d * (log(d) + 1) < r * (log(d) + 1)) 
		{
			int k = d + 1;
			T[0] = L[0] = R[0] = 1; f[0] = 0;
			for(int i = 1; i <= k + 1; ++i)
			{
				f[i] = (f[i - 1] + fpow(i, d)) % mod_v;
				T[i] = T[i - 1] * inv[i] % mod_v;
				L[i] = L[i - 1] * (r - k + i - 1) % mod_v;
				R[i] = R[i - 1] * (r - i + 1) % mod_v;
			}

			for(int i = 1; i <= k; ++i)
			{
				calc_t c = L[k - i] * R[i] % mod_v
						 * T[i] % mod_v * T[k - i] % mod_v;
				if((i ^ k) & 1) ans = (ans - c * f[i]) % mod_v;
				else ans = (ans + c * f[i]) % mod_v;
			}

			ans = (ans - 1) % mod_v;
		} else {
			for(int i = 2; i <= r; ++i)
				ans = (ans + fpow(i, d)) % mod_v;
		}

		return ans;
	}
} psum;

int solve()
{
	std::scanf("%s", input_str);
	root.init(input_str);

	calc_t sum = 0;
	for(int d = 1; d < root.len(); ++d)
	{
//		printf("(%d, %d)\n", d, root(d));
		if(!miu(d)) continue;

		calc_t s = 0;
		for(int g = 1; d * g < root.len(); ++g)
			s = (s + psum(root(d * g), d)) % mod_v;

		sum = (sum + miu(d) * s) % mod_v;
	}

	return (sum + mod_v) % mod_v;
}

int main()
{
	one.n = one.x[0] = 1;
	miu.init(MaxN - 1);
	psum.init(MaxN - 1);

	int t;
	std::scanf("%d", &t);
	while(t --> 0)
		std::printf("%d\n", solve());
	return 0;
}
