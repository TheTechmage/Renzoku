Creating a config file

- Don't pick a tokenizer
- Don't search for [config] parsers
- Look at serialization

Planned Config file format:
```
Watcher {
    Filter = "*.cpp", "*.hpp", "*.c", "*.h", "include/*.hxx";
    Include = "Makefile";
    Exclude = "src/Config.cpp";
}
Compiler {
    Command = "make";
    Enabled = True;
}
Tester {
    Command = "make test";
    IgnoreStatus = 2,3,4;
    Enabled = True;
}
Coverage { }; // Future DLL?
Runner {
    Command = "bin/program --workdir=\"$cwd\"";
    Enabled = True;
}
```
