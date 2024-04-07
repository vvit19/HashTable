global AsmStrcmp

section .text
AsmStrcmp:
        vmovdqu ymm1, yword [rdi]
        vpcmpeqb ymm0, ymm1, yword [rsi]
        vpmovmskb rax, ymm0
        ret
