#include "libasm.h"
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

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

#define BG_GREEN  "\033[42m"
#define BG_RED    "\033[41m"

// ╔════════════════════════════════════════════════════════════════════════════╗
// ║                               EMOJI SYMBOLS                                ║
// ╚════════════════════════════════════════════════════════════════════════════╝

#define EMOJI_PASS    "✅"
#define EMOJI_FAIL    "❌"
#define EMOJI_TEST    "🧪"
#define EMOJI_ROCKET  "🚀"
#define EMOJI_CHECK   "✓"
#define EMOJI_CROSS   "✗"
#define EMOJI_BOOK    "📚"
#define EMOJI_GEAR    "⚙️"
#define EMOJI_TROPHY  "🏆"
#define EMOJI_WARNING "⚠️"

// ╔════════════════════════════════════════════════════════════════════════════╗
// ║                              TEST STATISTICS                               ║
// ╚════════════════════════════════════════════════════════════════════════════╝

static int g_total_tests = 0;
static int g_passed_tests = 0;
static int g_failed_tests = 0;

// ╔════════════════════════════════════════════════════════════════════════════╗
// ║                              HELPER MACROS                                 ║
// ╚════════════════════════════════════════════════════════════════════════════╝

#define PRINT_HEADER(title) \
	printf("\n" CYAN BOLD "┌──────────────────────────────────────────────────────────────┐\n"); \
	printf("│  " EMOJI_TEST "  %-54s │\n", title); \
	printf("└──────────────────────────────────────────────────────────────┘" RST "\n\n")

#define PRINT_TABLE_HEADER() \
	printf(DIM "  ┌─────┬────────────────────────────────┬──────────┬──────────┬────────┐\n"); \
	printf("  │ " BOLD "#" RST DIM "   │ " BOLD "Test Case" RST DIM "                      │ " BOLD "Expected" RST DIM " │ " BOLD "Got" RST DIM "      │ " BOLD "Status" RST DIM " │\n"); \
	printf("  ├─────┼────────────────────────────────┼──────────┼──────────┼────────┤" RST "\n")

#define PRINT_TABLE_FOOTER() \
	printf(DIM "  └─────┴────────────────────────────────┴──────────┴──────────┴────────┘" RST "\n")

#define RECORD_RESULT(pass) \
	do { \
		g_total_tests++; \
		if (pass) g_passed_tests++; \
		else g_failed_tests++; \
	} while(0)

// ╔════════════════════════════════════════════════════════════════════════════╗
// ║                             TEST STRUCTURES                                ║
// ╚════════════════════════════════════════════════════════════════════════════╝

typedef struct s_strlen_case {
	const char *label;
	const char *input;
} t_strlen_case;

typedef struct s_strcpy_case {
	const char *label;
	const char *input;
} t_strcpy_case;

typedef struct s_strcmp_case {
	const char *label;
	const char *lhs;
	const char *rhs;
} t_strcmp_case;

typedef struct s_strdup_case {
	const char *label;
	const char *input;
} t_strdup_case;

// ╔════════════════════════════════════════════════════════════════════════════╗
// ║                             ft_strlen TESTS                                ║
// ╚════════════════════════════════════════════════════════════════════════════╝

static void run_strlen_tests(void)
{
	const t_strlen_case cases[] = {
		{"empty string",        ""},
		{"single char",         "A"},
		{"short ascii",         "Hello"},
		{"with spaces",         "libasm rocks"},
		{"with newline",        "line1\nline2"},
		{"alphabet",            "abcdefghijklmnopqrstuvwxyz"},
		{"digits",              "0123456789"},
		{"special chars",       "!@#$%^&*()"},
		{"tabs and spaces",     "\t\t  \t  "},
		{"long string",         "The quick brown fox jumps over the lazy dog"},
	};
	const size_t total = sizeof(cases) / sizeof(*cases);

	PRINT_HEADER("ft_strlen - String Length");
	PRINT_TABLE_HEADER();

	for (size_t i = 0; i < total; ++i) {
		const size_t expected = strlen(cases[i].input);
		const size_t got = ft_strlen(cases[i].input);
		const int pass = (expected == got);
		RECORD_RESULT(pass);

		printf("  │ " CYAN "%-3zu" RST " │ %-30.30s │ " YELLOW "%-8zu" RST " │ %-8zu │ %s │\n",
			i + 1, cases[i].label, expected, got,
			pass ? GREEN EMOJI_CHECK " PASS" RST : RED EMOJI_CROSS " FAIL" RST);
	}

	PRINT_TABLE_FOOTER();
}

