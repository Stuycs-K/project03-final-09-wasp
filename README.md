[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-22041afd0340ce965d47ae6ef1cefeee28c7c493a6346c4f15d667ab976d596c.svg)](https://classroom.github.com/a/Vh67aNdh)
# Bridging Connections

### Hive Mind

Patrick Was [19]
       
### Project Description:

Originally, I was going into this project expecting it to be a game of bridge--that did not exactly happen. It became more and more of an abridged version of bridge, and suddenly it was an abridged abridged bridge... until it just became my own game (though not entirely finished) that I have made myself. Essentially, this is a card game that involves waiting for the next player and only being able to see the previous player's card. 
 
### Instructions:

**Vocabulary:** Fortunately, since the creation of this odd version of my game, the vocabulary list has DRASTICALLY decreased. As a matter of fact, there is only one word that is even relevant at this point: 

- *Trick*: A round where each player takes turns placing a card on the table, and the card with the highest value (determined by rules of the game) wins.

**Game:** The goal of the game is to get as close to possible to 6 tricks. You type in a card that is in your hand (displayed at the start of every turn). You can either base your judgement off the plays of others (though you can only see the previous player's card...) or try to wing it. 

The suites are in order alphabetically (so clubs are weakest). Other standard rules apply, like Ace being the most powerful member of the suite. 

**Compiling:** Before joining the game, at least one person must run "make games", which will compile and run ./game. This sets up many useful things, like files, shared memory, and semaphores. Once this has been run, every player may then run "make player", and they will join the player queue. Everything after that is rather self-explanatory, since all that is really required of the player is to type a card that is in the hand. 

**BUGS:**

