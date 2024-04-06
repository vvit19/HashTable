global AsmHashCrc32

section .text
AsmHashCrc32:
        mov eax, 0xEDB88320
        push rdi
        add rdi, rsi    ; rdi += strlen
        lea rsi, [rdi]
        pop rdi
.While:
        crc32 eax, byte [rdi]
        inc rdi
        cmp rdi, rsi
        jl .While
        ret
