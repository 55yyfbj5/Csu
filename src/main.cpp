#include <iostream>
#include "utils.h"
#include "application.h"

int main(void) {
    Application main_app{};

    if(main_app.initialize()) {
        main_app.run();
    }
    main_app.run();

    return 0;
}
