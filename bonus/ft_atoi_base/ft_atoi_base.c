// Check the base doesn't contain - or + sign, has at least 2 characters and no duplicates.
// Define symbol and move the char pointer to the first digit after - or + sign or white space.
// Check every digit is in the base range.
// Recursive call to convert the rest of the string to number by getting the base digit and multiplying it by the power of the base.

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

// Temporary ft_strlen for testing (replace with your asm version later)
size_t ft_strlen(const char *str)
{
    size_t len = 0;
    while (str[len])
        len++;
    return len;
}

int	is_whitespace(int c)
{
	if (c == 32 || c == 9 || c == 11 || c == 12 || c == 13 || c == 10)
	{
		return (1);
	}
	return (0);
}

char *ft_strchr(const char *str, int c)
{
	int i = 0;
	while (str[i])
	{
		if (str[i] == (char)c)
			return ((char *)&str[i]);
		i++;
	}
	if (c == '\0')
		return ((char *)&str[i]);
	return (NULL);
}

int has_duplicates(char *str)
{
    bool seen[256] = { false };

    int i = 0;
    while (str[i])
    {
        unsigned char current_char = (unsigned char)str[i];

        if (seen[current_char])
        {
            return 1; // Duplicate found
        }
        seen[current_char] = true;
        i++;
    }
    return 0; // No duplicates
}

int get_digit_value(char c, char *base)
{
    int i = 0;
    while (base[i])
    {
        if (base[i] == c)
            return i;
        i++;
    }
    return -1;
}

int recursive_atoi(char *base, char *str)
{
    int base_len = ft_strlen(base);

    if (!*str)
        return (0);
    if (*(str + 1) == '\0')
        return (get_digit_value(*str, base));

    char *end = str;
    while (*(end + 1))
          end++;

    char last = *end;
    *end = '\0';  // temporarily truncate

    int result = recursive_atoi(base, str) * base_len + get_digit_value(last, base);

    *end = last;  // restore
    return result;
}

int ft_atoi_base(char *str, char *base)
{
    if (ft_strlen(base) < 2 || ft_strchr(base, '-') || ft_strchr(base, '+') || has_duplicates(base))
    {
        return (0);
    }

    int sign = 1;
    while (*str == '-' || *str == '+' || is_whitespace(*str))
    {
        if (*str == '-')
        {
            sign *= -1;
        }
        str++;
    }

    char *ptr = str;
    while (*ptr)
    {
        if (!ft_strchr(base, *ptr))
        {
            return (0);
        }
        ptr++;
    }
    return (sign * recursive_atoi(base, str));
}

int main(int argc, char **argv)
{
    if (argc != 3)
    {
        printf("Usage: %s <string> <base>\n", argv[0]);
        printf("Examples:\n");
        printf("  %s 42 0123456789          (decimal)\n", argv[0]);
        printf("  %s 1010 01                (binary)\n", argv[0]);
        printf("  %s 2A 0123456789ABCDEF    (hex)\n", argv[0]);
        printf("  %s -42 0123456789         (negative)\n", argv[0]);
        return 1;
    }

    char *str = argv[1];
    char *base = argv[2];

    int result = ft_atoi_base(str, base);
    printf("ft_atoi_base(\"%s\", \"%s\") = %d\n", str, base, result);

    return 0;
}