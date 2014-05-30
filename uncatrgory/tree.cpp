/*
 * 给定一棵树，每个点有权
 * 求出任意两点间路径权值与、或、异或和期望值
 * 保留三位小数
 *  N <= 100000, Wi <= 2^20
 *  多组数据 T <= 5
 */
#include <cstdio>
#include <cstring>

template<int VSize, int ESize>
class graph
{
	static const int max_bit = 21;
	int total;
	int deg[VSize + 1];
	int point[ESize + 1];
	int weight[VSize + 1];
	int head[VSize + 1];
	int next[ESize + 1];

	long long xor_v[max_bit];
	long long and_v[max_bit];
	long long or_v[max_bit];
	long long xor_value[VSize + 1][max_bit][2];
	long long and_value[VSize + 1][max_bit][2];
	long long or_value[VSize + 1][max_bit][2];
public:
	void init()
	{
		total = 0;
		std::memset(xor_v, 0, sizeof(xor_v));
		std::memset(xor_value, 0, sizeof(xor_value));
		std::memset(or_v, 0, sizeof(or_v));
		std::memset(or_value, 0, sizeof(or_value));
		std::memset(and_v, 0, sizeof(and_v));
		std::memset(and_value, 0, sizeof(and_value));
		std::memset(deg, 0, sizeof(deg));
		std::memset(head, 0, sizeof(head));
	}
	void add_edge(int u, int v)
	{
		++deg[v];
		point[++total] = v;
		next[total] = head[u];
		head[u] = total;
	}
	void read_weight(int N)
	{
		for(int i = 1; i <= N; ++i)
			std::scanf("%d", weight + i);
	}
public:
	void tsort(int N)
	{
		int qhead = 0, qtail = 0;
		int seq[VSize + 1];
		int mark[VSize + 1];
		std::memset(mark, 0, sizeof(mark));
		for(int i = 1; i <= N; ++i)
		{
			if(deg[i] == 1)
			{
				seq[qtail++] = i;
				--deg[i];
			}
		}

		while(qhead != qtail)
		{
			for(int k = head[seq[qhead++]]; k; k = next[k])
			{
				int v = point[k];
				if(--deg[v] == 1)
					seq[qtail++] = v;
			}
		}

		for(int i = 0; i != qtail; ++i)
		{
			int u = seq[i];
			mark[u] = 1;
			for(int b = 0; b != max_bit; ++b)
			{
				xor_value[u][b][(weight[u] >> b) & 1] += 1;
				or_value[u][b][(weight[u] >> b) & 1] += 1;
				and_value[u][b][(weight[u] >> b) & 1] += 1;
			}

			for(int k = head[u]; k; k = next[k])
			{
				int v = point[k];
				if(mark[v]) continue;
				int w = weight[v];
				for(int b = 0; b != max_bit; ++b)
				{
					int p = (w >> b) & 1;
					xor_value[v][b][1] += xor_value[u][b][p ^ 1];
					xor_value[v][b][0] += xor_value[u][b][p];
					or_value[v][b][1] += or_value[u][b][1];
					or_value[v][b][p] += or_value[u][b][0];
					and_value[v][b][p] += and_value[u][b][1];
					and_value[v][b][0] += and_value[u][b][0];
				}
			}
		}

		for(int i = qtail - 1; i >= 0; --i)
		{
			int u = seq[i];
			mark[u] = 2;
			int w = weight[u];
			for(int b = 0; b != max_bit; ++b)
			{
				int p = (w >> b) & 1;
				if(p == 1) 
				{
					xor_v[b] += xor_value[u][b][0] * xor_value[u][b][0] 
						+ xor_value[u][b][1] * xor_value[u][b][1];
				} else { 
					xor_v[b] += 2 * xor_value[u][b][0] * xor_value[u][b][1];
				}
				and_v[b] += and_value[u][b][1] * and_value[u][b][1];
				or_v[b] += or_value[u][b][1] * (
					or_value[u][b][1] + or_value[u][b][0] * 2);
			}

			for(int k = head[u]; k; k = next[k])
			{
				int v = point[k];
				if(mark[v] == 2)
					continue;

				for(int b = 0; b != max_bit; ++b)
				{
					int p = (w >> b) & 1;
					int q = (weight[v] >> b) & 1;
					if(p) 
					{
						xor_v[b] -= xor_value[v][b][0] * xor_value[v][b][0] 
							+ xor_value[v][b][1] * xor_value[v][b][1];
					} else {
						xor_v[b] -= 2 * xor_value[v][b][0] 
							* xor_value[v][b][1];
					}

					if(p != 0)
					{
						and_v[b] -= and_value[v][b][1] * and_value[v][b][1];
					}
					or_v[b] -= or_value[v][b][1] * (
						or_value[v][b][1] + or_value[v][b][0] * 2);
					if(p) or_v[b] -= or_value[v][b][0] * or_value[v][b][0];
				}
			}
		}
	}

	void print(int N)
	{
		long long vxor = 0;
		long long vand = 0;
		long long vor = 0;
		for(int i = max_bit - 1; i >= 0; --i)
		{
			vxor = 2 * vxor + xor_v[i];
			vand = 2 * vand + and_v[i];
			vor = 2 * vor + or_v[i];
		}

		double d = double(N) * N;
		std::printf("%.3lf %.3lf %.3lf", vand / d, vor / d, vxor / d);
	}
};

graph<100000, 200000> gp;

int main()
{
	int T;
	std::scanf("%d", &T);
	while(T--)
	{
		gp.init();
		int N;
		std::scanf("%d", &N);
		gp.read_weight(N);
		for(int i = 1; i != N; ++i)
		{
			int u, v;
			std::scanf("%d %d", &u, &v);
			gp.add_edge(u, v);
			gp.add_edge(v, u);
		}

		gp.tsort(N);
		gp.print(N);
		std::printf("\n");
	}
	return 0;
}
