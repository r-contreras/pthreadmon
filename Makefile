PREFLAGS=-g `pkg-config --cflags gtk+-3.0` 
POSTFLAGS =  -lm -pthread `pkg-config --libs gtk+-3.0`
all: bin/pthreadmon
	@echo ""
	@echo "Ejecutable en bin/pthreadmon"
	

bin/pthreadmon:  src/main.c src/controller/arena.c src/controller/pokemon.c src/controller/pokedex.c src/view/main_window.c src/view/fight_window.c src/view/choose_pokemon_window.c | bin/.
	gcc $(PREFLAGS) -L/$ORIGIN/lib $^ -o $@ $(POSTFLAGS)

.PRECIOUS: %/.
%/.:
	mkdir -p $(dir $@)

.PHONY: clean
clean:
	rm -rf bin
