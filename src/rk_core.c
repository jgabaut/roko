#include "rk_core.h"

const char* string_roko_version(void) {
	return ROKO_API_VERSION_STRING;
}

const int int_roko_version(void) {
	return ROKO_API_VERSION_INT;
}

void usage(const char* prog) {
    fprintf(stderr,"\nUsage: %s [rkasm file]\n", prog);
}

const char* RK_OP_Str[RK_TOT_OP] = {
    "RK_PANIC",
    "RK_IMM_I64",
    "RK_INVALID_OP",
    "RK_IMM_CHAR",
    "RK_PRINTNL",
    "RK_IMM_STRING",
    "RK_READ_I64",
    "RK_WRITE_I64",
    "RK_READ_CHAR",
    "RK_WRITE_CHAR",
    "RK_WRITE_STRING",
    "RK_LOAD_I64",
    "RK_STORE_I64",
    "RK_LOAD_CHAR",
    "RK_STORE_CHAR",
    "RK_ADD_I64",
    "RK_SUB_I64",
    "RK_DIV_I64",
    "RK_MUL_I64",
    "RK_DIVMOD_I64",
    "RK_POW_I64",
    "RK_JMP_I64",
    "RK_JMPNEG_I64",
    "RK_JMPZERO_I64",
    "RK_HALT",
};

const char* RK_Word_Type_Str[RK_TOT_TYPES] = {
    "RK_NULL",
    "I64",
    "F64",
    "char",
    "string",
};

void rk_init(Roko* rk, int verbose_level) {
    if (!rk) {
        return;
    }
    for (int i = 0; i < RK_MEM_SIZE; i++) {
        rk->memory[i].type = RK_TYPE_INT64;
        rk->memory[i].data.as_i64 = RK_INVALID_OP;
    }
    rk->reg.type = RK_TYPE_INT64;
    rk->reg.data.as_i64 = RK_INVALID_OP;
    rk->ic = 0;
    rk->curr_op = RK_INVALID_OP;
    rk->operand.type = RK_TYPE_INT64;
    rk->operand.data.as_i64 = RK_INVALID_OP;
    rk->ir.type = RK_TYPE_INT64;
    rk->ir.data.as_i64 = RK_INVALID_OP;
    rk->coredump_fd = stderr;
    const char* outfile_path = "./roko_out.txt";
    FILE* outfile = fopen(outfile_path, "w");
    if (!outfile) {
        fprintf(stderr,"\n[ERROR] at %s(): failed opening outfile {%s}.\n", __func__, outfile_path);
    }
    rk->out_fd = outfile;
    rk->verbose_level = verbose_level;
}

