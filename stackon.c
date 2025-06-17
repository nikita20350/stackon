
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILE_NAME "program.st"
#define DA_CAP 512
#define COMMAND_MAX_LEN 511
#define STACK_CAP 512
#define HEAP_CAP 640000

#define DIV_Z_LABEL "divz"
#define DIV_OK_LABEL "divok"
#define EXIT_LABEL "exit"
#define MOD_Z_LABEL "modz"
#define MOD_OK_LABEL "modok"
typedef enum Instruction {
    Instr_Push,
    Instr_Plus,
    Instr_Minus,
    Instr_Mul,
    Instr_Div,
    Instr_Mod,
    Instr_Equals,
    Instr_Greater,
    Instr_Less,
    Instr_Or,
    Instr_And,
    Instr_Not,
    Instr_Dup,
    Instr_Over,
    Instr_Dump,
    Instr_If,
    Instr_Else,
    Instr_While,
    Instr_Do,
    Instr_End,
    Instr_Mem,
    Instr_Load,
    Instr_Store,
    Instr_Syscall3
} Instr;

typedef struct Expression {
    Instr instr;
    int param, row, col;
} Expr;

typedef struct program {
    int cap, size;
    Expr **items;
    char *FILENAME;
} Program;

typedef struct da_char {
    int cap, size;
    char *items;
} StringBuilder;

typedef struct da_int {
    int cap, size;
    int *items;
} da_int;

typedef struct stack {
    int len;
    int items[STACK_CAP];
} Stack;

#define da_init(da, capacity) \
    (da).size = 0;            \
    (da).cap = capacity;      \
    (da).items = malloc(capacity * sizeof(da.items[0]) + 1);

#define da_append(da, val)                                       \
    if ((da).size == (da).cap) {                                 \
        (da).items = reallocf((da).items, (da).size + (da).cap); \
        if ((da).items == NULL) {                                \
            exit(1);                                             \
        }                                                        \
    }                                                            \
    (da).items[(da).size++] = val;

