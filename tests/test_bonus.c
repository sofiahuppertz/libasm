#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ╔════════════════════════════════════════════════════════════════════════════╗
// ║                              ANSI COLOR CODES                              ║
// ╚════════════════════════════════════════════════════════════════════════════╝

#define RST     "\033[0m"
#define BOLD    "\033[1m"
#define DIM     "\033[2m"

#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"
#define WHITE   "\033[37m"

// ╔════════════════════════════════════════════════════════════════════════════╗
// ║                               EMOJI SYMBOLS                                ║
// ╚════════════════════════════════════════════════════════════════════════════╝

#define EMOJI_PASS    "✅"
#define EMOJI_FAIL    "❌"
#define EMOJI_TEST    "🧪"
#define EMOJI_ROCKET  "🚀"
#define EMOJI_CHECK   "✓"
#define EMOJI_CROSS   "✗"
#define EMOJI_LIST    "📋"
#define EMOJI_LINK    "🔗"
#define EMOJI_TROPHY  "🏆"
#define EMOJI_WARNING "⚠️"
#define EMOJI_SORT    "🔀"
#define EMOJI_TRASH   "🗑️"
#define EMOJI_PLUS    "➕"
#define EMOJI_NUM     "🔢"
#define EMOJI_STAR    "⭐"

// ╔════════════════════════════════════════════════════════════════════════════╗
// ║                              TEST STATISTICS                               ║
// ╚════════════════════════════════════════════════════════════════════════════╝

static int g_total_tests = 0;
static int g_passed_tests = 0;
static int g_failed_tests = 0;

// ╔════════════════════════════════════════════════════════════════════════════╗
// ║                              LIST STRUCTURE                                ║
// ╚════════════════════════════════════════════════════════════════════════════╝

typedef struct s_list
{
	void            *data;
	struct s_list   *next;
} t_list;

// ╔════════════════════════════════════════════════════════════════════════════╗
// ║                        ASSEMBLY FUNCTION DECLARATIONS                      ║
// ╚════════════════════════════════════════════════════════════════════════════╝

// ft_atoi_base
int     ft_atoi_base(const char *str, const char *base);

// ft_list functions
void    ft_list_push_front(t_list **begin_list, void *data);
int     ft_list_size(t_list *begin_list);
void    ft_list_sort(t_list **begin_list, int (*cmp)());
void    ft_list_remove_if(t_list **begin_list, void *data_ref,
			int (*cmp)(), void (*free_fct)(void *));
int     ft_strcmp(const char *s1, const char *s2);

// ╔════════════════════════════════════════════════════════════════════════════╗
// ║                              HELPER MACROS                                 ║
// ╚════════════════════════════════════════════════════════════════════════════╝

#define PRINT_HEADER(emoji, title) \
	printf("\n" CYAN BOLD "┌──────────────────────────────────────────────────────────────┐\n"); \
	printf("│  " emoji "  %-54s │\n", title); \
	printf("└──────────────────────────────────────────────────────────────┘" RST "\n\n")

#define TEST_PASS(name) \
	do { \
		g_total_tests++; \
		g_passed_tests++; \
		printf("  " GREEN EMOJI_CHECK RST "  " BOLD "%-40s" RST " " GREEN "[PASS]" RST "\n", name); \
	} while(0)

#define TEST_FAIL(name) \
	do { \
		g_total_tests++; \
		g_failed_tests++; \
		printf("  " RED EMOJI_CROSS RST "  " BOLD "%-40s" RST " " RED "[FAIL]" RST "\n", name); \
	} while(0)

#define TEST_RESULT(name, pass) \
	do { \
		if (pass) TEST_PASS(name); \
		else TEST_FAIL(name); \
	} while(0)

// ╔════════════════════════════════════════════════════════════════════════════╗
// ║                             HELPER FUNCTIONS                               ║
// ╚════════════════════════════════════════════════════════════════════════════╝

static char *ft_strdup(const char *s)
{
	size_t len = strlen(s) + 1;
	char *dup = malloc(len);
	if (dup)
		memcpy(dup, s, len);
	return dup;
}

