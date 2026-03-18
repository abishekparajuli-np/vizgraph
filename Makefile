CXX = g++
CXXFLAGS = -std=c++17 -Wall
SFML_FLAGS = -lsfml-graphics -lsfml-window -lsfml-system

TARGET = visualizer
SRCS = main.cpp utils.cpp linegraph.cpp
OBJS = $(SRCS:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $(TARGET) $(SFML_FLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)