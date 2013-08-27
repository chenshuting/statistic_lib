/*
 * =====================================================================================
 *
 *       Filename:  statistic_lib.h
 *
 *    Description:  Define the statistic lib data structrues and functions 
 *
 *        Version:  1.0
 *        Created:  08/23/13 10:21:12
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Shuting Chen (cst)
 *      Copyright:  ucas.ict.bwstor @2013
 *
 * =====================================================================================
 */

#ifndef _STATISTIC_LIB_H
#define _STATISTIC_LIB_H

#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/spinlock_types.h>
#include <linux/spinlock.h>
#include <asm/spinlock_types.h>
#include <asm/spinlock.h>

#define DESCRIPTION_LEN 16
#define UNIT_LEN 8 
#define KLOG_TITLE      "K"
#define STAT_NAME "STATISTIC_LIB"


#define X_ASSERT(assert)    do {  \
	if (unlikely(!(assert))) {                  \
		printk(KERN_EMERG                   \
			KLOG_TITLE": Assertion failure in %s() at %s:%d: \"%s\"\n", \
			__FUNCTION__, __FILE__, __LINE__, # assert); \
        }                               \
} while (0)

#define STATERR(f, arg...) do { \
	printk(KERN_ERR "["STAT_NAME"][%s.%d]: ", __FUNCTION__, __LINE__); \
	printk(f"\n", ## arg); \
} while (0)

enum{
	UNIT_MS = 0,
	UNIT_SECOND,
	UNIT_MINITE,
	UNIT_NUMBERS,
};

typedef enum{
	ARITHMETIC_PROGRESSION,
	GEOMETRIC_PROGRESSOIN,
}policy_type_t;

typedef struct slot_common {
	int slot_number; //the number of slots
	policy_type_t policy_type; //the policy of current type
	int step; 		//the step of slots
	unsigned int max_num; //Maximun value of slots
	unsigned int min_num; //Minimum value of slots
	spinlock_t lock; //spinlock which would protect the slot
	atomic64_t total_amount; //Record the total amount
	atomic64_t total_cnt; //Record the total cnt
	char unit_type[UNIT_LEN]; //the unit of slot
	char description[DESCRIPTION_LEN]; //description of current slot type
}slot_common_t; //size: 64 bytes

typedef struct common_slot_stat {
	struct slot_common common;	//generic descript of slots
	atomic64_t slots[0];	//slots
}common_slot_stat_t;

void init_common_statistic(void *, int, char *, policy_type_t, int , char *);
void inc_common_statistic(void *, unsigned long long);
int print_common_statistic(void *, char *, unsigned int);

#endif