static void print_list_inline(t_list *list)
{
	printf(DIM "     → " RST);
	if (!list) {
		printf(YELLOW "(empty)" RST "\n");
		return;
	}
	while (list) {
		printf(CYAN "\"%s\"" RST, (char *)list->data);
		if (list->next)
			printf(" → ");
		list = list->next;
	}
	printf("\n");
}

static void free_list(t_list **list)
{
	t_list *current = *list;
	while (current) {
		t_list *next = current->next;
		free(current->data);
		free(current);
		current = next;
	}
	*list = NULL;
}

static int list_matches(t_list *list, const char **expected, int count)
{
	for (int i = 0; i < count; i++) {
		if (!list)
			return 0;
		if (strcmp((char *)list->data, expected[i]) != 0)
			return 0;
		list = list->next;
	}
	return (list == NULL);
}

static int count_list(t_list *list)
{
	int count = 0;
	while (list) {
		count++;
		list = list->next;
	}
	return count;
}

// ╔════════════════════════════════════════════════════════════════════════════╗
// ║                           ft_atoi_base TESTS                               ║
// ╚════════════════════════════════════════════════════════════════════════════╝

typedef struct s_atoi_case {
	const char *label;
	const char *str;
	const char *base;
	int expected;
} t_atoi_case;

static void test_atoi_base(void)
{
	PRINT_HEADER(EMOJI_NUM, "ft_atoi_base - String to Integer Conversion");

	printf(DIM "  ┌─────┬──────────────────────────┬────────────────────────┬──────────┬──────────┬────────┐\n");
	printf("  │ " BOLD "#" RST DIM "   │ " BOLD "Test Case" RST DIM "                │ " BOLD "Input" RST DIM "                  │ " BOLD "Expected" RST DIM " │ " BOLD "Got" RST DIM "      │ " BOLD "Status" RST DIM " │\n");
	printf("  ├─────┼──────────────────────────┼────────────────────────┼──────────┼──────────┼────────┤" RST "\n");

	const t_atoi_case cases[] = {
		// Basic decimal tests
		{"Basic decimal",           "42",           "0123456789",                    42},
		{"Negative decimal",        "-42",          "0123456789",                   -42},
		{"Positive sign",           "+123",         "0123456789",                   123},
		{"Zero",                    "0",            "0123456789",                     0},
		{"Large number",            "2147483647",   "0123456789",            2147483647},

		// Whitespace handling
		{"Leading spaces",          "  42",         "0123456789",                    42},
		{"Leading tabs",            "\t\t42",       "0123456789",                    42},
		{"Mixed whitespace",        " \t\n42",      "0123456789",                    42},
		{"Whitespace + negative",   "   -42",       "0123456789",                   -42},

		// Sign handling
		{"Multiple signs (even -)", "---+-+--42",   "0123456789",                    42},
		{"Multiple signs (odd -)",  "---42",        "0123456789",                   -42},
		{"Plus then minus",         "+-42",         "0123456789",                   -42},

		// Hexadecimal (base 16)
		{"Hex lowercase",           "2a",           "0123456789abcdef",              42},
		{"Hex ff",                  "ff",           "0123456789abcdef",             255},
		{"Hex uppercase",           "FF",           "0123456789ABCDEF",             255},
		{"Hex negative",            "-ff",          "0123456789abcdef",            -255},

		// Binary (base 2)
		{"Binary 42",               "101010",       "01",                            42},
		{"Binary negative",         "-101010",      "01",                           -42},
		{"Binary 255",              "11111111",     "01",                           255},

		// Other bases
		{"Base 36 (zzz)",           "zzz",          "0123456789abcdefghijklmnopqrstuvwxyz", 46655},
		{"Base 3",                  "1120",         "012",                           42},
		{"Base 8 (octal)",          "52",           "01234567",                      42},

		// Edge cases - invalid base
		{"Empty base",              "42",           "",                               0},
		{"Base length 1",           "42",           "0",                              0},
		{"Base with duplicates",    "42",           "01234556789",                    0},
		{"Base with + sign",        "42",           "0123456789+",                    0},
		{"Base with - sign",        "42",           "0123456789-",                    0},
		// Edge cases - string
		{"Empty string",            "",             "0123456789",                     0},
		{"Only whitespace",         "   ",          "0123456789",                     0},
		{"Only sign",               "-",            "0123456789",                     0},
	};

	const size_t total = sizeof(cases) / sizeof(*cases);

	for (size_t i = 0; i < total; ++i) {
		// Copy strings to mutable buffers (ft_atoi_base modifies the string temporarily)
		char str_buf[64];
		char base_buf[64];
		strncpy(str_buf, cases[i].str, sizeof(str_buf) - 1);
		str_buf[sizeof(str_buf) - 1] = '\0';
		strncpy(base_buf, cases[i].base, sizeof(base_buf) - 1);
		base_buf[sizeof(base_buf) - 1] = '\0';

		int got = ft_atoi_base(str_buf, base_buf);
		int pass = (got == cases[i].expected);

		g_total_tests++;
		if (pass) g_passed_tests++;
		else g_failed_tests++;

		// Format input display (truncate if too long)
		char input_display[24];
		if (strlen(cases[i].str) > 8 || strlen(cases[i].base) > 10)
			snprintf(input_display, sizeof(input_display), "\"%.5s..\" base%.4s..",
				cases[i].str, cases[i].base);
		else
			snprintf(input_display, sizeof(input_display), "\"%s\" base%zu",
				cases[i].str, strlen(cases[i].base));

		printf("  │ " CYAN "%-3zu" RST " │ %-24.24s │ %-22.22s │ " YELLOW "%-8d" RST " │ %-8d │ %s │\n",
			i + 1, cases[i].label, input_display, cases[i].expected, got,
			pass ? GREEN EMOJI_CHECK " PASS" RST : RED EMOJI_CROSS " FAIL" RST);
	}

	printf(DIM "  └─────┴──────────────────────────┴────────────────────────┴──────────┴──────────┴────────┘" RST "\n");
}

