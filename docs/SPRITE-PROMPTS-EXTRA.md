# Sprite prompts — the six still drawn by hand

Everything in levels 5 and 7 is a real picture now except these six things, which the game
still draws out of circles and rectangles. That is why the erupting lava looks cheap: it is
a stack of discs, not a picture of lava.

Same rules as `SPRITE-PROMPTS.md`: paste the global rules block once, then one prompt per
artboard, export at 2x, save under the exact filename. Nothing here replaces a file that
already exists, so generating them cannot break anything that is already working.

**All six are generated and wired in (2 October 2026). Kept here as the record of what was asked for.**

| # | File | Folder | Artboard (1x) | Frames | What it replaces |
|---|---|---|---|---|---|
| 1 | `magma_eruption.png` | `assets/magma/` | 1024×256 | 4 | the lava flooding out of a vent |
| 2 | `magma_steam.png` | `assets/magma/` | 512×128 | 4 | the steam jet's plume |
| 3 | `magma_catwalk.png` | `assets/magma/` | 128×128 | 2 | the iron planks over the lake |
| 4 | `magma_tube.png` | `assets/magma/` | 256×128 | 2 | the lava tube's two mouths |
| 5 | `magma_heart.png` | `assets/magma/` | 512×256 | 2 | the heart of the volcano, the goal |
| 6 | `frozen_bridge.png` | `assets/frozen/` | 128×128 | 2 | the crumbling planks over the crevasse |

---

## 1. `magma_eruption.png` — 1024×256, 4 frames, each cell 256×256

This is the one that matters most. It is seen from directly above and it grows: frame 1 is
the lava just breaking the surface, frame 4 is the full flood at its widest.

```
Artboard 1024 x 256, 4 frames side by side, each 256 x 256. TRANSPARENT background, no checkerboard.

A pool of molten rock flooding outward over a dark basalt floor, seen from DIRECTLY ABOVE, flat top-down, no perspective and no horizon.

Every frame, built from the outside in:
- An outer ring of broken crust: 18 to 24 irregular five- and six-sided plates of cold rock, #2E2A2E, each 18-34 px across, tilted at different angles and shoved apart, with 3 px #1A171A gaps between them. The plates must NOT form a neat circle: let them crowd in places and leave the lava showing through in others.
- Between the plates, thin veins of #FF6A0D, 3-6 px wide.
- Inside that: a band of cooling rock, #52301E, blotchy, with 10 darker #35200F patches.
- Inside that: the lava itself, #FF8A12, with 8 brighter #FFC53D swirls that curl rather than radiate, and 5 small islands of crust #3A3338 floating on it.
- At the very centre: a hot core, a soft-edged blob of #FFE9A8 fading out into #FFC53D over about 40 px.
- Scattered over the lava: 20 pinpoint sparks 2-4 px, #FFF3C4.
- Around the whole thing, a 10 px outer glow of #FF6A0D at 35% opacity.

Frame by frame, the SAME pool growing (keep the shapes, scale them):
- Frame 1: overall diameter 110 px. Crust plates still almost closed over the top, only four veins of lava showing. Core small and dim.
- Frame 2: diameter 170 px. Plates clearly shoved apart, lava showing between all of them.
- Frame 3: diameter 220 px. Plates pushed right out to the rim, the lava band at its brightest.
- Frame 4: diameter 248 px, nearly filling the cell. Add 10 thrown blobs of lava 6-14 px, #FFC53D, sitting just outside the crust ring with a short motion streak behind each.

Keep the pool centred in its cell in all four frames. No text, no outline around the cell, no drop shadow onto a background.
```

## 2. `magma_steam.png` — 512×128, 4 frames, each cell 128×128

The jet that shoves the ball. Drawn as a burst so the game can rotate it to whichever way
the jet blows.

```
Artboard 512 x 128, 4 frames side by side, each 128 x 128. TRANSPARENT.

A blast of superheated steam coming straight up out of the floor, seen from DIRECTLY ABOVE, so it reads as a ring of vapour spreading outward from the centre of the cell.

Every frame:
- A ring of 9 overlapping soft-edged puffs, #D8D2CC at 70% opacity, each 26-40 px across, arranged around the centre with uneven gaps.
- Inside the ring, 5 smaller puffs #F0ECE6 at 50%.
- At the centre, a hot white-yellow point #FFF3C4, 14 px, fading out over 20 px.
- Threaded through the puffs, 6 thin curling wisps 3 px wide, #EDE7E1 at 40%, curving clockwise.
- 8 sparks 2-3 px #FFC53D carried in the steam, nearer the centre than the rim.

Frame by frame, one burst over and over:
- Frame 1: ring diameter 48 px, puffs tight and bright.
- Frame 2: diameter 78 px.
- Frame 3: diameter 104 px, puffs thinning, opacity dropped by a quarter.
- Frame 4: diameter 124 px, puffs ragged and faint, half the opacity of frame 1, the centre point almost gone.

The puffs must look like the SAME puffs expanding, not four different clouds. No background, no cell outline.
```

## 3. `magma_catwalk.png` — 128×128, 2 frames, each cell 64×128

One plank of the iron catwalk, standing tall in its cell. The game lays five of them side by
side to make a bridge.

