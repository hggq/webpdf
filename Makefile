CXX = g++
CXXFLAGS_BASE = -std=c++20 -Wall -Wextra -Iinclude/webpdf
LDFLAGS_BASE = -lz

BUILD_DIR = build
LIB_DIR = $(BUILD_DIR)/lib
BIN_DIR = example

SRCS = src/webpdf.cpp src/request.cpp src/gzip.cpp src/ttfparser.cpp \
       src/parsecss.cpp src/parsehtml.cpp src/parsesvg.cpp
OBJS = $(SRCS:.cpp=.o)

LIB_TARGET = $(LIB_DIR)/libwebpdf.a

EXAMPLE_TARGETS = $(BIN_DIR)/webpdf-hello $(BIN_DIR)/webpdf-render-html

.PHONY: all debug clean

all: CXXFLAGS = $(CXXFLAGS_BASE) -O2 -DNDEBUG
all: LDFLAGS = $(LDFLAGS_BASE)
all: $(LIB_TARGET) $(EXAMPLE_TARGETS)

debug: CXXFLAGS = $(CXXFLAGS_BASE) -fsanitize=address -g -O1
debug: LDFLAGS = $(LDFLAGS_BASE) -fsanitize=address
debug: $(LIB_TARGET) $(EXAMPLE_TARGETS)

$(LIB_DIR):
	mkdir -p $(LIB_DIR)

$(LIB_TARGET): $(OBJS) | $(LIB_DIR)
	ar rcs $(LIB_TARGET) $(OBJS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(BIN_DIR)/webpdf-hello: example/hello.cpp $(LIB_TARGET)
	$(CXX) $(CXXFLAGS) -o $@ $< -L$(LIB_DIR) -lwebpdf $(LDFLAGS)

$(BIN_DIR)/webpdf-render-html: example/render_html.cpp $(LIB_TARGET)
	$(CXX) $(CXXFLAGS) -o $@ $< -L$(LIB_DIR) -lwebpdf $(LDFLAGS)

clean:
	rm -f $(OBJS)
	rm -rf $(BUILD_DIR)
	rm -f $(EXAMPLE_TARGETS)
	rm -f output.pdf