/*
 * POJ-2046 Power Strings
 *  KMP
 */ 
#include <cstdio>
#include <cstring>

char str[1000001];
int next[1000001];

int main()
{
	while(std::scanf("%s", str))
	{
		if(str[0] == '.')
			break;
		int len = std::strlen(str);

		next[0] = -1;
		int i = 1, j = 0;
		while(i != len)
		{
			if(j == -1 || str[i] == str[j])
				next[i++] = j++;
			else j = next[j];
		}

		int l = len - (next[len - 1] + 1);
		if(len % l) std::printf("1\n");
		else std::printf("%d\n", len / l);
	}
	return 0;
}
