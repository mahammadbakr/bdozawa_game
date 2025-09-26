# Hide and Seek Game Setup Guide

## Overview
This guide will help you set up the hide and seek game in Unreal Engine. The game features two cubes that spawn at random locations and can be picked up by the player.

## C++ Classes Created

### 1. HideAndSeekCube.h & HideAndSeekCube.cpp
- **Purpose**: Base C++ class for the hide and seek cubes
- **Features**:
  - Static mesh component for visual representation
  - Sphere collision component for pickup detection
  - Auto-pickup when player gets close
  - Blueprint events for visual feedback

### 2. Modified BdozawaGameMode
- **Purpose**: Game mode that spawns cubes at random locations
- **Features**:
  - Configurable number of cubes (default: 2)
  - Random spawn area configuration
  - Automatic cube spawning on game start

## Blueprint Setup Instructions

### Step 1: Create Blueprint for HideAndSeekCube
1. In the Content Browser, right-click and select **Blueprint Class**
2. In the **Pick Parent Class** dialog, search for "HideAndSeekCube" and select it
3. Name it `BP_HideAndSeekCube`
4. Open the Blueprint

### Step 2: Configure the Blueprint
1. **Set the Static Mesh**:
   - Select the **CubeMesh** component
   - In the Details panel, set **Static Mesh** to a cube mesh (Engine/BasicShapes/Cube)
   - Set **Material** to a bright color (e.g., red or blue) for visibility

2. **Configure Collision**:
   - The **PickupSphere** component is already set up for collision detection
   - Adjust the **Sphere Radius** if needed (default: 100 units)

3. **Add Visual Effects** (Optional):
   - In the **Event Graph**, implement the Blueprint events:
     - `OnCubePickedUp`: Add particle effects, sound, or animation
     - `OnPlayerEnterRange`: Add glow effect or highlight
     - `OnPlayerLeaveRange`: Remove effects

### Step 3: Create Blueprint Game Mode
1. In the Content Browser, right-click and select **Blueprint Class**
2. Search for "BdozawaGameMode" and select it
3. Name it `BP_HideAndSeekGameMode`
4. Open the Blueprint

### Step 4: Configure the Game Mode Blueprint
1. **Set Cube Class**:
   - In the Details panel, find **Hide And Seek** category
   - Set **Cube Class** to `BP_HideAndSeekCube`

2. **Configure Spawn Area** (Optional):
   - Adjust **Spawn Area Min** and **Spawn Area Max** to define where cubes can spawn
   - Default: -1000 to 1000 units in X and Y, 0 to 0 in Z

3. **Set Number of Cubes**:
   - Set **Number Of Cubes** to 2 (or desired amount)

### Step 5: Set Up the Level
1. Open your main level (e.g., `Lvl_ThirdPerson`)
2. In the **World Settings** panel:
   - Set **Game Mode Override** to `BP_HideAndSeekGameMode`

### Step 6: Test the Game
1. Compile the C++ code in Visual Studio
2. Open Unreal Editor
3. Play the level
4. You should see:
   - A message "Spawned 2 hide and seek cubes!" when the game starts
   - Two colored cubes at random locations
   - When you walk close to a cube, it will be picked up automatically
   - A message "Cube Picked Up!" when you collect a cube

## Game Features

### Automatic Pickup
- Cubes are automatically picked up when the player gets within 100 units
- No manual input required - just walk close to the cubes

### Visual Feedback
- On-screen debug messages show when cubes are spawned and picked up
- Blueprint events allow for custom visual effects

### Random Spawning
- Cubes spawn at random locations within the defined area
- Each game session will have different cube positions

## Customization Options

### Cube Appearance
- Change the static mesh to any shape you want
- Apply different materials for different colors
- Add particle effects or animations

### Pickup Behavior
- Modify the sphere radius for different pickup ranges
- Add manual pickup with mouse click instead of automatic
- Add inventory system to track collected cubes

### Game Rules
- Change the number of cubes to spawn
- Add win condition when all cubes are collected
- Add timer for time-based gameplay
- Add scoring system

## Troubleshooting

### Cubes Not Spawning
- Check that the Game Mode is set correctly in World Settings
- Verify that the Cube Class is set in the Game Mode Blueprint
- Check the Output Log for any error messages

### Cubes Not Picking Up
- Ensure the player character has a Pawn collision channel
- Check that the PickupSphere collision settings are correct
- Verify the player is close enough to the cube

### Visual Issues
- Make sure the static mesh is set on the CubeMesh component
- Check that materials are applied correctly
- Verify the cube is not hidden or scaled to zero
