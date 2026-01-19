#include "libasm.h"
#include "errno.h"
#include "fcntl.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "unistd.h"

#define COLOR_PASS "\033[32m"
#define COLOR_FAIL "\033[31m"
#define COLOR_INFO "\033[36m"
#define COLOR_RESET "\033[0m"

#define LOG_SECTION(title) \
	printf("\n" COLOR_INFO "==> %s" COLOR_RESET "\n", title)

#define LOG_RESULT(fn, index, status, fmt, ...)                                    \
	printf("%s[%s #%zu] " fmt "%s\n",                                            \
		(status) ? COLOR_PASS : COLOR_FAIL, fn, index, __VA_ARGS__, COLOR_RESET)

typedef struct s_strlen_case
{
	const char *label;
	const char *input;
}	t_strlen_case;

typedef struct s_strcpy_case
{
	const char *label;
	const char *input;
}	t_strcpy_case;

typedef struct s_strcmp_case
{
	const char *label;
	const char *lhs;
	const char *rhs;
}	t_strcmp_case;

static void	run_strlen_tests(void)
{
	const t_strlen_case cases[] = {
		{"empty string", ""},
		{"single char", "A"},
		{"short ascii", "Hello"},
		{"with spaces", "libasm rocks"},
		{"with newline", "line1\nline2"},
		{"longer buffer", "abcdefghijklmnopqrstuvwxyz"},
	};
	const size_t total = sizeof(cases) / sizeof(*cases);

	LOG_SECTION("ft_strlen");
	for (size_t i = 0; i < total; ++i)
	{
		const size_t expected = strlen(cases[i].input);
		const size_t got = ft_strlen(cases[i].input);
		const int pass = (expected == got);

		LOG_RESULT("ft_strlen", i + 1, pass,
			"%s \"%s\" -> expected=%zu, got=%zu",
			cases[i].label, cases[i].input, expected, got);
	}
}

static void	run_strcpy_tests(void)
{
	const t_strcpy_case cases[] = {
		{"empty string", ""},
		{"single char", "Z"},
		{"short ascii", "Hello"},
		{"sentence", "Copying into destination buffer"},
		{"with newline", "First line\nSecond line"},
	};
	char buffer[128];
	const size_t total = sizeof(cases) / sizeof(*cases);

	LOG_SECTION("ft_strcpy");
	for (size_t i = 0; i < total; ++i)
	{
		memset(buffer, 0, sizeof(buffer));
		const char *expected = strcpy(buffer, cases[i].input);

		memset(buffer, 0, sizeof(buffer));
		char *got = ft_strcpy(buffer, cases[i].input);

		const int pass = (strcmp(expected, got) == 0) && (buffer == got);

		LOG_RESULT("ft_strcpy", i + 1, pass,
			"%s \"%s\" -> expected=\"%s\", got=\"%s\"",
			cases[i].label, cases[i].input, expected, got);
	}
}

static void	run_strcmp_tests(void)
{
	const t_strcmp_case cases[] = {
		{"identical empty", "", ""},
		{"identical short", "abc", "abc"},
		{"lhs shorter", "abc", "abcd"},
		{"rhs shorter", "abcd", "abc"},
		{"different mid", "abXdef", "abYdef"},
		{"case sensitivity", "Case", "case"},
	};
	const size_t total = sizeof(cases) / sizeof(*cases);

	LOG_SECTION("ft_strcmp");
	for (size_t i = 0; i < total; ++i)
	{
		const int expected = strcmp(cases[i].lhs, cases[i].rhs);
		const int got = ft_strcmp(cases[i].lhs, cases[i].rhs);
		const int pass = ((expected == 0 && got == 0)
				|| (expected < 0 && got < 0)
				|| (expected > 0 && got > 0));

		LOG_RESULT("ft_strcmp", i + 1, pass,
			"%s \"%s\" vs \"%s\" -> expected=%d, got=%d",
			cases[i].label, cases[i].lhs, cases[i].rhs, expected, got);
	}
}

