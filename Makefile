Build: BootLoader Kernel
	@cat Mount/BootLoader.bin > Mount/Boot.bin
	@cat Mount/Kernel.bin >> Mount/Boot.bin
	@cat Mount/BootLoader.bin >> Mount/Boot.bin
	
Clean:
	@cd Source/BootLoader; make Clean -s;
	@cd Source/Kernel; make Clean -s;
	@rm -rf Mount/Boot.bin
	@rm -rf Mount/Linker.map
	
Debug: Build
	@qemu-system-x86_64 -hda Mount/Boot.bin -boot c -m 64 -monitor stdio -no-kvm -vga std
	
	
BootLoader:
	@cd Source/BootLoader; make -s
	
Kernel:
	@cd Source/Kernel; make -s