// ╔════════════════════════════════════════════════════════════════════════════╗
// ║                         ft_list_push_front TESTS                           ║
// ╚════════════════════════════════════════════════════════════════════════════╝

static void test_push_front(void)
{
	PRINT_HEADER(EMOJI_PLUS, "ft_list_push_front - Add Element to Front");

	// Test 1: Push to empty list
	{
		t_list *list = NULL;
		ft_list_push_front(&list, ft_strdup("first"));
		int pass = (list != NULL && strcmp(list->data, "first") == 0 && list->next == NULL);
		TEST_RESULT("Push to empty list", pass);
		print_list_inline(list);
		free_list(&list);
	}

	// Test 2: Push multiple elements
	{
		t_list *list = NULL;
		ft_list_push_front(&list, ft_strdup("third"));
		ft_list_push_front(&list, ft_strdup("second"));
		ft_list_push_front(&list, ft_strdup("first"));
		const char *expected[] = {"first", "second", "third"};
		int pass = list_matches(list, expected, 3);
		TEST_RESULT("Push 3 elements (order check)", pass);
		print_list_inline(list);
		free_list(&list);
	}

	// Test 3: Push single character strings
	{
		t_list *list = NULL;
		ft_list_push_front(&list, ft_strdup("c"));
		ft_list_push_front(&list, ft_strdup("b"));
		ft_list_push_front(&list, ft_strdup("a"));
		const char *expected[] = {"a", "b", "c"};
		int pass = list_matches(list, expected, 3);
		TEST_RESULT("Push single char strings", pass);
		print_list_inline(list);
		free_list(&list);
	}

	// Test 4: Push many elements
	{
		t_list *list = NULL;
		for (int i = 9; i >= 0; i--) {
			char buf[16];
			snprintf(buf, sizeof(buf), "item%d", i);
			ft_list_push_front(&list, ft_strdup(buf));
		}
		int pass = (count_list(list) == 10);
		TEST_RESULT("Push 10 elements", pass);
		printf(DIM "     → (10 elements, first: \"%s\", last: \"item9\")" RST "\n",
			list ? (char *)list->data : "null");
		free_list(&list);
	}

	// Test 5: Push empty string
	{
		t_list *list = NULL;
		ft_list_push_front(&list, ft_strdup(""));
		int pass = (list != NULL && strlen(list->data) == 0);
		TEST_RESULT("Push empty string", pass);
		print_list_inline(list);
		free_list(&list);
	}
}

