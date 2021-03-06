/****************************************************************************
 ****************************************************************************
 ***
 ***   This header was automatically generated from a Linux kernel header
 ***   of the same name, to make information necessary for userspace to
 ***   call into the kernel available to libc.  It contains only constants,
 ***   structures, and macros generated from the original header, and thus,
 ***   contains no copyrightable information.
 ***
 ***   To edit the content of this header, modify the corresponding
 ***   source file (e.g. under external/kernel-headers/original/) then
 ***   run bionic/libc/kernel/tools/update_all.py
 ***
 ***   Any manual change here will be lost the next time this script will
 ***   be run. You've been warned!
 ***
 ****************************************************************************
 ****************************************************************************/
#ifndef __LINUX_RAW_H
#define __LINUX_RAW_H
#define _LINUX_RAW_H
#define _LINUX_RAW_H_
#define _UAPI_LINUX_RAW_H
#define _UAPI_LINUX_RAW_H_
#define _UAPI__LINUX_RAW_H
#define _UAPI__LINUX_RAW_H_
#define __LINUX_RAW_H_
#include <museum/8.0.0/bionic/libc/linux/types.h>
#define RAW_SETBIND _IO(0xac, 0)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define RAW_GETBIND _IO(0xac, 1)
struct raw_config_request {
  int raw_minor;
  __u64 block_major;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  __u64 block_minor;
};
#define MAX_RAW_MINORS CONFIG_MAX_RAW_DEVS
#endif
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
