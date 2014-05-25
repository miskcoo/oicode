/*
 * BZOJ-1486 最小圈
 *  负权环+分数规划
 */
#include <cstdio>
#include <cstring>

template<int VSize, int ESize>
class graph
{
	int total;
	int vertex_number;
	int head[VSize + 1];
	int next[ESize + 1];
	int point[ESize + 1];
	double weight[ESize + 1];
public:
	void init(int vnum)
	{
		total = 0;
		vertex_number = vnum;
		std::memset(head, 0, sizeof(head));
		std::memset(mark, 0, sizeof(mark));
	}

	void add_edge(int u, int v, double w)
	{
		point[++total] = v;
		weight[total] = w;
		next[total] = head[u];
		head[u] = total;
	}
private:
	double dist[VSize + 1];
	int mark[VSize + 1];
	int mark_value;
public:
	bool has_neg_circle_check(int u, double diff)
	{
		mark[u] = mark_value;
		for(int k = head[u]; k; k = next[k])
		{
			int v = point[k];
			double w = weight[k] - diff;
			double dist_v = dist[u] + w;
			if(mark[v] == mark_value && dist_v < dist[v])
				return true;
			if(mark[v] == mark_value)
				continue;
			if(dist_v < 0.0)
			{
				dist[v] = dist_v;
				if(has_neg_circle_check(v, diff))
					return true;
			}
		}
		// 一定要清理
		mark[u] = 0;
		return false;
	}

	bool has_neg_circle(double diff)
	{
		for(int i = 1; i <= vertex_number; ++i)
		{
			++mark_value;
			dist[i] = 0.0;
			if(has_neg_circle_check(i, diff))
				return true;
		}
		return false;
	}
};

graph<3000, 10000> gp;

int main()
{
	std::freopen("circle.in", "r", stdin);
	std::freopen("circle.out", "w", stdout);
	int N, M;
	std::scanf("%d %d", &N, &M);
	gp.init(N);
	for(int i = 0; i != M; ++i)
	{
		int u, v;
		double w;
		std::scanf("%d %d %lf", &u, &v, &w);
		gp.add_edge(u, v, w);
	}

	double epsilon = 1.0e-9;
	double r = 1.0e8, l = -1.0e8;
	while(r - l > epsilon)
	{
		double m = (l + r) * 0.5;
		if(gp.has_neg_circle(m))
			r = m;
		else l = m;
	}
	std::printf("%.8f\n", l);
	return 0;
}
