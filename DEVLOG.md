# Dev Log:

This document must be updated daily by EACH group member... even if there is only 1.

## Patrick Was

### 2025-01-06 - Basic files, skeleton

**~35 minutes:** Editing README.md, DEVLOG.md, and PROPOSAL.md. README.md includes how to play the game (not just the game on the computer, but also the actual game), though both PROPOSAL and README still have to be finished (proposal just to have more specifications). 

**30 minutes:** Added skeleton (bridge.h file), though this might greatly be modified. More .h files and certainly more functions will need to be created. 

### 2025-01-07 - Rules, FLOWCHART.md, editing README.md and DEVLOG.md.

**40 minutes:** Clarifying rules of the game, appending them in the README.md. Created a FLOWCHART.md to help with organization and to get a more specific flow of the game than would be offered in either the PROPOSAL or README (for personal use). 

### 2025-01-08 - Added files, changes to bridge.c and bridge.h

**40 minutes:** bridge.c and bridge.h skeleton structure changed to include more functions. Also, added more <.h> files relating to the project, and fixed bugs (ex. having stdin.h instead of stdio.h). 

### 2025-01-09 - Working on PLAYER QUEUE 

**40 minutes**: Implemented the semahpores that are going to be used for player queue. Seems to work for 2 players, but not really for 4. 

### 2025-01-10 - Working on DISPLAYING CARDS and CARD VALUE

**40 minutes:** Worked on a card value function that returns some arbitrary (one that I assigned) numerical value to the cards. All that matters is that some cards are greater than others; their raw point values don't actually matter. 

### 2025-01-13 - Once again working on PLAYER QUEUE

**40 minutes:** (Desparately) attempting to extend this player queue to that of 4 players. 

### 2025-01-14 - Working on PLAYER (includes queue) and GAME LOGIC

**40 minutes:** Queue basically seems to work now, though I somehow broke it in the middle and had to refix it (though my refixing it was putting it into some prior state that I thought didn't work? Life sure is confusing). 

**35 minutes:** Game logic such as comparison of cards and when that should actually take place. 

### 2025-01-15 - FLOWCHART, shared memory massacre

**40 minutes:** moving things to diffferent files, making more spots in shared memory for other things such as point values, current winner, etc. Shared memory saved my life. 

### 2025-01-17 - Crazy File Changes

**60 minutes:** Turns out, some of those files I created may not be necessary after all. FLOWCHART.md seems rather redundant and unnecessary, especially since I know what I'm doing. Additionally, removed some unnecessary h files, like bridgeSemaphores.h or bridge.h. Replaced with game.c and player.c.

### 2025-01-18 - Handling

**50 minutes:** Like handling a baby, we want to be careful when handling these cards. Thus, a nice shuffle should happen in some text file, and then that text file can be read by each process (I do not want to go through the pain of putting arrays into shared memory. No thank you. This is okay). Also, made sure that each player gets exactly 13 cards. 

### 2025-01-19 - Reinvestigation

**75 minutes:** Semaphores kind of just broke. I don't know why, but I managed to fix it again. 

### 2025-01-20 - Bug Fixes

**90 minutes:** Attempted to fix bugs regarding the lack of displaying of victors after tricks are over. What makes this weirder is that rounds to end properly.  

### 2025-01-21 - Working on Video

**30 minutes:** Getting final video for presentation.
