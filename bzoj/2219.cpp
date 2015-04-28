/* BZOJ-2219: 数论之神
 *   大步小步，原根，高次剩余 */
#include <cstdio>
#include <cmath>
#include <map>
#include <cstdlib>
#include <algorithm>

typedef long long value_t;

value_t power(value_t x, value_t p, value_t mod_v)
{
	value_t v = 1;
	while(p)
	{
		if(p & 1) v = x * v % mod_v;
		x = x * x % mod_v;
		p >>= 1;
	}

	return v;
}

value_t power(value_t x, value_t p)
{
	value_t v = 1;
	while(p)
	{
		if(p & 1) v = x * v;
		x = x * x;
		p >>= 1;
	}

	return v;
}

value_t gcd(value_t a, value_t b)
{
	return b ? gcd(b, a % b) : a;
}

struct fact_t
{
	int num;
	value_t val;
	value_t v[40], p[40];
};

fact_t get_fact(value_t x)
{
	int tot = 0;
	fact_t fact;
	fact.val = x;
	for(value_t i = 2; i * i <= x; ++i)
	{
		if(x % i == 0)
		{
			fact.v[tot] = i;
			fact.p[tot] = 0;
			while(x % i == 0)
				x /= i, ++fact.p[tot];
			++tot;
		}
	}

	if(x != 1) 
	{
		fact.v[tot] = x;
		fact.p[tot++] = 1;
	}

	fact.num = tot;
	return fact;
}

value_t euler_phi(fact_t* fact)
{
	value_t v = 1;
	for(int i = 0; i != fact->num; ++i)
		v *= power(fact->v[i], fact->p[i] - 1, fact->val) * (fact->v[i] - 1);
	return v;
}

value_t primitive_root(value_t phi, value_t p)
{
	fact_t f = get_fact(phi);

	for(value_t g = 2; ; ++g)
	{
		bool flag = true;
		for(int i = 0; i != f.num; ++i)
		{
			if(power(g, f.val / f.v[i], p) == 1)
			{
				flag = false;
				break;
			}
		}

		if(flag) return g;
	}
}

value_t baby_step_giant_step(value_t a, value_t b, value_t p)
{
	if(b == 1) return 0;
	std::map<int, int> hash;
	value_t sp = (value_t)sqrt(double(p)) + 1;
	value_t x = b;
	for(value_t i = 1; i <= sp; ++i)
	{
		x = x * a % p;
		value_t z = x % p;
		if(!hash[z]) hash[z] = i;
	}

	value_t z = 1, base = power(a, sp, p);
	for(int i = 1; i <= sp; ++i)
	{
		z = z * base % p;
		if(hash.count(z)) 
			return i * sp - hash[z];
	}

	return -1;
}

value_t solve(value_t A, value_t B, value_t K)
{
	K = K * 2 + 1;
	value_t ans = 1;
	fact_t fact = get_fact(K);
	for(int i = 0; i != fact.num; ++i)
	{
		value_t p0 = fact.v[i], a0 = fact.p[i];
		value_t p = power(p0, a0);
		value_t b0 = B % p;
		if(b0 == 0)
		{
			ans *= power(p0, a0 - (a0 - 1) / A - 1);
		} else {
			value_t cnt = 0;
			while(b0 % p0 == 0) 
				b0 /= p0, ++cnt;
			if(cnt % A != 0) return 0;

			fact_t fp;
			fp.num = 1, fp.val = p;
			fp.v[0] = p0, fp.p[0] = a0 - cnt;
			value_t phi = euler_phi(&fp);
			value_t g = primitive_root(phi, p);
			value_t ind = baby_step_giant_step(g, b0 % p, p);
			value_t d = gcd(phi, A);
			if(ind % d != 0) return 0;
			ans *= d * power(p0, cnt / A * (A - 1));
		}
	}
	
	return ans;
}

int main()
{
	int t;
	std::scanf("%d", &t);
	for(int i = 0; i != t; ++i)
	{
		value_t A, B, K;
		std::scanf("%lld %lld %lld", &A, &B, &K);
		std::printf("%lld\n", solve(A, B, K));
	}
	return 0;
}
