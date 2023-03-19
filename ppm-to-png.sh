#!/bin/bash

# get the filename of the image to check from the command line argument
if [ -z "$1" ]
then
    echo "Please provide the filename of the image to check as an argument."
    exit
fi
filename="$1"

# use the file command to get information about the image file
file_info=$(file "$filename")

# extract the image dimensions from the file information
width=$(echo "$file_info" | grep -oE '[0-9]+ x [0-9]+' | cut -d' ' -f1)
height=$(echo "$file_info" | grep -oE '[0-9]+ x [0-9]+' | cut -d' ' -f3)

# scale the image based on it's height
if [ "$height" -gt 640 ]
then
    ffmpeg -i $1 -vf scale=-1:-1:flags=neighbor -loglevel quiet $2
elif [ "$height" -gt 320 ]
then
    ffmpeg -i $1 -vf scale=4*iw:4*ih:flags=neighbor -loglevel quiet $2
elif [ "$height" -gt 80 ]
then
    ffmpeg -i $1 -vf scale=8*iw:8*ih:flags=neighbor -loglevel quiet $2
else
    ffmpeg -i $1 -vf scale=32*iw:32*ih:flags=neighbor -loglevel quiet $2
fi

