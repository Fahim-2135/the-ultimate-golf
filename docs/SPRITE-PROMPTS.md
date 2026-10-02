# Sprite prompts — levels 5, 6 and 7

Everything to generate for Frozen Peak, Haunted Manor and Magma Core, in one file.

**How to use it:** paste the global rules block once at the top of the session, then one
prompt per artboard. Export each as PNG and save it into the folder named in its heading —
the filename must match exactly, because the game already loads it under that name. The
placeholders currently in those folders are the right size and frame count, so a correct
export is a straight swap with no code change.

---

## Status

**Already generated, keep as they are:** `frozen_pine`, `frozen_gondola`, `frozen_campfire`,
`manor_ghost`, `manor_bat`, `manor_mirror`, `manor_chandelier`.

**To generate — 35 files:**

| # | File | Artboard (1x) | Frames | Level |
|---|---|---|---|---|
| 1 | `frozen_snow_tile.png` | 256×256 | 1, seamless | Peak |
| 2 | `frozen_ice_tile.png` | 512×256 | 2, seamless | Peak |
| 3 | `frozen_snowball.png` | 256×64 | 4 | Peak |
| 4 | `frozen_icicle.png` | 128×64 | 2 | Peak |
| 5 | `frozen_ibex.png` | 256×64 | 4 | Peak |
| 6 | `frozen_hare.png` | 256×64 | 4 | Peak |
| 7 | `frozen_flags.png` | 512×128 | 4 | Peak |
| 8 | `frozen_toboggan.png` | 256×128 | 2 | Peak |
| 9 | `frozen_crate.png` | 128×128 | 1 | Peak |
| 10 | `frozen_boulder.png` | 384×128 | 3 | Peak |
| 11 | `frozen_spindrift.png` | 512×128 | 4 | Peak |
| 12 | `frozen_sign.png` | 128×128 | 1 | Peak |
| 13 | `manor_floor_tile.png` | 256×256 | 1, seamless | Manor |
| 14 | `manor_candle.png` | 256×64 | 4 | Manor |
| 15 | `manor_wisp.png` | 512×128 | 4 | Manor |
| 16 | `manor_lantern.png` | 512×128 | 4 | Manor |
| 17 | `manor_sparks.png` | 512×128 | 4 | Manor |
| 18 | `manor_fireplace.png` | 512×128 | 4 | Manor |
| 19 | `manor_rat.png` | 256×64 | 4 | Manor |
| 20 | `manor_portrait.png` | 256×128 | 2 | Manor |
| 21 | `manor_clock.png` | 384×128 | 3 | Manor |
| 22 | `manor_curtain.png` | 512×128 | 4 | Manor |
| 23 | `manor_cobweb.png` | 256×256 | 1 | Manor |
| 24 | `magma_rock_tile.png` | 256×256 | 1, seamless | Magma |
| 25 | `magma_lava_tile.png` | 512×256 | 2, seamless | Magma |
| 26 | `magma_crust.png` | 512×256 | 2 | Magma |
| 27 | `magma_vent.png` | 768×256 | 3 | Magma |
| 28 | `magma_geyser.png` | 384×128 | 3 | Magma |
| 29 | `magma_crystal.png` | 576×192 | 3 | Magma |
| 30 | `magma_raft.png` | 320×256 | 1 | Magma |
| 31 | `magma_bubble.png` | 512×128 | 4 | Magma |
| 32 | `magma_salamander.png` | 256×64 | 4 | Magma |
| 33 | `magma_smoke.png` | 512×128 | 4 | Magma |
| 34 | `magma_pumice.png` | 384×128 | 3 | Magma |
| 35 | `magma_lavafall.png` | 512×256 | 4 (each cell 128×256) | Magma |

Folders: `assets/frozen/`, `assets/manor/`, `assets/magma/`.

Sizes above are 1x. Export at 2x, so a 256×64 artboard becomes a 512×128 PNG — that is what
the game expects and what the placeholders already are.

---

## Global rules — paste this first

