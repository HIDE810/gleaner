TARGET	:=	gleaner
SRC		:= 	gleaner.cpp

ifeq ($(OS),Windows_NT)
all:	$(TARGET)
else
all:
	@echo "Error: Build this on Windows."
endif

$(TARGET):	$(SRC)
	@$(CXX) $^ -o $@

clean:
	@$(RM) $(TARGET)