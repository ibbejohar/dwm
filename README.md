## Personal Patched Suckless DWM

### First patch - Pertag

Resize and change layout indenpendly on each tags.

### Second patch - Barconfig

Able to freely rearrange bar modules

### Third patch - Blanktags

Make tags blank and draw square if there a are any windows in the tag.

### Fourth patch - Fix Barconfig bugs

If modules where to move from the original place the mouse click event on the modulen stop working. This fix patch fixis it.
This patch is generated from Claude.

### Fifth patch - Actualfullscreen

Fullscreens the window instead of just disabling the dwm bar.

### Sixth patch - Losefullscreen

Lose fullscreen when a new window spawn.

### Seventh patch - Attachbottom

Attach spawned windows bottom of stack instead of top of the stack.

### Eight patch - Focusdir

Directional focus instead of moving focus by circle through windows.

### Ninth patch - Placedir

Directional movement of windows instead of circle throught windows.

### Tenth patch - Smartborders

Draw only border if there is more than 1 windows or if they are floating window.

### Eleventh patch - Swallow

Swallow child process spawn from terminal and hide the terminal. 

**Additional dep**
libxcb
Xlib-libxcb
xcb-res

### Twelfth patch - Alwayscenter

Spawn floating windows center of the screen.

### Thirteenth patch - Namedscratchpads

Assign any window with unique id and able to spawn or hide/reveal with the same key.

### Fourteenth patch - Restartsig

Restart dwm

### Fifteenth patch - Restoreafterrestart

Restore state after restart dwm.

### Sixteenth patch - Keychord

Able to chain multiple keybining.

### Seventeenth patch - Splitstatus

Split status to two part. This is patched mave been modified by Claude to work for dwm6.8

### Eighteenth patch - Status2d

Give colors and able to draw on status. Modifed with Claude to work with the current heavly patched dwm.