int find_matching_end(FILE *f, int pc) {
    StringBuilder sb;
    da_init(sb, COMMAND_MAX_LEN);
    char buf;
    while (fread(&buf, 1, 1, f)) {
        if (buf != ' ' && buf != '\n' && buf != '\t') {
            da_append(sb, buf)
        } else if (sb.size > 0) {
            da_append(sb, '\0');
            if (strcmp(sb.items, "end") == 0) {
                return pc;
            }
            pc++;
            sb.size = 0;
        }
    }
    return -1;
}
void parse_program_from_file(FILE *f, Program *program) {
    int pc = 0;
    int x = 1, y = 1;
    da_int matching_stack;
    da_init(matching_stack, STACK_CAP);
    StringBuilder sb;
    da_init(sb, COMMAND_MAX_LEN);
    char buf;
    while (fread(&buf, 1, 1, f)) {
        if (buf != ' ' && buf != '\t' && buf != '\n') {
            da_append(sb, buf);
        } else {
            if (sb.size > 0) {
                da_append(sb, '\0');
                if (strcmp(sb.items, "+") == 0) {
                    Expr *expr = malloc(sizeof(Expr));
                    expr->instr = Instr_Plus;
                    expr->param = 0;
                    expr->row = y;
                    expr->col = x;
                    da_append(*program, expr);
                } else if (strcmp(sb.items, "-") == 0) {
                    Expr *expr = malloc(sizeof(Expr));
                    expr->instr = Instr_Minus;
                    expr->param = 0;
                    expr->row = y;
                    expr->col = x;
                    da_append(*program, expr);
                } else if (strcmp(sb.items, "*") == 0) {
                    Expr *expr = malloc(sizeof(Expr));
                    expr->instr = Instr_Mul;
                    expr->param = 0;
                    expr->row = y;
                    expr->col = x;
                    da_append(*program, expr);
                } else if (strcmp(sb.items, "/") == 0) {
                    Expr *expr = malloc(sizeof(Expr));
                    expr->instr = Instr_Div;
                    expr->param = 0;
                    expr->row = y;
                    expr->col = x;
                    da_append(*program, expr);
                } else if (strcmp(sb.items, "%") == 0) {
                    Expr *expr = malloc(sizeof(Expr));
                    expr->instr = Instr_Mod;
                    expr->param = 0;
                    expr->row = y;
                    expr->col = x;
                    da_append(*program, expr);
                } else if (strcmp(sb.items, "dump") == 0) {
                    Expr *expr = malloc(sizeof(Expr));
                    expr->instr = Instr_Dump;
                    expr->param = 0;
                    expr->row = y;
                    expr->col = x;
                    da_append(*program, expr);
                } else if (strcmp(sb.items, "=") == 0) {
                    Expr *expr = malloc(sizeof(Expr));
                    expr->instr = Instr_Equals;
                    expr->param = 0;
                    expr->row = y;
                    expr->col = x;
                    da_append(*program, expr);
                } else if (strcmp(sb.items, ">") == 0) {
                    Expr *expr = malloc(sizeof(Expr));
                    expr->instr = Instr_Greater;
                    expr->param = 0;
                    expr->row = y;
                    expr->col = x;
                    da_append(*program, expr);
                } else if (strcmp(sb.items, "<") == 0) {
                    Expr *expr = malloc(sizeof(Expr));
                    expr->instr = Instr_Less;
                    expr->param = 0;
                    expr->row = y;
                    expr->col = x;
                    da_append(*program, expr);
                } else if (strcmp(sb.items, "or") == 0) {
                    Expr *expr = malloc(sizeof(Expr));
                    expr->instr = Instr_Or;
                    expr->param = 0;
                    expr->row = y;
                    expr->col = x;
                    da_append(*program, expr);
                } else if (strcmp(sb.items, "and") == 0) {
                    Expr *expr = malloc(sizeof(Expr));
                    expr->instr = Instr_And;
                    expr->param = 0;
                    expr->row = y;
                    expr->col = x;
                    da_append(*program, expr);
                } else if (strcmp(sb.items, "not") == 0) {
                    Expr *expr = malloc(sizeof(Expr));
                    expr->instr = Instr_Not;
                    expr->param = 0;
                    expr->row = y;
                    expr->col = x;
                    da_append(*program, expr);
                } else if (strcmp(sb.items, "dup") == 0) {
                    Expr *expr = malloc(sizeof(Expr));
                    expr->instr = Instr_Dup;
                    expr->param = 0;
                    expr->row = y;
                    expr->col = x;
                    da_append(*program, expr);
                } else if (strcmp(sb.items, "over") == 0) {
                    Expr *expr = malloc(sizeof(Expr));
                    expr->instr = Instr_Over;
                    expr->param = 0;
                    expr->row = y;
                    expr->col = x;
                    da_append(*program, expr);
                } else if (strcmp(sb.items, "if") == 0) {
                    da_append(matching_stack, pc);
                    Expr *expr = malloc(sizeof(Expr));
                    expr->instr = Instr_If;
                    expr->param = -1;
                    expr->row = y;
                    expr->col = x;
                    da_append(*program, expr);
                } else if (strcmp(sb.items, "else") == 0) {
                    if (matching_stack.size < 1) {
                        printf("%s:%d:%d: Parsing error: `else` with no matching `if`\n", program->FILENAME, y, x);
                        exit(1);
                    }
                    program->items[matching_stack.items[matching_stack.size - 1]]->param = pc;
                    matching_stack.items[matching_stack.size - 1] = pc;
                    Expr *expr = malloc(sizeof(Expr));
                    expr->instr = Instr_Else;
                    expr->param = -1;
                    expr->row = y;
                    expr->col = x;
                    da_append(*program, expr);
                } else if (strcmp(sb.items, "while") == 0) {
                    da_append(matching_stack, pc);
                    Expr *expr = malloc(sizeof(Expr));
                    expr->instr = Instr_While;
                    expr->param = -1;
                    expr->row = y;
                    expr->col = x;
                    da_append(*program, expr);
                } else if (strcmp(sb.items, "do") == 0) {
                    Expr *expr = malloc(sizeof(Expr));
                    expr->instr = Instr_Do;
                    expr->param = matching_stack.items[matching_stack.size-- - 1];
                    expr->row = y;
                    expr->col = x;
                    da_append(matching_stack, pc);
                    da_append(*program, expr);
                } else if (strcmp(sb.items, "end") == 0) {
                    if (matching_stack.size < 1) {
                        printf("%s:%d:%d: Parsing error: `end` has no matching expression\n", program->FILENAME, y, x);
                        exit(1);
                    }
                    Expr *expr = malloc(sizeof(Expr));
                    expr->instr = Instr_End;
                    expr->param = -1;
                    expr->row = y;
                    expr->col = x;
                    if (program->items[matching_stack.items[matching_stack.size - 1]]->instr == Instr_Do) {
                        expr->param = program->items[matching_stack.items[matching_stack.size - 1]]->param;
                    }
                    program->items[matching_stack.items[matching_stack.size - 1]]->param = pc;
                    matching_stack.size--;
                    da_append(*program, expr);
                } else if (strcmp(sb.items, "mem") == 0) {
                    Expr *expr = malloc(sizeof(Expr));
                    expr->instr = Instr_Mem;
                    expr->param = -1;
                    expr->row = y;
                    expr->col = x;
                    da_append(*program, expr)
                } else if (strcmp(sb.items, "load") == 0) {
                    Expr *expr = malloc(sizeof(Expr));
                    expr->instr = Instr_Load;
                    expr->param = -1;
                    expr->row = y;
                    expr->col = x;
                    da_append(*program, expr)
                } else if (strcmp(sb.items, "store") == 0) {
                    Expr *expr = malloc(sizeof(Expr));
                    expr->instr = Instr_Store;
                    expr->param = -1;
                    expr->row = y;
                    expr->col = x;
                    da_append(*program, expr)
                } else if (strcmp(sb.items, "syscall3") == 0) {
                    Expr *expr = malloc(sizeof(Expr));
                    expr->instr = Instr_Syscall3;
                    expr->param = -1;
                    expr->row = y;
                    expr->col = x;
                    da_append(*program, expr)
                } else if (sb.items[0] == '/' && sb.items[1] == '/') {
                    // this is a comment, skip to the start of the next line
                    while (buf != '\n') {
                        fread(&buf, 1, 1, f);
                    }
                    pc--;
                    y++;
                } else {
                    char *endptr[1];
                    int num = (int)strtol(sb.items, endptr, 10);
                    if (**endptr != '\0') {
                        printf("%s:%d:%d: Invalid str in parsing an int in parse_program_from_file: %s\n", program->FILENAME, y, x, sb.items);
                        exit(1);
                    }
                    Expr *expr = malloc(sizeof(Expr));
                    expr->instr = Instr_Push;
                    expr->param = num;
                    expr->row = y;
                    expr->col = x;
                    da_append(*program, expr);
                }
                pc++;
                sb.size = 0;
            }
            if (buf == ' ') {
                x++;
            } else if (buf == '\t') {
                x += 4;
            } else if (buf == '\n') {
                x = 1;
                y++;
            }
        }
    }
}
const char *instr_to_cstr(Instr instr) {
    switch (instr) {
        case Instr_Push:
            return "Instr_Push";
        case Instr_Plus:
            return "Instr_Plus";
        case Instr_Minus:
            return "Instr_Minus";
        case Instr_Mul:
            return "Instr_Mul";
        case Instr_Div:
            return "Instr_Div";
        case Instr_Mod:
            return "Instr_Mod";
        case Instr_Equals:
            return "Instr_Equals";
        case Instr_Greater:
            return "Instr_Greater";
        case Instr_Less:
            return "Instr_Less";
        case Instr_Or:
            return "Instr_Or";
        case Instr_And:
            return "Instr_And";
        case Instr_Not:
            return "Instr_Not";
        case Instr_Dup:
            return "Instr_Dup";
        case Instr_Over:
            return "Instr_Over";
        case Instr_Dump:
            return "Instr_Dump";
        case Instr_If:
            return "Instr_If";
        case Instr_Else:
            return "Instr_Else";
        case Instr_While:
            return "Instr_While";
        case Instr_Do:
            return "Instr_Do";
        case Instr_End:
            return "Instr_End";
        case Instr_Mem:
            return "Instr_Mem";
        case Instr_Load:
            return "Instr_Load";
        case Instr_Store:
            return "Instr_Store";
        case Instr_Syscall3:
            return "Instr_Syscall3";
        default: {
            printf("Unreachable in instr_to_cstr\n");
            exit(1);
        }
    }
}
void program_dump(Program *program) {
    printf("--- Dumping program ---\n");
    for (int i = 0; i < program->size; i++) {
        printf("row: %d, col: %d, pc: %d: %s, %d\n", program->items[i]->row, program->items[i]->col, i, instr_to_cstr(program->items[i]->instr), program->items[i]->param);
    }
    printf("--- End of program ---\n");
}