// ╔════════════════════════════════════════════════════════════════════════════╗
// ║                           ft_list_size TESTS                               ║
// ╚════════════════════════════════════════════════════════════════════════════╝

static void test_list_size(void)
{
	PRINT_HEADER(EMOJI_LIST, "ft_list_size - Count Elements");

	// Test 1: Empty list
	{
		t_list *list = NULL;
		int size = ft_list_size(list);
		int pass = (size == 0);
		TEST_RESULT("Empty list size", pass);
		printf(DIM "     → Expected: 0, Got: %d" RST "\n", size);
	}

	// Test 2: Single element
	{
		t_list *list = NULL;
		ft_list_push_front(&list, ft_strdup("one"));
		int size = ft_list_size(list);
		int pass = (size == 1);
		TEST_RESULT("Single element list", pass);
		printf(DIM "     → Expected: 1, Got: %d" RST "\n", size);
		free_list(&list);
	}

	// Test 3: Three elements
	{
		t_list *list = NULL;
		ft_list_push_front(&list, ft_strdup("c"));
		ft_list_push_front(&list, ft_strdup("b"));
		ft_list_push_front(&list, ft_strdup("a"));
		int size = ft_list_size(list);
		int pass = (size == 3);
		TEST_RESULT("Three element list", pass);
		printf(DIM "     → Expected: 3, Got: %d" RST "\n", size);
		free_list(&list);
	}

	// Test 4: Ten elements
	{
		t_list *list = NULL;
		for (int i = 0; i < 10; i++)
			ft_list_push_front(&list, ft_strdup("x"));
		int size = ft_list_size(list);
		int pass = (size == 10);
		TEST_RESULT("Ten element list", pass);
		printf(DIM "     → Expected: 10, Got: %d" RST "\n", size);
		free_list(&list);
	}

	// Test 5: Fifty elements
	{
		t_list *list = NULL;
		for (int i = 0; i < 50; i++)
			ft_list_push_front(&list, ft_strdup("item"));
		int size = ft_list_size(list);
		int pass = (size == 50);
		TEST_RESULT("Fifty element list", pass);
		printf(DIM "     → Expected: 50, Got: %d" RST "\n", size);
		free_list(&list);
	}
}

// ╔════════════════════════════════════════════════════════════════════════════╗
// ║                           ft_list_sort TESTS                               ║
// ╚════════════════════════════════════════════════════════════════════════════╝