// ╔════════════════════════════════════════════════════════════════════════════╗
// ║                             ft_strcpy TESTS                                ║
// ╚════════════════════════════════════════════════════════════════════════════╝

static void run_strcpy_tests(void)
{
	const t_strcpy_case cases[] = {
		{"empty string",      ""},
		{"single char",       "Z"},
		{"short ascii",       "Hello"},
		{"sentence",          "Copying into destination"},
		{"with newline",      "First\nSecond"},
		{"numbers",           "12345"},
		{"special chars",     "!@#$%"},
		{"mixed content",     "abc123!@#"},
		{"whitespace only",   "   \t\n   "},
		{"longer string",     "Assembly is powerful!"},
	};
	char expected_buf[128];
	char got_buf[128];
	const size_t total = sizeof(cases) / sizeof(*cases);

	PRINT_HEADER("ft_strcpy - String Copy");
	PRINT_TABLE_HEADER();

	for (size_t i = 0; i < total; ++i) {
		memset(expected_buf, 'X', sizeof(expected_buf));
		memset(got_buf, 'X', sizeof(got_buf));

		strcpy(expected_buf, cases[i].input);
		char *ret = ft_strcpy(got_buf, cases[i].input);

		const int content_match = (strcmp(expected_buf, got_buf) == 0);
		const int return_correct = (ret == got_buf);
		const int pass = content_match && return_correct;
		RECORD_RESULT(pass);

		char expected_display[12], got_display[12];
		snprintf(expected_display, sizeof(expected_display), "\"%.6s%s\"",
			cases[i].input, strlen(cases[i].input) > 6 ? ".." : "");
		snprintf(got_display, sizeof(got_display), "\"%.6s%s\"",
			got_buf, strlen(got_buf) > 6 ? ".." : "");

		printf("  │ " CYAN "%-3zu" RST " │ %-30.30s │ %-8s │ %-8s │ %s │\n",
			i + 1, cases[i].label, expected_display, got_display,
			pass ? GREEN EMOJI_CHECK " PASS" RST : RED EMOJI_CROSS " FAIL" RST);
	}

	PRINT_TABLE_FOOTER();
}

// ╔════════════════════════════════════════════════════════════════════════════╗
// ║                             ft_strcmp TESTS                                ║
// ╚════════════════════════════════════════════════════════════════════════════╝

static void run_strcmp_tests(void)
{
	const t_strcmp_case cases[] = {
		{"identical empty",     "",       ""},
		{"identical short",     "abc",    "abc"},
		{"identical long",      "hello world", "hello world"},
		{"lhs shorter",         "abc",    "abcd"},
		{"rhs shorter",         "abcd",   "abc"},
		{"different at start",  "Xbc",    "abc"},
		{"different at mid",    "abXdef", "abYdef"},
		{"different at end",    "abcX",   "abcY"},
		{"case sensitivity",    "Case",   "case"},
		{"uppercase vs lower",  "ABC",    "abc"},
		{"numbers vs letters",  "123",    "abc"},
		{"space difference",    "a b",    "a  b"},
		{"null char boundary",  "ab\0c",  "ab\0d"},
		{"special vs alpha",    "!@#",    "abc"},
	};
	const size_t total = sizeof(cases) / sizeof(*cases);

	PRINT_HEADER("ft_strcmp - String Compare");

	printf(DIM "  ┌─────┬────────────────────────┬──────────────────────┬──────────┬──────────┬────────┐\n");
	printf("  │ " BOLD "#" RST DIM "   │ " BOLD "Test Case" RST DIM "              │ " BOLD "Strings" RST DIM "              │ " BOLD "Expected" RST DIM " │ " BOLD "Got" RST DIM "      │ " BOLD "Status" RST DIM " │\n");
	printf("  ├─────┼────────────────────────┼──────────────────────┼──────────┼──────────┼────────┤" RST "\n");

	for (size_t i = 0; i < total; ++i) {
		const int expected = strcmp(cases[i].lhs, cases[i].rhs);
		const int got = ft_strcmp(cases[i].lhs, cases[i].rhs);

		const int pass = ((expected == 0 && got == 0)
				|| (expected < 0 && got < 0)
				|| (expected > 0 && got > 0));
		RECORD_RESULT(pass);

		char exp_sign[4], got_sign[4];
		snprintf(exp_sign, sizeof(exp_sign), "%s", expected < 0 ? "<0" : (expected > 0 ? ">0" : "=0"));
		snprintf(got_sign, sizeof(got_sign), "%s", got < 0 ? "<0" : (got > 0 ? ">0" : "=0"));

		char strings[24];
		snprintf(strings, sizeof(strings), "\"%.6s\" vs \"%.6s\"", cases[i].lhs, cases[i].rhs);

		printf("  │ " CYAN "%-3zu" RST " │ %-22.22s │ %-20.20s │ " YELLOW "%-8s" RST " │ %-8s │ %s │\n",
			i + 1, cases[i].label, strings, exp_sign, got_sign,
			pass ? GREEN EMOJI_CHECK " PASS" RST : RED EMOJI_CROSS " FAIL" RST);
	}

	printf(DIM "  └─────┴────────────────────────┴──────────────────────┴──────────┴──────────┴────────┘" RST "\n");
}

