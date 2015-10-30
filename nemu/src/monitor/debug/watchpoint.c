#include <stdlib.h>
#include "monitor/watchpoint.h"
#include "monitor/expr.h"

#include "memory/memory.h"

#define NR_WP 32

static WP wp_list[NR_WP];
static WP *head, *free_;

void init_wp_list()
{
    int i;
    for(i = 0; i < NR_WP; i ++)
    {
        wp_list[i].NO = i;
        wp_list[i].next = &wp_list[i + 1];
    }
    wp_list[NR_WP - 1].next = NULL;

    head = NULL;
    free_ = wp_list;
}

/* TODO: Implement the functionality of watchpoint */

WP *get_WP_list()
{
    return head;
}

void add_WP(const char * expression, uint32_t old_value)
{
    if (head)
    {
        WP *tmp = head;
        while (tmp->next)
            tmp = tmp->next;
        tmp->next = malloc(sizeof(WP));
        tmp->next->NO = tmp->NO+1;
        tmp->next->expression = malloc(strlen(expression));
        strcpy(tmp->next->expression, expression);
        tmp->next->old_value = old_value;
        tmp->next->next = NULL;
    }
    else
    {
        head = malloc(sizeof(WP));
        head->NO = 0;
        head->expression = malloc(strlen(expression));
        strcpy(head->expression, expression);
        head->old_value = old_value;
        head->next = NULL;
    }
}

int del_WP(int n)
{
    int succ=0;
    WP *tmp1 = head;
    if (tmp1)
    {
        WP *tmp2 = tmp1->next;
        if (tmp1->NO == n)
        {
            free(tmp1->expression);
            free(tmp1);
            head = tmp2;
            succ = 1;
        }
        else
        {
            for (; tmp2; tmp1=tmp1->next, tmp2=tmp2->next)
                if (tmp2->NO == n)
                {
                    tmp1->next = tmp2->next;
                    free(tmp2->expression);
                    free(tmp2);
                    succ = 1;
                }
        }
    }
    return succ;
}

int check_WP()
{
    int check = 0;
    WP *tmp = head;
    for (; tmp; tmp = tmp->next)
    {
        uint32_t new_value = cal_str(tmp->expression);
        if (new_value != tmp->old_value)
        {
            printf("watchpoint %d: %s\n", tmp->NO, tmp->expression);
            printf("Old value = 0x%02x\n", tmp->old_value);
            printf("New value = 0x%02x\n", new_value);
            tmp->old_value = new_value;
            check = 1;
        }
    }
    return check;
}
