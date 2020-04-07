source = Graphics.cpp Main.cpp Input.cpp
bin = raytrace

CC = g++
LIBRARY = -lSDL2 -lSDL2_image -lm
CFLAGS = 
INCLUDE = .

$(bin) : $(source)
	$(CC) $(CFLAGS) -B$(INCLUDE) -o$(bin) $(source) $(LIBRARY)

clean:
	rm *.o
	rm $(bin)

run:
	./$(bin)