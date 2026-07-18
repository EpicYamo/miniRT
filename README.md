*This project has been created as part of the 42 curriculum by aaycan.*

# miniRT

A ray tracer built from scratch in C using MiniLibX, which grew far beyond the
mandatory subject into a fully interactive, real-time **scene editor** — move,
rotate, spawn, delete and texture objects live, with undo/redo, while the
scene is still being ray traced.

![status](https://img.shields.io/badge/mandatory-complete-brightgreen)
![status](https://img.shields.io/badge/bonus-complete-brightgreen)

## Description

miniRT starts as the classic 42 ray tracing project: parse a `.rt` scene
description, cast rays through a virtual camera, and render spheres, planes
and cylinders with ambient/diffuse lighting and hard shadows.

On top of the mandatory part, this implementation adds a large set of bonus
features and turns the renderer into something closer to a mini scene editor:

- **Full Phong lighting model** — ambient, diffuse and specular, with
  per-object shininess/specular strength (matte vs. metallic surfaces).
- **Checkerboard pattern** — per-object, computed in the object's own local
  face coordinates (not world space), so it stays clean even on rotated
  cubes, tilted planes, and cylinder caps/sides.
- **Colored, multiple, movable lights** — lights are full scene objects: they
  can be spawned, selected, moved, deleted, undone, and given their own
  color, brightness and visible radius. For performance, only the first two
  lights (in scene order) are evaluated while flying around in real time;
  all lights are evaluated in the final high-quality render.
- **Image texture mapping (XPM)** — any sphere, plane, cylinder, cube or
  triangular prism can load its own XPM image file at runtime (typed
  directly in the UI), with independent tiling/repeat control per object.
- **Bump mapping** — the brightness of the loaded texture is reused as a
  height field to perturb the surface normal, faking small-scale relief
  without changing the geometry.
- **Extra primitives** — an axis-agnostic **cube** (arbitrary orientation,
  ray vs. slab intersection) and a **triangular prism**, in two
  cross-section shapes (equilateral and right isosceles), with independently
  adjustable cross-section size and extrusion depth. Both are fully solid,
  three-dimensional objects — not flat triangles — built from 8 planar faces
  (2 triangular caps + 3 rectangular sides, each split into 2 triangles) and
  intersected with the Möller–Trumbore algorithm.
- **Antialiasing** — 2×2 supersampling on the final render pass.
- **Real-time navigation** — WASD + mouse-look flight camera, with a
  low-resolution "Navigate" mode for smooth movement and a high-quality
  "Render" mode (with a live percentage loading screen) for the final image.
- **Full scene editing** — click to select, move/rotate gizmos, an on-screen
  property panel (color, size, material, texture) editable via keyboard,
  spawn/delete for every object type, and a 50-step undo/redo stack.

## Instructions

### Compilation

```sh
make        # builds MiniLibX and miniRT
make re     # clean rebuild
make debug  # slower, non-native build (safe to run under valgrind)
make clean  # remove object files
make fclean # remove object files and the binary
```

The `Makefile` builds with `-march=native` by default for performance. If you
need to run under `valgrind`, use `make debug` — some modern CPU instruction
sets are not understood by valgrind's instruction decoder.

### Running

```sh
./miniRT scenes/epic_grand_hall.rt
```

Any `.rt` file following the scene format below can be loaded. A handful of
demo scenes are included in `scenes/`.

### Controls

**Camera**

| Key | Action |
|---|---|
| `Tab` | Toggle between Navigate (low-res, real-time) and Render (full quality) mode |
| `W A S D` | Move forward / left / back / right (Navigate mode only) |
| `Q` / `E` | Move down / up |
| `Shift` (hold) | Move faster |
| Right mouse button (hold) + move mouse | Look around |

**Selecting & editing objects** (works in either mode, object gizmos are only
visible in Navigate mode)

| Key | Action |
|---|---|
| Left click | Select the object under the cursor |
| `C` | Deselect |
| `M` | Toggle move gizmo on the selected object |
| `R` | Toggle rotate gizmo (plane, cylinder, cube, triangular prism only) |
| Click + drag a gizmo axis/ring | Move or rotate along that axis |
| `N` (while an axis is grabbed) | Type an exact numeric offset/angle for that axis |
| `P` | Cycle through the selected object's editable properties |
| `B` | Edit the highlighted property (type a number, or a texture filename when `TexID` is highlighted) |
| `J` | Reset the object's position to its original scene-file value |
| `K` | Reset the object's rotation to its original scene-file value |
| `Delete` | Delete the selected object |
| `Ctrl+Z` | Undo the last move / rotate / property change / spawn / delete |
| `Y` | Cycle through lights (including invisible, zero-diameter ones) so they can be grabbed and edited |

**Spawning objects** (only while nothing is selected)

| Key | Spawns |
|---|---|
| `1` | Sphere |
| `2` | Plane |
| `3` | Cylinder |
| `4` | Light |
| `5` | Cube |
| `6` | Right isosceles triangular prism |
| `7` | Equilateral triangular prism |

New objects appear a fixed distance in front of the camera, already selected
and in move mode.

### Editable properties (via `P` / `B`)

- **Sphere / cube:** `R`, `G`, `B`, size, `Checker`, `Shininess`, `Specular`,
  `TexID`, `Repeat`, `Bump`.
- **Plane:** same as above, without a size field (infinite surface).
- **Cylinder:** same as sphere/cube, plus `Height`.
- **Triangular prism:** same as sphere/cube, plus `Depth` (extrusion length)
  and `Kind` (`0` = right isosceles, `1` = equilateral — toggle and watch
  the cross-section change live).
- **Light:** `R`, `G`, `B`, `Diameter` (visible size, `0` = invisible),
  `Brightness`.

To load a texture on an object: select it, press `P` until `TexID` is
highlighted, press `B`, type the exact filename of an XPM file located in
the `textures/` directory (e.g. `sphere_texture.xpm`), and press `Enter`.
The same texture file can be reused by any number of objects, and different
objects can reference different files. To see bump mapping, also raise the
object's `Bump` value above `0` after assigning a texture — bump mapping
reuses the texture's own brightness as a height map, so a texture must be
assigned first.

> **Note on scene files:** checker, shininess, specular, texture and bump
> are runtime-only properties for now — they cannot yet be set directly
> inside a `.rt` file, only through the in-app property panel after
> loading. All demo scenes below only pre-configure geometry, primitives
> and lighting; follow the short "reveal the bonus materials" steps under
> each scene to see checker/texture/bump live.

## Scene File Format

The mandatory format is unchanged and fully backward compatible. Bonus
primitives and optional trailing fields are additive.

```
A ratio r,g,b                                        # ambient light
C pos_x,y,z dir_x,y,z fov                            # camera
L pos_x,y,z brightness [r,g,b diameter]              # light (color/diameter optional)
sp pos_x,y,z diameter r,g,b                          # sphere
pl pos_x,y,z normal_x,y,z r,g,b                       # plane
cy pos_x,y,z axis_x,y,z diameter height r,g,b         # cylinder
cu pos_x,y,z axis_x,y,z size r,g,b                    # cube (equal-sided, any orientation)
tr pos_x,y,z axis_x,y,z size depth kind r,g,b         # triangular prism
```

For `tr`: `axis` is the direction the prism is extruded along (like a
cylinder's axis); `size` is the characteristic size of the triangular
cross-section (perpendicular to the axis); `depth` is how far the prism
extends along the axis; `kind` is `0` for a right isosceles cross-section
or `1` for an equilateral cross-section.

Any number of `sp`, `pl`, `cy`, `cu`, `tr` and `L` lines may appear; `A` and
`C` must appear exactly once.

### Revealing checker / texture / bump on `epic_grand_hall.rt`

1. Load the scene and press `Tab` to enter Navigate mode.
2. Click the floor plane, press `P` until `Checker` is highlighted, press
   `B`, type `1`, press `Enter`.
3. Click the altar sphere (the white sphere on top of the cube altar at the
   far end of the hall), press `P` until `TexID` is highlighted, press `B`,
   type `sphere_texture.xpm`, press `Enter`.
4. With the same sphere still selected, press `P` until `Bump` is
   highlighted, press `B`, type `3`, press `Enter`.
5. Press `Tab` to render the final, high-quality image.

## Resources

- *Ray Tracing in One Weekend*, Peter Shirley — general ray/sphere
  intersection and diffuse/specular shading background.
- Möller–Trumbore ray/triangle intersection algorithm — used as the building
  block for every face of the triangular prism primitive.
- Standard ray/AABB slab test, adapted here to an arbitrarily oriented local
  frame for the cube primitive.
- Phong reflection model (Bui Tuong Phong, 1975) — ambient/diffuse/specular
  decomposition used throughout `lightning.c`.
- 42's official miniRT subject PDF for the mandatory format and bonus list.

### AI usage disclosure

Claude (Anthropic) was used throughout this project as a programming tutor
and pair-programmer: explaining the underlying vector/ray-tracing math
before each feature, proposing designs and trade-offs and helping
debug issues (including several genuine bugs it introduced and then found
and fixed, such as an out-of-bounds backup-array read, a rotation-basis
mismatch causing checkerboard artifacts).
All parsing/validation code for the mandatory part was written by the
author beforehand and was deliberately left untouched except where new
optional scene syntax (extra `L` fields, `cu`, `tr`) required additive,
backward-compatible changes.
