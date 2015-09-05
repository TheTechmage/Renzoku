#if 0
We will not use a "#pragma once" or a "#include guards" within this file due to
the fact that it needs to be included multiple times.
#endif
#ifndef CONFIG_SECTION
	#error "CONFIG_SECTION macro hasn't been defined!"
#endif

#ifndef COMPILE_STEP_POS
#define COMPILE_STEP_POS 0
#endif
CONFIG_SECTION(Watch)
CONFIG_SECTION(Compile)
CONFIG_SECTION(Test)
//CONFIG_SECTION(Coverage)
//CONFIG_SECTION(BrowserRefresher)
CONFIG_SECTION(Program)
