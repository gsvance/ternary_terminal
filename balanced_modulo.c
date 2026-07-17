#include <stdio.h>

int true_modulo(int n, unsigned int d)
{
    int m = n % ((int) d);
    return (m < 0) ? (m + d) : m;
}

int balanced_modulo(int n, unsigned int d)
{
    int h = ((int) d) / 2;
    return true_modulo(n + h, d) - h;
}

int main()
{
    const unsigned int d = 5;

    // Test
    for (int i = -10; i <= +10; i++)
    {
        printf("%d \%\% %u = %d\n", i, d, balanced_modulo(i, d));
    }

    return 0;
}
