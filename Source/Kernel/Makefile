CFLAGS = -c \
-Wall \
-Wextra \
-nostdlib \
-fno-builtin \
-nostartfiles \
-nodefaultlibs \
-fno-rtti \
-fno-stack-protector \
-std=c++0x \
-ffreestanding \
-mno-mmx \
-mno-sse \
-mno-sse2 \
-mno-sse3 \
-mno-3dnow \
-mno-red-zone \
-I . \
-mcmodel=kernel

SRC += $(wildcard Core/*.asm Core/*.cpp)
SRC += $(wildcard C++/*.cpp)
SRC += $(wildcard C++/Exceptions/*.cpp)
SRC += $(wildcard Exceptions/*.cpp)
SRC += $(wildcard Library/*.cpp)
SRC += $(wildcard Library/GL/*.cpp)

SRC += $(wildcard DeviceManager/*.cpp)
SRC += $(wildcard MemoryManager/*.cpp)
SRC += $(wildcard SyscallManager/*.asm SyscallManager/*.cpp)
SRC += $(wildcard TaskManager/*.asm TaskManager/*.cpp)
SRC += $(wildcard TaskManager/V86/*.asm TaskManager/V86/*.cpp)
SRC += $(wildcard VTManager/*.cpp)
SRC += $(wildcard UserManager/*.cpp)
SRC += $(wildcard NetworkManager/*.cpp)

SRC += $(wildcard Devices/*.cpp)
SRC += $(wildcard Devices/ATA/*.cpp)
SRC += $(wildcard Devices/Display/*.cpp)
SRC += $(wildcard Devices/Keyboard/*.cpp)
SRC += $(wildcard Devices/Mouse/*.cpp)
SRC += $(wildcard Devices/PCI/*.cpp)
SRC += $(wildcard Devices/Network/*.cpp)
SRC += $(wildcard Devices/Network/ne2000/*.cpp)

SRC += $(wildcard VFS/*.cpp)
SRC += $(wildcard Linker/*.cpp)
SRC += $(wildcard Shell/*.cpp)
SRC += $(wildcard FileSystems/RamFS/*.cpp)

OBJSx = $(SRC:.cpp=.o)
OBJS = $(OBJSx:.asm=.a.o)

Build: $(OBJS)
	@ld -T Kernel.ld -o ../../Mount/Kernel.bin $(OBJS) -Map ../../Mount/Linker.map
	@echo "OS has been builded successful!"
Clean:
	@rm -rf $(OBJS)
	@rm -rf ../../Mount/Kernel.bin
	
%.o: %.cpp
	@echo "[C++]     " $@
	@g++ $(CFLAGS) $^ -o $@

%.a.o: %.asm
	@echo "[ASM]     " $@
	@nasm $^ -f elf64 -o $@
