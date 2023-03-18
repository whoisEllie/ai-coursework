gcc dfs.c hashmap.c stack.c fileloader.c -o dfs
echo "Running DFS search:"
./dfs maze-Easy.txt
./dfs maze-Medium.txt
./dfs maze-Large.txt
./dfs maze-VLarge.txt

gcc astar.c minheap.c stack.c hashmap.c fileloader.c -o astar
echo "Running A* search:"
./astar maze-Easy.txt
./astar maze-Medium.txt
./astar maze-Large.txt
./astar maze-VLarge.txt
