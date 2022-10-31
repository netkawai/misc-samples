//
//  uname-wrapper.c
//  Today
//
//  Created by Masanori Kawai on 2022-10-20.
//

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h> // isdigit

#include <mach/mach.h>
#include <mach/host_info.h>
#include <mach/mach_host.h>
#include <mach/mach_error.h>


#include "uname-wrapper.h"


/* Because I don't want to use bsd/sys/utsname.h... */
char sysname[255];
char nodename[255];
char release[255];
char version[KERNEL_VERSION_MAX];
char machine[255];
char processor[255];

/* So we can be (theoretically) renamed */
char *progname;

/* Define some use flags */
#define SYSNAME      1
#define NODENAME     2
#define RELEASE      4
#define VERSION      8
#define MACHINE      16
#define ARCH         32

host_t host_self();


/*
 * This function will send a single Mach message in an attempt
 * to obtain the kernel version.  Once it has the reply from Mach,
 * it will iterate the string and obtain a realease number, system
 * name, and kernel version string.
 *
 * This is trying to be as efficient as possible, but will probably
 * end up being a very gnarly hack with no real value whatsoever.
 */
void detectOpSysVersion(void)
{
  kern_return_t kret;
  kernel_version_t kver;

  kret = host_kernel_version(host_self(), kver);

  if (kret != KERN_SUCCESS) {
    mach_error("host_kernel_version() failed.", kret);
  } else {
    int idx = 0, len = 0;

    /* Copy the kernel version to a string */
    strcpy(version, kver);
    len = strlen(version);

    /* Remove trailing \n, if it exists */
    if (version[len -1] == '\n')
      version[len - 1] = '\0';

    /* This is the slowest part of the whole thing... */
    for (idx = 0; idx < len; idx++) {

      /* Version: [0-9].[0-9]: */
      if (isdigit(version[idx]) && version[idx + 1] == '.' &&
          isdigit(version[idx + 2]) && version[idx + 3] == ':')
      {
        sprintf(release,
                "%c.%c\0",
                version[idx],
                version[idx + 2]);
        break;
      }

      /* Version: [0-9].[0-9].[0-9]: */
      if (isdigit(version[idx]) && version[idx + 1] == '.' &&
          isdigit(version[idx + 2]) && version[idx + 3] == '.' &&
          isdigit(version[idx + 4]) && version[idx + 5] == ':')
      {
        sprintf(release,
                "%c.%c.%c\0",
                version[idx],
                version[idx + 2],
                version[idx + 4]);
        break;
      }

      /* Version: [0-9][0-9].[0-9].[0-9]: */
      if (isdigit(version[idx]) && isdigit(version[idx + 1]) &&
          version[idx + 2] == '.' && isdigit(version[idx + 3]) &&
          version[idx + 4] == '.' && isdigit(version[idx + 5]) &&
          version[idx + 6] == ':')
      {
        sprintf(release,
                "%c%c.%c.%c\0",
                version[idx],
                version[idx + 1],
                version[idx + 3],
                version[idx + 5]);
        break;
      }
    }

    /*
     * Compute the system name using the major version.
     *
     * If release[1] != '.', then it's Darwin... and this program would
     * look very stupid if it printed 'NEXTSTEP 10.0' on an OSX box.
     */
    if (release[1] == '.') {
      switch (release[0]) {
        case '0':
        case '1':
        case '2':
        case '3':
          sprintf(sysname, "NEXTSTEP\0");
          break;

        case '4':
         sprintf(sysname, "OPENSTEP\0");
          break;

        case '5':
          sprintf(sysname, "Rhapsody\0");
          /* barf barf, Apple. */
          for (idx = 0; idx < len; idx++)
            if (version[idx] == '\n')
              version[idx] = ' ';
          break;

        default:
          sprintf(sysname, "Darwin\0");
      }
    } else {
      sprintf(sysname, "Darwin\0");
    }
  } /* if (kret != ... */

  /*
   * Let's get the hostname on our way out
   */
  gethostname(nodename, 255);
}

/*
 * Make another Mach call and get the hardware information.
 */
void detectHardware(void)
{
  kern_return_t kret;
  struct host_basic_info kbi;
  unsigned int count = HOST_BASIC_INFO_COUNT;
  char *pCpuType, *pCpuSubtype;

  kret = host_info(host_self(),
                   HOST_BASIC_INFO,
                   (host_info_t)&kbi,
                   &count);

  if (kret != KERN_SUCCESS) {
    mach_error("host_info() failed.", kret);
  } else {
    slot_name(kbi.cpu_type,       /* Architecture */
              kbi.cpu_subtype,    /* Processor */
              &pCpuType,
              &pCpuSubtype);

    bcopy(pCpuType, machine, 32);
    bcopy(pCpuSubtype, processor, 32);
  }
}


/*
 * public
 */
void getMachineString(char *machineStr)
{

  detectOpSysVersion();
  detectHardware();
  
    // Make BAD_ACCESS(SEGFAULT) artifically
    //  machineStr -= 200000000000000;
  
    sprintf(machineStr,"%s %s %s %s %s %s",sysname,nodename,release,version,machine,processor);

}

