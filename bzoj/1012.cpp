/* BZOJ-1012: [JSOI2008]最大数maxnumber
 *   树状数组 */
#include <cstdio>

const int MaxN = 200010;
int size, ta[MaxN];

void modify(int x, int v)
{
	for(; x; x -= x & -x)
		if(v > ta[x]) ta[x] = v;
}

int ask(int x)
{
	int v = 0;
	for(; x <= size; x += x & -x)
		if(v < ta[x]) v = ta[x];
	return v;
}

int main()
{
	int mod_v, len = 0, lastans = 0;
	std::scanf("%d %d", &size, &mod_v);
	for(int i = 0; i != size; ++i)
	{
		char op[2];
		int v;
		std::scanf("%s %d", op, &v);
		if(*op == 'A')
		{
			v = (v + lastans) % mod_v;
			modify(++len, v);
		} else {
			lastans = ask(len - v + 1);
			std::printf("%d\n", lastans);
		}
	}
	return 0;
}
