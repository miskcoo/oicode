#include <algorithm>
#include <cstring>
#include <cstdio>

char str[100010];

int main()
{
	std::scanf("%s", str);
	std::printf("%s", str);
	int len = std::strlen(str);
	for(int i = len - 1; i >= 0; --i)
		std::putchar(str[i]);
	return 0;
}