```
I'm making sprites for a top-down 2D golf game (raylib, 1920x1080). Follow these rules for every artboard:

- View: straight top-down (camera looking directly down at the ground). No perspective, no isometric angle.
- Style: clean flat vector art with soft cartoon shading. Must read clearly at small size.
- Light: coming from the TOP-LEFT. Highlights on the top-left, shade on the bottom-right.
- NO cast/drop shadows on the ground (the game draws shadows itself).
- NO text, labels, frame numbers, borders or guide lines in the export.
- Artboard size must be EXACTLY the size I give, in pixels.
- Sprite sheets: frames sit in a grid of equal cells with NO gaps and NO padding. Frame order is left-to-right.
- Background: TRANSPARENT (real alpha, never a grey checkerboard baked into the picture) unless I mark it NOT transparent.
- Export: PNG at 2x scale.
```

---

# FROZEN PEAK → `assets/frozen/`

## 1. `frozen_snow_tile.png` — 256×256, NOT transparent

```
Seamless packed mountain snow from directly above.
- Base: flat #EDF4FA over the whole artboard.
- 90 snow grains 1-2 px, half #FFFFFF half #D4E2F0, scattered evenly.
- 14 wind ripples: 2 px curved arcs 30-60 px long, all running left to right, #D0DFEE at 60%.
- 6 ice sparkles: 2 px white dots at 80%.
- Low contrast: the ball rolls over this.
- SEAMLESS: anything cut by an edge continues on the opposite edge at the same position. No large light or dark patches.
```

## 2. `frozen_ice_tile.png` — 512×256, 2 frames of 256, NOT transparent

```
Seamless glacier ice from directly above, with a slow shimmer.
- Base: flat #4E7E96 in each frame.
- 7 long cracks per frame: 2 px lines, 80-200 px, meeting at 3 junctions, #BFE4F2 at 70%. Identical in both frames.
- 18 trapped bubbles: 3-7 px circles #DCF2FA at 45%, each with a 1 px white dot top-left.
- 2 broad sheen bands: 100 px wide diagonals top-left to bottom-right, #7FB6CE at 20%.
- Frame 2 = frame 1 with the sheen bands moved 24 px right and the bubble highlights dimmer. Cracks do not move.
- SEAMLESS in both frames, and the frames must not bleed into each other.
```

## 3. `frozen_snowball.png` — 256×64, 4 frames of 64

```
A snowball rolling, from directly above. The game scales it up as it grows, so keep detail even.
- Circle radius 28 px centred at (32,32) in EVERY frame, identical position and size.
- Base #F4F9FF, shade crescent #C6D8E8 along the bottom-right inner edge, white highlight blob 14 px top-left.
- 3 packed-snow seams spiralling from a point 10 px up-left of centre out to the rim, 2 px, #D8E6F2.
- 7 clumps 3-5 px (#E2EDF7, 1 px #BCCEDE outline) and 4 grit specks 2 px (#6E6A66), none touching the rim.
- 2 px #A8C0D4 outline.
- Frames 2-4: rotate ONLY the seams, clumps and grit by 90, 180, 270 degrees. The outline, base, shade crescent and highlight stay put so the light stays top-left.
```

## 4. `frozen_icicle.png` — 128×64, 2 frames of 64

```
A hanging icicle seen from directly above, pointing down at the viewer, so it reads as a cone end-on.
- Frame 1: concentric rings centred at (32,32) - outer radius 26 px #8FC9DE, middle 17 px #B6E2F2, inner 9 px #E8F8FF, 4 px #FFFFFF core. 2 px #5E96AE outline on the outer ring only. 5 ridge lines 1 px #6FAFC8 running from the core to the rim like spokes. 3 melt drops 3 px #DFF4FF just outside the rim at the bottom-right.
- Frame 2: the same 20% larger, core brightened to 6 px pure white, outer ring at 85% opacity, plus 4 motion streaks (2 px #CFEAF7 at 50%, 10 px) radiating past the rim.
```

## 5. `frozen_ibex.png` — 256×64, 4 frames of 64

