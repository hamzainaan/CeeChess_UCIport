exe = cee
ver = 1.4-dev
src = attack.c bitboards.c board.c data.c evaluate.c \
      hashkeys.c init.c io.c makemove.c main.c misc.c \
      movegen.c perft.c pv.c search.c uci.c
flags = -O3 -s
compiler = gcc
      

help:

	@echo "To compile CeeChess, you can just type; "
	@echo "make win"
	@echo "or"
	@echo "make linux"

clean:

	rm $(exe)_$(ver).exe
	rm $(exe)_$(ver)
	@echo "Successfully cleaned."
	
win:

	$(compiler) $(src) -o $(exe)_$(ver).exe $(flags)
	strip $(exe)_$(ver).exe
	@echo "Compiled successfully."

linux:

	$(compiler) $(src) -o $(exe)_$(ver) $(flags) -lm
	strip $(exe)_$(ver)
	@echo "Compiled successfully."