// ╔════════════════════════════════════════════════════════════════════════════╗
// ║                             ft_strdup TESTS                                ║
// ╚════════════════════════════════════════════════════════════════════════════╝

static void run_strdup_tests(void)
{
	const t_strdup_case cases[] = {
		{"empty string",     ""},
		{"single char",      "X"},
		{"short string",     "Hello"},
		{"with spaces",      "Hello World"},
		{"with newline",     "Line1\nLine2"},
		{"numbers",          "1234567890"},
		{"special chars",    "!@#$%^&*()"},
		{"mixed content",    "abc123!@#XYZ"},
		{"whitespace",       "  \t\n  "},
		{"longer string",    "This is a longer test string for strdup"},
	};
	const size_t total = sizeof(cases) / sizeof(*cases);

	PRINT_HEADER("ft_strdup - String Duplicate");
	PRINT_TABLE_HEADER();

	for (size_t i = 0; i < total; ++i) {
		char *expected = strdup(cases[i].input);
		char *got = ft_strdup(cases[i].input);

		int pass = 0;
		if (expected && got) {
			pass = (strcmp(expected, got) == 0) && (got != cases[i].input);
		}
		RECORD_RESULT(pass);

		char exp_disp[12], got_disp[12];
		snprintf(exp_disp, sizeof(exp_disp), "\"%.6s%s\"",
			expected ? expected : "(null)", (expected && strlen(expected) > 6) ? ".." : "");
		snprintf(got_disp, sizeof(got_disp), "\"%.6s%s\"",
			got ? got : "(null)", (got && strlen(got) > 6) ? ".." : "");

		printf("  │ " CYAN "%-3zu" RST " │ %-30.30s │ %-8s │ %-8s │ %s │\n",
			i + 1, cases[i].label, exp_disp, got_disp,
			pass ? GREEN EMOJI_CHECK " PASS" RST : RED EMOJI_CROSS " FAIL" RST);

		free(expected);
		free(got);
	}

	PRINT_TABLE_FOOTER();
}

// ╔════════════════════════════════════════════════════════════════════════════╗
// ║                             ft_write TESTS                                 ║
// ╚════════════════════════════════════════════════════════════════════════════╝