```
Artboard 128 x 128, 2 frames side by side, each 64 x 128. TRANSPARENT.

A single plank of industrial iron grating, seen from DIRECTLY ABOVE, standing upright in the cell: 56 px wide and 120 px tall, centred, 4 px of empty space all round.

Frame 1, sound:
- The plate itself #4A4348, with a 3 px #6A6168 highlight along its top and left edges and a 3 px #2A2528 shadow along its bottom and right.
- Cut into it, 7 slots running across the plank, each 40 px wide and 7 px tall, evenly spaced down its length, showing through to nothing (transparent), each with a 1 px #2A2528 lip on its lower edge.
- A rivet in each corner, 6 px, #7A7178 with a 2 px #2A2528 dot off-centre.
- Along the two long edges, a 5 px raised kerb #5A5358.
- Through the slots, a faint #FF6A0D glow at 30%: the lava is under it.

Frame 2, about to give way — the SAME plank, same rivets in the same places:
- Tilt the whole plank 4 degrees clockwise.
- Two of the seven slots are torn open into a ragged hole, and the metal around them is heat-stained: #8A4A2E fading into #FF6A0D at the torn edge.
- The glow through the slots is now #FF8A12 at 70%.
- Three rivets are missing, leaving 6 px dark holes.

No background, no cell outline.
```

## 4. `magma_tube.png` — 256×128, 2 frames, each cell 128×128

The two ends of the lava tube: the ball falls into one and is spat out of the other.

```
Artboard 256 x 128, 2 frames side by side, each 128 x 128. TRANSPARENT.

Two ends of a lava pipe set into the rock floor, seen from DIRECTLY ABOVE.

Frame 1, the mouth that swallows (dark, pulling in):
- A ring of fitted basalt blocks, 10 of them, #3A3338, forming a circle 112 px across with a 4 px #241F24 gap between blocks.
- A 6 px #5A5358 highlight on the upper-left of each block.
- Inside the ring, the pipe: three concentric rings getting darker inward, #2A2429, #1A151A, then near black at the centre, so it reads as depth.
- Spiralling in from the rim, 4 thin #FF6A0D streaks 3 px wide, curving inward clockwise.
- A 6 px #FF6A0D rim light where the stone meets the hole, at 50%.

Frame 2, the mouth that spits (bright, pushing out) — the SAME ring of blocks:
- The centre is molten: #FFC53D at the middle fading out to #FF6A0D at the rim.
- The 4 streaks now curl OUTWARD, anticlockwise, in #FFE9A8.
- 12 sparks 2-4 px #FFF3C4 thrown clear of the ring.
- The stone blocks are heat-stained on their inner faces, #6A3A24.

No arrows, no text, no background.
```

## 5. `magma_heart.png` — 512×256, 2 frames, each cell 256×256

The goal. The hole sits dead centre of the cell, so the two frames must line up exactly.

```
Artboard 512 x 256, 2 frames side by side, each 256 x 256. TRANSPARENT.

The heart of the volcano: a ceremonial basalt basin with a hole at its centre, seen from DIRECTLY ABOVE.

Both frames share this, and it must not move between them:
- An outer ring of 12 carved basalt slabs, #35302F, forming a circle 240 px across, each slab with a 4 px #15110F gap to the next and a 5 px #58504C highlight along its outer edge.
- A band of worn gold inlay inside that, 10 px wide, #C9A227, chipped in four places.
- Inside the gold, a floor of smooth dark rock, #251F1E, with 6 hairline #FF6A0D cracks running from the centre to the gold band.

Frame 1, shut:
- The centre is a plug of cold rock 72 px across, #1A1614, sitting slightly proud, with a 4 px #3A3330 lip.
- The six cracks are dim: #8A3A12 at 50%.

Frame 2, open — everything above unchanged except the middle:
- The plug is gone. In its place a hole 72 px across: black at the rim, a soft #FF6A0D glow 20 px inside it, and a #FFE9A8 heat shimmer at the very centre.
- The six cracks are bright #FF8A12.
- 14 embers 2-5 px #FFC53D rising out of the hole and over the gold band.

No background, no cell outline.
```

## 6. `frozen_bridge.png` — 128×128, 2 frames, each cell 64×128

One plank of the crumbling bridge over the crevasse. Five side by side make a crossing.

```
Artboard 128 x 128, 2 frames side by side, each 64 x 128. TRANSPARENT.

A single weathered timber plank of a rope bridge, seen from DIRECTLY ABOVE, standing upright in the cell: 56 px wide and 120 px tall, centred, 4 px of empty space all round.

Frame 1, sound:
- The timber #8A6A48, with 5 darker #6A4F34 grain lines running the length of the plank and 3 lighter #A98455 ones.
- A 4 px #5A4028 shadow along the bottom and right edges, a 3 px #BE9566 highlight along the top and left.
- A rope lashing across each short end: 3 turns of #D8C9A8 cord 5 px thick, wrapped around the plank, with a frayed end hanging 8 px off one side.
- A dusting of snow on the upper third: an uneven 20 px band of #F2F6FA at 70%, following the grain.

Frame 2, cracked — the SAME plank, the SAME lashings in the SAME places:
- A jagged split running three quarters of the way down the middle of the plank, 4 px wide, showing through to transparent, with 6 px of lighter raw wood #C8A271 along both of its edges.
- Two splinters standing out 8 px from the split.
- The plank sags: tilt it 3 degrees anticlockwise.
- One rope turn has snapped and hangs loose.
- The snow has slid off: only a 6 px rim of it left at the top edge.

No background, no cell outline.
```

---

## When they come back

Check each one the same way as last time: alpha rather than a drawn checkerboard, the file
exactly twice the artboard above, frames of equal width, and anything described as "the
same" between frames genuinely in the same place. Drop them into the folder in the table
and tell me — the loading, the frame counts and the sizes all go in on this side, and the
hand-drawn versions come out.
