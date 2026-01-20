global ft_list_remove_if

extern free

section .text

; Removes all nodes where cmp(node->data, data_ref) == 0
;
; Prototype: void ft_list_remove_if(t_list **begin_list, void *data_ref,
;                                   int (*cmp)(), void (*free_fct)(void *))
;
; Arguments:
;   rdi = begin_list (t_list **) - pointer to the head pointer
;   rsi = data_ref (void *)      - reference data to compare against
;   rdx = cmp (function pointer) - comparison function
;   rcx = free_fct (function pointer) - function to free node data (can be NULL)
;
; Register usage:
;   rbx = begin_list (t_list **)
;   r12 = data_ref
;   r13 = cmp function pointer
;   r14 = free_fct function pointer
;   r15 = current node pointer

ft_list_remove_if:
    push    rbx
    push    r12
    push    r13
    push    r14
    push    r15

    mov     rbx, rdi            ; begin_list
    mov     r12, rsi            ; data_ref
    mov     r13, rdx            ; cmp
    mov     r14, rcx            ; free_fct

    ; Edge case checks
    test    rbx, rbx
    jz      .done
    test    r13, r13
    jz      .done

.loop:
    mov     r15, [rbx]          ; entry = *head_ptr
    test    r15, r15            ; if entry == NULL
    jz      .done               ;     end of list

    ; Compare entry->data with data_ref
    mov     rdi, [r15]          ; arg1 = entry->data
    mov     rsi, r12            ; arg2 = data_ref
    call    r13                 ; cmp(entry->data, data_ref)

    test    eax, eax
    jnz     .advance            ; if cmp != 0, skip this node

    ; Match found - remove node
    mov     rax, [r15 + 8]      ; rax = entry->next
    mov     [rbx], rax          ; *head_ptr = entry->next (unlink)

    ; Free entry->data
    test    r14, r14
    jz      .free_node

    mov     rdi, [r15]          ; rdi = entry->data
    call    r14                 ; free_fct(entry->data)

.free_node:
    mov     rdi, r15            ; rdi = entry
    call    free               ; free(entry)

    jmp     .loop               ; check new *head_ptr (don't advance)

.advance:
    lea     rbx, [r15 + 8]      ; pp = &entry->next
    jmp     .loop

.done:
    pop     r15
    pop     r14
    pop     r13
    pop     r12
    pop     rbx
    ret