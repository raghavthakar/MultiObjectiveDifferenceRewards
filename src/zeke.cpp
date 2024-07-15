#include "zeke.h"
#include "evolutionary_utils.h"
#include "environment.h"
#include "policy.h"
#include "team.h"
#include <vector>

const int NONE = std::numeric_limits<int>::min();

Zeke::Zeke() {
    int x = 2;
}

void Zeke::evolve() {
    std::cout<<"Evolve method!\n";
}

int main() {
    Zeke test_zeke;
    test_zeke.evolve();
}