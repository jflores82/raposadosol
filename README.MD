# Raposa do Sol SMS
This is a Sega Master System game "inspired" by the Atari 2600 game Solar Fox. 
"Raposa do Sol" means Sun Fox in pt-br.

Coded by [tibonev](http://classicgames.com.br)


using the [SMSDevkit by sverx](https://github.com/sverx/devkitSMS)


levels designed using [Tiled](http://mapeditor.org)


music done using [Furnace](https://tildearrow.org/furnace/)


To build it, you need a working copy of SDCC and SMSDevkit, with crt0, lib(SMSlib) and PSGlib inside their own folders, relative to main.c, if you're in a windows enviroment, then all you need to do is run build.bat.
Otherwise you might need to execute the commands yourself. 

There's a PHP companion tool, to convert Tiled's CSV output to C Array used in the code.

## Instructions:
- Move the ship with the D-Pad
- Hold Button 1 to use the speed boost.

Your mission, choose you accept it is to recover all the satelittes without getting shot by the turrets, good luck.
Oh yeah, there's a time limit also. oops.

All the assets (code, graphics and music) were created by tibonev. 
Everything is licensed under the MIT License (check out LICENSE for more info).
(c) 2024 tibonev.

### Version History: 

V0.15 (26/03/24)
- Added Fixed World Levels.
- Adjusted player bounderies within the screen.
- Fixed typo on DoLivesScreen.
- Fixed bug: 'hi-score not showing after game end'.
- Added "lives" jingle.
- Added fixed world music.
- Added random world music.

V0.14 (25/03/24)
- Added game over jingle.
- Added menu music.
- Added doGameEnd (ending screen for the fixed world campaign);
- Added Title Screen with logo.
- Started separating the code into multiple files for easier reading (will require rewrite of a lot of stuff, not sure if its worth the hassle for this game.)

V0.13 (19/03/24)
- Github Repository created.

V0.12 (12/03/24)
- Added random stars in the background.
- Randomized pallete for the level background (random world game);
- Reduced level timer counter to 10s.

V0.11 (11/03/24)
- Added background tile loading without tilemaps.

V0.10 (07/03/24)
- Fixed SFX looping bug. 

V0.09 (05/03/24)
- Added timer per level.
- Added on-screen counter for timer.
- If time is out, you die.
- Removed unused font-test asset.
- Added title screen music V1.
- Added player die explosion SFX.
- Added enemy_shot SFX.
- Added menu_tick SFX.
- Added pellet pickup SFX.

V0.08 (01/03/24)
- To avoid slowdown, moved all walls to background tiles.
- Have to remove scroll.
- fixed player getting stuck to walls
- added menu for gaming modes

V0.07 (29/02/24)
- Created PHP Tiled->Game Array conversion to facilitate the creation of levels.

V0.06 (28/02/24)
- Added Player explosion Animation
- fixed gamestate bug;

V0.05: (26/02/24)
- Added Walls Sprites
- Added Player-Walls Collisions

V0.04: (23/02/24)
- Fixed error on text rendering off position;

V0.03: (22/02/24)
- Rudimentary gamestates implemented (still buggy);
- Added pause button support to pause game;

V0.02: (21/02/24)
- Added rudimentary title screen
- Added lives / score showing screen

V0.01: (20/02/24)
- Random Generated Levels
- Player is controllable with 2 speeds
- Collision detection of both player-pellet / player-enemy shot are working

### To do:
- sfx continue playing bug? primary focus, V15 has "pickup_psg" disabled.
- Re-add Scrolling (how to shift all the collisions without slowing down?)
- Separate code into files / makefile (will require re-write, not worth the effort?)
- hi-score sram? (understand how to make sram work)