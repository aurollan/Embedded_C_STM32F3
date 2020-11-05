# ARM specific in linker script
In `02.LINKER_SCRIPT/OWN/readme.md` we have cover all linker script generalities.
Now we are going to focus on part specific to our ARM cortex microcontroller.

#### Linker script paricularities
##### ARM to THUMB
 The .glue_7t and.glue_7 sections are synthesized by the compiler when you
 specify the ARM-THUMB interworking option. The sections contain the
 “call veneers” between THUMB and ARM code and are accessed frequently by every
 call between ARM and THUMB. It's typically advantageous to place this
 smallamount of hot-spot code in RAM.

##### C++ .preinit/init/finit array
You can use function pointer array to call those functions at startup.
There are used by C++ compiler or by newlib and libc library which provide the 
function you need to work with this feature.

##### arm ex table
manage exeption handling


