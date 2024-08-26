# Daisy's Garden 2 Analysis (Legacy System)

## Game time

Every 10 ticks a game second passes.

## World Speed Simulation

1. 2 ticks per second (Slowest)
2. 4 ticks per second (Slow)
3. 8 ticks per second (Normal)
4. 16 ticks per second (Fast)
5. 32 ticks per second (Fastest)

## Movement

The character moves grid by grid and is affected by gravity with the same walking speed.

The walking speed is 8 ticks per block.

The falling speed is 8 ticks per block.

## Input types

### Keyboard

Arrow keys allow for movement in all 4 possible directions.

Key changes are only detected during ticks.

### Mouse

A block can be clicked to target it as a possible target. The character will attempt to reach the target block by priotizing vertical movement over horizontal movement.

## Tiles

### Air

- Air
- Fence
    - Kills Daisy when landing on top
- Bush
    - Kills Daisy when landing on top
- Left cloud
- Right cloud
- Sun
- Left arrow
- Right arrow
- Up arrow
- Down arrow

### Ground

- Ground
    - Is diggable
- Hard ground
- Trap
    - Is diggable
    - Characters can move inside a trap left or right
- Grass ground
    - Is diggable
- Hard grass ground
- Grass trap
    - Is diggable
    - Can move inside in all directions
- Mole hill
    - Kills the playable character when landing on it when the mole looks out of its hole 

## Items

All items need to be collected in order to make a garden finishable

### Fruits

- Apple (100 points)
- Lemon (80 points)
- Cherry (60 points)
- Pineapple (40 points)

### Vegetables

- Garlic (100 points)
    - Marmots run away for 10 seconds
    - Marmots stay away 2 blocks from the playable character
- Mushroom (80 points)
    - Makes invisible and invincible for 10 seconds
- Spinach (60 points)
- Carrot (40 points)
- Flowers
- Sunflower (100 points)
- Tulip (80 points)
- Yellow daisy (60 points)
- Rose (40 points)

## Characters

- Daisy
    - Is playable
- Marmot
    - Inherits Daisy's movement controls and speed
    - Follows Daisy within its static bounding box
    - Can steal items and release them somwehere else
- Grub
    - Moves left and right
    - Has a starting direction
    - Moves 16 ticks per block
    - Requires 6 ticks to turn around

## Entities

- Lift
    - Moves 8 ticks per block
    - Waits for 12 ticks
    - If it is at the border of its static bounding box, then it will change its movement from
        - up to left
        - left to down
        - down to right
        - right to up
    - If it collided with another tile and it is not at the border of its static bounding box, then it will change its movement from
        - up to down
        - left to right
        - down to up
        - right to left