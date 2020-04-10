source = *.cpp
bin = raytrace

CC = g++
LIBRARY = -lSDL2 -lSDL2_image -lm
CFLAGS = -g -Ofast
INCLUDE = .

$(bin) : $(source) *.hpp
	$(CC) $(CFLAGS) -B$(INCLUDE) -o$(bin) $(source) $(LIBRARY)

clean:
	rm $(bin)
	
run:
	./$(bin)