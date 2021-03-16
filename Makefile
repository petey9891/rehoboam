CXXFLAGS=-O2 -W -Wall -Wextra -Wno-unused-parameter -D_FILE_OFFSET_BITS=64                    
OBJECTS=rehoboam.o           
BINARIES=rehoboam            

# Where our library resides. You mostly only need to change the                               
# # RGB_LIB_DISTRIBUTION, this is where the library is checked out.                             
RGB_LIB_DISTRIBUTION=../rpi-rgb-led-matrix
RGB_INCDIR=$(RGB_LIB_DISTRIBUTION)/include
RGB_LIBDIR=$(RGB_LIB_DISTRIBUTION)/lib
RGB_LIBRARY_NAME=rgbmatrix
RGB_LIBRARY=$(RGB_LIBDIR)/lib$(RGB_LIBRARY_NAME).a
LDFLAGS=-L$(RGB_LIBDIR) -l$(RGB_LIBRARY_NAME) -lrt -lm -lpthread -lbrcmEGL -lbrcmGLESv2 -I/opt/vc/include -L/opt/vc/lib


all : $(BINARIES)

$(RGB_LIBRARY): FORCE
	$(MAKE) -C $(RGB_LIBDIR)

rehoboam: rehoboam.o $(RGB_LIBRARY)
	$(CXX) $(CXXFLAGS) rehoboam.o -o $@ $(LDFLAGS)

%.o : %.cpp
	$(CXX) -I$(RGB_INCDIR) $(CXXFLAGS) -c -o $@ $<

rehoboam.o: rehoboam.cpp
	$(CXX) -I$(RGB_INCDIR) $(CXXFAGS) $(MAGICK_CXXFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJECTS) $(BINARIES)

FORCE:
.PHONY: FORCE
