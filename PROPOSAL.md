# Final Project Proposal

## Group Members:

Patrick Was
       
# Intentions:

This project is meant to reinforce using pipes (perhaps sockets, if time allows; would require some changing and reorganizing) for multiplayer/LAN gaming through a card game. It will use semaphores and shared memory so that players have to wait for their turn and to update card values. 
    
# Intended usage:

When the first person runs the game, it will put up a queue indicating how many more people are left to join (4 is the exact number of players allowed). After all players have joined, the game will start. Players will have the option to bid by typing either pass or \<number\>\<suit\> (ex: 3S). Players will have the option to type out what card they want to play by typing in the first letter of the suit and either the card number or the first letter of the card (ex: red queen would be RQ). 

There will also be an additional display if a bid is made so that everyone can see the dummy's cards. 

# Technical Details:

Semaphores: Waiting for turn; nothing can be typed or processed on player's side while it is another player's turn (though they can quit with CTRL-C; perhaps also just makeit so that if they print "quit" they can quit, but anything else is replaced with "you cannot type right now").

(one of) sockets/pipes: Communication with different computers/same computer for the multiplayer experience. 

# Intended pacing:

**January 8th:** Get queue running. Perhaps, if time remains, get card randomization as well. 
**January 10th:** Update technical details accordingly, after more solidified plans have been made. 
**January 18th:** Get play testers (other than having play tested it myself earlier) to catch any bugs, rectify performance, etc. 

**IFF TIME REMAINS:** Make a 1v1 version, perhaps making it so that one person gets two hands. 