const char* rk_op_Str(RK_OP op) {
    switch (op) {
        case RK_PANIC: {
            return RK_OP_Str[RK_PANIC_I];
        }
        break;
        case RK_INVALID_OP: {
            return RK_OP_Str[RK_INVALID_OP_I];
        }
        break;
        case RK_IMM_I64: {
            return RK_OP_Str[RK_IMM_I64_I];
        }
        break;
        case RK_IMM_CHAR: {
            return RK_OP_Str[RK_IMM_CHAR_I];
        }
        break;
        case RK_PRINTNL: {
            return RK_OP_Str[RK_PRINTNL_I];
        }
        break;
        case RK_IMM_STRING: {
            return RK_OP_Str[RK_IMM_STRING_I];
        }
        break;
        case RK_READ_I64: {
            return RK_OP_Str[RK_READ_I64_I];
        }
        break;
        case RK_WRITE_I64: {
            return RK_OP_Str[RK_WRITE_I64_I];
        }
        break;
        case RK_READ_CHAR: {
            return RK_OP_Str[RK_READ_CHAR_I];
        }
        break;
        case RK_WRITE_CHAR: {
            return RK_OP_Str[RK_WRITE_CHAR_I];
        }
        break;
        case RK_WRITE_STRING: {
            return RK_OP_Str[RK_WRITE_STRING_I];
        }
        break;
        case RK_LOAD_I64: {
            return RK_OP_Str[RK_LOAD_I64_I];
        }
        break;
        case RK_STORE_I64: {
            return RK_OP_Str[RK_STORE_I64_I];
        }
        break;
        case RK_LOAD_CHAR: {
            return RK_OP_Str[RK_LOAD_CHAR_I];
        }
        break;
        case RK_STORE_CHAR: {
            return RK_OP_Str[RK_STORE_CHAR_I];
        }
        break;
        case RK_ADD_I64: {
            return RK_OP_Str[RK_ADD_I64_I];
        }
        break;
        case RK_SUB_I64: {
            return RK_OP_Str[RK_SUB_I64_I];
        }
        break;
        case RK_DIV_I64: {
            return RK_OP_Str[RK_DIV_I64_I];
        }
        break;
        case RK_MUL_I64: {
            return RK_OP_Str[RK_MUL_I64_I];
        }
        break;
        case RK_DIVMOD_I64: {
            return RK_OP_Str[RK_DIVMOD_I64_I];
        }
        break;
        case RK_POW_I64: {
            return RK_OP_Str[RK_POW_I64_I];
        }
        break;
        case RK_JMP_I64: {
            return RK_OP_Str[RK_JMP_I64_I];
        }
        break;
        case RK_JMPNEG_I64: {
            return RK_OP_Str[RK_JMPNEG_I64_I];
        }
        break;
        case RK_JMPZERO_I64: {
            return RK_OP_Str[RK_JMPZERO_I64_I];
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
    return "Unreachable";
}

const char* rk_type_Str(Word_Type t) {
    switch (t) {
        case RK_TYPE_NULL:
        case RK_TYPE_INT64:
        case RK_TYPE_FLOAT64:
        case RK_TYPE_CHAR:
        case RK_TYPE_STRING: {
            return RK_Word_Type_Str[t];
        }
        break;
        default: {
            assert(0 && "\n[ERROR] Unreachable!");
        }
        break;
    }

    return "Unreachable";
}

void rk_print_Word_2file(FILE* fp, Word w, int colored, Roko* rk) {
    char* color_esc;
    color_esc = (colored == 1 ? "\033[1;31m" : "");
    if (!fp) {
        fprintf(stderr,"%s\n[ERROR] at %s(): fp was NULL.\n", color_esc, __func__);
        return;
    }
    if (!rk) {
        fprintf(stderr,"%s\n[ERROR] at %s(): rk was NULL.\n", color_esc, __func__);
        return;
    }
    if (w.type == RK_TYPE_INT64 && w.data.as_i64 != RK_INVALID_OP) {
        color_esc = (colored == 1 ? "\033[1;36m" : "");
        fprintf(fp,"%s\t{%019" PRId64 "}", color_esc, w.data.as_i64);

        color_esc = (colored == 1 ? "\033[1;35m" : "");
        //TODO handle f64 words in memory
        fprintf(fp,"%s\t<%-13.13s>", color_esc, rk_op_Str(rk_op_from_Word(w)));

        color_esc = (colored == 1 ? "\033[1;37m" : "");
        fprintf(fp,"%s\t\t<%-16.16" PRId64 ">", color_esc, rk_operand_from_Word_i64(w).data.as_i64);

        color_esc = (colored == 1 ? "\033[1;34m" : "");
        fprintf(fp,"%s\t{0x%016" PRIx64 "}", color_esc, w.data.as_i64);
    } else if (w.type == RK_TYPE_FLOAT64) {
        color_esc = (colored == 1 ? "\033[1;35m" : "");
        fprintf(fp,"%s\t\t\t\t{%026.6f}", color_esc, (double) w.data.as_i64);
    } else if (w.type == RK_TYPE_CHAR) {
        color_esc = (colored == 1 ? "\033[1;36m" : "");
        fprintf(fp,"%s\t{%019" PRId64 "}", color_esc, w.data.as_i64);

        color_esc = (colored == 1 ? "\033[1;35m" : "");
        fprintf(fp,"%s\t<%-13.13s>", color_esc, rk_op_Str(rk_op_from_Word(w)));
        int64_t value = rk_operand_from_Word_i64(w).data.as_i64;
        if (value >=32 && value <= 255) {
            color_esc = (colored == 1 ? "\033[1;35m" : "");
            fprintf(fp,"%s\t\t{%c}", color_esc, (char) value);
        } else if (value == 0) {
            color_esc = (colored == 1 ? "\033[1;36m" : "");
            fprintf(fp,"%s\t\t{\\0}", color_esc);
        } else {
            color_esc = (colored == 1 ? "\033[1;37m" : "");
            fprintf(fp,"%s\t\t{\\¿}", color_esc);
        }
        color_esc = (colored == 1 ? "\033[1;33m" : "");
        fprintf(fp,"%s\t\t\t[0x%016" PRIx64 "]", color_esc, rk_operand_from_Word_i64(w).data.as_i64);
    } else if (w.type == RK_TYPE_STRING) {
        color_esc = (colored == 1 ? "\033[1;36m" : "");
        fprintf(fp,"%s\t{%019" PRId64 "}", color_esc, w.data.as_i64);
        color_esc = (colored == 1 ? "\033[1;35m" : "");
        fprintf(fp,"%s\t<%-13.13s>", color_esc, rk_op_Str(rk_op_from_Word(w)));
        char str_buf[RK_MEM_SIZE-5] = {0};
        int64_t str_offset = rk_operand_from_Word_i64(w).data.as_i64;
        for (int str_i = 0; str_i < RK_MEM_SIZE-5; str_i++) {
            str_buf[str_i] = rk->memory[str_offset].data.as_i64;
            if (str_buf[str_i] == 0 ) {
                break;
            }
            str_offset++;
        }
        //color_esc = (colored == 1 ? "\033[1;35m" : "");
        //fprintf(fp,"%s\t<%-13.13s>", color_esc, rk_op_Str(rk_op_from_Word(rk->memory[i])));
        color_esc = (colored == 1 ? "\033[1;33m" : "");
        fprintf(fp,"%s\t\t{%s}", color_esc, str_buf);
        color_esc = (colored == 1 ? "\033[1;36m" : "");
        fprintf(fp,"%s\t\t[0x%016" PRIx64 "]", color_esc, rk_operand_from_Word_i64(w).data.as_i64);
        //color_esc = (colored == 1 ? "\033[1;36m" : "");
        //fprintf(fp,"%s\t\t\t[%s]", color_esc, &(rk->memory[rk_operand_from_Word_String(rk->memory[i]).data.as_i64]));
    }
    if (w.data.as_i64 != RK_INVALID_OP && colored) {
        fprintf(fp,"%s\n", "\033[1;39m");
    } else if (w.data.as_i64 != RK_INVALID_OP){
        fprintf(fp,"\n");
    } else {
        #ifndef _WIN32
        fprintf(fp,"%s", "\033[1;39m");
        #endif
    }
}

void rk_print_Word(Word w, int colored, Roko* rk) {
    rk_print_Word_2file(rk->out_fd, w, colored, rk);
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
    fprintf(fp,"reg\t{%019" PRId64 "} {%s}\n", rk->reg.data.as_i64, rk_type_Str(rk->reg.type));
    fprintf(fp,"ic\t{%i}\n",rk->ic);
    fprintf(fp,"ir\t{%019" PRId64 "} {%s}\n", rk->ir.data.as_i64, rk_type_Str(rk->ir.type));
    fprintf(fp,"curr_op\t{%s} -> {%i}\n", rk_op_Str(rk->curr_op), rk->curr_op);
    fprintf(fp,"operand\t{%019" PRId64 "} {%s}\n", rk->operand.data.as_i64, rk_type_Str(rk->operand.type));
    fprintf(fp,"\nMemory (Only valid op locations are printed):\n");
    fprintf(fp,"\t\tType:\t\tAs i64\t\t\tAs RK_OP\t\tAs operand\t\tAs hex\n");
    for (int i=0; i < RK_MEM_SIZE; i++) {
        //if (i<10) printf("{%i}:\t{%+020li}\n", i, memory[i]);
        //if (i<11) {
        if (rk->memory[i].data.as_i64 != RK_INVALID_OP) {
            color_esc = (colored == 1 ? "\033[1;33m" : "");
            fprintf(fp,"%s[%i]:->", color_esc, i);

            color_esc = (colored == 1 ? "\033[1;32m" : "");
            fprintf(fp,"%s\t\t{%-8.8s}", color_esc, rk_type_Str(rk->memory[i].type));
        }

        rk_print_Word_2file(fp,rk->memory[i],colored,rk);
    }
}

void rk_dump(Roko* rk) {
    #ifndef _WIN32
    rk_dump_colored_2file(rk, rk->coredump_fd, 1);
    #else
    rk_dump_colored_2file(rk, rk->coredump_fd, 0);
    #endif
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

RK_OP rk_op_from_Word(Word w) {
    if (w.data.as_i64 < 0) {
        switch (w.data.as_i64) {
            case RK_INVALID_OP: {
                return RK_INVALID_OP;
            }
            break;
            case RK_PANIC: {
                return RK_PANIC;
            }
            break;
            default: {
                return RK_PANIC;
            }
            break;
        }
        return RK_PANIC;
    }

    RK_OP res = w.data.as_i64 / RK_WORD_FACTOR;
    return res;
}

Word rk_operand_from_Word_i64(Word w) {
    return (Word){ .type = RK_TYPE_INT64, .data.as_i64 = w.data.as_i64 % RK_WORD_FACTOR };
}

Word rk_operand_from_Word_String(Word w) {
    return (Word){ .type = RK_TYPE_STRING, .data.as_i64 = w.data.as_i64 % RK_WORD_FACTOR };
}

Word rk_operand_from_Word_f64(Word w) {
    return (Word){ .type = RK_TYPE_FLOAT64, .data.as_f64 = ((int64_t) w.data.as_f64) % RK_WORD_FACTOR };
}

int rk_do_op(RK_OP op, Word operand, Roko* rk) {
    switch (op) {
        case RK_INVALID_OP: {
            #ifndef _WIN32
            fprintf(stderr,"\033[1;33m[WARN] at %s(): Doing RK_INVALID_OP.\033[1;39m\n",__func__);
            #else
            fprintf(stderr,"[WARN] at %s(): Doing RK_INVALID_OP.\n",__func__);
            #endif
            rk->ic++;
            return 1;
        }
        break;
        case RK_PANIC: {
            #ifndef _WIN32
            fprintf(stderr,"\033[1;31m[PANIC] at %s(): Roko panic at ic [%i].\033[1;39m\n",__func__,rk->ic);
            #else
            fprintf(stderr,"[PANIC] at %s(): Roko panic at ic [%i].\n",__func__,rk->ic);
            #endif
            return 1;
        }
        break;
        case RK_IMM_I64: {
            if (operand.type != RK_TYPE_INT64) {
                fprintf(stderr,"\n[ERROR] at %s(): operand type error, expected [%i] was [%i].\n",__func__, RK_TYPE_INT64, operand.type);
                return 1;
            }
            rk->reg.data.as_i64 = operand.data.as_i64;
            rk->ic++;
            return 0;
        }
        break;
        case RK_IMM_CHAR: {
            if (operand.type != RK_TYPE_CHAR) {
                fprintf(stderr,"\n[ERROR] at %s(): operand type error, expected [%i] was [%i].\n",__func__, RK_TYPE_CHAR, operand.type);
                return 1;
            }
            rk->reg.data.as_i64 = operand.data.as_i64;
            rk->reg.type = RK_TYPE_CHAR;
            rk->ic++;
            return 0;
        }
        break;
        case RK_PRINTNL: {
            fprintf(stdout,"\n");
            rk->ic++;
            return 0;
        }
        break;
        case RK_IMM_STRING: {
            if (operand.type != RK_TYPE_STRING) {
                fprintf(stderr,"\n[ERROR] at %s(): operand type error, expected [%i] was [%i].\n",__func__, RK_TYPE_STRING, operand.type);
                return 1;
            }
            rk->reg.data.as_i64 = operand.data.as_i64;
            rk->reg.type = RK_TYPE_STRING;
            rk->ic++;
            return 0;
        }
        break;
        case RK_READ_I64: {
            if (rk->reg.type != RK_TYPE_INT64) {
                fprintf(stderr,"\n[ERROR] at %s(): reg type error, expected [%i] was [%i].\n",__func__, RK_TYPE_INT64, rk->reg.type);
                return 1;
            }
            if (rk->memory[operand.data.as_i64].type != RK_TYPE_INT64) {
                fprintf(stderr,"\n[ERROR] at %s(): memory access type error, expected [%i] was [%i].\n",__func__, RK_TYPE_INT64, rk->memory[operand.data.as_i64].type);
                return 1;
            }
            if (!rk->out_fd) {
                fprintf(stderr,"\n[ERROR] at %s(): rk->out_fd was NULL.\n",__func__);
                return 1;
            }
            if (operand.data.as_i64 > RK_MEM_SIZE-1) {
                fprintf(stderr,"\n[ERROR] at %s(): operand > RK_MEM_SIZE-1.\n",__func__);
                rk->ic++;
                return 1;
            }
            if (operand.data.as_i64 < 0) {
                fprintf(stderr,"\n\t[ERROR] at %s(): Invalid memory access, operand was negative.\n", __func__);
                return 1;
            }
            fprintf(stdout, "?\n");
            int read_res = scanf("%" SCNd64 , &(rk->memory[operand.data.as_i64].data.as_i64));
            rk->memory[operand.data.as_i64].type = RK_TYPE_INT64;
            if (read_res != 1) {
                fprintf(stderr,"\n\t[ERROR] at %s(): Failed to read word from stdin.\n", __func__);
                return 1;
            }
            //printf("Res [%i] for reading a word.\n", read_res);
            rk->ic++;
        }
        break;
        case RK_WRITE_I64: {
            if (rk->reg.type != RK_TYPE_INT64) {
                fprintf(stderr,"\n[ERROR] at %s(): reg type error, expected [%i] was [%i].\n",__func__, RK_TYPE_INT64, rk->reg.type);
                return 1;
            }
            if (rk->memory[operand.data.as_i64].type != RK_TYPE_INT64) {
                fprintf(stderr,"\n[ERROR] at %s(): memory access type error, expected [%i] was [%i].\n",__func__, RK_TYPE_INT64, rk->memory[operand.data.as_i64].type);
                return 1;
            }
            if (!rk->out_fd) {
                fprintf(stderr,"\n[ERROR] at %s(): rk->out_fd was NULL.\n",__func__);
                return 1;
            }
            if (operand.data.as_i64 > RK_MEM_SIZE-1) {
                fprintf(stderr,"\n[ERROR] at %s(): operand > RK_MEM_SIZE-1.\n",__func__);
                return 1;
            }
            if (operand.data.as_i64 < 0) {
                fprintf(stderr,"\n\t[ERROR] at %s(): Invalid memory access, operand was negative.\n", __func__);
                return 1;
            }
            fprintf(stdout, "%" PRId64, rk->memory[operand.data.as_i64].data.as_i64);
            rk->ic++;
        }
        break;
        case RK_READ_CHAR: {
            if (operand.type != RK_TYPE_INT64) {
                fprintf(stderr,"\n[ERROR] at %s(): reg type error, expected [%i] was [%i].\n",__func__, RK_TYPE_INT64, operand.type);
                return 1;
            }
            if (rk->memory[operand.data.as_i64].type != RK_TYPE_CHAR) {
                fprintf(stderr,"\n[ERROR] at %s(): memory access type error, expected [%i] was [%i].\n",__func__, RK_TYPE_CHAR, rk->memory[operand.data.as_i64].type);
                return 1;
            }
            if (!rk->out_fd) {
                fprintf(stderr,"\n[ERROR] at %s(): rk->out_fd was NULL.\n",__func__);
                return 1;
            }
            if (operand.data.as_i64 > RK_MEM_SIZE-1) {
                fprintf(stderr,"\n[ERROR] at %s(): operand > RK_MEM_SIZE-1.\n",__func__);
                rk->ic++;
                return 1;
            }
            if (operand.data.as_i64 < 0) {
                fprintf(stderr,"\n\t[ERROR] at %s(): Invalid memory access, operand was negative.\n", __func__);
                return 1;
            }
            fprintf(stdout, "?(char)\n");
            char read_ch = '\0';
            int read_res = scanf("%c", &read_ch);
            if (read_res != 1) {
                fprintf(stderr,"\n\t[ERROR] at %s(): Failed to read word from stdin.\n", __func__);
                return 1;
            }
            if (read_ch >= 32 && read_ch <= 255 ) {
                rk->memory[operand.data.as_i64].data.as_i64 = read_ch + (RK_WORD_FACTOR*RK_IMM_CHAR);
            }
            //printf("Res [%i] for reading a char.\n", read_res);
            rk->ic++;
        }
        break;
        case RK_WRITE_CHAR: {
            if (operand.type != RK_TYPE_INT64) {
                fprintf(stderr,"\n[ERROR] at %s(): operand type error, expected [%i] was [%i].\n",__func__, RK_TYPE_INT64, operand.type);
                return 1;
            }
            if (operand.data.as_i64 > RK_MEM_SIZE-1) {
                fprintf(stderr,"\n[ERROR] at %s(): operand > RK_MEM_SIZE-1.\n",__func__);
                return 1;
            }
            if (operand.data.as_i64 < 0) {
                fprintf(stderr,"\n\t[ERROR] at %s(): Invalid memory access, operand was negative.\n", __func__);
                return 1;
            }
            if (rk->memory[operand.data.as_i64].type != RK_TYPE_CHAR) {
                fprintf(stderr,"\n[WARN] at %s(): memory access type error, expected [%i] was [%i].",__func__, RK_TYPE_CHAR, rk->memory[operand.data.as_i64].type);
                fprintf(stderr,"\n[WARN] at %s(): silent cast of mem type [%i] to [%i].\n",__func__, rk->memory[operand.data.as_i64].type, RK_TYPE_CHAR);
                rk->memory[operand.data.as_i64].type = RK_TYPE_CHAR;
            }
            if (!rk->out_fd) {
                fprintf(stderr,"\n[ERROR] at %s(): rk->out_fd was NULL.\n",__func__);
                return 1;
            }
            int64_t value = rk_operand_from_Word_i64(rk->memory[operand.data.as_i64]).data.as_i64;
            if (value >= 32 && value <= 255) {
                fprintf(stdout, "%c", (char) value);
            } else if (value == 0) {
                //fprintf(stdout, "\\0");
                fprintf(stderr,"\t[WARN] at %s(): Tried printing \"\\0\".\n", __func__);
            } else {
                fprintf(stderr,"\t[WARN] at %s(): Tried printing \"\\{%" PRId64 "}\" as char.\n", __func__, value);
                fprintf(stdout, "\\¿");
            }
            rk->ic++;
        }
        break;
        case RK_WRITE_STRING: {
            if (operand.type != RK_TYPE_INT64) {
                fprintf(stderr,"\n[ERROR] at %s(): operand type error, expected [%i] was [%i].\n",__func__, RK_TYPE_INT64, operand.type);
                return 1;
            }
            if (operand.data.as_i64 > RK_MEM_SIZE-1) {
                fprintf(stderr,"\n[ERROR] at %s(): operand > RK_MEM_SIZE-1.\n",__func__);
                return 1;
            }
            if (operand.data.as_i64 < 0) {
                fprintf(stderr,"\n\t[ERROR] at %s(): Invalid memory access, operand was negative.\n", __func__);
                return 1;
            }
            int64_t value = -1;
            int64_t string_p_addr = operand.data.as_i64;
            if (rk->memory[string_p_addr].type != RK_TYPE_STRING) {
                fprintf(stderr,"\n[ERROR] at %s(): memory access type error, expected [%i] was [%i].\n",__func__, RK_TYPE_STRING, rk->memory[string_p_addr].type);
                return 1;
            }

            int64_t offset = rk_operand_from_Word_String(rk->memory[string_p_addr]).data.as_i64;

            do {
                if (rk->memory[offset].type != RK_TYPE_CHAR) {
                    fprintf(stderr,"\n[ERROR] at %s(): memory access type error, expected [%i] was [%i].\n",__func__, RK_TYPE_CHAR, rk->memory[offset].type);
                    fprintf(stderr,"\n[HINT] op was {%s}, maybe a missing null terminator?\n",rk_op_Str(op));
                    return 1;
                }
                value = rk_operand_from_Word_i64(rk->memory[offset]).data.as_i64;
                if (value >= 32 && value <= 255) {
                    fprintf(stdout, "%c", (char) value);
                } else if (value == 0) {
                } else {
                    fprintf(stdout, "\\¿");
                }
                offset++;
            } while (value != '\0' && offset < RK_MEM_SIZE && rk->curr_op != RK_INVALID_OP);

            if (value == '\0' && offset < RK_MEM_SIZE && rk->curr_op != RK_INVALID_OP) {
                rk->ic++;
                return 0;
            } else if (!(offset < RK_MEM_SIZE)) {
                fprintf(stderr,"\n[ERROR] at %s(): offet >= RK_MEM_SIZE. Missing null terminator?\nValue was {%" PRId64 "\n",__func__, value);
                return 1;
            } else {
                fprintf(stderr,"\n[ERROR] at %s(): Value was {%" PRId64 "\n",__func__, value);
                return 1;
            }
        }
        break;
        case RK_LOAD_I64: {
            if (rk->reg.type != RK_TYPE_INT64) {
                fprintf(stderr,"\n[ERROR] at %s(): reg type error, expected [%i] was [%i].\n",__func__, RK_TYPE_INT64, rk->reg.type);
                return 1;
            }
            if (rk->memory[operand.data.as_i64].type != RK_TYPE_INT64) {
                fprintf(stderr,"\n[ERROR] at %s(): memory access type error, expected [%i] was [%i].\n",__func__, RK_TYPE_INT64, rk->memory[operand.data.as_i64].type);
                return 1;
            }
            if (operand.data.as_i64 > RK_MEM_SIZE-1) {
                fprintf(stderr,"\n[ERROR] at %s(): operand > RK_MEM_SIZE-1.\n",__func__);
                return 1;
            }
            if (operand.data.as_i64 < 0) {
                fprintf(stderr,"\n\t[ERROR] at %s(): Invalid memory access, operand was negative.\n", __func__);
                return 1;
            }
            //printf("Loading a word from mem{%" PRId64 "}.\n", operand);
            rk->reg.data.as_i64 = rk->memory[operand.data.as_i64].data.as_i64;
            rk->reg.type = RK_TYPE_INT64;
            rk->ic++;
        }
        break;
        case RK_STORE_I64: {
            if (rk->reg.type != RK_TYPE_INT64) {
                fprintf(stderr,"\n[ERROR] at %s(): reg type error, expected [%i] was [%i].\n",__func__, RK_TYPE_INT64, rk->reg.type);
                return 1;
            }
            if (rk->memory[operand.data.as_i64].type != RK_TYPE_INT64) {
                fprintf(stderr,"\n[ERROR] at %s(): memory access type error, expected [%i] was [%i].\n",__func__, RK_TYPE_INT64, rk->memory[operand.data.as_i64].type);
                return 1;
            }
            if (operand.data.as_i64 < 0) {
                fprintf(stderr,"\n[ERROR] at %s(): operand < 0.\n",__func__);
                return 1;
            }
            if (operand.data.as_i64 > RK_MEM_SIZE-1) {
                fprintf(stderr,"\n[ERROR] at %s(): operand > RK_MEM_SIZE-1.\n",__func__);
                return 1;
            }
            if (operand.data.as_i64 < 0) {
                fprintf(stderr,"\n\t[ERROR] at %s(): Invalid memory access, operand was negative.\n", __func__);
                return 1;
            }
            //printf("Storing reg [%" PRId64 "] to mem{%" PRId64 "}.\n", reg, operand);
            rk->memory[operand.data.as_i64].data.as_i64 = rk->reg.data.as_i64;
            rk->ic++;
        }
        break;
        case RK_LOAD_CHAR: {
            if (operand.type != RK_TYPE_INT64) {
                fprintf(stderr,"\n[ERROR] at %s(): operand type error, expected [%i] was [%i].\n",__func__, RK_TYPE_INT64, operand.type);
                return 1;
            }
            if (rk->reg.type != RK_TYPE_CHAR) {
                fprintf(stderr,"\n[WARN] at %s(): reg type error, expected [%i] was [%i].",__func__, RK_TYPE_CHAR, rk->reg.type);
                fprintf(stderr,"\n[WARN] at %s(): silent cast of reg type [%i] to [%i].\n",__func__, rk->reg.type, RK_TYPE_CHAR);
                rk->reg.type = RK_TYPE_CHAR;
            }
            if (operand.data.as_i64 > RK_MEM_SIZE-1) {
                fprintf(stderr,"\n[ERROR] at %s(): operand > RK_MEM_SIZE-1.\n",__func__);
                return 1;
            }
            if (operand.data.as_i64 < 0) {
                fprintf(stderr,"\n\t[ERROR] at %s(): Invalid memory access, operand was negative.\n", __func__);
                return 1;
            }
            if (rk->memory[operand.data.as_i64].type != RK_TYPE_CHAR) {
                fprintf(stderr,"\n[ERROR] at %s(): memory access type error, expected [%i] was [%i].\n",__func__, RK_TYPE_CHAR, rk->memory[operand.data.as_i64].type);
                return 1;
            }
            //printf("Loading a word from mem{%" PRId64 "}.\n", operand);
            rk->reg.data.as_i64 = rk->memory[operand.data.as_i64].data.as_i64;
            rk->reg.type = RK_TYPE_CHAR;
            rk->ic++;
        }
        break;
        case RK_STORE_CHAR: {
            if (operand.type != RK_TYPE_INT64) {
                fprintf(stderr,"\n[ERROR] at %s(): operand type error, expected [%i] was [%i].\n",__func__, RK_TYPE_INT64, operand.type);
                return 1;
            }
            if (rk->reg.type != RK_TYPE_CHAR) {
                fprintf(stderr,"\n[WARN] at %s(): reg type error, expected [%i] was [%i].",__func__, RK_TYPE_CHAR, rk->reg.type);
                fprintf(stderr,"\n[WARN] at %s(): silent cast of reg type [%i] to [%i].\n",__func__, rk->reg.type, RK_TYPE_CHAR);
                rk->reg.type = RK_TYPE_CHAR;
            }
            if (operand.data.as_i64 < 0) {
                fprintf(stderr,"\n\t[ERROR] at %s(): Invalid memory access, operand was negative.\n", __func__);
                return 1;
            }
            if (operand.data.as_i64 > RK_MEM_SIZE-1) {
                fprintf(stderr,"\n[ERROR] at %s(): operand > RK_MEM_SIZE-1.\n",__func__);
                return 1;
            }
            if (rk->memory[operand.data.as_i64].type != RK_TYPE_CHAR) {
                fprintf(stderr,"\n[ERROR] at %s(): memory access type error, expected [%i] was [%i].\n",__func__, RK_TYPE_CHAR, rk->memory[operand.data.as_i64].type);
                return 1;
                //rk->memory[operand.data.as_i64].type = RK_TYPE_CHAR;
            }
            int64_t value = rk_operand_from_Word_i64(rk->reg).data.as_i64;
            if (value >= 0 && value <= 255) {
            //printf("Storing reg [%" PRId64 "] to mem{%" PRId64 "}.\n", reg, operand);
                rk->memory[operand.data.as_i64].data.as_i64 = rk->reg.data.as_i64;
                rk->memory[operand.data.as_i64].type = rk->reg.type;
            } else {
                fprintf(stderr,"\n[ERROR] at %s(): value range error, expected [>=0, <=255] was [%" PRId64 "].\n",__func__, value);
                return 1;
            }
            rk->ic++;
        }
        break;
        case RK_ADD_I64: {
            if (rk->reg.type != RK_TYPE_INT64) {
                fprintf(stderr,"\n[ERROR] at %s(): reg type error, expected [%i] was [%i].\n",__func__, RK_TYPE_INT64, rk->reg.type);
                return 1;
            }
            if (rk->memory[operand.data.as_i64].type != RK_TYPE_INT64) {
                fprintf(stderr,"\n[ERROR] at %s(): memory access type error, expected [%i] was [%i].\n",__func__, RK_TYPE_INT64, rk->memory[operand.data.as_i64].type);
                return 1;
            }
            if (operand.data.as_i64 > RK_MEM_SIZE-1) {
                fprintf(stderr,"\n[ERROR] at %s(): operand > RK_MEM_SIZE-1.\n",__func__);
                return 1;
            }
            if (operand.data.as_i64 < 0) {
                fprintf(stderr,"\n\t[ERROR] at %s(): Invalid memory access, operand was negative.\n", __func__);
                return 1;
            }
            int reg_sign = rk->reg.data.as_i64 < 0;
            int operand_sign = rk->memory[operand.data.as_i64].data.as_i64 < 0;
            //printf("Adding mem{%" PRId64 "} to reg.\n", operand);
            rk->reg.data.as_i64 += rk->memory[operand.data.as_i64].data.as_i64;
            int post_reg_sign = rk->reg.data.as_i64 < 0;

            if ((reg_sign == operand_sign) && reg_sign != post_reg_sign) {
                fprintf(stderr,"\n\033[1;31m\t[ERROR] at %s(): addition integer overflow\033[1,39m\n", __func__);
                return 1;
            }
            rk->ic++;
        }
        break;
        case RK_SUB_I64: {
            if (rk->reg.type != RK_TYPE_INT64) {
                fprintf(stderr,"\n[ERROR] at %s(): reg type error, expected [%i] was [%i].\n",__func__, RK_TYPE_INT64, rk->reg.type);
                return 1;
            }
            if (rk->memory[operand.data.as_i64].type != RK_TYPE_INT64) {
                fprintf(stderr,"\n[ERROR] at %s(): memory access type error, expected [%i] was [%i].\n",__func__, RK_TYPE_INT64, rk->memory[operand.data.as_i64].type);
                return 1;
            }
            if (operand.data.as_i64 > RK_MEM_SIZE-1) {
                fprintf(stderr,"\n[ERROR] at %s(): operand > RK_MEM_SIZE-1.\n",__func__);
                return 1;
            }
            if (operand.data.as_i64 < 0) {
                fprintf(stderr,"\n\t[ERROR] at %s(): Invalid memory access, operand was negative.\n", __func__);
                return 1;
            }
            //printf("Subtracting mem{%" PRId64 "} from reg.\n", operand);
            rk->reg.data.as_i64 -= rk->memory[operand.data.as_i64].data.as_i64;
            rk->ic++;
        }
        break;
        case RK_DIV_I64: {
            if (rk->reg.type != RK_TYPE_INT64) {
                fprintf(stderr,"\n[ERROR] at %s(): reg type error, expected [%i] was [%i].\n",__func__, RK_TYPE_INT64, rk->reg.type);
                return 1;
            }
            if (rk->memory[operand.data.as_i64].type != RK_TYPE_INT64) {
                fprintf(stderr,"\n[ERROR] at %s(): memory access type error, expected [%i] was [%i].\n",__func__, RK_TYPE_INT64, rk->memory[operand.data.as_i64].type);
                return 1;
            }
            if (operand.data.as_i64 > RK_MEM_SIZE-1) {
                fprintf(stderr,"\n[ERROR] at %s(): operand > RK_MEM_SIZE-1.\n",__func__);
                return 1;
            }
            if (operand.data.as_i64 < 0) {
                fprintf(stderr,"\n\t[ERROR] at %s(): Invalid memory access, operand was negative.\n", __func__);
                return 1;
            }
            if (rk->memory[operand.data.as_i64].data.as_i64 == 0) {
                fprintf(stderr,"\n\t[ERROR] at %s(): Divide-by-zero.\nExecution ended abnormally.\n", __func__);
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
        case RK_MUL_I64: {
            if (rk->reg.type != RK_TYPE_INT64) {
                fprintf(stderr,"\n[ERROR] at %s(): reg type error, expected [%i] was [%i].\n",__func__, RK_TYPE_INT64, rk->reg.type);
                return 1;
            }
            if (rk->memory[operand.data.as_i64].type != RK_TYPE_INT64) {
                fprintf(stderr,"\n[ERROR] at %s(): memory access type error, expected [%i] was [%i].\n",__func__, RK_TYPE_INT64, rk->memory[operand.data.as_i64].type);
                return 1;
            }
            if (operand.data.as_i64 > RK_MEM_SIZE-1) {
                fprintf(stderr,"\n[ERROR] at %s(): operand > RK_MEM_SIZE-1.\n",__func__);
                return 1;
            }
            if (operand.data.as_i64 < 0) {
                fprintf(stderr,"\n\t[ERROR] at %s(): Invalid memory access, operand was negative.\n", __func__);
                return 1;
            }
            //printf("Multiplying reg by mem{%" PRId64 "}.\n", operand);
            Word res = (Word){ .type = RK_TYPE_INT64, .data.as_i64 = (rk->reg.data.as_i64) * (rk->memory[operand.data.as_i64].data.as_i64) };
            if (rk->reg.data.as_i64 != 0 && (res.data.as_i64 / rk->reg.data.as_i64 != rk->memory[operand.data.as_i64].data.as_i64)) {
                fprintf(stderr,"\n\033[1;31m\t[ERROR] at %s(): multiplication integer overflow\033[1;39m\n", __func__);
                return 1;
            }
            rk->reg.data.as_i64 = res.data.as_i64;
            rk->ic++;
        }
        break;
        case RK_DIVMOD_I64: {
            if (rk->reg.type != RK_TYPE_INT64) {
                fprintf(stderr,"\n[ERROR] at %s(): reg type error, expected [%i] was [%i].\n",__func__, RK_TYPE_INT64, rk->reg.type);
                return 1;
            }
            if (rk->memory[operand.data.as_i64].type != RK_TYPE_INT64) {
                fprintf(stderr,"\n[ERROR] at %s(): memory access type error, expected [%i] was [%i].\n",__func__, RK_TYPE_INT64, rk->memory[operand.data.as_i64].type);
                return 1;
            }
            if (operand.data.as_i64 > RK_MEM_SIZE-1) {
                fprintf(stderr,"\n[ERROR] at %s(): operand > RK_MEM_SIZE-1.\n",__func__);
                return 1;
            }
            if (operand.data.as_i64 < 0) {
                fprintf(stderr,"\n\t[ERROR] at %s(): Invalid memory access, operand was negative.\n", __func__);
                return 1;
            }
            //printf("Doing modulo on reg by mem{%" PRId64 "}.\n", operand);
            rk->reg.data.as_i64 %= rk->memory[operand.data.as_i64].data.as_i64;
            rk->ic++;
        }
        break;
        case RK_POW_I64: {
            if (rk->reg.type != RK_TYPE_INT64) {
                fprintf(stderr,"\n[ERROR] at %s(): reg type error, expected [%i] was [%i].\n",__func__, RK_TYPE_INT64, rk->reg.type);
                return 1;
            }
            if (rk->memory[operand.data.as_i64].type != RK_TYPE_INT64) {
                fprintf(stderr,"\n[ERROR] at %s(): memory access type error, expected [%i] was [%i].\n",__func__, RK_TYPE_INT64, rk->memory[operand.data.as_i64].type);
                return 1;
            }
            if (operand.data.as_i64 > RK_MEM_SIZE-1) {
                fprintf(stderr,"\n[ERROR] at %s(): operand > RK_MEM_SIZE-1.\n",__func__);
                return 1;
            }
            if (operand.data.as_i64 < 0) {
                fprintf(stderr,"\n\t[ERROR] at %s(): Invalid memory access, operand was negative.\n", __func__);
                return 1;
            }
            int reg_sign = rk->reg.data.as_i64 < 0;
            int operand_sign = rk->memory[operand.data.as_i64].data.as_i64 < 0;
            //printf("Doing power on reg by mem{%" PRId64 "}.\n", operand);
            rk->reg.data.as_i64 = pow(rk->reg.data.as_i64, rk->memory[operand.data.as_i64].data.as_i64);
            int post_reg_sign = rk->reg.data.as_i64 < 0;
            if ((reg_sign == operand_sign) && reg_sign != post_reg_sign) {
                fprintf(stderr,"\n\033[1;31m\t[ERROR] at %s(): power integer overflow\033[1,39m\n", __func__);
                return 1;
            }
            rk->ic++;
        }
        break;
        case RK_HALT: {
            if (rk->verbose_level > 1) {
                rk_dump(rk);
            }
            return 0;
        }
        break;
        case RK_JMP_I64: {
            if (operand.type != RK_TYPE_INT64) {
                fprintf(stderr,"\n[ERROR] at %s(): operand type error, expected [%i] was [%i].\n",__func__, RK_TYPE_INT64, operand.type);
                return 1;
            }
            //printf("\n\nSetting IC to {%" PRId64 "}.\n", operand);
            rk->ic = operand.data.as_i64 -1;
        }
        break;
        case RK_JMPNEG_I64: {
            if (operand.type != RK_TYPE_INT64) {
                fprintf(stderr,"\n[ERROR] at %s(): operand type error, expected [%i] was [%i].\n",__func__, RK_TYPE_INT64, operand.type);
                return 1;
            }
            if (rk->reg.type != RK_TYPE_INT64) {
                fprintf(stderr,"\n[ERROR] at %s(): reg type error, expected [%i] was [%i].\n",__func__, RK_TYPE_INT64, rk->reg.type);
                return 1;
            }
            if (rk->reg.data.as_i64 < 0) {
                //printf("\n\nRegister was negative, setting IC to {%" PRId64 "}.\n", operand);
                rk->ic = operand.data.as_i64;
            } else {
                rk->ic++;
            }
        }
        break;
        case RK_JMPZERO_I64: {
            if (operand.type != RK_TYPE_INT64) {
                fprintf(stderr,"\n[ERROR] at %s(): operand type error, expected [%i] was [%i].\n",__func__, RK_TYPE_INT64, operand.type);
                return 1;
            }
            if (rk->reg.type != RK_TYPE_INT64) {
                fprintf(stderr,"\n[ERROR] at %s(): reg type error, expected [%i] was [%i].\n",__func__, RK_TYPE_INT64, rk->reg.type);
                return 1;
            }
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
        rk->curr_op = rk_op_from_Word(rk->ir);
        if (rk->curr_op == RK_IMM_STRING) {
            rk->operand = rk_operand_from_Word_String(rk->ir);
            rk->operand.type = RK_TYPE_STRING;
        } else if (rk->curr_op == RK_IMM_CHAR) {
            rk->operand = rk_operand_from_Word_i64(rk->ir);
            rk->operand.type = RK_TYPE_CHAR;
        } else {
            rk->operand = rk_operand_from_Word_i64(rk->ir);
            rk->operand.type = RK_TYPE_INT64;
        }
        op_res = rk_do_op(rk->curr_op, rk->operand, rk);

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
            if (rk->curr_op == RK_IMM_CHAR) {
                int64_t value = rk->operand.data.as_i64;
                if (value == 0) {
                    fprintf(rk->out_fd, "\\0");
                } else if (value >= 32 && value <= 255) {
                    fprintf(rk->out_fd, "%c", (char) rk->operand.data.as_i64);
                } else {
                    fprintf(rk->out_fd, "\\¿");
                }
            } else {
                fprintf(rk->out_fd, "%" PRId64, rk->operand.data.as_i64);
            }
            if (is_colored) {
                fprintf(rk->out_fd, "\033[1;39m\n");
            } else {
                fprintf(rk->out_fd, "\n");
            }
        }

        if (op_res != 0) {
            #ifndef _WIN32
            fprintf(stderr,"\n\033[1;31m\t[ERROR] at %s():\n\tIC {%i}\n\top_res = {%i} for op {%s} [%i].\033[1;39m\n", __func__, rk->ic, op_res, rk_op_Str(rk->curr_op), rk->curr_op);
            #else
            fprintf(stderr,"\n\t[ERROR] at %s():\n\tIC {%i}\n\top_res = {%i} for op {%s} [%i].\n", __func__, rk->ic, op_res, rk_op_Str(rk->curr_op), rk->curr_op);
            #endif

            if (rk->curr_op != RK_HALT) {
                #ifndef _WIN32
                fprintf(stderr,"\n\033[1;31m[ERROR] at %s(): op [%s] failed with [%i], curr_op was not RK_HALT.\033[1;39m\n", __func__, rk_op_Str(rk->curr_op), op_res);
                #else
                fprintf(stderr,"\n[ERROR] at %s(): op [%s] failed with [%i], curr_op was not RK_HALT.\n", __func__, rk_op_Str(rk->curr_op), op_res);
                #endif

            }
            rk_dump(rk);
            return op_res;
        }
    } while (rk->curr_op != RK_HALT && rk->curr_op != RK_PANIC && rk->ic < RK_MEM_SIZE); //End of execution loop

    if (rk->curr_op != RK_HALT) {
        #ifndef _WIN32
        fprintf(stderr,"\n\033[1;31m[ERROR] at %s(): execution reached end of memory without halting.\033[1;39m\n", __func__);
        #else
        fprintf(stderr,"\n[ERROR] at %s(): execution reached end of memory without halting.\n", __func__);
        #endif
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
    if (rk_op_from_Word(rk->memory[pos]) == RK_IMM_CHAR) {
        rk->memory[pos].type = RK_TYPE_CHAR;
    } else if (rk_op_from_Word(rk->memory[pos]) == RK_IMM_STRING) {
        rk->memory[pos].type = RK_TYPE_STRING;
    } else if (rk_op_from_Word(rk->memory[pos]) == RK_IMM_I64) {
        rk->memory[pos].type = RK_TYPE_INT64;
    }

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
        if (rk_op_from_Word(rk->memory[i]) == RK_PANIC || rk_op_from_Word(rk->memory[i]) == RK_INVALID_OP) {
            if (rk->verbose_level > 1) fprintf(stdout, "Done loading program from %s.\n", ( fp == stdin ? "stdin" : "file"));
            break;
        }
    }

    return res;
}
