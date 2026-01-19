global _has_duplicates

section .text

; rsp is the pointer to the array of seen (stack pointer)
; rdi is a ptr to str

_has_duplicates:
    ; allocate 256 bytes for seen[]
    sub rsp, 256

    ; zero-initialize
    xor eax, eax ; zeroing idiom
    mov rcx, 256  ; rcx is the loop counter register
    lea  rsi, [rsp] ; rsi = &seen[0]

.zero_loop:
    mov byte[rsi], al
    inc rsi
    loop .zero_loop

.next_char:
    movzx eax, byte[rdi]
    cmp al, 0
    je .no_duplicates

    cmp     byte [rsp+rax], 1
    je      .is_duplicated
    mov     byte [rsp+rax], 1

    inc rdi
    jmp .next_char

.is_duplicated:
    mov      eax, 1
    add     rsp, 256
    ret

.no_duplicates:
    xor     eax,eax
    add     rsp, 256
    ret
