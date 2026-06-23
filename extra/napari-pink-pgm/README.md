# napari-pink-pgm

A minimal [napari](https://napari.org) plugin that reads **Pink (extended) PGM**
images — 2D and 3D — including the files produced by Pink tools such as
`shortestpath3d`.

Supported variants: raw byte (`P5`, maxval 255 → uint8, 65535 → uint16),
raw int32 (`P8`), raw float32 (`P9`), and their ASCII forms (`P2`, `PB`, `PA`).
Voxel sizes from `#xdim/#ydim/#zdim` header comments are applied as the layer
scale.

## Install

```bash
cd napari-pink-pgm
pip install -e .
```

(Installs into the same Python environment as napari. `napari` and `npe2`
must be available there; `pip install napari[all]` if needed.)

## Use

Open napari and drag a `.pgm` file onto the canvas, or:

```bash
napari myvolume.pgm
```

From Python:

```python
import napari
viewer = napari.Viewer()
viewer.open("myvolume.pgm")          # 3D volumes load as a (z, y, x) image
napari.run()
```

A 3D volume arrives as a single image layer of shape `(depth, cols, rows)`;
scroll the slider to move through z.
