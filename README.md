Pink is an image-processing library developed at ESIEE Engineering for research and teaching purposes. It contains implementations of over 200 algorithms for image segmentation and filtering. Most operators come from mathematical morphology, but the library also includes operators from other fields. Pink is free software released under the CeCILL license.

## History

Pink was primarily created, developed, and maintained by Michel Couprie during his tenure as a professor at ESIEE Paris. Development continued for over 30 years.

If you have used previous versions of Pink, you will notice that this release has a different structure and that many components have been removed. Since the original version was not actively maintained for many years, I decided to temporarily remove some parts, e.g., the Python bindings, and to focus on reintroducing the most important command-line tools and the core library under a simpler structure maintained with CMake.

## License

Pink is distributed under the CeCILL license. See the LICENSE file for details.

## Installation

Installation follows a standard CMake workflow. Once the project is compiled, running `make install` is not required and not recommended. Instead, set up the environment variables described below because this version still uses the old-style lookup-table mechanism. The scripts included with this release search for compiled tools via the `PINK` environment variable.

## Environment variables

Below is a list of environment variables that should be set in your `~/.bashrc` (or equivalent) file.

```bash
export PINK="$HOME/Projects/Pink_Resurected" # <--------- Edit this to your Pink project root

# Depends on which packages you compiled — listed below. Comment out the ones you do not use.
export PINK_ARITH="$PINK/build/tools/ARITH"
export PINK_CONVERT="$PINK/build/tools/CONVERT"
export PINK_MORPHO="$PINK/build/tools/MORPHO"
export PINK_CONNECT="$PINK/build/tools/CONNECT"
export PINK_GA="$PINK/build/tools/GA"
export PINK_TOPO="$PINK/build/tools/TOPO"
export PINK_ORDRES="$PINK/build/tools/ORDRES"
export PINK_DRAW="$PINK/build/tools/DRAW"
export PINK_GEO="$PINK/build/tools/GEO"
export PINK_HISTO="$PINK/build/tools/HISTO"
export PINK_SIGNAL="$PINK/build/tools/SIGNAL"
export PINK_MESH3D="$PINK/build/tools/MESH3D"
export PINK_DIVERS="$PINK/build/tools/DIVERS"
export PINK_EXPERIMENTAL="$PINK/build/tools/EXPERIMENTAL"

export PATH="$PINK_ARITH:$PINK_CONVERT:$PINK_MORPHO:$PINK_CONNECT:$PINK_GA:$PINK_TOPO:$PINK_ORDRES:$PINK_DRAW:$PINK_GEO:$PINK_HISTO:$PINK_SIGNAL:$PINK_MESH3D:$PINK_DIVERS:$PINK_EXPERIMENTAL:$PATH"
```

After editing `~/.bashrc`, reload it with `source ~/.bashrc` (or open a new shell) to apply the changes.

## Contributing

Contributions are welcome via pull requests. If you submit a change, please include a short description of the problem you are solving and include tests or examples when appropriate.

## Contact

If you have questions or need help, please open an issue or submit a pull request on the repository.
