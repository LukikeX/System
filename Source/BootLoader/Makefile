BootLoader: ../../Mount/BootLoader.bin
	@echo "BootLoader has been compiled successful!"

Clean:
	@rm -rf ../../Mount/BootLoader.bin

../../Mount/BootLoader.bin: $(wildcard *.asm)
	@echo "Compiling Bootloader..."
	@nasm -f bin BootLoader.asm -o ../../Mount/BootLoader.bin