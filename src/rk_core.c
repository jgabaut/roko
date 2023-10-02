#include "rk_core.h"

const char* string_roko_version(void) {
	return ROKO_API_VERSION_STRING;
}

const int int_roko_version(void) {
	return ROKO_API_VERSION_INT;
}

void usage(const char* prog) {
    fprintf(stderr,"\nUsage: %s <.rkasm file>\n", prog);
}

const char* RK_OP_Str[RK_TOT_OP] = {
    "RK_INVALID_OP",
    "RK_READ",
    "RK_WRITE",
    "RK_LOAD",
    "RK_STORE",
    "RK_ADDINT",
    "RK_SUBINT",
    "RK_DIVINT",
    "RK_MULINT",
    "RK_DIVMOD",
    "RK_JMP",
    "RK_JMPNEG",
    "RK_JMPZERO",
    "RK_HALT",
};

void rk_init(Roko* rk) {
    if (!rk) {
        return;
    }
    for (int i = 0; i < RK_MEM_SIZE; i++) {
        rk->memory[i].data.as_i64 = 0;
    }
    rk->reg.data.as_i64 = 0;
    rk->ic = 0;
    rk->curr_op = RK_INVALID_OP;
    rk->operand.data.as_i64 = 0;
    rk->ir.data.as_i64 = 0;
    rk->coredump_fd = stderr;
    const char* outfile_path = "./roko_out.txt";
    FILE* outfile = fopen(outfile_path, "w");
    if (!outfile) {
        fprintf(stderr,"\n[ERROR] at %s(): failed opening outfile {%s}.\n", __func__, outfile_path);
    }
    rk->out_fd = outfile;
    rk->verbose_level = 1;
}

const char* rk_op_Str(RK_OP op) {
    switch (op) {
        case RK_INVALID_OP: {
            return RK_OP_Str[RK_INVALID_OP_I];
        }
        break;
        case RK_READ: {
            return RK_OP_Str[RK_READ_I];
        }
        break;
        case RK_WRITE: {
            return RK_OP_Str[RK_WRITE_I];
        }
        break;
        case RK_LOAD: {
            return RK_OP_Str[RK_LOAD_I];
        }
        break;
        case RK_STORE: {
            return RK_OP_Str[RK_STORE_I];
        }
        break;
        case RK_ADDINT: {
            return RK_OP_Str[RK_ADDINT_I];
        }
        break;
        case RK_SUBINT: {
            return RK_OP_Str[RK_SUBINT_I];
        }
        break;
        case RK_DIVINT: {
            return RK_OP_Str[RK_DIVINT_I];
        }
        break;
        case RK_MULINT: {
            return RK_OP_Str[RK_MULINT_I];
        }
        break;
        case RK_DIVMOD: {
            return RK_OP_Str[RK_DIVMOD_I];
        }
        break;
        case RK_JMP: {
            return RK_OP_Str[RK_JMP_I];
        }
        break;
        case RK_JMPNEG: {
            return RK_OP_Str[RK_JMPNEG_I];
        }
        break;
        case RK_JMPZERO: {
            return RK_OP_Str[RK_JMPZERO_I];
        }
        break;
        case RK_HALT: {
            return RK_OP_Str[RK_HALT_I];
        }
        break;
        default: {
            assert(0 && "\n[ERROR] Unreachable!");
        }
        break;
    }
}