static void test_list_sort(void)
{
	PRINT_HEADER(EMOJI_SORT, "ft_list_sort - Sort Elements");

	// Test 1: Sort empty list
	{
		t_list *list = NULL;
		ft_list_sort(&list, &ft_strcmp);
		int pass = (list == NULL);
		TEST_RESULT("Sort empty list", pass);
	}

	// Test 2: Sort single element
	{
		t_list *list = NULL;
		ft_list_push_front(&list, ft_strdup("only"));
		ft_list_sort(&list, &ft_strcmp);
		int pass = (list != NULL && strcmp(list->data, "only") == 0);
		TEST_RESULT("Sort single element", pass);
		print_list_inline(list);
		free_list(&list);
	}

	// Test 3: Sort two elements (already sorted)
	{
		t_list *list = NULL;
		ft_list_push_front(&list, ft_strdup("b"));
		ft_list_push_front(&list, ft_strdup("a"));
		ft_list_sort(&list, &ft_strcmp);
		const char *expected[] = {"a", "b"};
		int pass = list_matches(list, expected, 2);
		TEST_RESULT("Sort 2 elements (already sorted)", pass);
		print_list_inline(list);
		free_list(&list);
	}

	// Test 4: Sort two elements (reverse order)
	{
		t_list *list = NULL;
		ft_list_push_front(&list, ft_strdup("a"));
		ft_list_push_front(&list, ft_strdup("b"));
		ft_list_sort(&list, &ft_strcmp);
		const char *expected[] = {"a", "b"};
		int pass = list_matches(list, expected, 2);
		TEST_RESULT("Sort 2 elements (reverse order)", pass);
		print_list_inline(list);
		free_list(&list);
	}

	// Test 5: Sort five elements (random order)
	{
		t_list *list = NULL;
		ft_list_push_front(&list, ft_strdup("banana"));
		ft_list_push_front(&list, ft_strdup("apple"));
		ft_list_push_front(&list, ft_strdup("cherry"));
		ft_list_push_front(&list, ft_strdup("date"));
		ft_list_push_front(&list, ft_strdup("elderberry"));
		printf(DIM "     Before:" RST);
		print_list_inline(list);
		ft_list_sort(&list, &ft_strcmp);
		const char *expected[] = {"apple", "banana", "cherry", "date", "elderberry"};
		int pass = list_matches(list, expected, 5);
		TEST_RESULT("Sort 5 elements (fruits)", pass);
		printf(DIM "     After: " RST);
		print_list_inline(list);
		free_list(&list);
	}

	// Test 6: Sort with duplicates
	{
		t_list *list = NULL;
		ft_list_push_front(&list, ft_strdup("b"));
		ft_list_push_front(&list, ft_strdup("a"));
		ft_list_push_front(&list, ft_strdup("b"));
		ft_list_push_front(&list, ft_strdup("a"));
		ft_list_sort(&list, &ft_strcmp);
		const char *expected[] = {"a", "a", "b", "b"};
		int pass = list_matches(list, expected, 4);
		TEST_RESULT("Sort with duplicates", pass);
		print_list_inline(list);
		free_list(&list);
	}

	// Test 7: Sort already sorted list
	{
		t_list *list = NULL;
		ft_list_push_front(&list, ft_strdup("e"));
		ft_list_push_front(&list, ft_strdup("d"));
		ft_list_push_front(&list, ft_strdup("c"));
		ft_list_push_front(&list, ft_strdup("b"));
		ft_list_push_front(&list, ft_strdup("a"));
		ft_list_sort(&list, &ft_strcmp);
		const char *expected[] = {"a", "b", "c", "d", "e"};
		int pass = list_matches(list, expected, 5);
		TEST_RESULT("Sort already sorted list", pass);
		print_list_inline(list);
		free_list(&list);
	}

	// Test 8: Sort reverse sorted list
	{
		t_list *list = NULL;
		ft_list_push_front(&list, ft_strdup("a"));
		ft_list_push_front(&list, ft_strdup("b"));
		ft_list_push_front(&list, ft_strdup("c"));
		ft_list_push_front(&list, ft_strdup("d"));
		ft_list_push_front(&list, ft_strdup("e"));
		ft_list_sort(&list, &ft_strcmp);
		const char *expected[] = {"a", "b", "c", "d", "e"};
		int pass = list_matches(list, expected, 5);
		TEST_RESULT("Sort reverse sorted list", pass);
		print_list_inline(list);
		free_list(&list);
	}

	// Test 9: Sort numbers as strings
	{
		t_list *list = NULL;
		ft_list_push_front(&list, ft_strdup("10"));
		ft_list_push_front(&list, ft_strdup("2"));
		ft_list_push_front(&list, ft_strdup("1"));
		ft_list_push_front(&list, ft_strdup("20"));
		ft_list_sort(&list, &ft_strcmp);
		const char *expected[] = {"1", "10", "2", "20"};  // lexicographic order
		int pass = list_matches(list, expected, 4);
		TEST_RESULT("Sort numbers (lexicographic)", pass);
		print_list_inline(list);
		free_list(&list);
	}
}

// ╔════════════════════════════════════════════════════════════════════════════╗
// ║                         ft_list_remove_if TESTS                            ║
// ╚════════════════════════════════════════════════════════════════════════════╝

