/*
 * 437A - The Child and Homework 
 */
#include <cstdio>
#include <cstring>

char str[120];
int main()
{
	int d[4];
	for(int i = 0; i != 4; ++i)
	{
		std::scanf("%s", str);
		d[i] = std::strlen(str) - 2;
	}

	bool flag1[4] = { true, true, true, true };
	bool flag2[4] = { true, true, true, true };
	int count = 0;
	int ans = 0;
	for(int i = 0; i != 4; ++i)
	{
		for(int j = 0; j != 4; ++j)
		{
			if(i == j) continue;
			if(d[i] * 2 > d[j])
				flag1[i] = false;
			if(d[j] * 2 > d[i])
				flag2[i] = false;
		}

		if(flag1[i] || flag2[i])
		{
			++count;
			ans = i;
		}
	}

	char map[4] = { 'A', 'B', 'C', 'D' };
	if(count == 1)
		std::printf("%c", map[ans]);
	else std::printf("C");

	return 0;
}
