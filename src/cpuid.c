/* neXtgen - CPUID */
#include "system.h"


// this is set here
int syscall_method = 0;

void do_cpuid_test() {
    char *vendorstr;
    cpuid_info_t id;

    if (cpuid_present() == 0) printf("For some reason your CPU does not support CPUID.\n");

    cpuid_vendor(vendorstr);
    printf(" CPUID:  ");
    puts(vendorstr);
    cpuid_info(1, &id);
    printf(" ");
    if ((id.edx & CPUID_APIC) == CPUID_APIC) printf("APIC ");
    if ((id.edx & CPUID_ACPI) == CPUID_ACPI) printf("ACPI ");
    if ((id.edx & CPUID_PAE) == CPUID_PAE) printf("PAE ");
    if ((id.edx & CPUID_PSE36) == CPUID_PSE36) printf("PSE36 ");
    if ((id.edx & CPUID_MMX) == CPUID_MMX) printf("MMX ");
    if ((id.edx & CPUID_SSE) == CPUID_SSE) printf("SSE ");
    if ((id.edx & CPUID_SSE2) == CPUID_SSE2) printf("SSE2 ");
    if ((id.ecx & CPUID_SSE3) == CPUID_SSE3) printf("SSE3 ");
    if ((id.ecx & CPUID_SSE4_1) == CPUID_SSE4_1) printf("SSE4.1 ");
    if ((id.ecx & CPUID_SSE4_2) == CPUID_SSE4_2) printf("SSE4.2 ");
    if ((id.edx & CPUID_HTT) == CPUID_HTT) printf("HTT ");
    if ((id.edx & CPUID_IA64) == CPUID_IA64) printf("IA64 ");
    printf("\n");
}
