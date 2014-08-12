TMPDIR=/Volumes/RaidRamDisk/
GMOCK_LIB=/Users/guzuchao/3/code_third/gmock-1.7.0/lib/.libs/libgmock.a /Users/guzuchao/3/code_third/gmock-1.7.0/lib/.libs/libgmock_main.a /Users/guzuchao/3/code_third/gmock-1.7.0/gtest/lib/.libs/libgtest.a
GMOCK_INCLUDE=-I/Users/guzuchao/3/code_third/gmock-1.7.0/include -I/Users/guzuchao/3/code_third/gmock-1.7.0/gtest/include
CPP11_FLAGS=-g -O0 -std=c++11
test_use_gmock: test_use_gmock.cpp
	g++ $(CPP11_FLAGS) $^ $(GMOCK_INCLUDE) $(GMOCK_LIB) -o $(TMPDIR)/$@
test_member_function_with_gmock: test_member_function_with_gmock.cpp
	g++ $(CPP11_FLAGS) $^ $(GMOCK_INCLUDE) $(GMOCK_LIB) -o $(TMPDIR)/$@
all : test_use_gmock test_member_function_with_gmock
