global _has_duplicates

section .text

; rsp is the pointer to the array of seen
; rdi is a ptr to str

_has_duplicates:
    ; allocate 256 bytes for seen[]
    sub rsp, 256

    ; zero-initialize
    xor eax, eax
    mov rcx, 256  ; rcx is the loop counter register
    lea  rsi, [rsp] ; rsi = &seen[0]

.zero_loop:
    mov byte[rsi], al
    inc rsi
    loop .zero_loop

.next_char:
    mov dl, byte[rdi]
    cmp dl, 0
    je .no_duplicates

    movzx   eax, dl
    cmp     byte [rsp+rax], 1
    je      .duplicate
    mov     byte [rsp+rax], 1

    inc rdi
    jmp .next_char

.is_duplicated:
    mv      eax, 1
    add     rsp, 256
    ret

.no_duplicates
    xor     eax,eax
    add     rsp, 256
    ret
