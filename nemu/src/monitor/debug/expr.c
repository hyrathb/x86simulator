#include "nemu.h"

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <sys/types.h>
#include <regex.h>
#include <ctype.h>
#include <stdlib.h>
#include <elf.h>

enum
{
    NOTYPE = 256, EQ

    /* TODO: Add more token types */

};

static struct rule
{
    char *regex;
    int token_type;
} rules[] =
{

    /* TODO: Add more rules.
     * Pay attention to the precedence level of different rules.
     */

    {" +",	NOTYPE},				// spaces
    {"\\+", '+'},					// plus
    {"==", EQ}						// equal
};

#define NR_REGEX (sizeof(rules) / sizeof(rules[0]) )

static regex_t re[NR_REGEX];

extern char *strtab;
extern Elf32_Sym *symtab;
extern int nr_symtab_entry;

/* Rules are used for many times.
 * Therefore we compile them only once before any usage.
 */
void init_regex()
{
    int i;
    char error_msg[128];
    int ret;

    for(i = 0; i < NR_REGEX; i ++)
    {
        ret = regcomp(&re[i], rules[i].regex, REG_EXTENDED);
        if(ret != 0)
        {
            regerror(ret, &re[i], error_msg, 128);
            Assert(ret != 0, "regex compilation failed: %s\n%s", error_msg, rules[i].regex);
        }
    }
}

typedef struct token
{
    int type;
    char str[32];
} Token;

Token tokens[32];
int nr_token;

