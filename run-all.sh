gcc dfs.c hashmap.c stack.c fileloader.c -o dfs
echo "Running DFS search:"
./dfs maze-Easy.txt
./dfs maze-Medium.txt
./dfs maze-Large.txt
./dfs maze-VLarge.txt
echo ""
echo "Encoding images:"
./ppm-to-png.sh maze-Easy-dfs.ppm maze-Easy-dfs.png
./ppm-to-png.sh maze-Medium-dfs.ppm maze-Medium-dfs.png
./ppm-to-png.sh maze-Large-dfs.ppm maze-Large-dfs.png
./ppm-to-png.sh maze-VLarge-dfs.ppm maze-VLarge-dfs.png
rm ./maze-Easy-dfs.ppm
rm ./maze-Medium-dfs.ppm
rm ./maze-Large-dfs.ppm
rm ./maze-VLarge-dfs.ppm
echo ""

gcc astar.c minheap.c stack.c hashmap.c fileloader.c -o astar
echo "Running A* search:"
./astar maze-Easy.txt
./astar maze-Medium.txt
./astar maze-Large.txt
./astar maze-VLarge.txt
echo ""
echo "Encoding images:"
./ppm-to-png.sh maze-Easy-astar.ppm maze-Easy-astar.png
./ppm-to-png.sh maze-Medium-astar.ppm maze-Medium-astar.png
./ppm-to-png.sh maze-Large-astar.ppm maze-Large-astar.png
./ppm-to-png.sh maze-VLarge-astar.ppm maze-VLarge-astar.png
rm ./maze-Easy-astar.ppm
rm ./maze-Medium-astar.ppm
rm ./maze-Large-astar.ppm
rm ./maze-VLarge-astar.ppm
