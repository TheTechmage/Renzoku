#if 0
We will not use a "#pragma once" or a "#include guards" within this file due to
the fact that it needs to be included multiple times.
#endif
#ifndef CONFIG_SECTION
	#error "CONFIG_SECTION macro hasn't been defined!"
#endif
CONFIG_SECTION(Watcher)
CONFIG_SECTION(Compiler)
CONFIG_SECTION(Tester)
//CONFIG_SECTION(Coverage)
//CONFIG_SECTION(BrowserRefresher)
CONFIG_SECTION(Runner)
