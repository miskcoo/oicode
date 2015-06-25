/* BZOJ-3898: 打的士
 *   线段树 */
#include <cstdio>
#include <vector>
#include <algorithm>
 
const int inf = 2000000000;
const int MaxM = 1100001, MaxN = 1100001, MaxS = MaxN << 2;
 
int discrete[MaxS];
int val[MaxS], vid[MaxS], lazy[MaxS];
 
void cover(int now, int r, int right)
{
    vid[now] = right;
    val[now] = right - discrete[r - 1];
    lazy[now] = right;
}
 
void pushdown(int now, int head, int tail)
{
    if(!lazy[now] || head == tail) return;
    int m = (head + tail) >> 1;
    cover(now << 1, m, lazy[now]);
    cover((now << 1) + 1, tail, lazy[now]);
    lazy[now] = 0;
}
 
void pushup(int now)
{
    int l = now << 1, r = l + 1;
    val[now] = std::min(val[l], val[r]);
    vid[now] = vid[r];
}
 
void build(int now, int head, int tail)
{
    if(head == tail)
    {
        vid[now] = discrete[head - 1];
        return;
    }
 
    int m = (head + tail) >> 1;
    build(now << 1, head, m);
    build((now << 1) + 1, m + 1, tail);
    pushup(now);
//  printf("%d, %d\n", val[now], vid[now]);
}
 
void modify(int now, int head, int tail, int a, int b, int v)
{
    pushdown(now, head, tail);
    if(head == a && tail == b)
        return cover(now, tail, v);
 
    int m = (head + tail) >> 1;
    int l = now << 1, r = l + 1;
    if(b <= m) modify(l, head, m, a, b, v);
    else if(m < a) modify(r, m + 1, tail, a, b, v);
    else modify(l, head, m, a, m, v), modify(r, m + 1, tail, m + 1, b, v);
    pushup(now);
}
 
int ask(int now, int head, int tail, int a, int b)
{
    pushdown(now, head, tail);
    if(head == a && tail == b)
        return val[now];
 
    int m = (head + tail) >> 1;
    int l = now << 1, r = l + 1;
    if(b <= m) return ask(l, head, m, a, b);
    else if(m < a) return ask(r, m + 1, tail, a, b);
    return std::min( ask(l, head, m, a, m), 
            ask(r, m + 1, tail, m + 1, b) );
}
 
int find(int now, int head, int tail, int a, int b, int right)
{
    pushdown(now, head, tail);
    int m = (head + tail) >> 1;
    int l = now << 1, r = l + 1;
    if(head == a && tail == b)
    {
        if(vid[now] <= right)
            return b;
 
        if(a == b) return 0;
 
        if(vid[l] > right)
            return find(l, a, m, a, m, right);
        return std::max(find(r, m + 1, b, m + 1, b, right), m);
    }
 
    int ans = 0;
    if(b <= m) ans = find(l, head, m, a, b, right);
    else if(m < a) ans = find(r, m + 1, tail, a, b, right);
    else ans = std::max( find(l, head, m, a, m, right), 
            find(r, m + 1, tail, m + 1, b, right) );
    return ans;
}
 
struct ques_t
{
    int op, num;
    std::vector<int> cars;
} ques[MaxM];
 
int main()
{
    int n, vtot = 0;
    std::scanf("%d", &n);
    for(int i = 0; i != n; ++i)
    {
        char op[2];
        int num;
        std::scanf("%s %d", op, &num);
        if(*op == 'C')
        {
            ques[i].op = 1;
            for(int j = 0; j != num; ++j)
            {
                int v;
                std::scanf("%d", &v);
                ques[i].cars.push_back(v);
                discrete[vtot++] = v;
            }
 
            ques[i].cars.push_back(inf);
            ques[i].cars.push_back(-inf);
 
            std::sort(ques[i].cars.begin(), ques[i].cars.end());
        } else {
            ques[i].op = 0;
            ques[i].num = num;
        }
    }
 
    discrete[vtot++] = inf;
    discrete[vtot++] = -inf;
    std::sort(discrete, discrete + vtot);
    vtot = std::unique(discrete, discrete + vtot) - discrete;
 
    build(1, 1, vtot);
 
    int count = 0;
    for(int i = 0; i != n; ++i)
    {
        if(ques[i].op == 1)
        {
            ++count;
            std::vector<int>& cars = ques[i].cars;
            for(int j = 0; j < cars.size() - 1; ++j)
            {
                if(cars[j] == cars[j + 1]) continue;
                int l = std::lower_bound(discrete, discrete + vtot, cars[j]) - discrete + 2;
                int r = std::lower_bound(discrete, discrete + vtot, cars[j + 1]) - discrete + 1;
                r = find(1, 1, vtot, l, r, cars[j + 1]);
//              printf("%d, %d\n", l, r);
                if(r >= l) modify(1, 1, vtot, l, r, cars[j + 1]);
            }
 
        } else {
            int pos = std::lower_bound(discrete, discrete + vtot, ques[i].num) - discrete + 1;
            if(!count) std::puts("0");
            else if(pos > vtot - 1) std::puts("-1");
    //      else if(count < 2) std::puts("0");
            else {
                int ans = ask(1, 1, vtot, pos, vtot - 1);
                if(ans > inf / 2) ans = -1;
                printf("%d\n", ans);
            }
        }
    }
    return 0;
}
﻿