static void run_write_tests(void)
{
	PRINT_HEADER("ft_write - Write to File Descriptor");

	printf(DIM "  ┌─────┬────────────────────────────────┬────────────────────┬────────────────────┬────────┐\n");
	printf("  │ " BOLD "#" RST DIM "   │ " BOLD "Test Case" RST DIM "                      │ " BOLD "libc result" RST DIM "        │ " BOLD "asm result" RST DIM "         │ " BOLD "Status" RST DIM " │\n");
	printf("  ├─────┼────────────────────────────────┼────────────────────┼────────────────────┼────────┤" RST "\n");

	int test_num = 1;
	int pipefd[2];

	// Test 1: Write to stdout (redirect to /dev/null to avoid cluttering output)
	{
		const char *payload = "Test output to stdout\n";
		int saved_stdout = dup(STDOUT_FILENO);
		int devnull = open("/dev/null", O_WRONLY);
		dup2(devnull, STDOUT_FILENO);

		errno = 0;
		ssize_t std_written = write(STDOUT_FILENO, payload, strlen(payload));
		int std_errno = errno;

		errno = 0;
		ssize_t asm_written = ft_write(STDOUT_FILENO, payload, strlen(payload));
		int asm_errno = errno;

		dup2(saved_stdout, STDOUT_FILENO);
		close(saved_stdout);
		close(devnull);

		int pass = (std_written == asm_written && std_errno == asm_errno);
		RECORD_RESULT(pass);

		char libc_res[24], asm_res[24];
		snprintf(libc_res, sizeof(libc_res), "ret=%zd err=%d", std_written, std_errno);
		snprintf(asm_res, sizeof(asm_res), "ret=%zd err=%d", asm_written, asm_errno);

		printf("  │ " CYAN "%-3d" RST " │ %-30s │ %-18s │ %-18s │ %s │\n",
			test_num++, "write to stdout", libc_res, asm_res,
			pass ? GREEN EMOJI_CHECK " PASS" RST : RED EMOJI_CROSS " FAIL" RST);
	}

	// Test 2: Write to file
	{
		const char *payload = "Writing to file test\n";
		int fd = open("/tmp/libasm_test.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (fd >= 0) {
			errno = 0;
			ssize_t std_written = write(fd, payload, strlen(payload));
			int std_errno = errno;
			close(fd);

			fd = open("/tmp/libasm_test.txt", O_WRONLY | O_TRUNC);
			errno = 0;
			ssize_t asm_written = ft_write(fd, payload, strlen(payload));
			int asm_errno = errno;
			close(fd);
			unlink("/tmp/libasm_test.txt");

			int pass = (std_written == asm_written && std_errno == asm_errno);
			RECORD_RESULT(pass);

			char libc_res[24], asm_res[24];
			snprintf(libc_res, sizeof(libc_res), "ret=%zd err=%d", std_written, std_errno);
			snprintf(asm_res, sizeof(asm_res), "ret=%zd err=%d", asm_written, asm_errno);

			printf("  │ " CYAN "%-3d" RST " │ %-30s │ %-18s │ %-18s │ %s │\n",
				test_num++, "write to file", libc_res, asm_res,
				pass ? GREEN EMOJI_CHECK " PASS" RST : RED EMOJI_CROSS " FAIL" RST);
		}
	}

	// Test 3: Invalid fd (-1)
	{
		const char *payload = "test";
		errno = 0;
		ssize_t std_written = write(-1, payload, strlen(payload));
		int std_errno = errno;
		errno = 0;
		ssize_t asm_written = ft_write(-1, payload, strlen(payload));
		int asm_errno = errno;

		int pass = (std_written == asm_written && std_errno == asm_errno);
		RECORD_RESULT(pass);

		char libc_res[24], asm_res[24];
		snprintf(libc_res, sizeof(libc_res), "ret=%zd err=%d", std_written, std_errno);
		snprintf(asm_res, sizeof(asm_res), "ret=%zd err=%d", asm_written, asm_errno);

		printf("  │ " CYAN "%-3d" RST " │ %-30s │ %-18s │ %-18s │ %s │\n",
			test_num++, "invalid fd (-1)", libc_res, asm_res,
			pass ? GREEN EMOJI_CHECK " PASS" RST : RED EMOJI_CROSS " FAIL" RST);
	}

	// Test 4: Closed pipe
	if (pipe(pipefd) == 0) {
		close(pipefd[0]);
		close(pipefd[1]);
		const char *payload = "test";

		errno = 0;
		ssize_t std_written = write(pipefd[1], payload, strlen(payload));
		int std_errno = errno;
		errno = 0;
		ssize_t asm_written = ft_write(pipefd[1], payload, strlen(payload));
		int asm_errno = errno;

		int pass = (std_written == asm_written && std_errno == asm_errno);
		RECORD_RESULT(pass);

		char libc_res[24], asm_res[24];
		snprintf(libc_res, sizeof(libc_res), "ret=%zd err=%d", std_written, std_errno);
		snprintf(asm_res, sizeof(asm_res), "ret=%zd err=%d", asm_written, asm_errno);

		printf("  │ " CYAN "%-3d" RST " │ %-30s │ %-18s │ %-18s │ %s │\n",
			test_num++, "closed pipe fd", libc_res, asm_res,
			pass ? GREEN EMOJI_CHECK " PASS" RST : RED EMOJI_CROSS " FAIL" RST);
	}

	// Test 5: Write zero bytes
	{
		int fd = open("/dev/null", O_WRONLY);
		if (fd >= 0) {
			errno = 0;
			ssize_t std_written = write(fd, "test", 0);
			int std_errno = errno;
			errno = 0;
			ssize_t asm_written = ft_write(fd, "test", 0);
			int asm_errno = errno;
			close(fd);

			int pass = (std_written == asm_written && std_errno == asm_errno);
			RECORD_RESULT(pass);

			char libc_res[24], asm_res[24];
			snprintf(libc_res, sizeof(libc_res), "ret=%zd err=%d", std_written, std_errno);
			snprintf(asm_res, sizeof(asm_res), "ret=%zd err=%d", asm_written, asm_errno);

			printf("  │ " CYAN "%-3d" RST " │ %-30s │ %-18s │ %-18s │ %s │\n",
				test_num++, "write zero bytes", libc_res, asm_res,
				pass ? GREEN EMOJI_CHECK " PASS" RST : RED EMOJI_CROSS " FAIL" RST);
		}
	}

	printf(DIM "  └─────┴────────────────────────────────┴────────────────────┴────────────────────┴────────┘" RST "\n");
}

