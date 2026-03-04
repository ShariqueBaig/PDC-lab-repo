CXX = g++
CXXFLAGS = -std=c++17 -O3 -mavx2 -mfma -fno-tree-vectorize -Wall

TARGET = convolution_challenge
SOURCES = convolution_challenge.cpp

$(TARGET): $(SOURCES)
	$(CXX) $(CXXFLAGS) $(SOURCES) -o $(TARGET)

clean:
	rm -f $(TARGET)

.PHONY: clean
