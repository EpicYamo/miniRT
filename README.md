# PrismRT

**An experimental CPU ray tracer with a real-time interactive scene editor, written from scratch in C.**

![mandatory](https://img.shields.io/badge/lighting-full%20Phong-brightgreen)
![bonus](https://img.shields.io/badge/editor-real--time-blue)
![lang](https://img.shields.io/badge/language-C-lightgrey)

PrismRT began life as **miniRT**, the ray tracing project from the 42 common core.
That version was never submitted team circumstances changed mid-project so
instead of shelving it, I kept building. What started as "parse a scene file and
render three shapes" grew into a fully interactive playground: fly through the
scene in real time, click objects to select them, drag gizmos to move and rotate
them, edit materials and textures live, undo your mistakes, and then render a
final high-quality image with full Phong lighting, texture mapping, bump mapping
and antialiasing.

This is a **learning and experimentation project**, not a production engine.
Everything runs on the CPU by design: the goal was to understand every ray, every
intersection and every shading decision not to hide them behind a GPU API.

## Features

**Rendering**
- Full **Phong lighting model** ambient, diffuse and specular, with per-object
  shininess and specular strength (matte vs. metallic surfaces).
- **Multiple colored, movable lights** lights are first-class scene objects:
  spawn, select, drag, recolor, resize and delete them like any other object.
  While navigating, only the first two lights are evaluated for speed; the final
  render evaluates all of them.
- **Image texture mapping (XPM)** any object can load its own XPM texture at
  runtime, with independent tiling/repeat control per object.
- **Bump mapping** the texture's brightness is reused as a height field to
  perturb surface normals, faking small-scale relief without changing geometry.
- **Checkerboard pattern** computed in each object's local face coordinates,
  so it stays clean on rotated cubes, tilted planes and cylinder caps.
- **Antialiasing** supersampling on the final render pass.

**Geometry**
- Spheres, planes and cylinders (the classics), plus two extra solid primitives:
  an **axis-agnostic cube** (arbitrary orientation, ray vs. slab intersection in
  a local frame) and a **triangular prism** in two cross-section shapes
  (equilateral and right isosceles), built from 8 planar faces and intersected
  with the Möller–Trumbore algorithm.

**Editor**
- **Real-time navigation** WASD + mouse-look flight camera. `Tab` toggles
  between a low-resolution Navigate mode and a full-quality Render mode with a
  live progress display.
- **Full scene editing** click to select, move/rotate gizmos with numeric
  input, an on-screen property panel (color, size, material, texture), spawn and
  delete for every object type, and a 50-step undo/redo stack.

## Building

```
make        # builds MiniLibX and PrismRT
make re     # clean rebuild
make debug  # slower, non-native build (safe to run under valgrind)
make clean  # remove object files
make fclean # remove object files and the binary
```

The Makefile builds with `-march=native` for performance. If you need to run
under `valgrind`, use `make debug` some modern CPU instruction sets are not
understood by valgrind's instruction decoder.

## Running

```
./PrismRT scenes/25_epic_test.rt
```

Any `.rt` file following the scene format below can be loaded. Demo scenes are
included in `scenes/`.

## Controls

**Camera**

| Key | Action |
| --- | --- |
| `Tab` | Toggle Navigate (low-res, real-time) / Render (full quality) mode |
| `W A S D` | Move forward / left / back / right (Navigate mode only) |
| `Q` / `E` | Move down / up |
| `Shift` (hold) | Move faster |
| Right mouse (hold) + move | Look around |

**Selecting & editing objects** (gizmos are visible in Navigate mode)

| Key | Action |
| --- | --- |
| Left click | Select the object under the cursor |
| `C` | Deselect |
| `M` | Toggle move gizmo on the selected object |
| `R` | Toggle rotate gizmo (plane, cylinder, cube, prism) |
| Click + drag a gizmo axis/ring | Move or rotate along that axis |
| `N` (while an axis is grabbed) | Type an exact numeric offset/angle |
| `P` | Cycle through the selected object's editable properties |
| `B` | Edit the highlighted property (number, or texture filename for `TexID`) |
| `J` | Reset position to the original scene-file value |
| `K` | Reset rotation to the original scene-file value |
| `Delete` | Delete the selected object |
| `Ctrl+Z` | Undo the last move / rotate / property change / spawn / delete |
| `Y` | Cycle through lights (including invisible zero-diameter ones) |

**Spawning objects** (while nothing is selected)

| Key | Spawns |
| --- | --- |
| `1` | Sphere |
| `2` | Plane |
| `3` | Cylinder |
| `4` | Light |
| `5` | Cube |
| `6` | Right isosceles triangular prism |
| `7` | Equilateral triangular prism |

New objects appear in front of the camera, already selected and in move mode.

### Editable properties (via `P` / `B`)

- **Sphere / cube:** `R`, `G`, `B`, size, `Checker`, `Shininess`, `Specular`,
  `TexID`, `Repeat`, `Bump`.
- **Plane:** same, without a size field (infinite surface).
- **Cylinder:** same as sphere/cube, plus `Height`.
- **Triangular prism:** same as sphere/cube, plus `Depth` (extrusion length) and
  `Kind` (`0` = right isosceles, `1` = equilateral — toggle it and watch the
  cross-section change live).
- **Light:** `R`, `G`, `B`, `Diameter` (visible size, `0` = invisible),
  `Brightness`.

To load a texture: select an object, press `P` until `TexID` is highlighted,
press `B`, type the filename of an XPM file in `textures/`
(e.g. `sphere_texture.xpm`) and press `Enter`. To see bump mapping, also raise
the object's `Bump` value above `0` — bump mapping reuses the texture's own
brightness as a height map, so a texture must be assigned first.

> **Note:** checker, shininess, specular, texture and bump are runtime-only
> properties for now they cannot yet be set inside a `.rt` file, only through
> the in-app property panel after loading.

## Scene File Format

The original 42 miniRT format is fully supported and backward compatible; the
extra primitives and optional trailing fields are additive.

```
A ratio r,g,b                                  # ambient light
C pos_x,y,z dir_x,y,z fov                      # camera
L pos_x,y,z brightness [r,g,b diameter]        # light (color/diameter optional)
sp pos_x,y,z diameter r,g,b                    # sphere
pl pos_x,y,z normal_x,y,z r,g,b                # plane
cy pos_x,y,z axis_x,y,z diameter height r,g,b  # cylinder
cu pos_x,y,z axis_x,y,z size r,g,b             # cube (equal-sided, any orientation)
tr pos_x,y,z axis_x,y,z size depth kind r,g,b  # triangular prism
```

For `tr`: `axis` is the extrusion direction (like a cylinder's axis), `size` is
the cross-section size, `depth` is the extrusion length, and `kind` is `0` for
right isosceles or `1` for equilateral.

Any number of `sp`, `pl`, `cy`, `cu`, `tr` and `L` lines may appear; `A` and `C`
must appear exactly once.

### Quick demo: checker / texture / bump on `epic_grand_hall.rt`

1. Load the scene and press `Tab` to enter Navigate mode.
2. Click the floor plane, press `P` until `Checker` is highlighted, press `B`,
   type `1`, press `Enter`.
3. Click the altar sphere (the white sphere on the cube altar at the far end),
   press `P` until `TexID` is highlighted, press `B`, type
   `sphere_texture.xpm`, press `Enter`.
4. With the same sphere selected, press `P` until `Bump` is highlighted, press
   `B`, type `3`, press `Enter`.
5. Press `Tab` to render the final high-quality image.

## Project history

- **Origin:** started as 42's miniRT with a teammate. The team dissolved before
  submission for reasons outside the project, so this codebase was never
  evaluated at 42. The mandatory part of the curriculum was completed with a
  different project.
- **Evolution:** the nearly-finished ray tracer was later picked up again and
  extended far past the original subject: full Phong shading, textures, bump
  mapping, new primitives, real-time navigation, and a complete in-app scene
  editor with undo/redo.
- **Renamed** from miniRT to PrismRT, because at some point "mini" stopped being
  an accurate description.

## Resources

- *Ray Tracing in One Weekend*, Peter Shirley — ray/sphere intersection and
  shading background.
- Möller–Trumbore ray/triangle intersection — the building block for every face
  of the triangular prism.
- Standard ray/AABB slab test, adapted to an arbitrarily oriented local frame
  for the cube primitive.
- Phong reflection model (Bui Tuong Phong, 1975).
- 42's official miniRT subject PDF for the base scene format.

## AI usage disclosure

Claude (Anthropic) was used throughout this project as a programming tutor: explaining the underlying vector/ray-tracing math and performance trade-offs before each feature.