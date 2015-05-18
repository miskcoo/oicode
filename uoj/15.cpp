/* UOJ-#15. 【NOIP2014】生活大爆炸版石头剪刀布 */
#include <cstdio>

const int score_table[5][5] = 
	{ { 2, 0, 1, 1, 0 },
	  { 1, 2, 0, 1, 0 },
	  { 0, 1, 2, 0, 1 },
	  { 0, 0, 1, 2, 1 },
	  { 1, 1, 0, 0, 2 } };

int A[1000], B[1000];

int main()
{
	int n, na, nb;
	std::scanf("%d %d %d", &n, &na, &nb);
	for(int i = 0; i != na; ++i)
		std::scanf("%d", A + i);
	for(int i = 0; i != nb; ++i)
		std::scanf("%d", B + i);
	
	int pa = 0, pb = 0;
	int sa = 0, sb = 0;
	for(int i = 0; i != n; ++i)
	{
		int v = score_table[A[pa]][B[pb]];
		if(v == 1) ++sa;
		else if(v == 0) ++sb;
		++pa, ++pb;
		if(pa == na) pa = 0;
		if(pb == nb) pb = 0;
	}
	
	std::printf("%d %d", sa, sb);
	return 0;
}
