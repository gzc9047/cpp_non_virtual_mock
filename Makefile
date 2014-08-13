TMPDIR=/Volumes/RaidRamDisk/
GMOCK_LIB=$(GMOCK_PATH)/lib/.libs/libgmock.a $(GMOCK_PATH)/lib/.libs/libgmock_main.a $(GMOCK_PATH)/gtest/lib/.libs/libgtest.a
GMOCK_INCLUDE=-I$(GMOCK_PATH)/include -I$(GMOCK_PATH)/gtest/include
CPP11_FLAGS=-g -O0 -std=c++11
# Test
test_use_gmock: test_use_gmock.cpp
	g++ $(CPP11_FLAGS) $^ $(GMOCK_INCLUDE) $(GMOCK_LIB) -o $(TMPDIR)/$@
test_member_function_with_gmock: test_member_function_with_gmock.cpp
	g++ $(CPP11_FLAGS) $^ $(GMOCK_INCLUDE) $(GMOCK_LIB) -o $(TMPDIR)/$@
test_static_member_function_with_gmock: test_static_member_function_with_gmock.cpp
	g++ $(CPP11_FLAGS) $^ $(GMOCK_INCLUDE) $(GMOCK_LIB) -o $(TMPDIR)/$@
test_function_pointer_with_gmock: test_function_pointer_with_gmock.cpp
	g++ $(CPP11_FLAGS) $^ $(GMOCK_INCLUDE) $(GMOCK_LIB) -o $(TMPDIR)/$@
test_template_function_with_gmock: test_template_function_with_gmock.cpp
	g++ $(CPP11_FLAGS) $^ $(GMOCK_INCLUDE) $(GMOCK_LIB) -o $(TMPDIR)/$@
# Default
all: \
        test_template_function_with_gmock \
        test_function_pointer_with_gmock \
        test_static_member_function_with_gmock \
        test_member_function_with_gmock \
        test_use_gmock
test: all
	$(TMPDIR)/test_use_gmock \
        && $(TMPDIR)/test_member_function_with_gmock \
        && $(TMPDIR)/test_static_member_function_with_gmock \
        && $(TMPDIR)/test_function_pointer_with_gmock \
        && $(TMPDIR)/test_template_function_with_gmock
