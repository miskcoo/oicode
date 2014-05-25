/* 
 * BZOJ-2227 [ZJOI2011]看电影(movie)
 *  高精度+神证明 
 */
#include <cstdio>

template<int Size>
class big_number
{
	int len;
	static const long long limit = 1000000000;
	long long value[Size];
public:
	big_number()
	{
		len = 1;
		value[0] = 1;
	}

	void mul_number(long long v)
	{
		long long carry = 0;
		for(int i = 0; i != len; ++i)
		{
			value[i] = value[i] * v + carry;
			if(value[i] >= limit)
			{
				carry = value[i] / limit;
				value[i] %= limit;
			} else carry = 0;
		}

		while(carry != 0)
		{
			value[len++] = carry % limit;
			carry /= limit;
		}
	}

	void print()
	{
		std::printf("%lld", value[len - 1]);
		for(int i = len - 2; i >= 0; --i)
			std::printf("%09lld", value[i]);
	}
};

inline int gcd(int a, int b)
{
	return b ? gcd(b, a % b) : a;
}

int main()
{
	int T;
	std::scanf("%d", &T);
	while(T--)
	{
		int N, K;
		std::scanf("%d %d", &N, &K);
		if(N > K)
		{
			std::printf("0 1\n");
			continue;
		}

		int mc = K - N + 1;
		int value[200];
		for(int i = 0; i != N; ++i)
		{
			int g = gcd(K, mc);
			value[i] = K / g;
			mc /= g;
		}

		big_number<1000> up;
		for(int i = 1; i != N; ++i)
			up.mul_number(K + 1);
		up.mul_number(mc);
		up.print();
		std::printf(" ");

		big_number<1000> down;
		for(int i = 0; i != N; ++i)
			down.mul_number(value[i]);
		down.print();
		std::printf("\n");
	}
	return 0;
}
