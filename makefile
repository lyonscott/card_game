EXE:=game

CXX= g++

CXXFLAGS+= -std=c++98 -Wall -Wformat
INC+= -I./src

MAIN_SRC+= ./src/main.cpp \
./src/card.cpp
MAIN_OBJS= $(MAIN_SRC:.cpp=.o)


TEST:=tester
TEST_SRC+= ./test/card_test.cpp \
./src/card.cpp
TEST_OBJS= $(TEST_SRC:.cpp=.o)

$(EXE): $(MAIN_OBJS)
	$(CXX) $(CXXFLAGS) $(INC) -o $@ $^

$(TEST): $(TEST_OBJS)
	$(CXX) $(CXXFLAGS) $(INC) -o $@ $^

all: $(EXE) $(TEST)

clean:
	@rm -f $(EXE) $(MAIN_OBJS) $(TEST_OBJS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INC) -c -o $@ $^