static void	run_write_tests(void)
{
	const char *payload = "Hello from ft_write\n";
	ssize_t     std_written;
	ssize_t     asm_written;
	int         std_errno;
	int         asm_errno;

	LOG_SECTION("ft_write");

	int pipefd[2];
	int idx = 1;

	/* stdout */
	errno = 0;
	std_written = write(STDOUT_FILENO, payload, strlen(payload));
	std_errno = errno;
	errno = 0;
	asm_written = ft_write(STDOUT_FILENO, payload, strlen(payload));
	asm_errno = errno;
	LOG_RESULT("ft_write", idx++,
		(std_written == asm_written && std_errno == asm_errno),
		"stdout -> libc bytes=%zd errno=%d | asm bytes=%zd errno=%d",
		std_written, std_errno, asm_written, asm_errno);

	/* invalid fd */
	errno = 0;
	std_written = write(-1, payload, strlen(payload));
	std_errno = errno;
	errno = 0;
	asm_written = ft_write(-1, payload, strlen(payload));
	asm_errno = errno;
	LOG_RESULT("ft_write", idx++,
		(std_written == asm_written && std_errno == asm_errno),
		"bad fd -> libc bytes=%zd errno=%d | asm bytes=%zd errno=%d",
		std_written, std_errno, asm_written, asm_errno);

	/* closed pipe */
	if (pipe(pipefd) == 0)
	{
		close(pipefd[0]);
		close(pipefd[1]);
		errno = 0;
		std_written = write(pipefd[1], payload, strlen(payload));
		std_errno = errno;
		errno = 0;
		asm_written = ft_write(pipefd[1], payload, strlen(payload));
		asm_errno = errno;
		LOG_RESULT("ft_write", idx++,
			(std_written == asm_written && std_errno == asm_errno),
			"closed pipe fd -> libc bytes=%zd errno=%d | asm bytes=%zd errno=%d",
			std_written, std_errno, asm_written, asm_errno);
	}
	else
	{
		LOG_RESULT("ft_write", idx++, 0,
			"pipe() failed, errno=%d", errno);
	}
}

static void	run_read_tests(void)
{
	char std_buffer[64];
	char asm_buffer[64];
	ssize_t std_read;
	ssize_t asm_read;
	int std_errno;
	int asm_errno;
	int pipefd[2];
	int idx = 1;

	LOG_SECTION("ft_read");

	int fd_std = open("main.c", O_RDONLY);
	int fd_asm = open("main.c", O_RDONLY);
	if (fd_std >= 0 && fd_asm >= 0)
	{
		memset(std_buffer, 0, sizeof(std_buffer));
		memset(asm_buffer, 0, sizeof(asm_buffer));
		errno = 0;
		std_read = read(fd_std, std_buffer, sizeof(std_buffer) - 1);
		std_errno = errno;
		errno = 0;
		asm_read = ft_read(fd_asm, asm_buffer, sizeof(asm_buffer) - 1);
		asm_errno = errno;
		LOG_RESULT("ft_read", idx++,
			(std_read == asm_read && std_errno == asm_errno
				&& memcmp(std_buffer, asm_buffer, sizeof(std_buffer)) == 0),
			"read main.c -> libc bytes=%zd errno=%d | asm bytes=%zd errno=%d",
			std_read, std_errno, asm_read, asm_errno);
		close(fd_std);
		close(fd_asm);
	}
	else
	{
		LOG_RESULT("ft_read", idx++, 0,
			"open(\"main.c\") failed, errno=%d", errno);
		if (fd_std >= 0)
			close(fd_std);
		if (fd_asm >= 0)
			close(fd_asm);
	}

	errno = 0;
	std_read = read(-1, std_buffer, sizeof(std_buffer));
	std_errno = errno;
	errno = 0;
	asm_read = ft_read(-1, asm_buffer, sizeof(asm_buffer));
	asm_errno = errno;
	LOG_RESULT("ft_read", idx++,
		(std_read == asm_read && std_errno == asm_errno),
		"bad fd -> libc bytes=%zd errno=%d | asm bytes=%zd errno=%d",
		std_read, std_errno, asm_read, asm_errno);

	if (pipe(pipefd) == 0)
	{
		close(pipefd[0]);
		close(pipefd[1]);
		errno = 0;
		std_read = read(pipefd[0], std_buffer, sizeof(std_buffer));
		std_errno = errno;
		errno = 0;
		asm_read = ft_read(pipefd[0], asm_buffer, sizeof(asm_buffer));
		asm_errno = errno;
		LOG_RESULT("ft_read", idx++,
			(std_read == asm_read && std_errno == asm_errno),
			"closed pipe fd -> libc bytes=%zd errno=%d | asm bytes=%zd errno=%d",
			std_read, std_errno, asm_read, asm_errno);
	}
	else
	{
		LOG_RESULT("ft_read", idx++, 0,
			"pipe() failed, errno=%d", errno);
	}
}

int	main(void)
{
	printf(COLOR_INFO "Running libasm test suite..." COLOR_RESET "\n");
	run_strlen_tests();
	run_strcpy_tests();
	run_strcmp_tests();
	run_write_tests();
	run_read_tests();
	printf("\n" COLOR_INFO "All tests executed." COLOR_RESET "\n");
	return (0);
}