
all: .depend client server

DIR_PREFIX=../
include ../../MakeVars

INSTALL_DIR     = obv/tree
INSTALL_SRCS    = Makefile client.cc server.cc tree.idl \
		  tree_impl.cc tree_impl.h
INSTALL_SCRIPTS = tree

server: tree.h tree_impl.h tree.o tree_impl.o server.o $(DEPS)
	$(LD) $(CXXFLAGS) $(LDFLAGS) tree.o tree_impl.o server.o $(LDLIBS) -o server

client: tree.h tree_impl.h tree.o tree_impl.o client.o $(DEPS)
	$(LD) $(CXXFLAGS) $(LDFLAGS) tree.o tree_impl.o client.o $(LDLIBS) -o client 


tree.h tree.cc : tree.idl $(IDLGEN)
	$(IDL) tree.idl

run:
	tree

clean:
	rm -f tree.cc tree.h
	rm -f *.o core client server *~ .depend *.ref