```
A mountain goat walking, seen from directly above, FACING THE TOP of the image. It wanders along ledges in the game.
- Body: vertical oval 22 x 34 px centred at (32,36) in EVERY frame, same pixels in all four. Base #8A7254, highlight #AD9170 on the top-left, shade #5E4B33 bottom-right, 2 px #342918 outline.
- Head: 14 px circle at (32,16), same colours, with two curved horns sweeping back from it: 2 px #E8DCC0 arcs, 16 px long, curling toward the body.
- Four legs, 3 px wide, two per side, reaching out from the body to about y=12 and y=56.
- A short beard: 3 small #E8DCC0 strokes under the head.
- Frames 1-4 walk cycle: legs 1 and 3 swing forward while 2 and 4 swing back, then swap. The head bobs 1 px. Body and horns stay still.
```

## 6. `frozen_hare.png` — 256×64, 4 frames of 64

```
A white snow hare hopping, from directly above, FACING THE TOP.
- Body: oval 18 x 24 px centred at (32,38). Base #F4F9FF, shade #CBDCEC bottom-right, 2 px #9FB6C9 outline.
- Head: 11 px circle at (32,22), with two long ears: 4 x 14 px rounded shapes angled slightly outward from the top of the head, #F4F9FF with #E0B9BE inner line.
- Two black 2 px eyes, a 3 px #E0B9BE nose, a 6 px round tail at the bottom of the body.
- FRAME 1: sitting, ears up, legs tucked. FRAME 2: crouched, body 2 px shorter, ears tilted back. FRAME 3: mid-hop, body stretched 4 px longer, ears flat back, legs out front and back. FRAME 4: landing, body squashed 2 px wider, ears half up.
- The hop must read at small size: frame 3 is clearly stretched, frame 4 clearly squashed.
```

## 7. `frozen_flags.png` — 512×128, 4 frames of 128

```
A line of prayer flags strung on a rope, seen from directly above, rippling. The rope runs LEFT TO RIGHT across the middle of each frame.
- Rope: 2 px #6E5B42 line across the full width at y=64 in every frame.
- 6 flags hanging off it, evenly spaced, each a 16 x 22 px rounded rectangle, in this order: #D6463A, #E8A33C, #EFE4C8, #4E9B57, #4A7FC1, #D6463A. Each has a 1 px darker edge and a 2 px white tie at the rope.
- Frames 1-4: a wave travels left to right along the line. In each frame, every flag is offset downward by sin of its position plus the frame number - flag 1 leads, flag 6 trails by about a third of a cycle. Offsets up to 7 px. The rope sags slightly with them.
- Frame 4 must loop smoothly into frame 1.
```

## 8. `frozen_toboggan.png` — 256×128, 2 frames of 128

```
A wooden toboggan seen from directly above, sliding. In the game it runs down a lane and knocks the ball.
- Deck: rounded rectangle 54 x 92 px centred in the cell, made of 5 wooden slats (#B5803F, 1 px #7A5324 gaps), 2 px #5E3F1B outline, with the front end curled up (drawn as a lighter #D8A965 band across the top 14 px).
- Two steel runners: 4 px #9AA3AC strips down the left and right edges, extending 6 px past the back.
- A coil of rope at the front: 10 px circle of #D8CBA8.
- FRAME 2: the same toboggan with 6 small snow sprays (4-7 px, #FFFFFF at 70%) kicked up along both runners, and the rope coil shifted 2 px.
```

## 9. `frozen_crate.png` — 128×128

```
A wooden supply crate in the snow, from directly above.
- Square 108 x 108 px centred, base #A9793F, 3 px #5E3F1B outline.
- Plank lines: 3 vertical 2 px #7A5324 lines splitting the top into 4 planks.
- A diagonal brace across the top corner to corner, 6 px wide, #C79A5C with a 1 px outline.
- Four 5 px #8A8D91 metal corner brackets, one per corner, each with two 2 px rivets.
- A 10 px snow cap along the top-left edge only, #F2F9FF.
```

## 10. `frozen_boulder.png` — 384×128, 3 frames of 128

```
Three round ice boulders used as bumpers, one per cell, seen from directly above. Three different shapes, not copies.
- Each is a rough circle about 100 px across, centred in its cell.
- Fill: #BBD9E8 with 3 or 4 flat facets in #A6C7DA and #D3EAF5 so it reads as cut ice, 2 px #7FA0B4 outline.
- A white highlight blob 20 px across on the top-left of each.
- 5 small cracks per boulder: 1 px #7FA0B4 lines running inward from the rim.
- A 6 px snow cap on the top-left edge.
- Vary the three: one nearly round, one slightly flattened, one with a chunk missing from the bottom-right.
```