void simulate_program(Program *program) {
    struct stack prg_stack = {
        .len = 0,
        {0}};
    int pc = 0;
    char memory[HEAP_CAP];
    while (pc < program->size) {
        switch (program->items[pc]->instr) {
            case Instr_Plus: {
                if (prg_stack.len < 2) {
                    printf("%s:%d:%d: Simulation error: not enough arguments on the stack for `+`\n", program->FILENAME, program->items[pc]->row, program->items[pc]->col);
                    exit(1);
                }
                prg_stack.items[prg_stack.len - 2] = prg_stack.items[prg_stack.len - 2] + prg_stack.items[prg_stack.len - 1];
                prg_stack.len--;
                pc++;
            } break;
            case Instr_Minus: {
                if (prg_stack.len < 2) {
                    printf("%s:%d:%d: Simulation error: not enough arguments on the stack for `-`\n", program->FILENAME, program->items[pc]->row, program->items[pc]->col);
                    exit(1);
                }
                prg_stack.items[prg_stack.len - 2] = prg_stack.items[prg_stack.len - 2] - prg_stack.items[prg_stack.len - 1];
                prg_stack.len--;
                pc++;
            } break;
            case Instr_Mul: {
                if (prg_stack.len < 2) {
                    printf("%s:%d:%d: Simulation error: not enough arguments on the stack for `*`\n", program->FILENAME, program->items[pc]->row, program->items[pc]->col);
                    exit(1);
                }
                prg_stack.items[prg_stack.len - 2] = prg_stack.items[prg_stack.len - 2] * prg_stack.items[prg_stack.len - 1];
                prg_stack.len--;
                pc++;
            } break;
            case Instr_Div: {
                if (prg_stack.len < 2) {
                    printf("%s:%d:%d: Simulation error: not enough arguments on the stack for `/`\n", program->FILENAME, program->items[pc]->row, program->items[pc]->col);
                    exit(1);
                }
                if (prg_stack.items[prg_stack.len - 1] == 0) {
                    printf("%s:%d:%d: Simulation error: division by zero\n", program->FILENAME, program->items[pc]->row, program->items[pc]->col);
                }
                prg_stack.items[prg_stack.len - 2] = prg_stack.items[prg_stack.len - 2] / prg_stack.items[prg_stack.len - 1];
                prg_stack.len--;
                pc++;
            } break;
            case Instr_Mod: {
                if (prg_stack.len < 2) {
                    printf("%s:%d:%d: Simulation error: not enough arguments on the stack for `%%`\n", program->FILENAME, program->items[pc]->row, program->items[pc]->col);
                    exit(1);
                }
                if (prg_stack.items[prg_stack.len - 1] == 0) {
                    printf("%s:%d:%d: Simulation error: modulo by zero\n", program->FILENAME, program->items[pc]->row, program->items[pc]->col);
                }
                prg_stack.items[prg_stack.len - 2] = prg_stack.items[prg_stack.len - 2] % prg_stack.items[prg_stack.len - 1];
                prg_stack.len--;
                pc++;
            } break;
            case Instr_Equals: {
                if (prg_stack.len < 2) {
                    printf("%s:%d:%d: Simulation error: not enough arguments on the stack for `=`\n", program->FILENAME, program->items[pc]->row, program->items[pc]->col);
                }
                prg_stack.items[prg_stack.len - 2] = (prg_stack.items[prg_stack.len - 2] == prg_stack.items[prg_stack.len - 1]) ? 1 : 0;
                prg_stack.len--;
                pc++;
            } break;
            case Instr_Greater: {
                if (prg_stack.len < 2) {
                    printf("%s:%d:%d: Simulation error: not enough arguments on the stack for `>`\n", program->FILENAME, program->items[pc]->row, program->items[pc]->col);
                }
                prg_stack.items[prg_stack.len - 2] = (prg_stack.items[prg_stack.len - 2] > prg_stack.items[prg_stack.len - 1]) ? 1 : 0;
                prg_stack.len--;
                pc++;
            } break;
            case Instr_Less: {
                if (prg_stack.len < 2) {
                    printf("%s:%d:%d: Simulation error: not enough arguments on the stack for `<`\n", program->FILENAME, program->items[pc]->row, program->items[pc]->col);
                }
                prg_stack.items[prg_stack.len - 2] = (prg_stack.items[prg_stack.len - 2] < prg_stack.items[prg_stack.len - 1]) ? 1 : 0;
                prg_stack.len--;
                pc++;
            } break;
            case Instr_Or: {
                if (prg_stack.len < 2) {
                    printf("%s:%d:%d: Simulation error: not enough arguments on the stack for `or`\n", program->FILENAME, program->items[pc]->row, program->items[pc]->col);
                }
                prg_stack.items[prg_stack.len - 2] = (prg_stack.items[prg_stack.len - 2] || prg_stack.items[prg_stack.len - 1]) ? 1 : 0;
                prg_stack.len--;
                pc++;
            } break;
            case Instr_And: {
                if (prg_stack.len < 2) {
                    printf("%s:%d:%d: Simulation error: not enough arguments on the stack for `and`\n", program->FILENAME, program->items[pc]->row, program->items[pc]->col);
                }
                prg_stack.items[prg_stack.len - 2] = (prg_stack.items[prg_stack.len - 2] && prg_stack.items[prg_stack.len - 1]) ? 1 : 0;
                prg_stack.len--;
                pc++;
            } break;
            case Instr_Not: {
                if (prg_stack.len < 1) {
                    printf("%s:%d:%d: Simulation error: not enough arguments on the stack for `not`\n", program->FILENAME, program->items[pc]->row, program->items[pc]->col);
                }
                prg_stack.items[prg_stack.len - 1] = prg_stack.items[prg_stack.len - 1] ? 0 : 1;
                pc++;
            } break;
            case Instr_Dump: {
                if (prg_stack.len < 1) {
                    printf("%s:%d:%d: Simulation error: not enough arguments on the stack for `dump`\n", program->FILENAME, program->items[pc]->row, program->items[pc]->col);
                    exit(1);
                }
                printf("%d\n", prg_stack.items[prg_stack.len - 1]);
                pc++;
            } break;
            case Instr_If: {
                if (prg_stack.len < 1) {
                    printf("%s:%d:%d: Simulation error: not enough arguments on the stack for `if`\n", program->FILENAME, program->items[pc]->row, program->items[pc]->col);
                    exit(1);
                } else if (program->items[pc]->param < 0) {
                    printf("%s:%d:%d: Simulation error: no matching `end` for `if`\n", program->FILENAME, program->items[pc]->row, program->items[pc]->col);
                    exit(1);
                } else if (prg_stack.items[prg_stack.len - 1] == 0) {
                    pc = program->items[pc]->param + 1;
                } else {
                    pc++;
                }
                prg_stack.len--;
            } break;
            case Instr_Else: {
                if (program->items[pc]->param < 0) {
                    printf("%s:%d:%d: Simulation error: no matching `end` for `else`\n", program->FILENAME, program->items[pc]->row, program->items[pc]->col);
                    exit(1);
                }
                pc = program->items[pc]->param;
            } break;
            case Instr_While: {
                pc++;
            } break;
            case Instr_Do: {
                if (prg_stack.len < 1) {
                    printf("%s:%d:%d: Simulation error: not enought arguments on the stack for `while ... do`\n", program->FILENAME, program->items[pc]->row, program->items[pc]->col);
                    exit(1);
                }
                if (prg_stack.items[prg_stack.len - 1] == 0) {
                    pc = program->items[pc]->param + 1;
                } else {
                    pc++;
                }
                prg_stack.len--;
            } break;
            case Instr_End: {
                if (program->items[pc]->param > -1) {
                    pc = program->items[pc]->param;
                } else {
                    pc++;
                }
            } break;
            case Instr_Push: {
                prg_stack.items[prg_stack.len++] = program->items[pc]->param;
                pc++;
            } break;
            case Instr_Dup: {
                if (prg_stack.len < 1) {
                    printf("%s:%d:%d: Simulation error: not enough arguments on the stack for `dup`\n", program->FILENAME, program->items[pc]->row, program->items[pc]->col);
                    exit(1);
                }
                prg_stack.items[prg_stack.len++] = prg_stack.items[prg_stack.len - 1];
                pc++;
            } break;
            case Instr_Over: {
                if (prg_stack.len < 2) {
                    printf("%s:%d:%d: Simulation error: not enough arguments on the stack for `over`\n", program->FILENAME, program->items[pc]->row, program->items[pc]->col);
                    exit(1);
                }
                int t = prg_stack.items[prg_stack.len - 1];
                prg_stack.items[prg_stack.len - 1] = prg_stack.items[prg_stack.len - 2];
                prg_stack.items[prg_stack.len - 2] = t;
                pc++;
            } break;
            case Instr_Mem: {
                prg_stack.items[prg_stack.len++] = 0;
                pc++;
            } break;
            case Instr_Load: {
                if (prg_stack.len < 1) {
                    printf("%s:%d:%d: Simulation error: not enough arguments on the stack for `load`\n", program->FILENAME, program->items[pc]->row, program->items[pc]->col);
                    exit(1);
                }
                prg_stack.items[prg_stack.len - 1] = memory[prg_stack.items[prg_stack.len - 1]];
                pc++;
            } break;
            case Instr_Store: {
                if (prg_stack.len < 2) {
                    printf("%s:%d:%d: Simulation error: not enough arguments on the stack for `store`\n", program->FILENAME, program->items[pc]->row, program->items[pc]->col);
                    exit(1);
                }
                memory[prg_stack.items[prg_stack.len - 2]] = prg_stack.items[prg_stack.len - 1];
                prg_stack.len -= 2;
                pc++;
            } break;
            case Instr_Syscall3: {
                if (prg_stack.len < 4) {
                    printf("%s:%d:%d: Simulation error: not enough arguments on the stack for `syscall3`\n", program->FILENAME, program->items[pc]->row, program->items[pc]->col);
                    exit(1);
                }
                int fd = prg_stack.items[prg_stack.len - 1];
                int buf = prg_stack.items[prg_stack.len - 2];
                int sys_n = prg_stack.items[prg_stack.len - 4];
                if (sys_n != 4) {
                    printf("Unsupported syscall number in simulation: %d\n", sys_n);
                    exit(1);
                }
                if (fd == 1) {
                    printf("%s", &memory[buf]);
                } else if (fd == 2) {
                    fprintf(stderr, "%s", &memory[buf]);
                } else {
                    printf("Unsupported fd in simulation: %d\n", fd);
                    exit(1);
                }
                pc++;
            } break;
            default: {
                printf("%s:%d:%d: Unreachable in simulate_program\n", program->FILENAME, program->items[pc]->row, program->items[pc]->col);
                exit(1);
            }
        }
    }
}
void compile_program(Program *program, const char *filename) {
    char *dot_asm, *clang_cmd;
    asprintf(&dot_asm, "%s.s", filename);

    FILE *out_f = fopen(dot_asm, "w");
    if (!out_f) {
        printf("Error: cannot open output file %s\n", filename);
        exit(1);
    }
    fprintf(out_f, ".global _main\n");
    fprintf(out_f, ".align 4\n");
    fprintf(out_f, "_main:\n");
    fprintf(out_f, "    str lr, [sp, #-16]!\n");
    fprintf(out_f, "    mov fp, sp\n");
    int pc = 0, stack_size = 0;
    while (pc < program->size) {
        switch (program->items[pc]->instr) {
            case Instr_Plus: {
                fprintf(out_f, "; --- + ---\n");
                if (stack_size < 2) {
                    printf("Compilation error: not enough arguments on the stack for `+`\n");
                    exit(1);
                }
                fprintf(out_f, "    ldr x0, [sp], #16\n");
                fprintf(out_f, "    ldr x1, [sp]\n");
                fprintf(out_f, "    add x0, x1, x0\n");
                fprintf(out_f, "    str x0, [sp]\n");
                stack_size--;
            } break;
            case Instr_Minus: {
                fprintf(out_f, "; --- - ---\n");
                if (stack_size < 2) {
                    printf("Compilation error: not enough arguments on the stack for `-`\n");
                    exit(1);
                }
                fprintf(out_f, "    ldr x0, [sp], #16\n");
                fprintf(out_f, "    ldr x1, [sp]\n");
                fprintf(out_f, "    sub x0, x1, x0\n");
                fprintf(out_f, "    str x0, [sp]\n");
                stack_size--;

            } break;
            case Instr_Mul: {
                fprintf(out_f, "; --- * ---\n");
                if (stack_size < 2) {
                    printf("Compilation error: not enough arguments on the stack for `*`\n");
                    exit(1);
                }
                fprintf(out_f, "    ldr x0, [sp], #16\n");
                fprintf(out_f, "    ldr x1, [sp]\n");
                fprintf(out_f, "    mul x0, x1, x0\n");
                fprintf(out_f, "    str x0, [sp]\n");
                stack_size--;
            } break;
            case Instr_Div: {
                fprintf(out_f, "; --- / ---\n");
                if (stack_size < 2) {
                    printf("Compilation error: not enough arguments on the stack for `/`\n");
                    exit(1);
                }
                fprintf(out_f, "    ldr x0, [sp], #16\n");
                fprintf(out_f, "    cbz x0, %s%d\n", DIV_Z_LABEL, pc);

                fprintf(out_f, "    ldr x1, [sp]\n");
                fprintf(out_f, "    sdiv x0, x1, x0\n");
                fprintf(out_f, "    str x0, [sp]\n");
                fprintf(out_f, "    b %s%d\n", DIV_OK_LABEL, pc);

                fprintf(out_f, "%s%d:\n", DIV_Z_LABEL, pc);
                fprintf(out_f, "    adrp x0, divz_err@page\n");
                fprintf(out_f, "    add x0, x0, divz_err@pageoff\n");
                fprintf(out_f, "    bl _printf\n");
                fprintf(out_f, "b %s\n", EXIT_LABEL);

                fprintf(out_f, "%s%d:\n", DIV_OK_LABEL, pc);
                stack_size--;
            } break;
            case Instr_Mod: {
                fprintf(out_f, "; --- %% ---\n");
                if (stack_size < 2) {
                    printf("Compilation error: not enough arguments on the stack for `%%`\n");
                    exit(1);
                }
                fprintf(out_f, "    ldr x0, [sp], #16\n");
                fprintf(out_f, "    cbz x0, %s%d\n", MOD_Z_LABEL, pc);

                fprintf(out_f, "    ldr x1, [sp]\n");
                fprintf(out_f, "    sdiv x2, x1, x0\n");
                fprintf(out_f, "    msub x0, x2, x0, x1\n");
                fprintf(out_f, "    str x0, [sp]\n");
                fprintf(out_f, "    b %s%d\n", MOD_OK_LABEL, pc);

                fprintf(out_f, "%s%d:\n", MOD_Z_LABEL, pc);
                fprintf(out_f, "    adrp x0, modz_err@page\n");
                fprintf(out_f, "    add x0, x0, modz_err@pageoff\n");
                fprintf(out_f, "    bl _printf\n");
                fprintf(out_f, "b %s\n", EXIT_LABEL);

                fprintf(out_f, "%s%d:\n", MOD_OK_LABEL, pc);
                stack_size--;
            } break;
            case Instr_Equals: {
                fprintf(out_f, "; --- = ---\n");
                if (stack_size < 2) {
                    printf("Compilation error: not enough arguments on the stack for `=`\n");
                    exit(1);
                }
                fprintf(out_f, "    ldr x1, [sp], #16\n");
                fprintf(out_f, "    ldr x0, [sp]\n");
                fprintf(out_f, "    cmp x0, x1\n");
                fprintf(out_f, "    cset x0, eq\n");
                fprintf(out_f, "    str x0, [sp]\n");
            } break;
            case Instr_Greater: {
                fprintf(out_f, "; --- > ---\n");
                if (stack_size < 2) {
                    printf("Compilation error: not enough arguments on the stack for `>`\n");
                    exit(1);
                }
                fprintf(out_f, "    ldr x1, [sp], #16\n");
                fprintf(out_f, "    ldr x0, [sp]\n");
                fprintf(out_f, "    cmp x0, x1\n");
                fprintf(out_f, "    cset x0, gt\n");
                fprintf(out_f, "    str x0, [sp]\n");
            } break;
            case Instr_Less: {
                fprintf(out_f, "; --- < ---\n");
                if (stack_size < 2) {
                    printf("Compilation error: not enough arguments on the stack for `<`\n");
                    exit(1);
                }
                fprintf(out_f, "    ldr x1, [sp], #16\n");
                fprintf(out_f, "    ldr x0, [sp]\n");
                fprintf(out_f, "    cmp x0, x1\n");
                fprintf(out_f, "    cset x0, lt\n");
                fprintf(out_f, "    str x0, [sp]\n");
            } break;
            case Instr_Or: {
                fprintf(out_f, "; --- or ---\n");
                if (stack_size < 2) {
                    printf("Compilation error: not enough arguments on the stack for `and`\n");
                    exit(1);
                }
                fprintf(out_f, "    ldr x1, [sp], #16\n");
                fprintf(out_f, "    ldr x0, [sp]\n");
                fprintf(out_f, "    orr x0, x0, x1\n");
                fprintf(out_f, "    mov x1, #1\n");
                fprintf(out_f, "    and x0, x0, x1\n");
                fprintf(out_f, "    str x0, [sp]\n");
            } break;
            case Instr_And: {
                fprintf(out_f, "; --- and ---\n");
                if (stack_size < 2) {
                    printf("Compilation error: not enough arguments on the stack for `and`\n");
                    exit(1);
                }
                fprintf(out_f, "    ldr x1, [sp], #16\n");
                fprintf(out_f, "    ldr x0, [sp]\n");
                fprintf(out_f, "    and x0, x0, x1\n");
                fprintf(out_f, "    mov x1, #1\n");
                fprintf(out_f, "    and x0, x0, x1\n");
                fprintf(out_f, "    str x0, [sp]\n");
            } break;
            case Instr_Not: {
                fprintf(out_f, "; --- not ---\n");
                if (stack_size < 1) {
                    printf("Compilation error: not enough arguments on the stack for `not`\n");
                    exit(1);
                }
                fprintf(out_f, "    ldr x0, [sp]\n");
                fprintf(out_f, "    cmp x0, #0\n");
                fprintf(out_f, "    cset x0, eq\n");
                fprintf(out_f, "    str x0, [sp]\n");
            } break;
            case Instr_Dump: {
                fprintf(out_f, "; --- dump ---\n");
                if (stack_size < 1) {
                    printf("Compilation error: not enough arguments on the stack for `dump`\n");
                    exit(1);
                }
                fprintf(out_f, "    adrp x0, prtstr@page\n");
                fprintf(out_f, "    add x0, x0, prtstr@pageoff\n");
                fprintf(out_f, "    bl _printf\n");
                fprintf(out_f, "    add sp, sp, #16\n");
                stack_size--;
            } break;
            case Instr_If: {
                fprintf(out_f, "; --- if ---\n");
                if (stack_size < 1) {
                    printf("Compilation error: not enough arguments on the stack for `if`\n");
                    exit(1);
                } else if (program->items[pc]->param < 0) {
                    printf("Compilation error: No matching `end` for `if`\n");
                    exit(1);
                }
                fprintf(out_f, "    ldr x0, [sp], #16\n");
                fprintf(out_f, "    cbz x0, %df\n", program->items[pc]->param);
            } break;
            case Instr_Else: {
                fprintf(out_f, "; --- else ---\n");
                if (stack_size < 1) {
                    printf("Compilation error: not enough arguments on the stack for `else`\n");
                    exit(1);
                }
                fprintf(out_f, "    b %df\n", program->items[pc]->param);
                fprintf(out_f, "%d:    \n", pc);
            } break;
            case Instr_While: {
                fprintf(out_f, "; --- while ---\n");
                fprintf(out_f, "%d:\n", pc);
            } break;
            case Instr_Do: {
                fprintf(out_f, "; --- do ---\n");
                if (stack_size < 1) {
                    printf("Compilation error: not enough arguments on the stack for `while ... do`\n");
                    exit(1);
                }
                fprintf(out_f, "    ldr x0, [sp], #16\n");
                fprintf(out_f, "    cbz x0, %df\n", program->items[pc]->param);
                stack_size--;
            } break;
            case Instr_End: {
                fprintf(out_f, "; --- end ---\n");
                if (program->items[pc]->param > -1) {
                    fprintf(out_f, "    b %db\n", program->items[pc]->param);
                }
                fprintf(out_f, "%d:\n", pc);
            } break;
            case Instr_Mem: {
                fprintf(out_f, "; --- mem ---\n");
                fprintf(out_f, "    adrp x0, memory@page\n");
                fprintf(out_f, "    add x0, x0, memory@pageoff\n");
                fprintf(out_f, "    str x0, [sp, #-16]!\n");
                stack_size++;
            } break;
            case Instr_Load: {
                fprintf(out_f, "; --- load ---\n");
                fprintf(out_f, "    ldr x0, [sp], #16\n");
                fprintf(out_f, "    ldrb w0, [x0]\n");
                fprintf(out_f, "    str w0, [sp, #-16]!\n");
            } break;
            case Instr_Store: {
                if (stack_size < 2) {
                    printf("Compilation error: not enough arguments on the stack for `store`\n");
                    exit(1);
                }
                fprintf(out_f, "; --- store ---\n");
                fprintf(out_f, "    ldr x1, [sp], #16\n");
                fprintf(out_f, "    ldr x0, [sp], #16\n");
                fprintf(out_f, "    strb w1, [x0]\n");
                stack_size -= 2;
            } break;
            case Instr_Push: {
                fprintf(out_f, "; --- push %d ---\n", program->items[pc]->param);
                fprintf(out_f, "    ldr x0, =%d\n", program->items[pc]->param);
                fprintf(out_f, "    str x0, [sp, #-16]! \n");
                stack_size++;
            } break;
            case Instr_Dup: {
                fprintf(out_f, "; --- dup ---\n");
                if (stack_size < 1) {
                    printf("Compilation error: not enough arguments on the stack for `dup`\n");
                    exit(1);
                }
                fprintf(out_f, "    ldr x0, [sp]\n");
                fprintf(out_f, "    str x0, [sp, #-16]!\n");
                stack_size++;
            } break;
            case Instr_Over: {
                fprintf(out_f, "; --- over ---\n");
                if (stack_size < 2) {
                    printf("Compilation error: not enough arguments on the stack for `over`\n");
                    exit(1);
                }
                fprintf(out_f, "    ldr x0, [sp, #16]\n");
                fprintf(out_f, "    ldr x1, [sp]\n");
                fprintf(out_f, "    str x0, [sp]\n");
                fprintf(out_f, "    str x1, [sp, #16]\n");
            } break;
            case Instr_Syscall3: {
                fprintf(out_f, "; --- syscall3 ---\n");
                if (stack_size < 4) {
                    printf("Compilation error: not enough arguments on the stack for `syscall3`\n");
                    exit(1);
                }
                fprintf(out_f, "    ldr x0, [sp], #16\n");
                fprintf(out_f, "    ldr x1, [sp], #16\n");
                fprintf(out_f, "    ldr x2, [sp], #16\n");
                fprintf(out_f, "    ldr x16, [sp], #16\n");
                fprintf(out_f, "    svc #0x80\n");
            }
        }
        pc++;
    }
    fprintf(out_f, "%s:\n", EXIT_LABEL);
    fprintf(out_f, "    mov sp, fp\n");
    fprintf(out_f, "    ldr lr, [sp], #16\n");
    fprintf(out_f, "    mov x0, #0\n");
    fprintf(out_f, "    ret\n");
    fprintf(out_f, ".data\n");
    fprintf(out_f, "prtstr: .asciz \"%%d\\n\"\n");
    fprintf(out_f, "divz_err: .asciz \"Division by zero error\\n\"\n");
    fprintf(out_f, "modz_err: .asciz \"Modulo by zero error\\n\"\n");
    fprintf(out_f, ".align 2\n");
    fprintf(out_f, ".bss\n");
    fprintf(out_f, "memory: .fill %d, 1, 0\n", HEAP_CAP);
    fclose(out_f);

    asprintf(&clang_cmd, "clang -o %s %s", filename, dot_asm);
    system(clang_cmd);
}