static bool make_token(char *e)
{
    int position = 0;
    int i;
    regmatch_t pmatch;

    nr_token = 0;

    while(e[position] != '\0')
    {
        /* Try all rules one by one. */
        for(i = 0; i < NR_REGEX; i ++)
        {
            if(regexec(&re[i], e + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0)
            {
                char *substr_start = e + position;
                int substr_len = pmatch.rm_eo;

                Log("match rules[%d] = \"%s\" at position %d with len %d: %.*s", i, rules[i].regex, position, substr_len, substr_len, substr_start);
                position += substr_len;

                /* TODO: Now a new token is recognized with rules[i]. Add codes
                 * to record the token in the array ``tokens''. For certain
                 * types of tokens, some extra actions should be performed.
                 */

                switch(rules[i].token_type)
                {
                default:
                    panic("please implement me");
                }

                break;
            }
        }

        if(i == NR_REGEX)
        {
            printf("no match at position %d\n%s\n%*.s^\n", position, e, position, "");
            return false;
        }
    }

    return true;
}

uint32_t expr(char *e, bool *success)
{
    if(!make_token(e))
    {
        *success = false;
        return 0;
    }

    /* TODO: Insert codes to evaluate the expression. */
    panic("please implement me");
    return 0;
}

static int do_cal_str(char *s)
{
    int i;
    int len = strlen(s);
    int last = 0, lastnumi = 0,minop = 32767, minpos = -1, base = 0;
    char lastnumstr[15];
    char minopc = 0;
    for (i=0; i<len; ++i)
    {
        if (isalnum(*(s+i)) || *(s+i) == '_')
        {
            last = 1;
            lastnumstr[lastnumi++] = *(s+i);
        }
        else
        {
            lastnumi = 0;
            switch (*(s+i))
            {
            case '(':
            {
                last = 0;
                base += 10;
                if (base < minop)
                {
                    minop = base;
                    minpos = i;
                    minopc = *(s+i);
                }
                break;
            }
            case ')':
            {
                last = 1;
                base -=10;
                break;
            }
            case '*':
            {
                if (!last)
                {
                    last = 0;
                    if (base + 9 < minop)
                    {
                        minop = base + 9;
                        minpos = i;
                        minopc = *(s+i);
                    }
                    break;
                }
            }
            case '/':
            {
                last = 0;
                if (base + 7 < minop)
                {
                    minop = base + 7;
                    minpos = i;
                    minopc = *(s+i);
                }
                break;
            }
            case '+':
            case '-':
            {
                last = 0;
                if (base + 6 < minop)
                {
                    minop = base + 6;
                    minpos = i;
                    minopc = *(s+i);
                }
                break;
            }
            case '&':
            {
                last = 0;
                if (*(s+i+1) == '&')
                {
                    if (base + 5 < minop)
                    {
                        minop = base + 5;
                        minpos = i;
                        minopc = *(s+i);
                    }
                    ++i;
                }
                else
                {
                    if (base + 8 < minop)
                    {
                        minop = base + 8;
                        minpos = i;
                        minopc = *(s+i);
                    }
                }
                break;
            }
            case '|':
            {
                last = 0;
                if (*(s+i+1) == '|')
                {
                    if (base + 4 < minop)
                    {
                        minop = base +4;
                        minpos = i;
                        minopc = *(s+i);
                    }
                    ++i;
                }
                else
                {
                    if (base + 8 < minop)
                    {
                        minop = base + 8;
                        minpos = i;
                        minopc = *(s+i);
                    }
                }
                break;
            }
            case '!':
            case '=':
            {
                last = 0;
                if (*(s+i+1) == '=')
                {
                    if (base + 3 < minop)
                    {
                        minop = base + 3;
                        minpos = i;
                        minopc = *(s+i);
                    }
                    ++i;
                }
                break;
            }
            }
        }

    }
    if (minpos == -1)
    {
        lastnumstr[lastnumi] = 0;
        if (isdigit(lastnumstr[0]))
        {
            if (lastnumstr[0] == '0')
                switch (lastnumstr[1])
                {
                case 'x':
                    return strtoul(lastnumstr, NULL, 16);
                case 'b':
                    return strtoul(lastnumstr+2, NULL, 2);
                default:
                    return strtoul(lastnumstr, NULL, 8);
                }
            else
                return atoi(lastnumstr);
        }
        else
        {
            int i;
            for (i = 0; i< nr_symtab_entry; ++i)
            {
                if (ELF32_ST_TYPE((symtab+i)->st_name) == STT_OBJECT)
                    if (!strcmp(strtab + (symtab+i)->st_name, lastnumstr))
                        return (symtab+i)->st_value;
            }
        }
    }
    *(s+minpos) = 0;
    int ret = 0;
    if ((minopc == '*') && (minop % 10 == 9))
    {
        ret = swaddr_read(do_cal_str(s+minpos+1), 1);
    }
    else if (minopc != '(')
    {
        int op1, op2;
        if ((minop == 6) || (minop == 7))
        {
            op1 = do_cal_str(s);
            op2 = do_cal_str(s+minpos+1);
        }
        else
        {
            op1 = do_cal_str(s);
            op2 = do_cal_str(s+minpos+2);
        }
        switch (minopc)
        {
        case '*':
        {
            ret = op1*op2;
            break;
        }
        case '/':
        {
            ret = op1/op2;
            break;
        }
        case '+':
        {
            ret = op1 + op2;
            break;
        }
        case '-':
        {
            ret = op1 - op2;
            break;
        }
        case '&':
        {
            if (minop == 5)
                ret = op1 && op2;
            else
                ret = op1 & op2;
            break;
        }
        case '|':
        {
            if (minop == 4)
                ret = op1 | op2;
            else
                ret = op1 || op2;
            break;
        }
        case '!':
        {
            ret = op1 != op2;
            break;
        }
        case '=':
        {
            ret = op1 == op2;
            break;
        }
        }
    }
    else
    {
        *(s + len - 1) = 0;
        ret = do_cal_str(s+1);
    }
    return ret;
}

int cal_str(char *s)
{
    char *t = s;
    char prosesed[256];
    char *p = prosesed;
    for (; *t; t++)
    {
        if (*t == '$')
        {
            int32_t rep = 0;
            if (*(t+1) == 'e')
            {
                if (*(t+2) == 'a')
                {
                    rep = cpu.eax;
                    t += 3;
                }
                else if (*(t+2) == 'b')
                {
                    if (*(t+3) == 'x')
                        rep = cpu.ebx;
                    else
                        rep = cpu.ebp;
                    t += 3;
                }
                else if (*(t+2) == 'c')
                {
                    rep = cpu.ecx;
                    t+=3;
                }
                else if (*(t+2) == 'd')
                {
                    if (*(t+3) == 'x')
                        rep = cpu.edx;
                    else
                        rep = cpu.edi;
                    t += 3;
                }
                else if (*(t+2) == 's')
                {
                    if (*(t+3) == 'p')
                        rep = cpu.esp;
                    else
                        rep = cpu.esi;
                    t += 3;
                }
                else
                {
                    rep = cpu.eip;
                    t += 3;
                }
            }
            else  if(*(t+1) == 'a')
            {
                if (*(t+2) == 'x')
                    rep = cpu.ax;
                else if (*(t+2) == 'h')
                    rep = cpu.ah;
                else
                    rep = cpu.al;
                t += 2;
            }
            else  if(*(t+1) == 'c')
            {
                if (*(t+2) == 'x')
                    rep = cpu.cx;
                else if (*(t+2) == 'h')
                    rep = cpu.ch;
                else
                    rep = cpu.cl;
                t += 2;
            }
            else  if(*(t+1) == 'd')
            {
                if (*(t+2) == 'x')
                    rep = cpu.dx;
                else if(*(t+2) == 'h')
                    rep = cpu.dh;
                else if (*(t+2) == 'l')
                    rep = cpu.dl;
                else
                    rep = cpu.di;
                t += 2;
            }
            else  if(*(t+1) == 'b')
            {
                if (*(t+2) == 'x')
                    rep = cpu.bx;
                else if (*(t+2) == 'h')
                    rep = cpu.bh;
                else if (*(t+2) == 'l')
                    rep = cpu.bl;
                else
                    rep = cpu.bp;
                t += 2;
            }
            else  if(*(t+1) == 's')
            {
                if (*(t+2) == 'p')
                    rep = cpu.sp;
                else
                    rep = cpu.si;
                t += 2;
            }
            p += sprintf(p, "%d", rep);
        }
        else if (!isblank(*t))
            *(p++) = *t;
    }
    *p = 0;
    return do_cal_str(prosesed);
}