void rk_dump_colored_2file(Roko* rk, FILE* fp, int colored) {
    char* color_esc;
    color_esc = (colored == 1 ? "\033[1;31m" : "");
    if (!rk) {
        fprintf(stderr,"%s\n[ERROR] at %s(): rk was NULL.\n", color_esc, __func__);
        return;
    }
    if (!fp) {
        fprintf(stderr,"%s\n[ERROR] at %s(): fp was NULL.\n", color_esc, __func__);
        return;
    }
    color_esc = (colored == 1 ? "\033[1;33m" : "");
    fprintf(fp,"%s",color_esc);
    fprintf(fp,"Registers:\n");
    fprintf(fp,"reg\t{%019" PRId64 "}\n", rk->reg.data.as_i64);
    fprintf(fp,"ic\t{%i}\n",rk->ic);
    fprintf(fp,"ir\t{%019" PRId64 "}\n", rk->ir.data.as_i64);
    fprintf(fp,"curr_op\t{%s} -> {%i}\n", rk_op_Str(rk->curr_op), rk->curr_op);
    fprintf(fp,"operand\t{%019" PRId64 "}\n", rk->operand.data.as_i64);
    fprintf(fp,"\nMemory (Only non-zero locations are printed):\n");
    fprintf(fp,"\tType:\tAs i64\t\t\tAs hex\t\t\tAs f64\n");
    for (int i=0; i < RK_MEM_SIZE; i++) {
        //if (i<10) printf("{%i}:\t{%+020li}\n", i, memory[i]);
        //if (i<11) {
        if (rk->memory[i].data.as_i64 != 0) {
            color_esc = (colored == 1 ? "\033[1;34m" : "");
            //fprintf(fp,"%s", (rk->memory[i] == 0 ? "\033[1;34m" : "\033[1;35m"));
            fprintf(fp,"%s{%i}:\t{%i}\t{%019" PRId64 "}", color_esc, i, rk->memory[i].type, rk->memory[i].data.as_i64);
            color_esc = (colored == 1 ? "\033[1;35m" : "");
            fprintf(fp,"%s\t{%016" PRIx64 "}", color_esc, rk->memory[i].data.as_i64);
            color_esc = (colored == 1 ? "\033[1;36m" : "");
            fprintf(fp,"%s\t{%026.6f}", color_esc, (double)rk->memory[i].data.as_i64);
            if (colored) {
                fprintf(fp,"%s\n", "\033[1;39m");
            } else {
                fprintf(fp,"\n");
            }
        }
    }
}

void rk_dump(Roko* rk) {
    rk_dump_colored_2file(rk, rk->coredump_fd, 1);
}

int load_sum_prog(Roko* rk) {
    if (!rk) {
        return -1;
    }
    rk->memory[0].data.as_i64 = 1000000000000000007; // Read A
    rk->memory[1].data.as_i64 = 1000000000000000008; // Read B
    rk->memory[2].data.as_i64 = 2000000000000000007; // Load A
    rk->memory[3].data.as_i64 = 3000000000000000008; // Add B
    rk->memory[4].data.as_i64 = 2100000000000000009; // Store C
    rk->memory[5].data.as_i64 = 1100000000000000009; // Write C
    rk->memory[6].data.as_i64 = 4300000000000000000; // Halt
    rk->memory[7].data.as_i64 = 0; // Var A
    rk->memory[8].data.as_i64 = 0; // Var B
    rk->memory[9].data.as_i64 = 0; // Res C

    return 1;
}

int load_mul_prog(Roko* rk) {
    if (!rk) {
        return -1;
    }
    int i = 0;
    rk->memory[i++].data.as_i64 = 1000000000000000007; // Read A
    rk->memory[i++].data.as_i64 = 1000000000000000008; // Read B
    rk->memory[i++].data.as_i64 = 2000000000000000007; // Load A
    rk->memory[i++].data.as_i64 = 3300000000000000008; // Mul B
    rk->memory[i++].data.as_i64 = 2100000000000000009; // Store C
    rk->memory[i++].data.as_i64 = 1100000000000000009; // Write C
    rk->memory[i++].data.as_i64 = 4300000000000000000; // Halt
    rk->memory[i++].data.as_i64 = 0; // Var A
    rk->memory[i++].data.as_i64 = 0; // Var B
    rk->memory[i++].data.as_i64 = 0; // Res C

    return 1;
}

int load_max_prog(Roko* rk) {
    if (!rk) {
        return -1;
    }
    int i = 0;
    rk->memory[i++].data.as_i64 = 1000000000000000009; // Read A
    rk->memory[i++].data.as_i64 = 1000000000000000010; // Read B
    rk->memory[i++].data.as_i64 = 2000000000000000009; // Load A
    rk->memory[i++].data.as_i64 = 3100000000000000010; // Sub B
    rk->memory[i++].data.as_i64 = 4100000000000000007; // JMPN to 07
    rk->memory[i++].data.as_i64 = 1100000000000000009; // Write A
    rk->memory[i++].data.as_i64 = 4300000000000000000; // Halt
    rk->memory[i++].data.as_i64 = 1100000000000000010; // Write B
    rk->memory[i++].data.as_i64 = 4300000000000000000; // Halt
    rk->memory[i++].data.as_i64 = 0; // Var A
    rk->memory[i++].data.as_i64 = 0; // Var B

    return 1;
}

