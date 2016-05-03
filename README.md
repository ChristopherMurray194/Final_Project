# Third-Person Shooter Game Prototype
####BSc(Hons) Computer Science (Games)
####Christopher Murray

#How To Play
The **W,A,S,D** keys move the character forward,left,back, and right respectively.

You can rotate the camera using the **mouse**. **Left mouse button** fires the weapon and **right mouse button** aims down the sights.

To crouch, press the **C** key. To stand from crouch, press the **C** key again. Additionally the character can go into a prone position by double-pressing the **C** key. Then can return to a standing
position by double-pressing the **C** key a second time.

You are able to sprint by holding the **left shift** key whilst moving in any direction.

The camera can be rotated around the player's character by double-pressing the **Y** key. It can then be locked behind the player's character again by pressing the **Y** key a second time.

To exit the game at any time, or when you die, press the **Escape** key.

# In-Game features
The AI enemies by default will move along set patrol paths. If you move within their range of vision they will turn to shoot at you, in which case you may either try and escape or shoot them back. If you 
break the AI's line of sight to you, then they will move to the last position they saw you at. If they reach this position and do not see you after a set amount of time, they will resume their patrol.

The health bar at the top left of the screen represents the player's health. If this is fully depleted then the player dies and the game must be exited using the **Escape** key. To start playing again you 
will need to run the executable once again. The player's health does not regenerate. At the bottom right of the screen is the ammo counter.

There is no main objective, you can merely shoot at the AI enemies, once all AI enemies have been killed, there is little else to do.

Doors cannot be opened but you are able to simply walk through them.
