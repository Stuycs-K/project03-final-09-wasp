# Final Project Proposal

## Group Members:

Patrick Was
       
# Intentions:

This player is meant to primarily test semaphore and shared memory skills. Shared memory will be incredibly valuable among the processes, since things like player count can't really be done without it unless forking is used. It's also a great way for inter-process communication to occur. Files and semaphores are used for similar purposes. 

# Intended usage:

When the first person runs the game, it will put up a queue indicating how many more people are left to join (4 is the exact number of players allowed). After all players have joined, the game will start. Players will have the option to bid by typing either pass or \<number\>\<suit\> (ex: 3S). Players will have the option to type out what card they want to play by typing in the first letter of the suit and either the card number or the first letter of the card (ex: red queen would be RQ). 

There will also be an additional display if a bid is made so that everyone can see the dummy's cards. 

# Technical Details:

Semaphores: Waiting for turn; nothing can be typed or processed on player's side while it is another player's turn (though they can quit with CTRL-C; perhaps also just makeit so that if they print "quit" they can quit, but anything else is replaced with "you cannot type right now"). Also, one the room is full, no one else can join.

Allocating memory: Used most frequently for arrays, especially for the array that determines the player hand. 

Shared memory: Card statuses, point values, number of players. It's also used to track who is currently winning. 

Signals: Used to quit the game. Unfortunately, besides playing it all the way through, the only way to quit the game is through CONTROL+C.

# Intended pacing:

**January 8th:** Get queue running. Perhaps, if time remains, get card randomization as well. 
**January 12th:** Have all hands be appropriately distributed, with only a few small core features remaining to be added. 
**January 18th:** Get play testers (other than having play tested it myself earlier) to catch any bugs, rectify performance, etc. 

**IFF TIME REMAINS:** Make a 1v1 version, perhaps making it so that one person gets two hands. 
**Scrapped features:** Being able to see your teammate's hand. Now, there aren't really any teammates at all, though 4 players are required because the game would not make sense with 2 and 3 is a weird number honestly. 
