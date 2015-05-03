/* BZOJ-1606: [Usaco2008 Dec]Hay For Sale 购买干草
 *   背包+压位  */
#include <cstdio>
#include <bitset>

std::bitset<50001> f;

int main()
{
	f.set(0);
	int c, h;
	std::scanf("%d %d", &c, &h);
	for(int i = 0; i != h; ++i)
	{
		int v;
		std::scanf("%d", &v);
		f |= f << v;
	}

	for(int i = c; i >= 0; ++i)
	{
		if(f[i]) 
		{
			std::printf("%d", i);
			break;
		}
	}
	return 0;
}
