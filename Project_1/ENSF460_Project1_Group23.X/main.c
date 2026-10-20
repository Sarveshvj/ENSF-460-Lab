/* 
 * File:   main.c
 * Author: Steven Wu
 *
 * Created on October 19, 2026, 11:03 PM
 */


#include <xc.h>
#include "IOs.h"

int main(void) {

    IOinit();

    while(1) {

        Idle();

    }

    return 0;
}
