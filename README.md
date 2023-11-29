# asm6502

## Requirements:

By default, this project uses `cmake` to generate all the build files. On my machine, this happens to default to `make`.
I've configured `CMakeLists.txt` to use `clang`. Remove that declaration if you want to use a different 
generator/compiler.

This code runs great on Linux. I'm building with `clang=16.0.6` but `gcc=13.2` works great too. I am not going to offer any Windows support.

## Building:

This is the simplest way to get started:

```shell
cd asm6502
mkdir build
cd build
cmake ..
make asm6502
# Optional debug build (w/ debug symbols and extra debug output for the lexer):
make asm6502-dbg
```

## Feedback

An artist was presenting their latest painting at a gallery's open-house event, when a famous art critic walked up, 
and said:

> Critic: "May I give you some criticism?"
> Artist: "Sure thing."
> Critic: "It's useless."
> Artist: "I know, but you can give it to me anyway."

This is all to say that this project was mostly done as an exercise in systems programming and mucking around with 
bitwise operations. 

I really can't be asked to improve it stylistically, beyond refactoring some logic here and there. I know my naming 
conventions aren't particularly consistent, and sometimes are kinda misleading (e.g., I use a variable called `opcode` 
sometimes, and the `OP` struct has a member called `.opcode` when it should probably be `.mnemonic` or something.) Alas,
my time is much better spent implementing one of the `TODO`s or working on the emulator counterpart to this project.

If you have any suggestions for improvements, like where you think I can reasonably save a couple clock cycles, then 
I'll be glad to implement those kinds of changes.

## Contributing

This is strictly a personal learning project.
Please don't open a pull request or I will cry :'(
