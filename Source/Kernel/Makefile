CFLAGS = -c \
-Wall \
-Wextra \
-nostdlib \
-fno-builtin \
-nostartfiles \
-nodefaultlibs \
-fno-exceptions \
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
-mcmodel=kernel\
#-fexceptions \
#-O2

SRC += $(wildcard Core/*.asm Core/*.cpp)
SRC += $(wildcard C++/*.cpp)
SRC += $(wildcard Library/*.cpp)

SRC += $(wildcard DeviceManager/*.cpp)
SRC += $(wildcard MemoryManager/*.cpp)
SRC += $(wildcard SyscallManager/*.asm SyscallManager/*.cpp)
SRC += $(wildcard TaskManager/*.cpp)
SRC += $(wildcard TaskManager/V86/*.asm TaskManager/V86/*.cpp)

SRC += $(wildcard Devices/*.cpp)
SRC += $(wildcard Devices/Display/*.cpp)
SRC += $(wildcard Devices/Keyboard/*.cpp)

SRC += $(wildcard VTManager/*.cpp)



#SRC += $(wildcard Devices/ATA/*.cpp)
#SRC += $(wildcard Devices/Mouse/*.cpp)
#SRC += $(wildcard Devices/Character/*.cpp)
#SRC += $(wildcard Devices/PCI/*.cpp)

#SRC += $(wildcard VFS/*.cpp)
#SRC += $(wildcard FileSystems/RamFS/*.cpp)
#SRC += $(wildcard FileSystems/SFS/*.cpp)

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
	@gcc $(CFLAGS) $^ -o $@

%.a.o: %.asm
	@echo "[ASM]     " $@
	@nasm $^ -f elf64 -o $@
