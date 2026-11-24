#!/bin/bash
shopt -s globstar nullglob

nodependencies="-I./ -ffreestanding -fno-pie -fno-pic -fno-stack-protector -fno-builtin -mno-mmx -mno-sse -mno-sse2" 
cppdependencies="$nodependencies -fno-exceptions -fno-rtti -fno-use-cxa-atexit -fno-threadsafe-statics"

compile(){

	rm -rf ./build/
	mkdir -p ./build
	if [[ ! $? ]]; then
		exit
	fi

	nasm -f bin -o "./build/stage1.bin" "stage1.asm"

	#elf32 allows to be linked together
	nasm -f elf32 -o "./build/stage2.o" "stage2.asm"
	nasm -f elf32 -o "./build/kernel.o" "./system/kernel/kernel.s"
	nasm -f elf32 -o "./build/syscalls.o" "./system/syscalls.s"
	nasm -f elf32 -o "./build/idtasm.o" "./system/kernel/idt/idt.s"
	nasm -f elf32 -o "./build/israsm.o" "./system/kernel/idt/isr/isrs.asm"
	nasm -f elf32 -o "./build/stupidFixasm.o" "./system/biosIntFromC.asm"
	# gcc -m32 -c ./kernel/kernel_init.c -o ./build/kernel_init.o $nodependencies -O2 -nostdlib
	# gcc -m32 -c ./kernel/console.c -o ./build/console.o $nodependencies -O2 -nostdlib
	# gcc -m32 -c ./drivers/vga.c -o ./build/vga.o $nodependencies -O2 -nostdlib
	# gcc -m32 -c ./drivers/vbe.c -o ./build/vbe.o $nodependencies -O2 -nostdlib
	# gcc -m32 -c ./drivers/keyboard.c -o ./build/keyboard.o $nodependencies -O2 -nostdlib
	# gcc -m32 -c ./drivers/crypto.c -o ./build/crypto.o $nodependencies -O2 -nostdlib
	# gcc -m32 -c ./drivers/string.c -o ./build/string.o $nodependencies -O2 -nostdlib
	# gcc -m32 -c ./kernel/syscall_wrappers.c -o ./build/syscall_wrappers.o $nodependencies -O2 -nostdlib

	for file in ./feature_testing_and_debug/**/*.asm ; do #TO ADD A DIR TO COMPULOE STUFF, ADD HERE AND IN THE CPP ONE
		if [ -f "$file" ]; then
			name=$(basename "$file" .asm) # cool linux command htat gives the name
			nasm -f elf32 -o "./build/asm$name.o" $file
		fi
	done

	for file in ./system/**/*.c ; do #TO ADD A DIR TO COMPULOE STUFF, ADD HERE AND IN THE CPP ONE
		if [ -f "$file" ]; then
			name=$(basename "$file" .c) # cool linux command htat gives the name
			gcc -m32 -c "$file" -o ./build/"c$name".o $nodependencies -O2 -nostdlib
		fi
	done

	for file in ./system/**/*.cpp ; do #TO ADD A DIR TO COMPULOE STUFF, ADD HERE AND IN THE CPP ONE
		if [ -f "$file" ]; then
			name=$(basename "$file" .cpp) # cool linux command htat gives the name
			g++ -m32 -c "$file" -o ./build/"cpp$name".o $cppdependencies -O2 -nostdlib
		fi
	done


	ld -m elf_i386 --no-pie -T linker.ld \
	./build/*.o \
	-o ./build/linked.bin --oformat binary || { echo "Linker didn't link. Maybe something didn't compile?"; exit 1; }


	if [[ ! -f "./build/stage1.bin" ]]; then
		echo "Stage1 didnt compile"
		exit
	fi
	# if [[ ! $? ]]; then
	# 	echo "Linker didn't link. Maybe something didn't compile?"
	# 	exit
	# fi

	cat ./build/stage1.bin ./build/linked.bin > ./build/image.img

	# padding:
	truncate -s 64k ./build/image.img

}

run() {
	local kvmargs=""
	if [ -w /dev/kvm ]; then
		kvmargs="-enable-kvm -cpu host"
	else
		kvmargs="-accel tcg,thread=multi -cpu qemu64 -display sdl"
	fi
	qemu-system-x86_64 \
	  $kvmargs \
	  -smp 2 \
	  -m 4G \
	  -vga std -display gtk \
	  -drive file="./build/image.img",if=ide,format=raw \
	  -nic user

}


if [[ "$*" != *-s* ]]; then
	compile
fi

run 
