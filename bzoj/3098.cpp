/* BZOJ-3098: Hash Killer II
 *  概率论，生日攻击 */
#include <cstdlib>
#include <cstdio>
 
int main()
{
    int n = 100000, l = 12;
    std::printf("%d %d\n", n, l);
    for(int i = 0; i != n; ++i)
        std::putchar(std::rand() % 26 + 'a');
    return 0;
}