static void test_list_remove_if(void)
{
	PRINT_HEADER(EMOJI_TRASH, "ft_list_remove_if - Remove Matching Elements");

	// Test 1: Remove from empty list
	{
		t_list *list = NULL;
		ft_list_remove_if(&list, "test", &ft_strcmp, &free);
		int pass = (list == NULL);
		TEST_RESULT("Remove from empty list", pass);
	}

	// Test 2: Remove non-existent element
	{
		t_list *list = NULL;
		ft_list_push_front(&list, ft_strdup("a"));
		ft_list_push_front(&list, ft_strdup("b"));
		ft_list_remove_if(&list, "x", &ft_strcmp, &free);
		const char *expected[] = {"b", "a"};
		int pass = list_matches(list, expected, 2);
		TEST_RESULT("Remove non-existent element", pass);
		print_list_inline(list);
		free_list(&list);
	}

	// Test 3: Remove head element
	{
		t_list *list = NULL;
		ft_list_push_front(&list, ft_strdup("c"));
		ft_list_push_front(&list, ft_strdup("b"));
		ft_list_push_front(&list, ft_strdup("a"));
		ft_list_remove_if(&list, "a", &ft_strcmp, &free);
		const char *expected[] = {"b", "c"};
		int pass = list_matches(list, expected, 2);
		TEST_RESULT("Remove head element", pass);
		print_list_inline(list);
		free_list(&list);
	}

	// Test 4: Remove middle element
	{
		t_list *list = NULL;
		ft_list_push_front(&list, ft_strdup("c"));
		ft_list_push_front(&list, ft_strdup("b"));
		ft_list_push_front(&list, ft_strdup("a"));
		ft_list_remove_if(&list, "b", &ft_strcmp, &free);
		const char *expected[] = {"a", "c"};
		int pass = list_matches(list, expected, 2);
		TEST_RESULT("Remove middle element", pass);
		print_list_inline(list);
		free_list(&list);
	}

	// Test 5: Remove tail element
	{
		t_list *list = NULL;
		ft_list_push_front(&list, ft_strdup("c"));
		ft_list_push_front(&list, ft_strdup("b"));
		ft_list_push_front(&list, ft_strdup("a"));
		ft_list_remove_if(&list, "c", &ft_strcmp, &free);
		const char *expected[] = {"a", "b"};
		int pass = list_matches(list, expected, 2);
		TEST_RESULT("Remove tail element", pass);
		print_list_inline(list);
		free_list(&list);
	}

	// Test 6: Remove multiple occurrences
	{
		t_list *list = NULL;
		ft_list_push_front(&list, ft_strdup("cat"));
		ft_list_push_front(&list, ft_strdup("dog"));
		ft_list_push_front(&list, ft_strdup("cat"));
		ft_list_push_front(&list, ft_strdup("bird"));
		ft_list_push_front(&list, ft_strdup("cat"));
		printf(DIM "     Before:" RST);
		print_list_inline(list);
		ft_list_remove_if(&list, "cat", &ft_strcmp, &free);
		const char *expected[] = {"bird", "dog"};
		int pass = list_matches(list, expected, 2);
		TEST_RESULT("Remove multiple occurrences", pass);
		printf(DIM "     After: " RST);
		print_list_inline(list);
		free_list(&list);
	}

	// Test 7: Remove all elements
	{
		t_list *list = NULL;
		ft_list_push_front(&list, ft_strdup("same"));
		ft_list_push_front(&list, ft_strdup("same"));
		ft_list_push_front(&list, ft_strdup("same"));
		ft_list_remove_if(&list, "same", &ft_strcmp, &free);
		int pass = (list == NULL);
		TEST_RESULT("Remove all elements (same value)", pass);
		print_list_inline(list);
	}

	// Test 8: Remove only element
	{
		t_list *list = NULL;
		ft_list_push_front(&list, ft_strdup("only"));
		ft_list_remove_if(&list, "only", &ft_strcmp, &free);
		int pass = (list == NULL);
		TEST_RESULT("Remove only element", pass);
		print_list_inline(list);
	}

	// Test 9: Remove with NULL free function
	{
		t_list *list = NULL;
		char *static_str = ft_strdup("test");
		ft_list_push_front(&list, static_str);
		ft_list_push_front(&list, ft_strdup("keep"));
		ft_list_remove_if(&list, "test", &ft_strcmp, NULL);
		int pass = (list != NULL && strcmp(list->data, "keep") == 0);
		TEST_RESULT("Remove with NULL free_fct", pass);
		free(static_str);  // manual free since free_fct was NULL
		free_list(&list);
	}

	// Test 10: Remove consecutive head elements
	{
		t_list *list = NULL;
		ft_list_push_front(&list, ft_strdup("c"));
		ft_list_push_front(&list, ft_strdup("x"));
		ft_list_push_front(&list, ft_strdup("x"));
		ft_list_push_front(&list, ft_strdup("x"));
		ft_list_remove_if(&list, "x", &ft_strcmp, &free);
		const char *expected[] = {"c"};
		int pass = list_matches(list, expected, 1);
		TEST_RESULT("Remove consecutive head elements", pass);
		print_list_inline(list);
		free_list(&list);
	}
}