## 11. `frozen_spindrift.png` — 512×128, 4 frames of 128

```
A gust of blowing snow, seen from directly above, drifting LEFT TO RIGHT. It is drawn over the course to show the wind.
- Each frame: about 40 snow streaks, 1-3 px thick and 10-30 px long, all roughly horizontal, in #FFFFFF at 40-80% opacity, denser in the middle of the cell and thinning toward the top and bottom edges.
- Add 12 small round flakes 2-4 px at 60%.
- Frames 2-4 = frame 1 with everything moved 32, 64 and 96 px to the RIGHT, wrapping around the cell edge, so the four frames loop as a continuous drift.
- No hard edges anywhere: this is a semi-transparent overlay.
```

## 12. `frozen_sign.png` — 128×128

```
A wooden trail signpost in the snow, from directly above.
- Post: 14 px circle of #7A5324 at the centre with a 2 px darker outline (you are looking down the post).
- Two arrow boards sticking out from it, one pointing up-left and one pointing right, each 56 x 18 px, #D8A965 with a 2 px #5E3F1B outline and a 1 px lighter top edge.
- Each board has a painted arrow: a 3 px #5E3F1B chevron near its outer end.
- A ring of snow around the base: 30 px circle of #F2F9FF at 70%, behind the post.
```

---

# HAUNTED MANOR → `assets/manor/`

## 13. `manor_floor_tile.png` — 256×256, NOT transparent

```
Seamless dark hardwood floorboards from directly above. It sits in near-darkness, so keep it very dark and low contrast.
- Base #2A1F18. 6 horizontal planks 256 x 42 px, alternating #2A1F18 and #322620.
- 2 px #150F0B gap line between every plank.
- 4 plank-end lines, 2 px #150F0B, at x = 60, 128, 190, 240, each crossing only one plank, staggered so none line up.
- 40 grain lines: 1 px wavy strokes 30-90 px, #3D2E24 at 50%.
- 10 nail heads: 2 px #4A3A2E dots in pairs near the plank ends.
- SEAMLESS: planks and grain line up left-to-right and top-to-bottom.
```

## 14. `manor_candle.png` — 256×64, 4 frames of 64

```
A floor candle from directly above. FRAME 1 UNLIT, FRAMES 2-4 the lit flicker. The difference must be obvious at small size.
- Base plate, identical in all frames: 22 px radius circle centred, #4A4036, 2 px #2A231C outline, 14 px inner circle #5E5246.
- Candle stub, identical in all frames: 12 px radius circle, #E8E0CC, 2 px #A89C80 outline, 3 px #2A231C wick at the centre.
- FRAME 1: no flame, and desaturate everything - plate #3E362E, stub #BDB6A6. Cold and dead.
- FRAMES 2-4: add a 30 px #FFB340 glow at 30%, a 16 px #FFD564 flame blob and a 7 px #FFF8D2 core, plus a 2 px #FFC06A warm arc on the plate's top-left rim.
- Rotate the flame blob 0, 45, 90 degrees across frames 2-4 and change the glow radius to 30, 26, 34 px so it flickers. Frame 4 loops back to frame 2.
```

## 15. `manor_wisp.png` — 512×128, 4 frames of 128

```
A will-o-wisp: a small floating ball of cold light that drifts through the house carrying its own glow.
- Core: 14 px circle of #FFFFFF at the centre of every frame.
- Around it: a 34 px circle of #9FE8D8 at 70%, then a 60 px circle of #4FBFA8 at 25%, then a 100 px circle of #2E7F70 at 10%. Soft edges, no outline.
- 5 small trailing motes behind it, 3-6 px #9FE8D8 at 50%, strung out in a curve.
- Frames 1-4: the trailing motes swing from one side to the other and back, and the outer two glow circles pulse between 100% and 115% of their size. The core never moves.
- Everything semi-transparent: this is drawn as a light, not an object.
```

