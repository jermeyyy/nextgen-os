[BITS 32]

SECTION .text

[global _cpuid_present]
_cpuid_present:
    pushf
    pushf
    pop  eax
    mov  ecx,eax
    xor  eax,0x200000
    push eax
    popf
    pushf
    pop  eax
    popf
    xor  eax,ecx
    setnz al
    ret

[global _cpuid_vendor]
_cpuid_vendor:
    push ebp
    mov ebp, esp ; set up stack frame

    xor eax, eax ; clear out eax

    cpuid ; get the vendor string

    push ebp           ; safeguard
    mov ebp, [ebp + 8] ; get the char *
    
    mov [ebp], ebx     ; 1st dword of text
    mov [ebp + 4], edx ; 2nd  "    "   "
    mov [ebp + 8], ecx ; 3rd  "    "   "
    xor ebx, ebx
    mov [ebp + 12], ebx ; null terminator
 
    pop eax ; dummy
    pop ebp
    ret

[global _cpuid_info]
_cpuid_info:
    push ebp
    mov ebp, esp ; set up stack frame

    mov eax, [ebp + 8]

    cpuid ; get the cpuid

    push ebp
    mov ebp, [ebp + 12] ; get the struct *

    mov [ebp], eax  
    mov [ebp + 4], ebx 
    mov [ebp + 8], ecx 
    mov [ebp + 12], edx 
 
    pop eax
    pop ebp
    ret


