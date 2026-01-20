global ft_write
extern __errno_location

;jc = jump if carry

section .text

ft_write:
    mov rax, 1         ; SYS_WRITE for Linux
    syscall
    cmp rax, 0
    jl .error
    ret
.error:
    neg rax            ; Make error positive
    mov r8, rax        ; Save error code
    call __errno_location wrt ..plt
    mov [rax], r8      ; Set errno
    mov rax, -1        ; Return -1
    ret