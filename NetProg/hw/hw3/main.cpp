#include "Server.h"
#include <assert.h>

void userTests() {
    User user;

    assert(user.getUsername() == "");

    user.setUsername("Maria");

    assert(user.getUsername() == "Maria");

    assert(user.userIsOperator() == false);

    user.setOperator(true);

    assert(user.userIsOperator() == true);
}

void channelTests() {}

void serverTests() {}

int main(int argc, char** argv) {
    userTests();

    channelTests();

    serverTests();
    return 0;
}
