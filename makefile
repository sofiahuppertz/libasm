NAME        := libasm.a

# Mandatory sources
SRCS        := mandatory/ft_strlen.s mandatory/ft_strcpy.s mandatory/ft_strcmp.s \
               mandatory/ft_strdup.s mandatory/ft_write.s mandatory/ft_read.s
OBJS        := $(SRCS:.s=.o)

# Bonus sources - ft_atoi_base
BONUS_SRCS  := bonus/ft_atoi_base/ft_atoi_base.s \
               bonus/ft_atoi_base/atoi_base_recursive.s \
               bonus/ft_atoi_base/ft_strchr.s \
               bonus/ft_atoi_base/ft_iswhitespace.s \
               bonus/ft_atoi_base/has_duplicates.s \
               bonus/ft_atoi_base/get_digit_value.s

# Bonus sources - ft_list
BONUS_SRCS  += bonus/ft_list/ft_list_push_front.s \
               bonus/ft_list/ft_list_size.s \
               bonus/ft_list/ft_list_sort.s \
               bonus/ft_list/ft_list_remove_if.s

BONUS_OBJS  := $(BONUS_SRCS:.s=.o)

NASM        := nasm
NASMFLAGS   := -f macho64

AR          := ar
ARFLAGS     := rcs

RM          := rm -f

CC          := arch -x86_64 cc
CFLAGS      := -I.
LDFLAGS     := -L.
LDLIBS      := -lasm
TEST_BIN    := test
TEST_ATOI   := test_atoi_base
TEST_LIST   := test_list

.PHONY: all clean fclean re bonus

all: $(NAME)

$(NAME): $(OBJS)
	$(AR) $(ARFLAGS) $@ $^
	$(RM) $(OBJS)

bonus: $(OBJS) $(BONUS_OBJS)
	$(AR) $(ARFLAGS) $(NAME) $^
	$(RM) $(OBJS) $(BONUS_OBJS)

%.o: %.s
	$(NASM) $(NASMFLAGS) -o $@ $<

$(TEST_BIN): $(NAME) tests/test_mandatory.c
	$(CC) tests/test_mandatory.c $(CFLAGS) $(LDFLAGS) $(LDLIBS) -o $@

$(TEST_ATOI): bonus tests/test_atoi_base.c
	$(CC) tests/test_atoi_base.c $(CFLAGS) $(LDFLAGS) $(LDLIBS) -o $@

$(TEST_LIST): bonus tests/test_list.c
	$(CC) tests/test_list.c $(CFLAGS) $(LDFLAGS) $(LDLIBS) -o $@

clean:
	$(RM) $(OBJS) $(BONUS_OBJS)

fclean: clean
	$(RM) $(NAME) $(TEST_BIN) $(TEST_ATOI) $(TEST_LIST)

re: fclean all

