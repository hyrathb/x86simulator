#ifndef __WATCHPOINT_H__
#define __WATCHPOINT_H__

#include "common.h"

typedef struct watchpoint {
	int NO;
	struct watchpoint *next;
	/* TODO: Add more members if necessary */
	char * expression;
	uint32_t old_value;

} WP;

WP *get_WP_list();
void add_WP(const char * expression, uint32_t old_value);
int del_WP(int n);
int check_WP();
#endif