// ╔════════════════════════════════════════════════════════════════════════════╗
// ║                            PRINT SUMMARY                                   ║
// ╚════════════════════════════════════════════════════════════════════════════╝

static void print_summary(void)
{
	printf("\n");
	printf(CYAN BOLD "╔══════════════════════════════════════════════════════════════════╗\n");
	printf("║                        " EMOJI_TROPHY "  TEST SUMMARY  " EMOJI_TROPHY "                        ║\n");
	printf("╠══════════════════════════════════════════════════════════════════╣" RST "\n");

	printf(CYAN BOLD "║" RST "  Total Tests:  " BOLD "%-5d" RST "                                              " CYAN BOLD "║\n", g_total_tests);
	printf(CYAN BOLD "║" RST "  " GREEN EMOJI_PASS " Passed:     " BOLD "%-5d" RST "                                              " CYAN BOLD "║\n", g_passed_tests);
	printf(CYAN BOLD "║" RST "  " RED EMOJI_FAIL " Failed:     " BOLD "%-5d" RST "                                              " CYAN BOLD "║\n", g_failed_tests);

	float percentage = g_total_tests > 0 ? (float)g_passed_tests / g_total_tests * 100 : 0;

	printf(CYAN BOLD "╠══════════════════════════════════════════════════════════════════╣\n");

	if (g_failed_tests == 0) {
		printf("║" RST "  " GREEN BOLD EMOJI_TROPHY "  ALL TESTS PASSED! " EMOJI_ROCKET "                                      " CYAN BOLD "║\n");
	} else {
		printf("║" RST "  " YELLOW EMOJI_WARNING "  Some tests failed. Success rate: " BOLD "%.1f%%" RST "                     " CYAN BOLD "║\n", percentage);
	}

	printf("╚══════════════════════════════════════════════════════════════════╝" RST "\n\n");
}

// ╔════════════════════════════════════════════════════════════════════════════╗
// ║                                  MAIN                                      ║
// ╚════════════════════════════════════════════════════════════════════════════╝

int main(void)
{
	printf("\n");
	printf(MAGENTA BOLD "╔══════════════════════════════════════════════════════════════════╗\n");
	printf("║                                                                  ║\n");
	printf("║   " EMOJI_STAR "  " WHITE "B O N U S   T E S T   S U I T E" MAGENTA "   " EMOJI_STAR "                       ║\n");
	printf("║                                                                  ║\n");
	printf("║   " RST DIM "ft_atoi_base & ft_list in x86-64 Assembly" MAGENTA BOLD "                   ║\n");
	printf("║                                                                  ║\n");
	printf("╚══════════════════════════════════════════════════════════════════╝" RST "\n");

	test_atoi_base();
	test_push_front();
	test_list_size();
	test_list_sort();
	test_list_remove_if();

	print_summary();

	return (g_failed_tests > 0 ? 1 : 0);
}
