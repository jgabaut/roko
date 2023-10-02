#ifndef RK_CORE_H_
#define RK_CORE_H_
#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include <assert.h>
#include <stdlib.h>
#include "anvil__roko.h"

#define ROKO_MAJOR 0 /**< Represents current major release.*/
#define ROKO_MINOR 1 /**< Represents current minor release.*/
#define ROKO_PATCH 0 /**< Represents current patch release.*/


static const int ROKO_API_VERSION_INT = (ROKO_MAJOR*1000000+ROKO_MINOR*10000+ROKO_PATCH*100); /**< Represents current version with numeric format.*/
static const char ROKO_API_VERSION_STRING[] = "0.1.0"; /**< Represents current version with MAJOR.MINOR.PATCH format.*/

const char* string_roko_version(void);
const int int_roko_version(void);

void usage(const char* prog); /**< Prints correct invocation for main.*/

typedef enum Word_Type {
    RK_TYPE_INT64=0, /**< Defines integer type with 64bit len.*/
    RK_TYPE_FLOAT64, /**< Defines double precision float type.*/
    RK_TOT_TYPES, /**< Defines number of total types.*/
} Word_Type; /**< Defined types for Word data.*/

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
    RK_INVALID_OP=0,
    RK_READ=10,
    RK_WRITE=11,
    RK_LOAD=20,
    RK_STORE=21,
    RK_ADDINT=30,
    RK_SUBINT=31,
    RK_DIVINT=32,
    RK_MULINT=33,
    RK_DIVMOD=34,
    RK_JMP=40,
    RK_JMPNEG=41,
    RK_JMPZERO=42,
    RK_HALT=43,
} RK_OP; /**< Defines opcodes for Roko ops.*/

typedef enum RK_OP_Index {
    RK_INVALID_OP_I=0,
    RK_READ_I,
    RK_WRITE_I,
    RK_LOAD_I,
    RK_STORE_I,
    RK_ADDINT_I,
    RK_SUBINT_I,
    RK_DIVINT_I,
    RK_MULINT_I,
    RK_DIVMOD_I,
    RK_JMP_I,
    RK_JMPNEG_I,
    RK_JMPZERO_I,
    RK_HALT_I,
    RK_TOT_OP_I
} RK_OP_Index; /**< Defines zero-based, contiguous indexes for RK_OP.*/

#define RK_TOT_OP RK_TOT_OP_I /**< Defines the total number of defined opcodes.*/
#define RK_MAX_OPS (INT64_MAX / RK_WORD_FACTOR) /**< Defines the forced limit on total opcodes, by the max value of int64_t.*/

typedef struct Roko {
    Word memory[RK_MEM_SIZE+1]; /**< Defines VM RAM memory.*/
    Word reg; /**< Defines accumulator register.*/
    int ic; /**< Defines instruction counter.*/
    Word ir; /**< Defines instruction register, for current status.*/
    RK_OP curr_op; /**< Defines current op from instruction register.*/
    Word operand; /**< Defines current operan from instruction register.*/
    FILE* out_fd; /**< Defines output file for interactive text.*/
    FILE* coredump_fd; /**< Defines output for core dump.*/
    int verbose_level; /**< Defines current verbose level for tracing.*/
} Roko; /**< Defines VM.*/

extern const char* RK_OP_Str[RK_TOT_OP]; /**< Defines constant strings for RK_OP.*/

void rk_init(Roko* rk); /**< Initialises the pre-allocated passed Roko pointer.*/
const char* rk_op_Str(RK_OP op); /**< Returns a pointer to constant string for passed RK_OP.*/
void rk_dump_colored_2file(Roko* rk, FILE* fp, int colored); /**< Dumps Roko state to passed fp, bool for colored output.*/
void rk_dump(Roko* rk); /**< Dumps Roko state to passed fd with no color.*/
int load_sum_prog(Roko* rk); /**< Loads hardcoded instructions for int sum program.*/
int load_mul_prog(Roko* rk); /**< Loads hardcoded instructions for int mul program.*/
int load_max_prog(Roko* rk); /**< Loads hardcoded instructions for max program.*/
int load_bad_prog(Roko* rk); /**< Loads hardcoded instructions for a non-halting program.*/
RK_OP op_from_Word(Word w); /**< Returns a RK_OP from the passed Word.*/
Word operand_from_Word(Word w); /**< Returns an operand from passed Word.*/
int rk_do_op(RK_OP op, Word operand, Roko* rk); /**< Returns 0 if the requested op is successful, other values are errors.*/
int rk_execute(Roko* rk); /**< Starts execution from current Roko state.*/
int rk_load_word_from_file(Roko* rk, FILE* fp, int64_t pos); /**< Returns 1 if successful, other values are errors.*/
int load_program_from_file(FILE* fp, Roko* rk); /**< Returns 1 if successful, other values are errors.*/

#endif //RK_CORE_H_