// ╔════════════════════════════════════════════════════════════════════════════╗
// ║                              ft_read TESTS                                 ║
// ╚════════════════════════════════════════════════════════════════════════════╝

static void run_read_tests(void)
{
	PRINT_HEADER("ft_read - Read from File Descriptor");

	printf(DIM "  ┌─────┬────────────────────────────────┬────────────────────┬────────────────────┬────────┐\n");
	printf("  │ " BOLD "#" RST DIM "   │ " BOLD "Test Case" RST DIM "                      │ " BOLD "libc result" RST DIM "        │ " BOLD "asm result" RST DIM "         │ " BOLD "Status" RST DIM " │\n");
	printf("  ├─────┼────────────────────────────────┼────────────────────┼────────────────────┼────────┤" RST "\n");

	int test_num = 1;
	char std_buf[128];
	char asm_buf[128];
	int pipefd[2];

	// Test 1: Read from makefile
	{
		int fd_std = open("makefile", O_RDONLY);
		int fd_asm = open("makefile", O_RDONLY);
		if (fd_std >= 0 && fd_asm >= 0) {
			memset(std_buf, 0, sizeof(std_buf));
			memset(asm_buf, 0, sizeof(asm_buf));

			errno = 0;
			ssize_t std_read = read(fd_std, std_buf, 64);
			int std_errno = errno;
			errno = 0;
			ssize_t asm_read = ft_read(fd_asm, asm_buf, 64);
			int asm_errno = errno;

			int pass = (std_read == asm_read && std_errno == asm_errno
				&& memcmp(std_buf, asm_buf, 64) == 0);
			RECORD_RESULT(pass);

			char libc_res[24], asm_res[24];
			snprintf(libc_res, sizeof(libc_res), "ret=%zd err=%d", std_read, std_errno);
			snprintf(asm_res, sizeof(asm_res), "ret=%zd err=%d", asm_read, asm_errno);

			printf("  │ " CYAN "%-3d" RST " │ %-30s │ %-18s │ %-18s │ %s │\n",
				test_num++, "read from makefile", libc_res, asm_res,
				pass ? GREEN EMOJI_CHECK " PASS" RST : RED EMOJI_CROSS " FAIL" RST);

			close(fd_std);
			close(fd_asm);
		}
	}

	// Test 2: Read from /dev/zero
	{
		int fd_std = open("/dev/zero", O_RDONLY);
		int fd_asm = open("/dev/zero", O_RDONLY);
		if (fd_std >= 0 && fd_asm >= 0) {
			memset(std_buf, 'X', sizeof(std_buf));
			memset(asm_buf, 'X', sizeof(asm_buf));

			errno = 0;
			ssize_t std_read = read(fd_std, std_buf, 32);
			int std_errno = errno;
			errno = 0;
			ssize_t asm_read = ft_read(fd_asm, asm_buf, 32);
			int asm_errno = errno;

			int pass = (std_read == asm_read && std_errno == asm_errno
				&& memcmp(std_buf, asm_buf, 32) == 0);
			RECORD_RESULT(pass);

			char libc_res[24], asm_res[24];
			snprintf(libc_res, sizeof(libc_res), "ret=%zd err=%d", std_read, std_errno);
			snprintf(asm_res, sizeof(asm_res), "ret=%zd err=%d", asm_read, asm_errno);

			printf("  │ " CYAN "%-3d" RST " │ %-30s │ %-18s │ %-18s │ %s │\n",
				test_num++, "read from /dev/zero", libc_res, asm_res,
				pass ? GREEN EMOJI_CHECK " PASS" RST : RED EMOJI_CROSS " FAIL" RST);

			close(fd_std);
			close(fd_asm);
		}
	}

	// Test 3: Invalid fd
	{
		errno = 0;
		ssize_t std_read = read(-1, std_buf, sizeof(std_buf));
		int std_errno = errno;
		errno = 0;
		ssize_t asm_read = ft_read(-1, asm_buf, sizeof(asm_buf));
		int asm_errno = errno;

		int pass = (std_read == asm_read && std_errno == asm_errno);
		RECORD_RESULT(pass);

		char libc_res[24], asm_res[24];
		snprintf(libc_res, sizeof(libc_res), "ret=%zd err=%d", std_read, std_errno);
		snprintf(asm_res, sizeof(asm_res), "ret=%zd err=%d", asm_read, asm_errno);

		printf("  │ " CYAN "%-3d" RST " │ %-30s │ %-18s │ %-18s │ %s │\n",
			test_num++, "invalid fd (-1)", libc_res, asm_res,
			pass ? GREEN EMOJI_CHECK " PASS" RST : RED EMOJI_CROSS " FAIL" RST);
	}

	// Test 4: Closed pipe
	if (pipe(pipefd) == 0) {
		close(pipefd[0]);
		close(pipefd[1]);

		errno = 0;
		ssize_t std_read = read(pipefd[0], std_buf, sizeof(std_buf));
		int std_errno = errno;
		errno = 0;
		ssize_t asm_read = ft_read(pipefd[0], asm_buf, sizeof(asm_buf));
		int asm_errno = errno;

		int pass = (std_read == asm_read && std_errno == asm_errno);
		RECORD_RESULT(pass);

		char libc_res[24], asm_res[24];
		snprintf(libc_res, sizeof(libc_res), "ret=%zd err=%d", std_read, std_errno);
		snprintf(asm_res, sizeof(asm_res), "ret=%zd err=%d", asm_read, asm_errno);

		printf("  │ " CYAN "%-3d" RST " │ %-30s │ %-18s │ %-18s │ %s │\n",
			test_num++, "closed pipe fd", libc_res, asm_res,
			pass ? GREEN EMOJI_CHECK " PASS" RST : RED EMOJI_CROSS " FAIL" RST);
	}

	// Test 5: Read zero bytes
	{
		int fd = open("makefile", O_RDONLY);
		if (fd >= 0) {
			errno = 0;
			ssize_t std_read = read(fd, std_buf, 0);
			int std_errno = errno;
			close(fd);

			fd = open("makefile", O_RDONLY);
			errno = 0;
			ssize_t asm_read = ft_read(fd, asm_buf, 0);
			int asm_errno = errno;
			close(fd);

			int pass = (std_read == asm_read && std_errno == asm_errno);
			RECORD_RESULT(pass);

			char libc_res[24], asm_res[24];
			snprintf(libc_res, sizeof(libc_res), "ret=%zd err=%d", std_read, std_errno);
			snprintf(asm_res, sizeof(asm_res), "ret=%zd err=%d", asm_read, asm_errno);

			printf("  │ " CYAN "%-3d" RST " │ %-30s │ %-18s │ %-18s │ %s │\n",
				test_num++, "read zero bytes", libc_res, asm_res,
				pass ? GREEN EMOJI_CHECK " PASS" RST : RED EMOJI_CROSS " FAIL" RST);
		}
	}

	printf(DIM "  └─────┴────────────────────────────────┴────────────────────┴────────────────────┴────────┘" RST "\n");
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
	printf("║   " EMOJI_ROCKET "  " WHITE "L I B A S M   T E S T   S U I T E" MAGENTA "   " EMOJI_ROCKET "                     ║\n");
	printf("║                                                                  ║\n");
	printf("║   " RST DIM "x86-64 Assembly Functions for macOS" MAGENTA BOLD "                         ║\n");
	printf("║                                                                  ║\n");
	printf("╚══════════════════════════════════════════════════════════════════╝" RST "\n");

	run_strlen_tests();
	run_strcpy_tests();
	run_strcmp_tests();
	run_strdup_tests();
	run_write_tests();
	run_read_tests();

	print_summary();

	return (g_failed_tests > 0 ? 1 : 0);
}
