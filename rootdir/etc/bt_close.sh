#!/system/bin/sh
btconfig /dev/smd3 rawcmd 0x03 0x0c  0x00 &
setprop persist.sys.closebt 0
exit 0
