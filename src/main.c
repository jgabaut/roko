#include "rk_core.h"

int main (int argc, char** argv) {

    printf("\nRoko interpreter v%s.\n\tLoads a program from the passed .rkasm file.\n", string_roko_version());
    printf("Built with amboso v%s.\n",get_ANVIL__API__LEVEL__());
    printf("Tag version: [%s].\n",get_ANVIL__VERSION__());
    printf("Tag version desc: [%s].\n",get_ANVIL__VERSION__DESC__());
    printf("Tag version date: [%s].\n",get_ANVIL__VERSION__DATE__());
    printf("Tag version author: [%s].\n",get_ANVIL__VERSION__AUTHOR__());

    if (argc < 2 ) {
        usage(argv[0]);
        return -1;
    }
    FILE* rkasm_file = fopen(argv[1],"r");
    if (!rkasm_file) {
        fprintf(stderr,"\n\t[ERROR] at %s(): Failed opening rkasm file [%s].\n", __func__, argv[1]);
        return -1;
    }
    Roko* rk = (Roko*) malloc(sizeof(Roko));
    if (!rk) {
        fprintf(stderr,"\n\t[ERROR] at %s(): Failed allocation..\n", __func__);
        return 1;
    }
    rk_init(rk);

    int load_res = load_program_from_file(rkasm_file, rk);

    if (load_res != 1) {
        fprintf(stderr,"\n\t[ERROR] at %s(): load_program_from_file returned [%i].\n", __func__, load_res);
        rk_dump(rk);
        free(rk);
        return 1;
    }

    int res = rk_execute(rk);

    printf("Quitting.\033[1;39m\n");
    printf("%s", (res == 0 ? "\033[1;32m" : "\033[1;31m"));
    printf("Res: {%i}\033[1;39m\n", res);
    fclose(rk->out_fd);
    free(rk);
    return res;
}