## 16. `manor_lantern.png` — 512×128, 4 frames of 128

```
An old oil lantern hanging on a chain, seen from directly above, swinging. The chain hangs from the TOP of each cell.
- Chain: a vertical line of 6 small 4 px #6E6A60 links from y=0 down to the lantern.
- Lantern body: a 34 x 34 px rounded square, #8A6E32 frame with a 2 px #4A3A18 outline and four glass panels of #FFE9A8 at 70%.
- A 10 px #FFD564 flame blob at the middle, with a 46 px #FFB340 glow at 25% behind the whole lantern.
- FRAME 1: lantern centred at (64,78), chain straight. FRAME 2: lantern swung 14 px LEFT, chain tilted to follow, lantern tilted 10 degrees. FRAME 3: back to centred but 4 px higher. FRAME 4: swung 14 px RIGHT, tilted -10 degrees.
- The glow moves with the lantern. Frame 4 loops into frame 1.
```

## 17. `manor_sparks.png` — 512×128, 4 frames of 128

```
A burst of sparks from a broken wall sconce, seen from directly above. Four frames of one burst, left to right.
- All sparks come from a point at the centre of the cell.
- FRAME 1: a 10 px white-hot flash at the centre plus 6 short 6 px #FFE9A8 streaks radiating out.
- FRAME 2: 18 sparks thrown out to a radius of 30 px - 2 px bright #FFF3C4 heads with 8 px #FFB340 tails pointing back at the centre.
- FRAME 3: the same sparks out at 50 px, dimmer (#FFB340 heads, #D1541F tails), a few starting to curve downward, plus 6 tiny embers left behind near the middle.
- FRAME 4: the last sparks at 70 px, down to 40% opacity, mostly orange-red, with 3 fading embers.
- Transparent background, no outlines, everything glowing.
```

## 18. `manor_fireplace.png` — 512×128, 4 frames of 128

```
A stone fireplace with a fire in it, seen from directly above.
- Hearth, identical in all frames: a 100 x 70 px rounded stone surround, #6E6A60 with a 2 px #3A362E outline and 6 visible stone blocks picked out in #807C72.
- Inside it, 3 charred logs: dark #3A2A1C bars crossed over each other, with glowing #D1541F cracks along them.
- Fire: layered blobs over the logs, centred - outer 56 px #FF8A1F at 70%, middle 36 px #FFC53D, inner 18 px #FFF3B0.
- Frames 1-4: redraw the three fire blobs with different irregular edges, rotating each roughly 25 degrees per frame, and move 6 small embers (3 px #FFD37A) to new spots. The hearth and logs never move.
```

## 19. `manor_rat.png` — 256×64, 4 frames of 64

```
A rat scurrying, seen from directly above, FACING THE TOP.
- Body: oval 16 x 26 px centred at (32,34), #4A4038 with a #6B5E52 highlight on the top-left and a 2 px #241E18 outline.
- Head: 10 px circle at (32,18) with two 6 px round ears at its top-left and top-right, and a 2 px pink #D89A9A nose.
- Tail: a 2 px #B49A8A curve 30 px long trailing from the bottom of the body, curling to one side.
- Four small legs, 2 px, poking out at the sides.
- Frames 1-4: the legs alternate in a fast scurry, the body shifts 1 px side to side, and the tail curl swings from one side to the other and back. The head stays put.
```

## 20. `manor_portrait.png` — 256×128, 2 frames of 128

```
An old oil portrait lying face-up, seen from directly above, in a gold frame. The eyes look in a different direction in each frame.
- Frame: 96 x 116 px rounded rectangle, #8A6E32 with a 3 px #4A3A18 outline and 8 small carved scroll bumps around the outside.
- Canvas inside: 76 x 96 px, a dark green-brown gradient (#3A3A2C to #241F18).
- On it, a simple pale portrait: a 36 px oval face in #C9B79A, a dark #2A2118 collar below it, hair as a #3A2A1C shape around the top.
- Eyes: two 7 px white ovals with 4 px black pupils.
- FRAME 1: both pupils pushed to the LEFT side of the eyes. FRAME 2: both pushed to the RIGHT. Everything else identical between the two frames.
```

