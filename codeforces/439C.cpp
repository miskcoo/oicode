/*
 * Codeforces 439C. Devu and Partitioning of the Array
 */
#include <cstdio>
#include <cstring>

int odd[100000];
int even[100000];
int main()
{
	int n, k, p;
	std::scanf("%d %d %d", &n, &k, &p);
	int odd_n = 0, even_n = 0;
	for(int i = 0; i != n; ++i)
	{
		int v;
		std::scanf("%d", &v);
		if(v & 1) odd[odd_n++] = v;
		else even[even_n++] = v;
	}

	if(p <= even_n)
	{
		int g = k - p;
		if(g <= odd_n && (odd_n - g) % 2 == 0)
		{
			std::puts("YES");
			if(k == p)
			{
				for(int i = 0; i < p - 1; ++i)
					std::printf("1 %d\n", even[i]);
				std::printf("%d ", odd_n + even_n - (p - 1));
				for(int i = p - 1; i != even_n; ++i)
					std::printf("%d ", even[i]);
				for(int i = 0; i != odd_n; ++i)
					std::printf("%d ", odd[i]);
			} else {
				for(int i = 0; i != p; ++i)
					std::printf("1 %d\n", even[i]);
				int cc = odd_n - g;
				std::printf("%d ", cc + even_n - p + 1);
				for(int i = 0; i != cc; ++i)
					std::printf("%d ", odd[i]);
				for(int i = p; i != even_n; ++i)
					std::printf("%d ", even[i]);
				std::printf("%d\n", odd[cc]);
				for(int i = cc + 1; i < odd_n; ++i)
					std::printf("1 %d\n", odd[i]);
			}
		} else std::puts("NO");
	} else {
		int oe = p - even_n;
		int g = odd_n - oe * 2 - (k - p);
		if(g >= 0 && g % 2 == 0)
		{
			std::puts("YES");
			for(int i = 0; i != even_n; ++i)
				std::printf("1 %d\n", even[i]);
			int od = 0;
			if(k == p)
			{
				for(int i = even_n; i < p - 1; ++i)
				{
					std::printf("2 %d %d\n", odd[od], odd[od + 1]);
					od += 2;
				}

				std::printf("%d ", odd_n - od);
				while(od != odd_n)
					std::printf("%d ", odd[od++]);
			} else {
				for(int i = even_n; i != p; ++i)
				{
					std::printf("2 %d %d\n", odd[od], odd[od + 1]);
					od += 2;
				}

				int r = odd_n - od - (k - p);
				if(r)
				{
					std::printf("%d ", r + 1);
					for(int i = 0; i <= r; ++i)
						std::printf("%d ", odd[od++]);
					std::printf("\n");
					while(od != odd_n)
						std::printf("1 %d\n", odd[od++]);
				}
			}
		} else std::puts("NO");
	}
	return 0;
}
