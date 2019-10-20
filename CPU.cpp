#include <cstdio>
#include <cassert>
#include <iostream>
//#include <string>
#include <cmath>
#include "my_map.h"
#include "ctime"

using std::cout;
using std::endl;
using std::max;
using std::min;

#define DEBUG

#include "my_stack.h"
#include "dump.h"
#include "my_file_lib.h"
#include "super_define.h"
#include "instructions.h"

const char VERSION[4] = "2.1";

//registers
int registers[10] = {};

/*!
 * main function of CPU
 *
 * @param[in] code input code to run
 * @param[int] code_size size of code
 * @return code of error
 */

int processing(char* code, size_t code_size);



int main(int argc, char *argv[]) {

    char name_of_input_file[FILENAME_MAX + 1] = {};  //"/Users/alex/Desktop/~X/cpu_code.bin";
    strcpy(name_of_input_file,  X_("cpu_code.bin"));
//    cout << name_of_input_file << endl;

    char* code = nullptr;
    size_t code_size = get_buffer(name_of_input_file, &code);
    if (code == nullptr) {
        dump(DUMP_INFO, "CPU: Error opening input file");
        return 3;
    }

    return processing(code, code_size);
}

int processing(char* code, size_t code_size) {
    if (code == nullptr) {
        dump(DUMP_INFO, "cpu: code == nullptr");
        return 1;
    }

    char* code_to_free = code;
    char* code_begin = code + sizeof(Signature);

    ((Signature*)code)->dump();

    if (strcmp(((Signature*)code)->version_, VERSION) != 0) {
        dump(DUMP_INFO, "Versions of asmed code and cpu are not equal!\n");
        return 5;
    }

    code += sizeof(Signature);

    Stack(St);
    Stack(Func_calls);

    std::clock_t timer = std::clock();

#define DEF_CMD(name, num, next, cpu_code, disasm_code) \
    case num: cpu_code; code += next; break;

    while (code_to_free + code_size > code) {
//        St.dump(DUMP_INFO);
//        Func_calls.dump(DUMP_INFO);
        switch (*code) {

            #include "commands.h"

            default:
                printf("CPU: Unknown command!\n");
                return 2;
        }
    }
#undef DEF_CMD

    secure_free(&code_to_free);

    printf("Runing time: %f\n", (double)(std::clock() - timer) / CLOCKS_PER_SEC);
    printf("\nCPU is OK\n\n");

    return 0;
}