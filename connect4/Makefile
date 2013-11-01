LINK_TARGET = connect4

OBJS =  \
 player.o \
 ranplayer.o \
 main.o

clean : 
	rm -f $(REBUILDABLES)
	echo Clean done

all : $(LINK_TARGET)
	echo All done

$(LINK_TARGET) : $(OBJS)
	g++ -g -o $@ $^

%.o : %.cpp
	g++ -g -o $@ -c $<

main.o : main.cpp player.h ranplayer.h
player.o : player.h
ranplayer.o : ranplayer.h