TMPDIR=/Volumes/RaidRamDisk/
GMOCK_LIB=$(GMOCK_PATH)/lib/.libs/libgmock.a $(GMOCK_PATH)/lib/.libs/libgmock_main.a $(GMOCK_PATH)/gtest/lib/.libs/libgtest.a
GMOCK_INCLUDE=-I$(GMOCK_PATH)/include -I$(GMOCK_PATH)/gtest/include
CPP11_FLAGS=-g -O0 -std=c++11 -fno-inline -falign-functions=32
SRC=$(wildcard *.cpp ./*.cpp)
TARGETS=$(patsubst %.cpp, %, $(SRC))
TARGETS_PATH=$(patsubst %.cpp, $(TMPDIR)/%.out, $(SRC))
RECENT=test_type_mapper
RECENT_PATH=$(patsubst %, $(TMPDIR)/%.out, $(RECENT))
# Test
%: %.cpp
	g++ $(CPP11_FLAGS) $^ $(GMOCK_INCLUDE) $(GMOCK_LIB) -o $(TMPDIR)/$@.out
# Default
all: $(TARGETS)
# Recent
recent: $(RECENT)
# Test
test: all
	echo $(TARGETS_PATH) | tr " " "\n" | while read test; do echo "Test: $$test"; $$test; done
# Recent test
recent-test: recent
	echo $(RECENT_PATH) | tr " " "\n" | while read test; do echo "Test: $$test"; $$test; done
