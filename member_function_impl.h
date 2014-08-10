#include <string>

using namespace std;


struct NonMocked {
    string returnNonMockedString();
};

struct Mocked {
    string returnMockedString();
};