int load_bad_prog(Roko* rk) { //Missing halt.
    if (!rk) {
        return -1;
    }
    int i = 0;
    rk->memory[i++].data.as_i64 = 1000000000000000002; // Read A
    rk->memory[i++].data.as_i64 = 1100000000000000002; // Write A
    rk->memory[i++].data.as_i64 = 0; // Var A

    return 1;
}

RK_OP op_from_Word(Word w) {
    RK_OP res = w.data.as_i64 / RK_WORD_FACTOR;
    if (res>=0) {
        return res;
    } else {
        fprintf(stderr,"\n\t[ERROR] at %s(): w/RK_WORD_FACTOR was negative.\n", __func__);
        return RK_INVALID_OP;
    }
}

Word operand_from_Word(Word w) {
    return (Word){ .type = RK_TYPE_INT64, .data.as_i64 = w.data.as_i64 % RK_WORD_FACTOR };
}

int rk_do_op(RK_OP op, Word operand, Roko* rk) {
    switch (op) {
        case RK_INVALID_OP: {
            fprintf(stderr,"\033[1;33m[WARN] at %s(): Doing RK_INVALID_OP.\033[1;39m\n",__func__);
            rk->ic++;
            return 0;
        }
        break;
        case RK_READ: {
            if (!rk->out_fd) {
                fprintf(stderr,"\n[ERROR] at %s(): rk->out_fd was NULL.\n",__func__);
                rk->ic++;
                return 1;
            }
            if (operand.data.as_i64 > RK_MEM_SIZE-1) {
                fprintf(stderr,"\n[ERROR] at %s(): operand > RK_MEM_SIZE-1.\n",__func__);
                rk->ic++;
                return 1;
            }
            fprintf(stdout, "?\n");
            int read_res = scanf("%" SCNd64 , &(rk->memory[operand.data.as_i64].data.as_i64));
            if (read_res != 1) {
                fprintf(stderr,"\n\t[ERROR] at %s(): Failed to read word from stdin.\n", __func__);
                rk->ic++;
                return 1;
            }
            //printf("Res [%i] for reading a word.\n", read_res);
            rk->ic++;
        }
        break;
        case RK_WRITE: {
            if (!rk->out_fd) {
                fprintf(stderr,"\n[ERROR] at %s(): rk->out_fd was NULL.\n",__func__);
                rk->ic++;
                return 1;
            }
            if (operand.data.as_i64 > RK_MEM_SIZE-1) {
                fprintf(stderr,"\n[ERROR] at %s(): operand > RK_MEM_SIZE-1.\n",__func__);
                rk->ic++;
                return 1;
            }
            fprintf(stdout, "%" PRId64 "\n", rk->memory[operand.data.as_i64].data.as_i64);
            rk->ic++;
        }
        break;
        case RK_LOAD: {
            if (operand.data.as_i64 > RK_MEM_SIZE-1) {
                fprintf(stderr,"\n[ERROR] at %s(): operand > RK_MEM_SIZE-1.\n",__func__);
                rk->ic++;
                return 1;
            }
            //printf("Loading a word from mem{%" PRId64 "}.\n", operand);
            rk->reg.data.as_i64 = rk->memory[operand.data.as_i64].data.as_i64;
            rk->ic++;
        }
        break;
        case RK_STORE: {
            if (operand.data.as_i64 < 0) {
                fprintf(stderr,"\n[ERROR] at %s(): operand < 0.\n",__func__);
                rk->ic++;
                return 1;
            }
            if (operand.data.as_i64 > RK_MEM_SIZE-1) {
                fprintf(stderr,"\n[ERROR] at %s(): operand > RK_MEM_SIZE-1.\n",__func__);
                rk->ic++;
                return 1;
            }
            //printf("Storing reg [%" PRId64 "] to mem{%" PRId64 "}.\n", reg, operand);
            rk->memory[operand.data.as_i64].data.as_i64 = rk->reg.data.as_i64;
            rk->ic++;
        }
        break;
        case RK_ADDINT: {
            int reg_sign = rk->reg.data.as_i64 < 0;
            int operand_sign = operand.data.as_i64 < 0;
            //printf("Adding mem{%" PRId64 "} to reg.\n", operand);
            rk->reg.data.as_i64 += rk->memory[operand.data.as_i64].data.as_i64;
            int post_reg_sign = rk->reg.data.as_i64 < 0;

            if ((reg_sign == operand_sign) && reg_sign != post_reg_sign) {
                fprintf(stderr,"\n\033[1;31m\t[ERROR] at %s(): addition integer overflow\033[1,39m\n", __func__);
                rk_dump(rk);
                rk->ic++;
                return 1;
            }
            rk->ic++;
        }
        break;
        case RK_SUBINT: {
            //printf("Subtracting mem{%" PRId64 "} from reg.\n", operand);
            rk->reg.data.as_i64 -= rk->memory[operand.data.as_i64].data.as_i64;
            rk->ic++;
        }
        break;
        case RK_DIVINT: {
            if (operand.data.as_i64 < 0) {
                fprintf(stderr,"\n\t[ERROR] at %s(): Invalid memory access, operand was negative.\n", __func__);
                rk->ic++;
                return 1;
            }
            if (rk->memory[operand.data.as_i64].data.as_i64 == 0) {
                fprintf(stderr,"\n\t[ERROR] at %s(): Divide-by-zero.\nExecution ended abnormally.\n", __func__);
                rk->ic++;
                return 1;
            }
            if (rk->memory[operand.data.as_i64].data.as_i64 > rk->reg.data.as_i64) {
                rk->reg.data.as_i64 = 0;
                rk->ic++;
                return 0;
            }
            //printf("Dividing reg by mem{%" PRId64 "}.\n", operand);
            //TODO Handle remainder?
            Word res = (Word){ .type = RK_TYPE_INT64, .data.as_i64 = (int64_t) ((rk->reg.data.as_i64) / (rk->memory[operand.data.as_i64].data.as_i64)) };
            rk->reg.data.as_i64 = res.data.as_i64;
            rk->ic++;
        }
        break;
        case RK_MULINT: {
            //printf("Multiplying reg by mem{%" PRId64 "}.\n", operand);
            Word res = (Word){ .type = RK_TYPE_INT64, .data.as_i64 = (rk->reg.data.as_i64) * (rk->memory[operand.data.as_i64].data.as_i64) };
            if (rk->reg.data.as_i64 != 0 && (res.data.as_i64 / rk->reg.data.as_i64 != rk->memory[operand.data.as_i64].data.as_i64)) {
                fprintf(stderr,"\n\033[1;31m\t[ERROR] at %s(): multiplication integer overflow\033[1;39m\n", __func__);
                rk_dump(rk);
                rk->ic++;
                return 1;
            }
            rk->reg.data.as_i64 = res.data.as_i64;
            rk->ic++;
        }
        break;
        case RK_DIVMOD: {
            //printf("Doing modulo on reg by mem{%" PRId64 "}.\n", operand);
            rk->reg.data.as_i64 %= rk->memory[operand.data.as_i64].data.as_i64;
            rk->ic++;
        }
        case RK_HALT: {
            //printf("\n\nEnding Roko execution.\n");
            rk_dump(rk);
            return 0;
        }
        break;
        case RK_JMP: {
            //printf("\n\nSetting IC to {%" PRId64 "}.\n", operand);
            rk->ic = operand.data.as_i64 -1;
        }
        break;
        case RK_JMPNEG: {
            if (rk->reg.data.as_i64 < 0) {
                //printf("\n\nRegister was negative, setting IC to {%" PRId64 "}.\n", operand);
                rk->ic = operand.data.as_i64;
            } else {
                rk->ic++;
            }
        }
        break;
        case RK_JMPZERO: {
            if (rk->reg.data.as_i64 == 0) {
                //printf("\n\nRegister was 0, setting IC to {%" PRId64 "}.\n", operand);
                rk->ic = operand.data.as_i64 -1;
            } else {
                rk->ic++;
            }
        }
        break;
        default: {
            fprintf(stderr,"\n\t[ERROR] at %s(): Unhandled RK_OP {%i}.\n", __func__, op);
            rk_dump(rk);
            if (!rk->out_fd) {
                fprintf(stderr,"\n[ERROR] at %s(): rk->out_fd was NULL.\n",__func__);
                return 1;
            }
            fprintf(rk->out_fd, "\n\n\033[1;31m[ERROR] Ending Roko execution.\033[1;39m\n");
            return 1;
        }
        break;
    }
    return 0;
}

