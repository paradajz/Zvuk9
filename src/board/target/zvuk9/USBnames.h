#if defined(FW_APP)
#define USB_PRODUCT UNICODE_STRING("Zvuk9")
#elif defined(FW_BOOT)
#define USB_PRODUCT UNICODE_STRING("Zvuk9 DFU")
#elif defined(FW_CDC)
#define USB_PRODUCT UNICODE_STRING("Zvuk9 CDC")
#endif
