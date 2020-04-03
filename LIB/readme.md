        "ld": ["-Wl,--gc-sections", "-Wl,--wrap,main", "-Wl,--wrap,_malloc_r",
               "-Wl,--wrap,_free_r", "-Wl,--wrap,_realloc_r", "-Wl,--wrap,_memalign_r",
               "-Wl,--wrap,_calloc_r", "-Wl,--wrap,exit", "-Wl,--wrap,atexit",
               "-Wl,-n", "--specs=nano.specs"]


			   thumbv7em-none-eabihf, for the Cortex-M4F and Cortex-M7F processors
			   https://en.wikipedia.org/wiki/ARM_Cortex-M#Cortex-M4

https://docs.rust-embedded.org/discovery/05-led-roulette/build-it.html

https://sourceware.org/binutils/docs/ld/Options.html
https://manpages.debian.org/testing/binutils-arm-none-eabi/arm-none-eabi-ld.1.en.html


https://sourceware.org/newlib/libc.html#Syscalls
