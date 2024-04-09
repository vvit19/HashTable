global AsmStrcmp

section .text
AsmStrcmp:
        vmovdqa ymm1, yword [rdi]
        vpcmpeqb ymm0, ymm1, yword [rsi]
        vpmovmskb rax, ymm0
        ret
