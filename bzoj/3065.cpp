/* BZOJ-3065: 带插入区间K小值
 *  替罪羊树套权值线段树 */
#include <cstdio>
#include <algorithm>

const int MaxN = 70010, MaxV = 70000;

struct seg_t
{
	int w;
	seg_t *l, *r;
};

seg_t *seg_nil;

void destroy(seg_t* now)
{
	if(now == seg_nil) return;
	destroy(now->l);
	destroy(now->r);
	delete now;
}

seg_t* insert(seg_t* n, int l, int r, int pos, int v)
{
	if(n->w + v == 0) return seg_nil;
	if(n == seg_nil)
	{
		n = new seg_t;
		n->l = n->r = seg_nil;
		n->w = v;
	} else n->w += v;
	if(l == r) return n;

	int m = (l + r) >> 1;
	if(pos <= m) n->l = insert(n->l, l, m, pos, v);
	else n->r = insert(n->r, m + 1, r, pos, v);
	return n;
}

struct scap_t
{
	int size, val;
	scap_t *l, *r;
	seg_t *seg;
};

scap_t *scap_nil, *root, *rebuild_node, *rebuild_fa;
double alpha;
int record_num, seg_num;
int value[MaxN], record[MaxN];
seg_t* seg_record[MaxN];

void destroy(scap_t* now)
{
	if(now == scap_nil) return;
	destroy(now->seg);
	destroy(now->l);
	record[++record_num] = now->val;
	destroy(now->r);
	delete now;
}

scap_t* scap_build(int l, int r)
{
	if(l > r) return scap_nil;

	scap_t* n = new scap_t;
	if(l == r) 
	{
		n->size = 1;
		n->val = record[l];
		n->l = n->r = scap_nil;
		n->seg = insert(seg_nil, 0, MaxV, record[l], 1);
		return n;
	}

	int m = (l + r) >> 1;
	n->val = record[m];
	n->l = scap_build(l, m - 1);
	n->r = scap_build(m + 1, r);
	n->size = n->l->size + n->r->size + 1;
	n->seg = seg_nil;
	for(int i = l; i <= r; ++i)
		n->seg = insert(n->seg, 0, MaxV, record[i], 1);
	return n;
}

scap_t* scap_rebuild(scap_t* now)
{
	record_num = 0;
	destroy(now);
	return scap_build(1, record_num);
}

int scap_modify(scap_t* now, int pos, int v)
{
	int old_val = 0;
	int sz = now->l->size;
	if(sz + 1 == pos)
	{
		old_val = now->val;
		now->val = v;
	} else if(pos <= sz) {
		old_val = scap_modify(now->l, pos, v);
	} else {
		old_val = scap_modify(now->r, pos - sz - 1, v);
	}

	now->seg = insert(now->seg, 0, MaxV, old_val, -1);
	now->seg = insert(now->seg, 0, MaxV, v, 1);
	return old_val;
}

scap_t* scap_insert(scap_t* now, int pos, int v)
{
	if(now == scap_nil)
	{
		scap_t *n = new scap_t;
		n->val = v, n->size = 1;
		n->l = n->r = scap_nil;
		n->seg = insert(seg_nil, 0, MaxV, v, 1);
		return n;
	}

	now->seg = insert(now->seg, 0, MaxV, v, 1);
	int sz = now->l->size;
	if(pos <= sz) now->l = scap_insert(now->l, pos, v);
	else now->r = scap_insert(now->r, pos - sz - 1, v);
	now->size = now->l->size + now->r->size + 1;
	if(now->size * alpha < std::max(now->l->size, now->r->size))
		rebuild_node = now;
	if(now->l == rebuild_node || now->r == rebuild_node)
		rebuild_fa = now;
	return now;
}

void scap_query(scap_t* now, int l, int r)
{
	if(l > r) return;
	int lsz = now->l->size + 1, sz = now->size;
	if(l == 1 && r == sz)
	{
		seg_record[seg_num++] = now->seg;
		return;
	}

	if(l <= lsz && r >= lsz)
		record[record_num++] = now->val;

	if(r < lsz) 
	{
		scap_query(now->l, l, r);
	} else if(l > lsz) { 
		scap_query(now->r, l - lsz, r - lsz);
	} else {
		scap_query(now->l, l, lsz - 1);
		scap_query(now->r, 1, r - lsz);
	}
}

int find_kth(int l, int r, int k)
{
	seg_num = record_num = 0;
	scap_query(root, l, r);
	int low = 0, high = MaxV;
	while(low < high)
	{
		int mid = (low + high) >> 1, sum = 0;
		for(int i = 0; i != seg_num; ++i)
			sum += seg_record[i]->l->w;
		for(int i = 0; i != record_num; ++i)
			sum += record[i] >= low && record[i] <= mid;
		if(k <= sum)
		{
			high = mid;
			for(int i = 0; i != seg_num; ++i)
				seg_record[i] = seg_record[i]->l;
		} else {
			for(int i = 0; i != seg_num; ++i)
				seg_record[i] = seg_record[i]->r;
			k -= sum;
			low = mid + 1;
		}
	}

	return low;
}

void init()
{
	static seg_t seg_nil_base;
	seg_nil = &seg_nil_base;
	seg_nil->w = 0;
	seg_nil->l = seg_nil->r = seg_nil;

	static scap_t scap_nil_base;
	scap_nil = &scap_nil_base;
	scap_nil->l = scap_nil->r = scap_nil;
	scap_nil->seg = seg_nil;
	scap_nil->size = 0;

	alpha = 0.8;
}

int main()
{
	int n;
	std::scanf("%d", &n);
	for(int i = 1; i <= n; ++i)
		std::scanf("%d", record + i);

	init();
	root = scap_build(1, n);

	int m, lastans = 0;
	std::scanf("%d", &m);
	for(int i = 0; i != m; ++i)
	{
		char op[2];
		std::scanf("%s", op);
		if(*op == 'Q')
		{
			int x, y, k;
			std::scanf("%d %d %d", &x, &y, &k);
			x ^= lastans, y ^= lastans, k ^= lastans;
			std::printf("%d\n", lastans = find_kth(x, y, k));
		} else {
			int x, val;
			std::scanf("%d %d", &x, &val);
			x ^= lastans, val ^= lastans;
			if(*op == 'M') 
			{
				scap_modify(root, x, val);
			} else {
				rebuild_fa = 0;
				rebuild_node = 0;
				root = scap_insert(root, x - 1, val);
				if(rebuild_node)
				{
					if(rebuild_node == root) root = scap_rebuild(root);
					else if(rebuild_fa->l == rebuild_node)
						rebuild_fa->l = scap_rebuild(rebuild_node);
					else rebuild_fa->r = scap_rebuild(rebuild_node);
				}
			} 
		}
	}

	destroy(root);
	return 0;
}
