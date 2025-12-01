global _ft_strlen

section .text

_ft_strlen:
    mov rsi, rdi

.find_null:
    cmp byte [rsi], 0
    je .done
    inc rsi
    jmp .find_null
.done:
    sub rsi, rdi
    mov rax, rsi
    ret