int rk_execute(Roko* rk) {
    if (!rk) {
        fprintf(stderr,"[ERROR] at %s(): rk was NULL.\n",__func__);
        return 1;
    }
    int op_res = -1;

    do  {
        rk->ir = rk->memory[rk->ic];
        rk->curr_op = op_from_Word(rk->ir);
        Word operand = operand_from_Word(rk->ir);
        op_res = rk_do_op(rk->curr_op, operand, rk);

        if (rk->verbose_level > 0) {
            if (!rk->out_fd) {
                fprintf(stderr,"[ERROR] at %s(): rk->out_fd was NULL.\n",__func__);
                return 1;
            }
            int is_colored = 0;
            char* color_esc = (is_colored ? (op_res == 0 ? "\033[1;32m" : "\033[1;31m" ) : "");
            //Print string for last operation done
            fprintf(rk->out_fd, "%s%s  ", color_esc, rk_op_Str(rk->curr_op));
            //Print current instruction counter
            //fprintf(rk->out_fd, "\tIC: {%i}", rk->ic);
            //Print operand for last operation done
            fprintf(rk->out_fd, "%" PRId64, operand.data.as_i64);
            if (is_colored) {
                fprintf(rk->out_fd, "\033[1;39m\n");
            } else {
                fprintf(rk->out_fd, "\n");
            }
        }

        if (op_res != 0) {
            fprintf(stderr,"\n\033[1;31m\t[ERROR] at %s():\n\tIC {%i}\n\top_res = {%i} for op {%s} [%i].\033[1;39m\n", __func__, rk->ic, op_res, rk_op_Str(rk->curr_op), rk->curr_op);
            rk_dump(rk);
            return op_res;
        }
    } while (rk->curr_op != RK_HALT && rk->ic < RK_MEM_SIZE);

    if (rk->curr_op != RK_HALT) {
        fprintf(stderr,"\n\033[1;31m[ERROR] at %s(): execution reached end of memory without halting.\033[1;39m\n", __func__);
        return 1;
    }
    return op_res;
}

