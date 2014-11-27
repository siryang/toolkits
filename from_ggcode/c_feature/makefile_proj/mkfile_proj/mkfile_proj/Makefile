
CC=gcc
CXX=g++
TARGET=mkfile_proj.exe
SOURCE=../code/main.cpp

LIBS=-lpthread

INCLUDE=.

OBJ=$(join $(addsuffix ../obj/, $(dir $(SOURCE))), $(notdir $(SOURCE:.cpp=.o)))

all: $(TARGET)
	
.PYTHON: all

clean:
	@-rm -f $(TARGET) $(OBJ)
	
$(TARGET):$(OBJ)
	@$(CXX) -o $@ $^ $(LIBS)

$(OBJ):$(SOURCE)
	$(CXX) -c $< -o $@
	