## 21. `manor_clock.png` — 384×128, 3 frames of 128

```
A grandfather clock lying face-up, seen from directly above, with its pendulum swinging.
- Case: 64 x 112 px rounded rectangle, dark wood #4A3521 with a 2 px #2A1D11 outline and a lighter #6B5030 inner panel.
- Clock face at the top: a 42 px circle of #EFE4C8 with a 2 px #8A6E32 ring, 12 small tick marks, and two black hands.
- Below it, a glass window: a 36 x 50 px #9BD7E8 shape at 30% opacity.
- Pendulum inside that window: a 3 px #C9A24A rod from the top of the window down to a 14 px #E8C96A disc.
- FRAME 1: pendulum hanging straight down. FRAME 2: swung 18 degrees LEFT. FRAME 3: swung 18 degrees RIGHT. Case, face and hands identical in all three.
```

## 22. `manor_curtain.png` — 512×128, 4 frames of 128

```
A tattered curtain at a broken window, seen from directly above, billowing inward.
- A window ledge along the TOP of each cell: a 110 x 16 px #4A4038 bar with a 2 px #241E18 outline.
- The curtain hangs down from it: a 90 px wide sheet of faded #8A7C6E at 85% opacity, with 5 vertical folds picked out in #6E6358, a ragged torn bottom edge, and two holes.
- A pale #B9C6D4 moonlight wash spilling over the top 30 px of the curtain, at 25%.
- Frames 1-4: the curtain billows - the bottom edge swings left, centre, right, centre, and the folds shift with it. The ledge never moves. Frame 4 loops into frame 1.
```

## 23. `manor_cobweb.png` — 256×256

```
A thick dusty cobweb anchored at the TOP-LEFT corner, seen from directly above.
- Anchor at (10,10). 7 straight radials, 1 px #E2DED2 at 60%, fanning out across the quarter circle to the far edges.
- 6 spiral arcs crossing them at radii 38, 68, 102, 140, 182, 226 px, each arc SAGGING away from the anchor between radials, 1 px #E2DED2, fading from 65% opacity at the inside to 25% at the outside.
- Tear out the spiral segments between the 5th and 6th radial beyond radius 100 px, and leave 3 loose threads trailing from the break.
- 14 dust motes 2-3 px #CFC9B8 caught on the threads.
```

---

# MAGMA CORE → `assets/magma/`

## 24. `magma_rock_tile.png` — 256×256, NOT transparent

```
Seamless cooled volcanic basalt from directly above. Dark, so glowing things pop against it.
- Base #2B2A2C, broken into about 14 irregular plates 40-90 px across with 2 px #15151A cracks between them.
- Fill the plates with #2B2A2C, #323135 or #262528 at random, each with a 1 px #3E3D42 highlight along its top-left edge.
- 9 of the cracks get a 1 px #8A2E12 ember glow along part of their length; 4 junctions get a 2 px #D1541F dot.
- 50 ash specks 1-2 px #4A4850 at 40%.
- SEAMLESS: every plate and crack cut by an edge continues on the opposite edge.
```

## 25. `magma_lava_tile.png` — 512×256, 2 frames of 256, NOT transparent

```
Seamless molten lava from directly above, slowly flowing. Very high contrast against the basalt.
- Base #FF8A12. A web of about 16 dark cooled plates (#5E2A14, 30-70 px) separated by 6-10 px channels of lava.
- In every channel: a 3 px #FFE066 core line with #FFB020 either side.
- 7 bright pools 10-18 px #FFF3C4 where channels meet.
- 1 px #3E1A0C outline around every plate plus a 1 px #7A3A1E top-left highlight.
- Frame 2 = frame 1 with every plate drifted 5 px right and 2 px down, channels redrawn to fit, pools 15% larger.
- SEAMLESS in both frames; the frames must not bleed into each other.
```

## 26. `magma_crust.png` — 512×256, 2 frames of 256

