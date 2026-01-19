global _ft_list_remove_if

; rdi = **begin_list
; rsi = *data_ref
; rdx = cmp
; rcx = free_func

_ft_list_remove_if:
    push rbx
    push r12
    push r13
    push r14

    mov     rbx, rdi ; begin_list
    mov     r12, rsi   ; data_ref
    mov     r13, rdx    ; cmp
    mov     r14, rcx    ; free



