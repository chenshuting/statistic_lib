#statistic_lib
=============

>It's a linux kernel module, and the main function of this module is to help you statistic your count in your module.

## 1.Design and Implement your own data strucrure
<p>#define SLOT_LEN 10</p>
<li>typedef struct my_common_stat {</li>
<li>  struct slot_common common;</li>
<li>  atomic64_t slots[SLOT_LEN];</li>
<li>}my_common_stat_t;</li>
<li>If you just want to use the basic function of this module, you can just use common_slot_stat_t to define your varialbe.</li>
</ul>

##2. Dfine your variable
<li>struct a {</li>
<li>  ……</li>
<li>  my_common_stat_t my_variable;</li>
<li>};</li>

##3. initialize the variable
<li>init_common_statistic(&my_variable, SLOT_LEN, "ms", ARITHMETIC_PROGRESSION, 200, "read bio");</li>

##4. use to increment count
<li>inc_common_statistic((void *)&my_variable, amount);</li>

##5. fill buffer to print or other use
<li>print_common_statistic((void *)&asd->finish, buf, buf_size);</li>

##6. Modify Makefile
###Add include head file
<li>EXTRA_CFLAGS += -I$(PWD)/stat_lib </li>
###Add to include EXPORT_SYMBOL
<li>KBUILD_EXTRA_SYMBOLS += $(PWD)/stat_lib/Module.symvers </li>
###add to complile statistic module
<li>make -c stat_lib </li>
###add to install statistic module
<li>make -c stat_lib install</li>

##Note:
<li>  You just need to add this module to your code path, compile and install this module fisrtly.</li>
  
