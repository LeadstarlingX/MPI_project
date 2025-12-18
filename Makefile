# Makefile for MPI Random Matrix Average Distribution (32-it g++)

# Compiler and Flags
CXX = g++
# -DMSMPI_NO_SAL is required for MS-MPI headers to work with g++
CXXFLAGS = -DMSMPI_NO_SAL -I"C:\Program Files (x86)\Microsoft SDKs\MPI\Include"
# Using Lib\x86 because g++ is 32-bit
LDFLAGS = -L"C:\Program Files (x86)\Microsoft SDKs\MPI\Lib\x86" -lmsmpi

# Files
TARGET = main.exe
SRC = main.cpp

# Default target
all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(SRC) -o $(TARGET) $(CXXFLAGS) $(LDFLAGS)

# Run target (defaults to 4 processes)
run: $(TARGET)
	mpiexec -n 4 .\$(TARGET)

# Clean target
clean:
	@if exist $(TARGET) del $(TARGET)
	@echo Cleaned up $(TARGET)

.PHONY: all clean run
