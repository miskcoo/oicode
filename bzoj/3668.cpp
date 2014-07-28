/*
 * BZOJ 3668: [Noi2014]起床困难综合症
 */
#include <cstdio>

int N, M;
char bits[2][31];
char ans[31];
char op[5];

int resolv_to_int(char* bit)
{
	int v = 0;
	for(int p = 30; p >= 0; --p)
		v = v * 2 + bit[p];
	return v;
}

int main()
{
	for(int i = 0; i != 31; ++i)
	{
		bits[0][i] = 0;
		bits[1][i] = 1;
	}

	std::scanf("%d %d", &N, &M);
	for(int i = 0; i != N; ++i)
	{
		int v;
		std::scanf("%s %d", op, &v);
		switch(op[0])
		{
		case 'A':
			for(int i = 0; i != 31; ++i)
			{
				bits[0][i] &= (v >> i) & 1;
				bits[1][i] &= (v >> i) & 1;
			}
			break;
		case 'O':
			for(int i = 0; i != 31; ++i)
			{
				bits[0][i] |= (v >> i) & 1;
				bits[1][i] |= (v >> i) & 1;
			}
			break;
		case 'X':
			for(int i = 0; i != 31; ++i)
			{
				bits[0][i] ^= (v >> i) & 1;
				bits[1][i] ^= (v >> i) & 1;
			}
			break;
		}
	}

	// find first 1 from high bits to low bits
	int first1 = 30;
	for(; first1 >= 0; --first1)
	{
		if(bits[0][first1] == 0 && bits[1][first1] == 0)
			continue;
		if(bits[0][first1]) continue;
		if((1 << first1) <= M)
			break;
	}

	if(first1 == -1)
	{
		std::printf("%d", resolv_to_int(bits[0]));
	} else {
		ans[first1] = 1;
		int cur = 1 << first1;
		for(; first1 >= 0; --first1)
		{
			if(bits[0][first1] == 0 && bits[1][first1] == 0)
				continue;
			if(bits[0][first1]) continue;
			if((cur | (1 << first1)) <= M)
			{
				cur |= 1 << first1;
				ans[first1] = 1;
			}
		}
		for(int i = 0; i != 31; ++i)
			ans[i] = bits[ans[i]][i];
		std::printf("%d\n", resolv_to_int(ans));
	}
	return 0;
}
