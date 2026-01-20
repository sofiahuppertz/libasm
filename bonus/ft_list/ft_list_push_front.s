global ft_list_push_front

extern malloc

section .text

; Inserts a new node at the front of a linked list
;
; Prototype: void ft_list_push_front(t_list **begin_list, void *data)
;
; Arguments:
;   rdi = begin_list (t_list **) - pointer to the head pointer
;   rsi = data (void *)          - data to store in new node
;
; t_list struct layout (16 bytes total):
;   offset 0:  data  (qword - 8 bytes)
;   offset 8:  next  (qword - 8 bytes)

ft_list_push_front:
    ; Save callee-saved registers
    push    rbx
    push    r12

    mov     rbx, rdi            ; rbx = begin_list (t_list **)
    mov     r12, rsi            ; r12 = data

    ; Allocate memory for new node
    mov     rdi, 16             ; sizeof(t_list) = 16 bytes
    call    malloc
    test    rax, rax
    jz      .malloc_fail        ; if malloc returned NULL, exit

    ; Initialize new node
    mov     qword[rax], r12     ; new_node->data = data
    mov     rcx, [rbx]          ; rcx = *begin_list (current head)
    mov     qword[rax + 8], rcx ; new_node->next = current head

    ; Update head pointer to new node
    mov     [rbx], rax          ; *begin_list = new_node

.done:
    pop     r12
    pop     rbx
    ret

.malloc_fail:
    xor     eax, eax            ; return NULL (optional, function is void)
    jmp     .done