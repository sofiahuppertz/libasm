#include <stdio.h>
#include <stdlib.h>

// Declare the assembly function
int ft_atoi_base(const char *str, const char *base);

int main(int argc, char **argv)
{
    if (argc != 3)
    {
        printf("Usage: %s <string> <base>\n", argv[0]);
        printf("Example: %s \"42\" \"0123456789\"\n", argv[0]);
        printf("Example: %s \"  -2a\" \"0123456789abcdef\"\n", argv[0]);
        return 1;
    }

    const char *str = argv[1];
    const char *base = argv[2];

    int result = ft_atoi_base(str, base);

    printf("ft_atoi_base(\"%s\", \"%s\") = %d\n", str, base, result);

    return 0;
}