void usage(void) {
    printf("Usage: ./stackon <mode> -[flags] <prg_name.st>\n");
    printf("Possible modes:\n");
    printf("sim: simulate the output of the progran\ncom: compile the program to assembly\ndbg: dump, simulate, compile and run the program\n");
    printf("Possible flags:\n");
    printf("-r: works with `com` mode, immediately runs the compiled program if successful");
}

int main(int argc, char *argv[]) {
    char *mode;
    char *flags;
    char *filename;
    char *outcmd;
    if (argc < 3) {
        usage();
        exit(1);
    }
    if (argc == 3) {
        mode = argv[1];
        filename = argv[2];
    } else if (argc == 4) {
        mode = argv[1];
        flags = argv[2];
        filename = argv[3];
        if (flags[0] != '-') {
            usage();
            exit(1);
        }
    } else {
        usage();
        exit(1);
    }

    int n = strlen(filename);
    assert(filename[n - 3] == '.' && filename[n - 2] == 's' && filename[n - 1] == 't' && "Incorrect file extension, should be .st\n");
    char outfile[n - 2];
    printf("mode: %s, file: %s\n", mode, filename);

    static Program program;
    da_init(program, DA_CAP);
    program.FILENAME = filename;
    FILE *f = fopen(filename, "r");
    if (!f) {
        printf("Error: cannot open input file %s\n", filename);
        exit(1);
    }

    strncpy(outfile, filename, n - 3);
    outfile[n - 3] = '\0';

    asprintf(&outcmd, "./%s", outfile);
    parse_program_from_file(f, &program);
    if (strcmp(mode, "sim") == 0) {
        printf("--- Simulation result ---\n");
        simulate_program(&program);
    } else if (strcmp(mode, "com") == 0) {
        compile_program(&program, outfile);
        if (flags[1] == 'r') {
            printf("--- Compilation result ---\n");
            system(outcmd);
        }
    } else if (strcmp(mode, "dbg") == 0) {
        program_dump(&program);
        printf("--- Simulation result ---\n");
        simulate_program(&program);
        printf("\n");
        compile_program(&program, outfile);
        printf("--- Compilation result ---\n");
        system(outcmd);
    }
    fclose(f);
    return 0;
}
