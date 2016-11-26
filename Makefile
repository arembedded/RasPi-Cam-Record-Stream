INC_DIR=/home/arembedded/Projects/camera

split: split_cam.c
	gcc split_cam.c -o split.o

all: split

clean:
	rm -rf *.o	