int rk_load_word_from_file(Roko* rk, FILE* fp, int64_t pos) {
    int res = -1;
    if (!rk) {
        fprintf(stderr,"\n[ERROR] at %s(): rk was NULL.\n",__func__);
        return res;
    }
    if (!fp) {
        fprintf(stderr,"\n[ERROR] at %s(): fp was NULL.\n",__func__);
        return res;
    }
    if (pos < 0 || pos > RK_MEM_SIZE || pos > INT64_MAX) {
        fprintf(stderr,"\n[ERROR] at %s(): invalid position [%" PRId64 "].\n", __func__, pos);
        return res;
    }

    res = fscanf(fp,"%" SCNd64 , &(rk->memory[pos].data.as_i64));
    if (res != 1) {
        fprintf(stderr,"\n\t[ERROR] at %s(): Failed to read word from fp.\n", __func__);
        return res;
    }
    return res;
}

int load_program_from_file(FILE* fp, Roko* rk) {
    int res = -1;
    if (!rk) {
        fprintf(stderr,"\n[ERROR] at %s(): rk was NULL.\n",__func__);
        return res;
    }
    if (!fp) {
        fprintf(stderr,"\n[ERROR] at %s(): fp was NULL.\n",__func__);
        return res;
    }
    for (int i = 0; i < RK_MEM_SIZE; i++) {
        res = rk_load_word_from_file(rk, fp, i);
        if (res != 1) {
            fprintf(stderr,"\n\t[ERROR] at %s(): Failed loading word at pos {%i} Res [%i].\n", __func__, i, res);
            return res;
        }
        if (rk->memory[i].data.as_i64 == RK_EOF) {
            fprintf(stdout, "Done loading prog from file.\n");
            break;
        }
    }

    return res;
}
