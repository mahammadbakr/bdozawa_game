# Quick Setup Guide - Hide and Seek Cubes Not Spawning

## Step 1: Open Unreal Editor
1. Open your Bdozawa project in Unreal Editor
2. If you get build errors, click "Yes" to compile the C++ code

## Step 2: Create Blueprint for HideAndSeekCube
1. In the **Content Browser**, right-click in an empty area
2. Select **Blueprint Class**
3. In the **Pick Parent Class** dialog, search for "HideAndSeekCube" 
4. If you don't see it, click **All Classes** and search for "HideAndSeekCube"
5. Select it and click **Select**
6. Name it `BP_HideAndSeekCube`
7. **Double-click** to open the Blueprint

## Step 3: Configure the Cube Blueprint
1. In the Blueprint editor, select the **CubeMesh** component in the Components panel
2. In the **Details** panel:
   - Set **Static Mesh** to `Engine/BasicShapes/Cube` (or any cube mesh)
   - Set **Material** to a bright color (red, blue, green, etc.)
   - Make sure **Collision** is enabled

3. Select the **PickupSphere** component
4. In the **Details** panel:
   - Set **Sphere Radius** to `100.0` (or adjust as needed)
   - Make sure **Collision** is enabled

5. **Compile** and **Save** the Blueprint

## Step 4: Create Blueprint Game Mode
1. In the **Content Browser**, right-click and select **Blueprint Class**
2. Search for "BdozawaGameMode" and select it
3. Name it `BP_HideAndSeekGameMode`
4. **Double-click** to open it

## Step 5: Configure the Game Mode Blueprint
1. In the Blueprint editor, look for the **Hide And Seek** category in the Details panel
2. Set **Cube Class** to `BP_HideAndSeekCube` (the Blueprint you created)
3. Set **Number Of Cubes** to `2`
4. Adjust **Spawn Area Min** and **Spawn Area Max** if needed (default should work)
5. **Compile** and **Save** the Blueprint

## Step 6: Set Up Your Level
1. Open your main level (e.g., `Lvl_ThirdPerson`)
2. Go to **Edit** → **Project Settings**
3. In **Project Settings**, go to **Maps & Modes**
4. Set **Default GameMode** to `BP_HideAndSeekGameMode`
5. **Save** the project

## Step 7: Test the Game
1. **Play** the level
2. You should see:
   - A message "Spawned 2 hide and seek cubes!" when the game starts
   - Two colored cubes somewhere in the level
   - When you walk close to a cube, it should disappear and show "Cube Picked Up!"

## Troubleshooting

### If cubes still don't spawn:
1. Check the **Output Log** (Window → Developer Tools → Output Log)
2. Look for any error messages
3. Make sure the **Cube Class** is set correctly in the Game Mode Blueprint

### If you can't find the HideAndSeekCube class:
1. Make sure the C++ code compiled successfully
2. Try refreshing the Content Browser (right-click → Refresh)
3. Restart the Unreal Editor

### If the game mode doesn't work:
1. Make sure you set the **Default GameMode** in Project Settings
2. Or manually set it in **World Settings** (Window → World Settings)

## Quick Test
1. Open the level
2. Press **Play**
3. Look around the level for colored cubes
4. Walk close to them - they should disappear when you get near

The cubes spawn at random locations within the spawn area, so they might be hidden behind objects or in corners. Try looking around the entire level!
