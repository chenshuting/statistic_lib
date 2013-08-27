#statistic_lib
=============

>It's a linux kernel module, and the main function of this module is to help you statistic your count in your module.

## 1.Design and Implement your own data strucrure
>#define SLOT_LEN 10
>typedef struct my_common_stat {
>  struct slot_common common;
>  atomic64_t slots[SLOT_LEN];
>}my_common_stat_t;
>If you just want to use the basic function of this module, you can just use common_slot_stat_t to define your varialbe.

##2. Dfine your variable
>struct a {
>  ……
>  my_common_stat_t my_variable;
>};

##3. initialize the variable
>init_common_statistic(&my_variable, SLOT_LEN, "ms", ARITHMETIC_PROGRESSION, 200, "read bio");

##4. use to increment count
>inc_common_statistic((void *)&my_variable, amount);

#5. fill buffer to print or other use
>print_common_statistic((void *)&asd->finish, buf, buf_size);

#6. Modify Makefile
>#Add include head file
>EXTRA_CFLAGS += -I$(PWD)/stat_lib 
>#Add to include EXPORT_SYMBOL
>KBUILD_EXTRA_SYMBOLS += $(PWD)/stat_lib/Module.symvers 
>add to complile statistic module
>make -c stat_lib 
>#add to install statistic module
>make -c stat_lib install

##Note:
>  You just need to add this module to your code path, compile and install this module fisrtly.
  
