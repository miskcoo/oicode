/*
 * BZOJ-1010 玩具装箱
 *  DP+斜率优化 
 */
#include <cstdio>  
    
int N, L;  
long long sum[50001];  
long long f[50001];  
int seq[50001];  
    
template<typename T>  
inline T sqr(T v)  
{  
    return v * v;  
}  
    
inline double get_slope(int i, int j)  
{  
    return (f[j] + sqr<double>(j + sum[j])  
        - f[i] - sqr<double>(i + sum[i]))  
        / (j - i + sum[j] - sum[i]);  
}  
    
int main()  
{     
    sum[0] = seq[0] = 0;  
    int head = 0, tail = 0;  
    std::scanf("%d %d", &N, &L);  
    for(int i = 1; i <= N; ++i)  
    {  
        int v;  
        std::scanf("%d", &v);  
        sum[i] = sum[i - 1] + v;  
            
        double right = i + sum[i] - L - 1;  
        while(head < tail && get_slope(seq[head], seq[head + 1]) < 2 * right)  
            ++head;  
                
        int k = seq[head];  
        f[i] = f[k] + sqr<long long>(right - sum[k] - k);  
            
        while(head < tail && get_slope(seq[tail - 1], seq[tail]) > get_slope(seq[tail], i))  
            --tail;  
        seq[++tail] = i;  
    }  
    std::printf("%lld", f[N]);  
    return 0;  
}
