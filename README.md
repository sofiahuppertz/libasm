# libasm

A collection of functions written in x86-64 assembly (NASM syntax) for macOS.

## Assembly Topics Learned

### 1. Registers

#### General Purpose Registers
| 64-bit | 32-bit | 16-bit | 8-bit (low) | 8-bit (high) |
|--------|--------|--------|-------------|--------------|
| rax    | eax    | ax     | al          | ah           |
| rbx    | ebx    | bx     | bl          | bh           |
| rcx    | ecx    | cx     | cl          | ch           |
| rdx    | edx    | dx     | dl          | dh           |
| rsi    | esi    | si     | sil         | -            |
| rdi    | edi    | di     | dil         | -            |
| r8-r15 | r8d-r15d | r8w-r15w | r8b-r15b | -          |

#### Special Registers
- `rsp` - Stack pointer
- `rbp` - Base pointer
- `rip` - Instruction pointer

### 2. Calling Convention (System V AMD64 ABI)

#### Function Arguments
| Argument | Register |
|----------|----------|
| 1st      | rdi      |
| 2nd      | rsi      |
| 3rd      | rdx      |
| 4th      | rcx      |
| 5th      | r8       |
| 6th      | r9       |
| Return   | rax      |

#### Register Preservation
| Callee-saved (must preserve) | Caller-saved (can clobber) |
|------------------------------|---------------------------|
| rbx, rbp, r12-r15            | rax, rcx, rdx, rsi, rdi, r8-r11 |

### 3. Basic Instructions

#### Data Movement
```asm
mov     rax, rbx        ; rax = rbx
mov     rax, [rbx]      ; rax = *rbx (dereference)
mov     [rax], rbx      ; *rax = rbx
mov     rax, 42         ; rax = 42 (immediate)
lea     rax, [rbx + 8]  ; rax = rbx + 8 (load effective address)
movzx   eax, bl         ; zero-extend byte to dword
```

#### Arithmetic
```asm
add     rax, rbx        ; rax += rbx
sub     rax, rbx        ; rax -= rbx
inc     rax             ; rax++
dec     rax             ; rax--
imul    rax, rbx        ; rax *= rbx (signed)
```

#### Bitwise
```asm
xor     eax, eax        ; eax = 0 (efficient zeroing idiom)
and     rax, rbx        ; rax &= rbx
or      rax, rbx        ; rax |= rbx
```

### 4. Comparison and Jumps

#### Comparison Instructions
```asm
cmp     rax, rbx        ; compare rax with rbx (sets flags)
test    rax, rax        ; test if rax is zero (more efficient than cmp rax, 0)
```

#### Conditional Jumps
| Instruction | Meaning | Use case |
|-------------|---------|----------|
| je / jz     | Jump if equal / zero | Signed & unsigned |
| jne / jnz   | Jump if not equal / not zero | Signed & unsigned |
| jg / jnle   | Jump if greater | Signed |
| jl / jnge   | Jump if less | Signed |
| ja / jnbe   | Jump if above | Unsigned |
| jb / jnae   | Jump if below | Unsigned |

### 5. Stack Operations

```asm
push    rbx             ; Save rbx to stack (rsp -= 8, [rsp] = rbx)
pop     rbx             ; Restore rbx from stack (rbx = [rsp], rsp += 8)
```

The stack grows **downward** (toward lower addresses):
```
High addresses
+------------------+
|  previous data   |
+------------------+ <- rsp (before push)
|   pushed value   |
+------------------+ <- rsp (after push)
Low addresses
```

### 6. Memory Addressing

#### Struct Access
```asm
; For a struct: { void *data; struct *next; }
; Offset 0: data (8 bytes)
; Offset 8: next (8 bytes)

mov     rax, [rdi]          ; rax = node->data
mov     rax, [rdi + 8]      ; rax = node->next
```

### 7. Loops

#### Simple Loop
```asm
.loop:
    cmp     rdi, 0
    je      .done
    ; ... do work ...
    dec     rdi
    jmp     .loop
.done:
```

#### Linked List Traversal
```asm
.loop:
    test    rdi, rdi        ; while (current != NULL)
    jz      .done
    ; process node...
    mov     rdi, [rdi + 8]  ; current = current->next
    jmp     .loop
.done:
```

### 8. Function Calls

#### Calling External Functions
```asm
extern _malloc
extern _free

    mov     rdi, 16         ; size = 16 bytes
    call    _malloc         ; rax = malloc(16)
```

#### Calling Function Pointers
```asm
    mov     r12, rsi        ; save function pointer in callee-saved register
    ; ... later ...
    mov     rdi, arg1
    mov     rsi, arg2
    call    r12             ; call through function pointer
```

### 9. System Calls (macOS)

```asm
; macOS syscall numbers have 0x2000000 base
_ft_write:
    mov     rax, 0x2000004  ; SYS_WRITE
    syscall
    jc      .error          ; jump if carry (error occurred)
    ret
```

### 10. NASM Syntax

```asm
global _function_name       ; export symbol
extern _external_func       ; import symbol

section .text               ; code section

_function_name:
    ; function body
    ret

.local_label:               ; local label (starts with .)
    ; ...
```

### 11. Efficient Idioms

```asm
xor     eax, eax            ; Zero register (preferred over mov eax, 0)
test    rax, rax            ; Check if zero (preferred over cmp rax, 0)
lea     rax, [rbx + rcx*4]  ; Multiply and add in one instruction
```

### 12. Recursion

```asm
; Recursive function example (atoi_base)
_recursive_atoi:
    ; Base case: empty string
    cmp     byte[rsi], 0
    je      .ret_zero

    ; Save callee-saved registers
    push    rbx
    push    r12

    ; ... process and recurse ...

    call    _recursive_atoi     ; recursive call

    ; Combine results
    imul    r13, rax            ; result * base_len
    add     rax, r13

    ; Restore and return
    pop     r12
    pop     rbx
    ret

.ret_zero:
    xor     eax, eax
    ret
```

Key points:
- Must save callee-saved registers before recursive call
- Need clear base case to stop recursion
- Stack grows with each call (be mindful of stack depth)

### 13. Common Patterns

#### Function Prologue/Epilogue
```asm
_my_function:
    push    rbx             ; Save callee-saved registers
    push    r12

    ; ... function body ...

    pop     r12             ; Restore in reverse order
    pop     rbx
    ret
```

#### Pointer-to-Pointer Pattern (for list head updates)
```asm
    ; rbx = &head (pointer to head pointer)
    mov     rax, [rbx]      ; rax = *head (current node)
    mov     rcx, [rax + 8]  ; rcx = current->next
    mov     [rbx], rcx      ; *head = current->next (removes head)
```

## Building

```bash
make            # Build mandatory functions
make bonus      # Build mandatory + bonus functions
make test       # Build and run mandatory tests
make test_list  # Build and run list tests
make clean      # Remove object files
make fclean     # Remove all generated files
make re         # Rebuild everything
```

## Project Structure

```
libasm/
├── mandatory/
│   ├── ft_strlen.s
│   ├── ft_strcpy.s
│   ├── ft_strcmp.s
│   ├── ft_strdup.s
│   ├── ft_write.s
│   └── ft_read.s
├── bonus/
│   ├── ft_atoi_base/
│   │   ├── ft_atoi_base.s
│   │   └── ...
│   └── ft_list/
│       ├── ft_list_push_front.s
│       ├── ft_list_size.s
│       ├── ft_list_sort.s
│       └── ft_list_remove_if.s
├── tests/
│   ├── test_mandatory.c
│   ├── test_atoi_base.c
│   └── test_list.c
└── makefile
```
