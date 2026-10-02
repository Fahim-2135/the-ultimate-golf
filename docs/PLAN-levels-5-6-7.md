# Levels 5, 6 and 7 — the plan

Written 2026-10-02. Three new courses, the menu that has to hold seven of them, and the
five new systems they need. Sounds are deliberately left out: they get wired when the
sound files arrive.

## 1. Why these three

The first four worlds each own a kind of trouble: Foundry is machinery on a timer,
Shoreline is ground that changes under you, Event Horizon is forces acting at a distance,
Lost Temple is puzzles with doors and plates. The next three each have to bring something
the engine cannot do yet, or they are just repaints.

| Level | The world | The new idea |
|---|---|---|
| 5 Frozen Peak | a mountain climb in rising wind | **a force that acts on the whole course**, and ice that barely stops the ball |
| 6 Haunted Manor | a dark house, one candle at a time | **you cannot see the course**; light is spent and earned |
| 7 Magma Core | a descent into an erupting volcano | **the map shrinks as you play**; the only level with time pressure |

## 2. The five new systems

1. **Wind** — a vector added to the ball every frame while it moves, with its own clock:
   breeze, gust, gale. Drawn as drifting snow and a spinning weather vane. Falls on
   `update_ball` exactly like the vacuum strip in level 3, but it covers the whole course
   instead of a rectangle.
2. **The light radius** — the manor draws its floor and obstacles into a render texture,
   then lays a near-black cover over everything except a circle around the ball and a
   circle around each lit candle. Alpha-only drawing, no shaders.
3. **Growth** — the snowball keeps a radius that goes up as it travels, which means its
   collision circle grows too, and it shatters when it hits something solid.
4. **Impulse** — the geyser is the first thing in the game that adds speed in one frame
   rather than pushing continuously.
5. **A hazard line that moves one way** — rising lava. The tide in level 2 already moves a
   boundary, but it comes back; this one does not, so the course below it is gone for good.

## 3. The three courses

### Level 5 — Frozen Peak
Four acts, bottom to top: base camp, the glacier, the ice cave, the summit ridge. The wind
strengthens by act, so the same shot gets harder as you climb.

Objects: packed snow (normal), black ice (friction ~0), deep powder (friction high),
crevasses (hazard), cracking ice bridges (2 passes then gone, back after 5s), the growing
snowball, snow drift pits (sink in 3s, "BURIED!"), icicles (drop when passed under),
the gondola (carries the ball over the widest crevasse), pines (solid), campfires (melt a
circle of ice back to grip), and one avalanche event that pushes the ball back a section.

### Level 6 — Haunted Manor
A hall, a gallery, a library, the crypt. You start with a small light. Candles you roll
over stay lit for the rest of the level, so the house gets more readable the more strokes
you spend — the level's own progression.

Objects: the light radius, lightable candles, blink lamps (flash for ~1s at random and
show a slice of map), a hidden trapdoor they can reveal (drops to a basement shortcut),
the grandfather clock (chimes every 30s), phasing walls (flip on the chime), ghosts (shove
the ball, but only in the dark), cobwebs (drag), bad trapdoors (timed, fall = reset),
the chandelier (drops when crossed under), mirror pairs (enter one, leave the other),
bats, séance plates, the crypt door.

### Level 7 — Magma Core
A descent: the rim, the terraces, the lava lake, the heart. Lava rises the whole time.

Objects: rising lava, eruption vents (erupt at random, flood a radius, drain back; the
hole itself is lethal), lava tubes (in one end, out the other), geysers (impulse),
rock rafts (safe platforms drifting on lava), cooling crust (safe grey / deadly glowing),
cracked rock bridges, steam vents, lava falls, rock showers, obsidian crystals (bouncy),
and the heart, which only opens for a few seconds after an eruption drains.

## 4. What the shell has to learn

- **Seven levels, not four.** `card_texture`, `brief_*`, `score_points` and the start/update/
  draw/unload switches all grow to seven.
- **Menu paging.** Page 1 keeps the familiar 2x2 of levels 1-4. A right arrow (and the
  Right/Left keys) slides to page 2 with levels 5-7. Cards stay live: every level keeps
  being stepped, and one card is redrawn per frame as now.
- **scores.txt grows to seven columns.** Old three-column files must still load, so the
  reader pads what is missing instead of rejecting the line.
- **Stroke limits** for the new levels get set by the bot, the same way as 18/25/20/18.

## 5. Order of work

1. Split the levels out of the single file into `src/levels/*.c`, included in the same
   order. Pure text move, no behaviour change, verified by a build and a screenshot.
2. Generalise the shell to seven levels and add the menu page.
3. Level 5, then 6, then 7 — each built on the closest existing level, then fuzz-tested.
4. Briefings, scoring, leaderboard columns, README and screenshots.
5. Sounds last, when the files arrive.

## 6. What is deliberately not done

- No sounds, by request.
- No new difficulty modes: the three existing ones scale the new levels too.
- The levels keep the same 1920x1080 design grid and the same `u` scale.