```
A slab of cooling crust the ball stands on. FRAME 1 SAFE, FRAME 2 HOT. The shape must be pixel-identical between them - only the colours change.
- Slab: an irregular 9-sided polygon about 220 px across, centred.
- 3 main cracks running rim to rim through the middle with a kink, plus 3 short branches. Draw this geometry ONCE and reuse it exactly in both frames.
- FRAME 1: plates #3A3A3E with 3 tone patches in #45454B, cracks #1A1A1E, a 4 px #55555C highlight along the top-left rim, 8 ash specks.
- FRAME 2: the same plates tinted #5E3326, the same cracks now a 4 px #FFD24A core with a 12 px #FF6A1F bleed, a 20 px #FF8A12 glow at 35% around the whole slab, and 5 #FFE9A8 embers along the cracks.
```

## 27. `magma_vent.png` — 768×256, 3 frames of 256

```
A volcanic vent hole from directly above. The hole is in the same place and the same size in all three frames - only what comes out changes.
- Rim, all frames: a ragged ring, outer radius 88 px, inner 60 px, #3A3A3E with a 2 px #1A1A1E outline and 9 chipped notches around the outside.
- FRAME 1 dormant: the hole is a gradient from #241E1A at its edge to pure black in the middle. 4 ash specks on the rim.
- FRAME 2 building: the hole glows from #8A2E12 at the edge to #FF8A12 in the middle, 5 bubbles 4-8 px #FFC53D near the centre, and a 6 px #D1541F glow on the rim's inner edge.
- FRAME 3 erupting: the hole is #FFF3C4, the rim's inner edge white-hot #FFE9A8, 14 thrown lava blobs 6-14 px (#FF8A12 with #FFE066 cores) out to radius 120 px, and 8 droplets 3 px beyond them.
```

## 28. `magma_geyser.png` — 384×128, 3 frames of 128

```
A lava geyser firing straight up, from directly above, so the plume reads as rings spreading from the centre. All three frames share the centre.
- FRAME 1 starting: a ring at radius 20 px, 8 px thick, #FFC53D, with a 10 px #FFF3C4 core and 6 droplets 3 px just outside.
- FRAME 2 full burst: rings at radius 52, 36 and 20 px in #FF6A1F (70%), #FFB020 and #FFE066, a 14 px #FFFFF0 core, and 16 spray blobs 5-11 px #FFC53D between radius 40 and 62.
- FRAME 3 falling back: the rings expand to 70, 50, 30 px and drop to 40% opacity, the core shrinks to 8 px #FFC53D, and 20 droplets 2-5 px #FF8A12 scatter out to radius 64.
```

## 29. `magma_crystal.png` — 576×192, 3 frames of 192

```
Three obsidian crystal clusters used as bouncy obstacles, one per cell, from directly above. Three arrangements, not copies.
- Each cluster is centred and fits inside a 150 px circle: 6 sharp four-sided shards radiating from the middle, 48-68 px long, 20-32 px wide at the base, tapering to points.
- Shards: fill #1A1620, a #3E2E52 facet stripe down the middle of each, 2 px #0C0A10 outline, a 2 px #8A6FD1 highlight along the top-left edge of every shard, and a 4 px #C9B6FF glint near the tip of three of them.
- Base: a 32 px #D1541F glow at 45% at the centre with a 12 px #FF8A12 core, as if the rock below is still hot.
- Vary the three: one even 6-shard star, one with two shards shortened, one with 7 shards and a dimmer base.
```

## 30. `magma_raft.png` — 320×256

```
A slab of cooled crust floating on lava, used as a moving platform. It must look like it is melting from the edges inward.
- Shape: an irregular 9-sided polygon about 280 x 210 px, no two edges the same length.
- Surface: 7 plates separated by 4 px #1A1A1E cracks, plates filled #3A3A3E, #45454B and #323135, each with a 1 px #55555C top-left highlight.
- Molten rim: a 14 px band following the whole outline, #FF6A1F outside fading to #8A2E12 where it meets the rock, with 9 brighter #FFC53D licks reaching 8 px further out at random points.
- A 28 px #FF8A12 halo at 30% outside the rim.
- The 3 cracks nearest the rim glow 1 px #FFB020, fading toward the middle. 7 ash specks on the inner plates.
```

## 31. `magma_bubble.png` — 512×128, 4 frames of 128

