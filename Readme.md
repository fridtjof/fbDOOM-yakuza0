# DOOM in Yakuza 0

## How to use

- Download `libtgsa.dll` in the Releases section (off to the right)
- place it next to Yakuza0.exe in the game directory
- Put an appropriate `doom.wad` in there as well
- Go to your arcade of choice in-game and play any arcade game :)

### Controls

- Movement uses the left stick (right stick works too but is just duplicated)
- B/RT - Fire, back in menus
- A/LT - Use, select in menus
- X/LB?/RB? - Escape (Pause menu, back)
- No weapon selection :(

## How to build

- I built this with mingw-w64, so I don't know if it works with MSVC as well (it needs make, anyway..)
- run `make PLATFORM=windows NOSDL=1 SHARED=1`
- rename fbdoom.dll to libtgsa.dll

## TODOs
- Inputs need more debouncing(?) they're a bit prone to repeats or skips
- Doom is running kind of slow, feels weird/sluggish?
- ~~Add something for input 4~~? None of the y0 arcade games map more than 3 buttons (aside from movement)
- Sound
  - technically possible - Yakuza provides a buffer for output
  - but Doom would actually have to produce sound (the raw samples kind) first
- See if this works with other games from the studio
