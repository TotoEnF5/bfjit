# bfjit

A cool brainfuck jut-in-time compiler

## Building

### Dependencies

You will need to install [lightning](https://www.gnu.org/software/lightning/) on your system. You can find the instructions for how to do so [here](https://www.gnu.org/software/lightning/manual/html_node/Installation.html).

bfjit uses [premake](https://premake.github.io) as its build system. Make sure it is installed too.

### Compiling

```bash
premake5 gmake
make
```

The binaries will be located in `bin/debug`.

## Performance on my machine

On `bf/mandelbrot.bf`:

- With `--compiled`: 1.004s
- With `--interpreted` (no JIT): 833.15s
- With out options (JIT enabled): 673.13s

Not the performance I hoped for, but hey it's *a bit* faster at least.
