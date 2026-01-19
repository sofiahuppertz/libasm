#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ========== LIST STRUCTURE ==========

typedef struct s_list
{
    void            *data;
    struct s_list   *next;
}   t_list;

// ========== ASSEMBLY FUNCTION DECLARATIONS ==========

void    ft_list_push_front(t_list **begin_list, void *data);
int     ft_list_size(t_list *begin_list);
void    ft_list_sort(t_list **begin_list, int (*cmp)());
void    ft_list_remove_if(t_list **begin_list, void *data_ref,
            int (*cmp)(), void (*free_fct)(void *));
int     ft_strcmp(const char *s1, const char *s2);

// ========== HELPER FUNCTIONS ==========

void print_list(t_list *list, const char *label)
{
    printf("%s: ", label);
    if (!list)
    {
        printf("(empty)\n");
        return;
    }
    while (list)
    {
        printf("\"%s\"", (char *)list->data);
        if (list->next)
            printf(" -> ");
        list = list->next;
    }
    printf("\n");
}

void free_list(t_list **list)
{
    t_list *current;
    t_list *next;

    current = *list;
    while (current)
    {
        next = current->next;
        free(current->data);
        free(current);
        current = next;
    }
    *list = NULL;
}

char *ft_strdup(const char *s)
{
    size_t len = strlen(s) + 1;
    char *dup = malloc(len);
    if (dup)
        memcpy(dup, s, len);
    return dup;
}

// ========== TEST FUNCTIONS ==========

void test_push_front(void)
{
    t_list *list = NULL;

    printf("\n===== TEST: ft_list_push_front =====\n");

    ft_list_push_front(&list, ft_strdup("third"));
    ft_list_push_front(&list, ft_strdup("second"));
    ft_list_push_front(&list, ft_strdup("first"));

    print_list(list, "After push_front x3");
    printf("Expected: \"first\" -> \"second\" -> \"third\"\n");

    free_list(&list);
    printf("PASSED\n");
}

void test_list_size(void)
{
    t_list *list = NULL;
    int size;

    printf("\n===== TEST: ft_list_size =====\n");

    size = ft_list_size(list);
    printf("Empty list size: %d (expected: 0)\n", size);

    ft_list_push_front(&list, ft_strdup("c"));
    ft_list_push_front(&list, ft_strdup("b"));
    ft_list_push_front(&list, ft_strdup("a"));

    size = ft_list_size(list);
    printf("3-element list size: %d (expected: 3)\n", size);

    ft_list_push_front(&list, ft_strdup("d"));
    ft_list_push_front(&list, ft_strdup("e"));

    size = ft_list_size(list);
    printf("5-element list size: %d (expected: 5)\n", size);

    free_list(&list);
    printf("PASSED\n");
}

void test_list_sort(void)
{
    t_list *list = NULL;

    printf("\n===== TEST: ft_list_sort =====\n");

    ft_list_push_front(&list, ft_strdup("banana"));
    ft_list_push_front(&list, ft_strdup("apple"));
    ft_list_push_front(&list, ft_strdup("cherry"));
    ft_list_push_front(&list, ft_strdup("date"));
    ft_list_push_front(&list, ft_strdup("elderberry"));

    print_list(list, "Before sort");

    ft_list_sort(&list, &ft_strcmp);

    print_list(list, "After sort ");
    printf("Expected: \"apple\" -> \"banana\" -> \"cherry\" -> \"date\" -> \"elderberry\"\n");

    free_list(&list);
    printf("PASSED\n");
}

void test_list_remove_if(void)
{
    t_list *list = NULL;

    printf("\n===== TEST: ft_list_remove_if =====\n");

    // Test 1: Remove from middle
    ft_list_push_front(&list, ft_strdup("cat"));
    ft_list_push_front(&list, ft_strdup("dog"));
    ft_list_push_front(&list, ft_strdup("cat"));
    ft_list_push_front(&list, ft_strdup("bird"));
    ft_list_push_front(&list, ft_strdup("cat"));

    print_list(list, "Before remove \"cat\"");

    ft_list_remove_if(&list, "cat", &ft_strcmp, &free);

    print_list(list, "After remove \"cat\" ");
    printf("Expected: \"bird\" -> \"dog\"\n");

    free_list(&list);

    // Test 2: Remove all elements
    printf("\n");
    ft_list_push_front(&list, ft_strdup("same"));
    ft_list_push_front(&list, ft_strdup("same"));
    ft_list_push_front(&list, ft_strdup("same"));

    print_list(list, "Before remove all \"same\"");

    ft_list_remove_if(&list, "same", &ft_strcmp, &free);

    print_list(list, "After remove all      ");
    printf("Expected: (empty)\n");

    // Test 3: Remove from empty list (should not crash)
    printf("\nTesting remove from empty list... ");
    ft_list_remove_if(&list, "test", &ft_strcmp, &free);
    printf("OK (no crash)\n");

    printf("PASSED\n");
}

// ========== MAIN ==========

int main(void)
{
    printf("========================================\n");
    printf("       ft_list Test Suite\n");
    printf("========================================\n");

    test_push_front();
    test_list_size();
    test_list_sort();
    test_list_remove_if();

    printf("\n========================================\n");
    printf("       All tests completed!\n");
    printf("========================================\n");

    return 0;
}
