#include <windows.h>
#define FAMILY_ID        0x0f00   // EAX[11:8] - Bit 11 thru 8 contains family

unsigned int cpu_family_id()
{
    unsigned int reg_eax = 0;
    unsigned int reg_edx = 0;
    unsigned int junk, junk1;
    unsigned int vendor_id[3] = {0, 0, 0};
	 __try {                    // verify cpuid instruction is supported
		 __asm__("cpuid" : "=a" (reg_eax), "=b" (*vendor_id),
			   "=c" (vendor_id[2]), "=d" (vendor_id[1]) : "a" (0));
		 __asm__("cpuid" : "=a" (reg_eax), "=b"(junk), "=c"(junk1),
				"=d" (reg_edx) : "a" (1));
					   // eax contains cpu family type info
					 // edx has info whether Hyper-Threading
// Technology is available
	 }
	 __except (EXCEPTION_EXECUTE_HANDLER ) {
			 return NO_CPUID;        // CPUID is not supported and so
									// it is not a recent family CPU
	 }
	return (reg_eax & FAMILY_ID);
}