"""napari reader for Pink (extended) PGM images, 2D and 3D.

Pink writes an extended Netpbm header:

    P5                  <- magic (raw bytes); other variants below
    #xdim 1             <- optional voxel-size comment lines
    #ydim 1
    #zdim 1
    rs cs ds            <- dimensions: "rs cs" (2D) or "rs cs ds" (3D)
    255                 <- maxval
    <raw binary data>   <- row-major, x fastest, then y, then z

Magic / data type mapping used here:
    P5 / P7 : raw bytes  -> uint8  (maxval 255)  or uint16 (maxval 65535)
    P8      : raw int32              PB : ascii int32
    P9      : raw float32            PA : ascii float32
    P2      : ascii bytes -> uint8
"""

import numpy as np

_RAW = {"P5", "P7", "P8", "P9"}
_ASCII = {"P2", "PA", "PB"}


def napari_get_reader(path):
    """Return a reader callable if `path` looks like a Pink PGM, else None."""
    if isinstance(path, list):
        path = path[0]
    if not isinstance(path, str) or not path.lower().endswith((".pgm", ".pgm3d")):
        return None
    return reader_function


def _dtype_for(magic, maxval):
    if magic in ("P5", "P7", "P2"):
        return np.dtype("<u2") if maxval > 255 else np.dtype("u1")
    if magic in ("P8", "PB"):
        return np.dtype("<i4")
    if magic in ("P9", "PA"):
        return np.dtype("<f4")
    raise ValueError("unsupported PGM magic: %s" % magic)


def _read_pink_pgm(path):
    """Parse a Pink PGM file. Returns (numpy_array, scale_or_None)."""
    with open(path, "rb") as f:
        magic = f.readline().split()[0].decode("ascii")

        xdim = ydim = zdim = None
        dims = None
        maxval = None

        # 1st non-comment line = dimensions
        while dims is None:
            line = f.readline()
            if not line:
                raise ValueError("unexpected end of header (dimensions)")
            s = line.lstrip()
            if s.startswith(b"#"):
                parts = s[1:].split()
                if len(parts) == 2:
                    key, val = parts[0].decode(), parts[1].decode()
                    if key == "xdim":
                        xdim = float(val)
                    elif key == "ydim":
                        ydim = float(val)
                    elif key == "zdim":
                        zdim = float(val)
                continue
            dims = [int(t) for t in s.split()]

        # next non-comment line = maxval
        while maxval is None:
            line = f.readline()
            if not line:
                raise ValueError("unexpected end of header (maxval)")
            s = line.lstrip()
            if s.startswith(b"#"):
                continue
            maxval = int(s.split()[0])

        rest = f.read()

    dt = _dtype_for(magic, maxval)

    if magic in _ASCII:
        arr = np.array(rest.split(), dtype=dt)
    else:
        arr = np.frombuffer(rest, dtype=dt)

    # dims = [rs, cs] or [rs, cs, ds] (a trailing band count of 1 is ignored)
    rs = dims[0]
    cs = dims[1]
    ds = dims[2] if len(dims) >= 3 else 1

    n = rs * cs * ds
    arr = np.asarray(arr[:n])

    if ds > 1:
        arr = arr.reshape((ds, cs, rs))          # (z, y, x) for napari
        scale = (zdim, ydim, xdim) if None not in (xdim, ydim, zdim) else None
    else:
        arr = arr.reshape((cs, rs))              # (y, x)
        scale = (ydim, xdim) if None not in (xdim, ydim) else None

    if scale is not None and not all(s and s > 0 for s in scale):
        scale = None

    return arr, scale


def reader_function(path):
    """napari reader: returns a list of (data, add_kwargs, layer_type)."""
    paths = [path] if isinstance(path, str) else path
    layers = []
    for p in paths:
        data, scale = _read_pink_pgm(p)
        name = p.replace("\\", "/").split("/")[-1]
        meta = {"name": name}
        if scale is not None:
            meta["scale"] = scale
        layers.append((data, meta, "image"))
    return layers
