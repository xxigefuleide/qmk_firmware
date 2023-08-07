BLUETOOTH_ENABLE = yes
BLUETOOTH_DRIVER = custom
NO_USB_STARTUP_CHECK = no
OPT_DEFS += -DCORTEX_ENABLE_WFI_IDLE=TRUE

include keyboards/planck_blueism/blueism/blueism.mk
