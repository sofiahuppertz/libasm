global _ft_iswhitespace

section .text

_ft_iswhitespace:
    cmp edi, 32
    je      .is_whitespace
    cmp edi, 9
    je      .is_whitespace
    cmp edi, 11
    je      .is_whitespace
    cmp edi, 12
    je      .is_whitespace
    cmp edi, 13
    je      .is_whitespace
    cmp edi, 10
    je      .is_whitespace

    mov rax, 0
    ret

.is_whitespace:
    mov rax, 1
    ret

