/* neXtgen - CPUID */

#ifndef CPUID_H
#define CPUID_H

typedef struct TAGcpuid_info_t {
    unsigned int eax;
    unsigned int ebx;
    unsigned int ecx;
    unsigned int edx;
} cpuid_info_t;

int cpuid_present();

void cpuid_vendor(char *);

void cpuid_info(int, cpuid_info_t *);



// EDX
#define CPUID_PAE       (1<<6)
#define CPUID_APIC      (1<<9)
#define CPUID_PSE36     (1<<17)
#define CPUID_ACPI      (1<<22)
#define CPUID_MMX       (1<<23)
#define CPUID_SSE       (1<<25)
#define CPUID_SSE2      (1<<26)
#define CPUID_HTT       (1<<28)
#define CPUID_IA64      (1<<30)
// ECX
#define CPUID_SSE3        (1<<0)
#define CPUID_SSE4_1    (1<<19)
#define CPUID_SSE4_2    (1<<20)
#define CPUID_VMX        (1<<5)


#define CPUID_NEEDED_MASK (CPUID_FPU | CPUID_VME | CPUID_MSR | CPUID_PAE | CPUID_APIC | CPUID_PSE36)

#endif
