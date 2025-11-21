all: bin/roast
	
bin/roast: src/roast.c src/read_temp.c src/toggle_heat.c
	$(CC) $(CFLAGS) -o bin/roast src/roast.c src/read_temp.c src/toggle_heat.c -I include

clean:
	rm -f bin/*
