    global _ft_strdup

    extern _ft_strlen
    extern _ft_strcpy
    extern _malloc

    section .text

_ft_strdup:
    push rdi             
    call _ft_strlen    
    inc rax       
    mov rdi, rax      
    call _malloc           
    test rax, rax
    je .malloc_fail
    pop rsi               
    mov rdi, rax         
    call _ft_strcpy
    ret

.malloc_fail:
    xor eax, eax
    ret