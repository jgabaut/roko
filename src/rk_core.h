#ifndef RK_CORE_H_
#define RK_CORE_H_
#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include <assert.h>
#include <stdlib.h>
#include <math.h>
#include "anvil__roko.h"

#define ROKO_MAJOR 0 /**< Represents current major release.*/
#define ROKO_MINOR 1 /**< Represents current minor release.*/
#define ROKO_PATCH 1 /**< Represents current patch release.*/


static const int ROKO_API_VERSION_INT = (ROKO_MAJOR*1000000+ROKO_MINOR*10000+ROKO_PATCH*100); /**< Represents current version with numeric format.*/
static const char ROKO_API_VERSION_STRING[] = "0.1.1-dev"; /**< Represents current version with MAJOR.MINOR.PATCH format.*/

const char* string_roko_version(void);
const int int_roko_version(void);

void usage(const char* prog); /**< Prints correct invocation for main.*/

typedef enum Word_Type {
    RK_TYPE_NULL=0, /**< Defines NULL type.*/
    RK_TYPE_INT64, /**< Defines integer type with 64bit len.*/
    RK_TYPE_FLOAT64, /**< Defines double precision float type.*/
    RK_TYPE_CHAR, /**< Defines char type.*/
    RK_TYPE_STRING, /**< Defines string type.*/
    RK_TOT_TYPES, /**< Defines number of total types.*/
} Word_Type; /**< Defined types for Word data.*/

extern const char* RK_Word_Type_Str[RK_TOT_TYPES]; /**< Defines constant strings for Word_Type.*/

typedef struct Word {
    Word_Type type; /**< Type for the word.*/
    union {
        int64_t as_i64; /**< Field used when type == RK_TYPE_INT64*/
        double as_f64; /**< Field used when type == RK_TYPE_FLOAT64*/
    } data; /**< Data field for the word.*/
} Word; /**< Defines a Word struct for Roko.*/

#define RK_MEM_SIZE 100 /**< Defines static memory array size for Roko.*/
#define RK_WORD_FACTOR 100000000000000000 /**< Defines factor for Word operand modulo.*/
#define RK_EOF -999999999999999999 /**< Defines flag value to end a rkasm file.*/

typedef enum RK_OP {
    RK_PANIC=-92,
    RK_IMM_I64=0,
    RK_INVALID_OP=1,
    RK_IMM_CHAR=2,
    RK_PRINTNL=3,
    RK_IMM_STRING=4,
    RK_READ_I64=10,
    RK_WRITE_I64=11,
    RK_READ_CHAR=12,
    RK_WRITE_CHAR=13,
    RK_WRITE_STRING=14,
    RK_LOAD_I64=20,
    RK_STORE_I64=21,
    RK_LOAD_CHAR=22,
    RK_STORE_CHAR=23,
    RK_ADD_I64=30,
    RK_SUB_I64=31,
    RK_DIV_I64=32,
    RK_MUL_I64=33,
    RK_DIVMOD_I64=34,
    RK_POW_I64=35,
    RK_JMP_I64=40,
    RK_JMPNEG_I64=41,
    RK_JMPZERO_I64=42,
    RK_HALT=43,
    RK_SET_VERBOSE=50
} RK_OP; /**< Defines opcodes for Roko ops.*/

typedef enum RK_OP_Index {
    RK_PANIC_I=0,
    RK_IMM_I64_I,
    RK_INVALID_OP_I,
    RK_IMM_CHAR_I,
    RK_PRINTNL_I,
    RK_IMM_STRING_I,
    RK_READ_I64_I,
    RK_WRITE_I64_I,
    RK_READ_CHAR_I,
    RK_WRITE_CHAR_I,
    RK_WRITE_STRING_I,
    RK_LOAD_I64_I,
    RK_STORE_I64_I,
    RK_LOAD_CHAR_I,
    RK_STORE_CHAR_I,
    RK_ADD_I64_I,
    RK_SUB_I64_I,
    RK_DIV_I64_I,
    RK_MUL_I64_I,
    RK_DIVMOD_I64_I,
    RK_POW_I64_I,
    RK_JMP_I64_I,
    RK_JMPNEG_I64_I,
    RK_JMPZERO_I64_I,
    RK_HALT_I,
    RK_SET_VERBOSE_I,
    RK_TOT_OP_I
} RK_OP_Index; /**< Defines zero-based, contiguous indexes for RK_OP.*/

#define RK_TOT_OP RK_TOT_OP_I /**< Defines the total number of defined opcodes.*/
#define RK_MAX_OPS ((INT64_MAX / RK_WORD_FACTOR) +1) /**< Defines the forced limit on total opcodes, by the max value of int64_t.*/

typedef struct Roko {
    Word memory[RK_MEM_SIZE+1]; /**< Defines VM RAM memory.*/
    Word reg; /**< Defines accumulator register.*/
    int ic; /**< Defines instruction counter.*/
    Word ir; /**< Defines instruction register, for current status.*/
    RK_OP curr_op; /**< Defines current op from instruction register.*/
    Word operand; /**< Defines current operand from instruction register.*/
    FILE* out_fd; /**< Defines output file for interactive text.*/
    FILE* coredump_fd; /**< Defines output for core dump.*/
    int verbose_level; /**< Defines current verbose level for tracing.*/
} Roko; /**< Defines VM.*/

extern const char* RK_OP_Str[RK_TOT_OP]; /**< Defines constant strings for RK_OP.*/

void rk_init(Roko* rk, int verbose_level); /**< Initialises the pre-allocated passed Roko pointer.*/
const char* rk_op_Str(RK_OP op); /**< Returns a pointer to constant string for passed RK_OP.*/
const char* rk_type_Str(Word_Type t); /**< Returns a pointer to constant string for passed Word_Type.*/
void rk_print_Word_2file(FILE* fp, Word w, int colored, Roko* rk); /**< Tries printing passed Word to passed file pointer.*/
void rk_print_Word(Word w, int colored, Roko* rk); /**< Tries printing passed Word to passed fp, bool for colored output.*/
void rk_dump_colored_2file(Roko* rk, FILE* fp, int colored); /**< Dumps Roko state to passed fp, bool for colored output.*/
void rk_dump(Roko* rk); /**< Dumps Roko state to passed fd with no color.*/
int load_sum_prog(Roko* rk); /**< Loads hardcoded instructions for int sum program.*/
int load_mul_prog(Roko* rk); /**< Loads hardcoded instructions for int mul program.*/
int load_max_prog(Roko* rk); /**< Loads hardcoded instructions for max program.*/
int load_bad_prog(Roko* rk); /**< Loads hardcoded instructions for a non-halting program.*/
RK_OP rk_op_from_Word(Word w); /**< Returns a RK_OP from the passed Word.*/
Word rk_operand_from_Word_i64(Word w); /**< Returns an i64 operand from passed Word.*/
Word rk_operand_from_Word_String(Word w); /**< Returns a String operand from passed Word.*/
Word rk_operand_from_Word_f64(Word w); /**< Returns an f64 operand from passed Word.*/
int rk_do_op(RK_OP op, Word operand, Roko* rk); /**< Returns 0 if the requested op is successful, other values are errors.*/
int rk_execute(Roko* rk); /**< Starts execution from current Roko state.*/
int rk_load_word_from_file(Roko* rk, FILE* fp, int64_t pos); /**< Returns 1 if successful, other values are errors.*/
int load_program_from_file(FILE* fp, Roko* rk); /**< Returns 1 if successful, other values are errors.*/

#endif //RK_CORE_H_
