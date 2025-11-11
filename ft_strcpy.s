global _ft_strcpy

section .text

_ft_strcpy:
    mov rax, rdi
.loop:
    mov dl, [rsi]
    mov [rdi], dl
    inc rdi
    inc rsi
    cmp dl, 0
    jne .loop
    ret
