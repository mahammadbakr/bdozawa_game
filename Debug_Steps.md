# Debug Steps - Why Cubes Aren't Spawning

## Step 1: Check the Output Log
1. Open Unreal Editor
2. Go to **Window** → **Developer Tools** → **Output Log**
3. Play the level
4. Look for these messages in the log:
   - "SpawnHideAndSeekCubes called!"
   - "About to spawn X cubes"
   - "Attempting to spawn cube X at location: ..."
   - "Successfully spawned cube X" or "Failed to spawn cube X"

## Step 2: Check if GameMode is Working
If you DON'T see "SpawnHideAndSeekCubes called!" in the log:
- The GameMode is not being used
- Go to **Edit** → **Project Settings** → **Maps & Modes**
- Set **Default GameMode** to your Blueprint GameMode

## Step 3: Check if CubeClass is Set
If you see "CubeClass is not set!" in the log:
- The Blueprint GameMode doesn't have the Cube Class set
- Open your Blueprint GameMode
- In the Details panel, find **Hide And Seek** category
- Set **Cube Class** to your BP_HideAndSeekCube

## Step 4: Check if Spawning is Working
If you see "Attempting to spawn cube X" but "Failed to spawn cube X":
- The Blueprint class might not be valid
- Make sure BP_HideAndSeekCube is compiled and saved
- Try using the C++ class directly instead

## Step 5: Check if Cubes are Spawning but Hidden
If you see "Successfully spawned cube X":
- The cubes are spawning but might be:
  - Too small to see
  - Wrong color (invisible)
  - Spawning underground
  - Spawning outside the visible area

## Quick Fix - Use C++ Class Directly
If Blueprint setup is too complex, let's use the C++ class directly:

1. Open your Blueprint GameMode
2. Set **Cube Class** to `HideAndSeekCube` (the C++ class, not the Blueprint)
3. This should work immediately

## Alternative: Spawn Cubes at Fixed Locations
If random spawning isn't working, let's spawn cubes at fixed locations:

1. Open your level
2. Place some basic cubes manually in the level
3. Convert them to HideAndSeekCube Blueprints
4. This will help us test if the pickup system works

## What to Look For:
- **Yellow text** on screen saying "Spawned X hide and seek cubes!"
- **Red/Blue/Green cubes** somewhere in the level
- **Console messages** in the Output Log

## If Nothing Works:
1. Try spawning cubes at the origin (0,0,0) first
2. Make sure the cubes are large and brightly colored
3. Check if the GameMode is actually being used
