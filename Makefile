CXX = g++
CXXFLAGS = -std=c++11 -pg `Magick++-config --cxxflags --cppflags`
LDFLAGS = -pg `Magick++-config --ldflags --libs`

SRC = main.cpp
OBJ = $(SRC:.cpp=.o)
EXEC = image_resizer

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CXX) -o $@ $^ $(LDFLAGS)

%.o: %.cpp
	$(CXX) -c $< $(CXXFLAGS)

clean:
	rm -f $(OBJ) $(EXEC) gmon.out
