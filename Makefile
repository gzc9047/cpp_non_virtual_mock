TMPDIR=/Volumes/RaidRamDisk/
GMOCK_LIB=$(GMOCK_PATH)/lib/.libs/libgmock.a $(GMOCK_PATH)/lib/.libs/libgmock_main.a $(GMOCK_PATH)/gtest/lib/.libs/libgtest.a
GMOCK_INCLUDE=-I$(GMOCK_PATH)/include -I$(GMOCK_PATH)/gtest/include
CPP11_FLAGS=-g -O0 -std=c++11 -fno-inline -falign-functions=32
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
test_variable_template: test_variable_template.cpp
	g++ $(CPP11_FLAGS) $^ $(GMOCK_INCLUDE) $(GMOCK_LIB) -o $(TMPDIR)/$@
test_more_case: test_more_case.cpp
	g++ $(CPP11_FLAGS) $^ $(GMOCK_INCLUDE) $(GMOCK_LIB) -o $(TMPDIR)/$@
test_type_mapper: test_type_mapper.cpp
	g++ $(CPP11_FLAGS) $^ $(GMOCK_INCLUDE) $(GMOCK_LIB) -o $(TMPDIR)/$@
test_special_call: test_special_call.cpp
	g++ $(CPP11_FLAGS) $^ $(GMOCK_INCLUDE) $(GMOCK_LIB) -o $(TMPDIR)/$@
# Default
all: \
        test_special_call \
        test_type_mapper \
        test_more_case \
        test_variable_template \
        test_template_function_with_gmock \
        test_function_pointer_with_gmock \
        test_static_member_function_with_gmock \
        test_member_function_with_gmock \
        test_use_gmock
# Recent
recent: test_more_case
# Test
test: all
	$(TMPDIR)/test_use_gmock \
        && $(TMPDIR)/test_member_function_with_gmock \
        && $(TMPDIR)/test_static_member_function_with_gmock \
        && $(TMPDIR)/test_function_pointer_with_gmock \
        && $(TMPDIR)/test_template_function_with_gmock \
        && $(TMPDIR)/test_variable_template \
        && $(TMPDIR)/test_more_case \
        && $(TMPDIR)/test_type_mapper
# Recent test
recent-test: recent
	$(TMPDIR)/test_more_case
