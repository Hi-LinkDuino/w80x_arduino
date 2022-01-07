#ifndef __CSI_CONFIG_H__
#define __CSI_CONFIG_H__

#ifdef __cplusplus
extern "C" {
#endif

#define CONFIG_CHIP_SL04 1
#define CONFIG_KERNEL_NONE 1
#define CONFIG_HAVE_VIC 1
#define CONFIG_SEPARATE_IRQ_SP 1
#define CONFIG_ARCH_INTERRUPTSTACK 4096
#define CONFIG_IRQ_VECTOR_SIZE   256
#define USE_UART0_PRINT            1

#ifdef CONFIG_KERNEL_NONE
#define CONFIG_SYSTEM_SECURE  1
#endif

#ifdef __cplusplus
}
#endif

#endif
