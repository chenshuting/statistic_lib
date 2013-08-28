/*
 * =====================================================================================
 *
 *       Filename:  statistic_lib.c
 *
 *    Description:  Implemnt the statistic lib data structrues and functions 
 *
 *        Version:  1.0
 *        Created:  08/23/13 10:28:57
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Shuting Chen (cst), cicy0125@gmail.com
 *      Copyright:  ucas.ict.bwstor @2013
 *
 * =====================================================================================
 */

#include "statistic_lib.h"

#include <linux/module.h>
#include <linux/time.h>
#include <linux/timer.h>
#include <linux/string.h>
#include <linux/fs.h>
#include <linux/vmalloc.h>

MODULE_LICENSE("Dual BSD/GPL");

/*
 Description:
	Transfer amount to slots index
 Params:
		  amount: The amount of statistic variable which would used to classify slots.
		  policy: The policy type of calculate the slot index
			step: The step which would used to calculate slot index
	slot_number: The slot_number used to verify the boundary of classify slots.
*/
inline int amount_2_offset(unsigned int amount, policy_type_t policy, 
	int step, int slot_number)
{
	int offset = -1, number = 0;

	if (slot_number <= 0 || step == 0) {
		return offset;
	}

	switch(policy){
	case ARITHMETIC_PROGRESSION:
		offset = amount / step;
		break;
	case GEOMETRIC_PROGRESSOIN:
		number = amount / step;
		offset = 0;
		while (number) {
			offset++;
			number /= step;
		}
		break;
	case NO_PROGRESSION:
	default:
		break;
	}
	return (offset >= slot_number ? (slot_number - 1) : offset);
}


/*
 Description:
	This function used to initialize the common statistic base object
 Params:
			obj: The specific statistic obj based on common statistic structure
	slot_number: The number of slots that we would classify the amount count
	  unit_type: The unit type of count
	policy_type: The policy type of calculate the slot index
		   step: The step which would used to calculate slot index
	description: The description info used to print statistic info
*/
void init_common_statistic(void *obj, int slot_number, char * unit_type, policy_type_t policy_type,
	int step, char *description)
{
	int i = 0;
	struct common_slot_stat * stat = (struct common_slot_stat *)obj;
	struct slot_common *common = &stat->common;
	
	common->slot_number = slot_number;
	memcpy(common->unit_type, unit_type, UNIT_LEN);
	common->policy_type = policy_type;
	common->step = step;
	memcpy(common->description, description, DESCRIPTION_LEN);
	common->max_num = 0;
	common->min_num = INT_MAX;
	atomic64_set(&common->total_amount, 0);
	atomic64_set(&common->total_cnt, 0);
	common->lock = SPIN_LOCK_UNLOCKED;
	for(i = 0; i < slot_number; i++) {
		atomic64_set(&stat->slots[i], 0);
	}	
}
EXPORT_SYMBOL(init_common_statistic);

/*
 Description:
	This function used to increment the count of the slot specified by amount
 Params:
	   obj: The specific statistic obj based on common statistic structure
	amount: The amount of statistic variable which would used to classify slots.
*/
void inc_common_statistic(void *obj, unsigned long long amount)
{
	common_slot_stat_t *stat = (common_slot_stat_t *)obj;
	slot_common_t *common = &stat->common;
	unsigned long flag;
	int offset = amount_2_offset(amount, common->policy_type, 
		common->step, common->slot_number);

	if (amount > common->max_num || amount < common->min_num) {
		spin_lock_irqsave(&common->lock, flag);
		if (amount > common->max_num) {
			//update maximun number
			common->max_num = amount;
		} else if (amount < common->min_num) {
			//update minimun number
			common->min_num = amount;
		}
		spin_unlock_irqrestore(&common->lock, flag);
	}

	if (offset >= 0)
		atomic64_inc(&stat->slots[offset]);
	atomic64_inc(&common->total_cnt);
	atomic64_add(amount, &common->total_amount);
}
EXPORT_SYMBOL(inc_common_statistic);

/*
 Description:
	Print common slot statistic info
 Params:
	obj: The specific statistic obj based on common statistic structure
	buf: The buffer which is used to filled with data in obj
   buf_size: The size of the buffer
 Return:
 	len: The length of filled buffer
*/
//TODO:modify name
int fill_common_statistic(void *obj, char *buf, unsigned int buf_size)
{
	common_slot_stat_t *stat = (common_slot_stat_t *)obj;
	slot_common_t *common = &stat->common;
	int len = 0, i = 0;
	unsigned long flag, average = 0, total_cnt = 0;

	//print the description information
	len = snprintf(buf, buf_size - len, "------- %s info: -------\n", common->description);

	//print slots except the last slots
	for (i = 0; i < common->slot_number - 1; i++) {
		len += snprintf(buf + len, buf_size - len,"%d - %d %s: %ld \n", 
			i * common->step, (i + 1) * common->step, 
			common->unit_type,
			atomic64_read(&stat->slots[i]));
	}

	//print last slots
	if (i < common->slot_number) {
		len += snprintf(buf + len, buf_size - len, " >%d %s: %ld \n", 
			common->slot_number * common->step, common->unit_type,
			atomic64_read(&stat->slots[i]));
	}
	
	//print maximun, minimun and avarge number
	spin_lock_irqsave(&common->lock, flag);
	total_cnt = atomic64_read(&common->total_cnt);
	average = 0;
	if (total_cnt > 0) {
		average = atomic64_read(&common->total_amount) / total_cnt;
	}
	len += snprintf(buf + len, buf_size - len, 
		"Maximun: %u%s | Minimun: %u%s | Average: %lu%s\n",
		common->max_num, common->unit_type,
		common->min_num, common->unit_type,
		average, common->unit_type);
	spin_unlock_irqrestore(&common->lock, flag);

	len += snprintf(buf + len, buf_size - len, 
		"Total Amount:%ld | Total Count:%ld\n",
		atomic64_read(&common->total_amount),
		atomic64_read(&common->total_cnt));
	
	return len;
}
EXPORT_SYMBOL(fill_common_statistic);
