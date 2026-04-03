# Stardew valley for TI84 

This project is still a work in progress, check in later to find more info

Everyone can use this project how ever they like.

Right now you can only walk around on a tilemap, but I will work on this further.

## Installation

To actually play the game you just have to download the bin/SVPort.8xp file to your TI84 plus CE-T using the TI Connect CE software.

If you're using a newer model you have to install the jailbreak which you can find [here](https://calcplex.com/ti84plusce-jailbreak-tutorial/)




## For development:

Everyone is alowed to pull, copy and commit to/from this git.


### Adding thinks:
#### Entities:
1. Add the png files to gfx folder.
2. Add the data to convimg.yaml.
3. Run make gfx.
4. Make a new struct heriting from Entity struct.
5. Implement code.

#### Tilemaps:
1. Check if you use the right tileset.
2. Add .csv file to tilemap folder.
3. Add to makefile.
4. Create new gfx_tilemap_t and init variables in begin().

#### Animations:
1. Add all the png's in a dir to the gfx folder.
2. Create gfx_sprite_t* <name of animation>[4][<number of frames>];
3. Create new function for the amount of frames if necesarry.
4. Add the function you're using to logic().


### TODO:

#### Software:
-Clean the code up  
-Different files for readability  
-Bigger tilemap  
-Easier Tilemaplayers  
-Bigger world -> different tilemaps  

-___Better performance___

#### Start adding actual features from stardew:
-Crop growing, the village, npc's

