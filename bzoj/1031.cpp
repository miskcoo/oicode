/*
 * BZOJ-1031 [JSOI2007]字符加密Cipher
 *  后缀数组
 */
#include <cstdio>
#include <cstring>
#include <algorithm>

int temp[2][200001];
int wa[200001], wb[200001];
char str[200001];
int sa[200001];
void suffix_array(char* str, int* sa, 
	int*& rank, int len, int alpha_size)
{
	int* kx = temp[0], *ky = temp[1];
	int m = alpha_size;
	for(int i = 0; i != m; ++i)
		wa[i] = 0;
	for(int i = 0; i != len; ++i)
		++wa[kx[i] = str[i]];
	for(int i = 1; i != m; ++i)
		wa[i] += wa[i - 1];
	for(int i = len - 1; i >= 0; --i)
		sa[--wa[kx[i]]] = i;

	for(int p = 0, j = 1; p != len; j <<= 1, m = p)
	{
		p = 0;
		for(int i = len - j; i != len; ++i)
			ky[p++] = i;
		for(int i = 0; i != len; ++i)
			if(sa[i] >= j) ky[p++] = sa[i] - j;

		for(int i = 0; i != len; ++i)
			wb[i] = kx[ky[i]];
		for(int i = 0; i != m; ++i)
			wa[i] = 0;
		for(int i = 0; i != len; ++i)
			++wa[wb[i]];
		for(int i = 1; i != m; ++i)
			wa[i] += wa[i - 1];
		for(int i = len - 1; i >= 0; --i)
			sa[--wa[wb[i]]] = ky[i];

		std::swap(kx, ky);
		kx[sa[0]] = 0;
		p = 1;
		for(int i = 1; i != len; ++i)
		{
			int a = sa[i], b = sa[i - 1];
			int na = a + j, nb = b + j;
			if(ky[a] == ky[b] && na < len && nb < len && ky[na] == ky[nb])
				kx[a] = p - 1;
			else kx[a] = p++;
		}
	}

	rank = kx;
}

int main()
{
	std::scanf("%s", str);
	int len = std::strlen(str);
	for(int i = 0; i != len; ++i)
		str[i + len] = str[i];
	str[len <<= 1] = 0;
	int* rank;
	suffix_array(str, sa, rank, len, 127);
	for(int i = 0; i != len; ++i)
	{
		if(sa[i] < (len >> 1))
			std::putchar(str[sa[i] + (len >> 1) - 1]);
	}
	return 0;
}
