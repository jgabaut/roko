#include "rk_core.h"

int main (int argc, char** argv) {

    //printf("\nRoko interpreter v%s.\n\tLoads a program from the passed .rkasm file, or from stdin.\n", string_roko_version());

    /*
    printf("Built with amboso v%s.\n",get_ANVIL__API__LEVEL__());
    printf("Tag version: [%s].\n",get_ANVIL__VERSION__());
    printf("Tag version desc: [%s].\n",get_ANVIL__VERSION__DESC__());
    printf("Tag version date: [%s].\n",get_ANVIL__VERSION__DATE__());
    printf("Tag version author: [%s].\n",get_ANVIL__VERSION__AUTHOR__());
    */
    FILE* prog_file = NULL;

    if (argc == 2 ) {
        prog_file = fopen(argv[1],"r");
        if (!prog_file) {
            fprintf(stderr,"\n\t[ERROR] at %s(): Failed opening program file [%s].\n", __func__, argv[1]);
            return -1;
        }
    } else if (argc != 1) {
        usage(argv[0]);
        return 1;
    } else {
        prog_file = stdin;
    }

    Roko* rk = (Roko*) malloc(sizeof(Roko));
    if (!rk) {
        fprintf(stderr,"\n\t[ERROR] at %s(): Failed allocation..\n", __func__);
        return 1;
    }

    rk_init(rk,argc);

    int load_res = load_program_from_file(prog_file, rk);

    if (load_res != 1) {
        fprintf(stderr,"\n\t[ERROR] at %s(): load_program_from_file returned [%i].\n", __func__, load_res);
        rk_dump(rk);
        free(rk);
        return 1;
    }

    int res = rk_execute(rk);

    /*
    printf("\nQuitting.\033[1;39m\n");
    */
    #ifndef _WIN32
    printf("%s", (res == 0 ? "\033[1;32m" : "\033[1;31m"));
    printf("Res: {%i}\033[1;39m\n", res);
    #else
    printf("Res: {%i}\n", res);
    #endif
    fclose(rk->out_fd);
    free(rk);
    return res;
}
