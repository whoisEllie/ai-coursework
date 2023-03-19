# Pathfinding (DFS and A*)
## University of Exeter ECM2423 Continuous Assessment

#### Package contents

This package includes the following code:
- `astar.c` - an implementation of the A* algorithm using the C programming language
- `dfs.c` - an implementation of the depth-first search algorithm in the C programming language
- `fileloader.c/h` - a generic function for loading a .txt file into the format used by both `astar.c` and `dfs.c`
- `hashmap.c/h` - a hashmap used for storing information that necessitates fast retrieval, used from [https://github.com/tidwall/hashmap.c](https://github.com/tidwall/hashmap.c)
- `minheap.c/h` - a generalised implementation of a min heap data structure, for use with `astar.c`
- `stack.c/h` - a generalised implementation of a stack data structure, used with both `astar.c` and `dfs.c`

In order to run the code, a number of shell scripts are included:
- `run-all.sh` is an all-purpose script used to run both algorithms on all 4 provided mazes. It also converts output .ppm images to .png using the ffmpeg library, and upscales them as required. If you do not have ffmpeg, you can get it from [here](https://ffmpeg.org/)
- `ppm-to-png.sh` is the script which converts and upscales the output .ppm images to the more common and usable .png. It's used within `run-all.sh`
- `clean-up.sh` removes any generated files from the source folder, including images, maze solutions and compiled binaries.

#### Running the code

To run the above scripts, you first must give them permission to execute on your system. you can do so on unix-based systems with the following commands:
```
chmod +rwx run-all.sh
chmod +rwx ppm-to-png.sh
chmod +rwx clean-up.sh
```
On windows and othe operating systems, you may have to execute files manually. To do so, you can run the following commands:

```
  gcc dfs.c hashmap.c stack.c fileloader.c -o dfs
  gcc astar.c minheap.c stack.c hashmap.c fileloader.c -o astar
```

Compiles both the dfs and astar binaries.

Both of these binaries run with the same syntax:

```
  ./binary path-to-maze
```

For example:
```
  ./astar maze-Easy.txt
```
will run the astar algorithm on the `maze-Easy.txt` file bundled.

This will generate .ppm images and solution files.

In order to convert an image from .ppm to .png, you can use the `ffmpeg` library:
```
  ffmpeg -i $1 -vf scale=X*iw:X*ih:flags=neighbor -loglevel quiet $2
```

Where X is the factor you wish to scale the image by. This can also be individual for the width and the height, but this is not recommended.

#### Understanding the output

In the command line, the program will print statistics about it's performance, including it's runtime, the amount of nodes explored, and the amount of nodes in the final path. It will also output 2 files: an image visualisation in .ppm format and a .txt containing the final solution path as determined by the algorithm.

The file names will also correspond to the type of algorithm you've used to run them, and whether or not you've run the binaries through `run-all.sh`. If you have run through the script, you will find just a converted .png file, rather than a .ppm. A sample output for solving `maze-Easy.txt` with the dfs algorithm is:

```
  maze-Easy-dfs.ppm // the output ppm image, assuming it hasn't been converted with ppm-to-png.sh
  maze-Easy-solution-dfs.txt // The output path file
```
