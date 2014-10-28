/* BZOJ-3709: [PA2014]Bohater
 *  贪心 */
#include <cstdio>
#include <algorithm>

struct monster_t
{
	int d, a, id;
} monsters[100000];
int index[100000];

bool compare(const monster_t& i, const monster_t& j)
{
	bool get1 = i.a - i.d > 0;
	bool get2 = j.a - j.d > 0;
	if(get1 ^ get2) return get1;
	if(get1) return i.d < j.d;
	return i.a > j.a;
}

bool solve(int n, long long z)
{
	for(int i = 0; i != n; ++i)
	{
		if(z <= monsters[i].d)
			return false;
		z += monsters[i].a - monsters[i].d;
		index[i] = monsters[i].id + 1;
	}

	return true;
}

int main()
{
	int n, z;
	std::scanf("%d %d", &n, &z);
	for(int i = 0; i != n; ++i)
	{
		std::scanf("%d %d", &monsters[i].d, &monsters[i].a);
		monsters[i].id = i;
	}

	std::sort(monsters, monsters + n, compare);
	if(solve(n, z))
	{
		std::printf("TAK\n");
		for(int i = 0; i != n; ++i)
			std::printf("%d ", index[i]);
	} else std::printf("NIE");
	return 0;
}

