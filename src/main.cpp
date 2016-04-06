

#include <unistd.h>
#include <comm_def.h>
#include "init.h"
#include "net.h"

lonlife_net lnet;

int main(int argc, char** argv) {
    init();
    lnet.init();
    lnet.start();
    sleep(10000);
}