```
A lava bubble swelling and popping on the surface of the lake, seen from directly above. Four frames of one pop.
- FRAME 1: a 20 px dome, #FF8A12 with a #FFC53D highlight on the top-left and a thin #FFE066 rim.
- FRAME 2: swollen to 44 px, the skin stretched thinner - more #FFE066 showing through, a 10 px #FFF3C4 hot spot on top.
- FRAME 3: bursting - a ragged 56 px ring of #FFE9A8 with the middle torn open to dark #5E2A14, and 10 droplets 3-6 px thrown outward.
- FRAME 4: gone - a 60 px flat ripple ring, 2 px #FFB020 at 50%, with 5 fading droplets.
- Transparent background: this is drawn on top of the lava tile.
```

## 32. `magma_salamander.png` — 256×64, 4 frames of 64

```
A fire salamander skittering along the rock, seen from directly above, FACING THE TOP.
- Body: a long oval 14 x 30 px centred at (32,34), base #2A1A16 with glowing #FF6A1F markings - three bands across its back - and a 2 px #150C0A outline.
- Head: 10 px rounded shape at (32,16) with two 2 px #FFE066 eyes.
- Tail: a tapering 18 px extension from the bottom of the body, curving to one side, with a dull #D1541F glow near the base.
- Four short splayed legs, 2 px, two per side.
- Frames 1-4: the body ripples side to side like a lizard - the tail swings left, centre, right, centre, the legs alternate, and the glowing bands pulse slightly brighter on frames 2 and 4.
```

## 33. `magma_smoke.png` — 512×128, 4 frames of 128

```
A plume of volcanic smoke seen from directly above, drifting and thinning. It is drawn over the course.
- Each frame: 5 overlapping soft clouds, 30-70 px across, in #4A4850 and #5E5C66, at 25-45% opacity, no outlines, edges very soft.
- A few darker #3A3840 wisps curling through them at 30%.
- Frames 1-4: the whole plume drifts 18 px to the RIGHT and 10 px UP each frame while growing about 8% and losing about 8% opacity, so it reads as smoke rising and dispersing. Frame 4 should be the faintest and largest.
- Nothing hard-edged anywhere.
```

## 34. `magma_pumice.png` — 384×128, 3 frames of 128

```
Three lumps of pumice floating on lava, one per cell, from directly above. Three different shapes.
- Each is a rough blob about 80 px across, pale grey #8C8A92 with a 2 px #5A5860 outline.
- Pock it with 12 small holes, 4-8 px, in #6E6C74, spread unevenly.
- A 6 px #B5B2BA highlight along the top-left edge.
- Around each, a thin 6 px #FF8A12 rim at 60% where it meets the lava, with 4 brighter #FFC53D licks.
- Vary the three: one round, one long and narrow, one with a split down the middle.
```

## 35. `magma_lavafall.png` — 512×256, 4 frames, each cell 128 wide × 256 tall

```
Artboard 512 x 256, 4 frames side by side, each 128 x 256. TRANSPARENT.

A sheet of lava pouring DOWNWARD (from the top of the cell to the bottom), seen from directly above.
- Each frame: a 100 px wide band of flowing lava down the middle of the cell, #FF8A12, with 6 brighter #FFE066 streams running its length and 4 darker #8A2E12 crust streaks.
- At the top of the band: a 16 px #FFF3C4 lip where the lava comes over.
- At the bottom: a spray of 14 droplets 4-10 px thrown outward, #FFC53D.
- Frames 1-4: move every stream, streak and droplet DOWN by 64 px per frame, wrapping back to the top, so the four frames loop as a continuous pour. The lip and the band's outline stay put.
```

---

## Before sending them over

- **Transparency.** Open each PNG: if you can see a grey checkerboard as part of the picture, the export is wrong. Alpha, not a drawn checkerboard.
- **Exact artboards.** Doubled by the 2x export — a 256×64 prompt becomes a 512×128 file.
- **Paired frames must match.** `magma_crust` frames 1 and 2 identical in shape, `manor_candle` frame 1 obviously dead next to 2-4, `frozen_snowball` circle not drifting between frames.
- **Tiles must not seam.** Put two copies side by side and look for a line: snow, ice, floor, basalt, lava.
