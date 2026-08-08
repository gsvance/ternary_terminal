#include <stdio.h>

int true_modulo(int n, unsigned int d)
{
    int signed_d = d;
    int remainder = n % signed_d;
    if (remainder < 0) {
        return remainder + signed_d;
    }
    return remainder;
}

int balanced_modulo(int n, unsigned int d)
{
    int signed_d = d;
    int half_range = signed_d / 2;
    return true_modulo(n + half_range, d) - half_range;
}

int balanced_divide(int n, unsigned int d)
{
    int m = balanced_modulo(n, d);
    int signed_d = d;
    return (n - m) / signed_d;
}

int main()
{
    const unsigned int d = 5;

    // Test
    for (int n = -10; n <= +10; n++) {
        printf("%d %%%% %u = %d\n", n, d, balanced_modulo(n, d));
        printf("%d // %u = %d\n", n, d, balanced_divide(n, d));
        if (n != 10) {
            printf("\n");
        }
    }

    return 0;
}
