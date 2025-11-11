global _ft_write
extern ___error

section .text

_ft_write:
    mov rax, 0x2000004 ; SYS_WRITE 
    syscall
    jc .error
    ret
.error:
    mov r8, rax
    call ___error
    mov [rax], r8
    mov rax, -1
    ret