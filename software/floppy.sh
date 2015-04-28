make-pc
i386-elf-objcopy -O binary cmd cmd.bin
dd conv=sync if=cmd.bin of=/dev/fd0
