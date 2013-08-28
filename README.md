#statistic_lib
=============

>It's a linux kernel module, and the main function of this module is to help you statistic your count in your module.

## 1.Design and Implement your own data strucrure
<p>#define SLOT_LEN 10</p>
<ul>
<li>typedef struct my_common_stat {</li>
<li>  struct slot_common common;</li>
<li>  atomic64_t slots[SLOT_LEN];</li>
<li>}my_common_stat_t;</li>
</ul>
<p>If you just want to use the basic function of this module, you can just use common_slot_stat_t to define your varialbe.</p>


##2. Dfine your variable
<ul>
<li>struct a {</li>
<li>  ……</li>
<li>  my_common_stat_t my_variable;</li>
<li>};</li>
</ul>

##3. initialize the variable
<ul><li>init_common_statistic(&my_variable, SLOT_LEN, "ms", ARITHMETIC_PROGRESSION, 200, "read bio");</li></ul>

##4. use to increment count
<ul><li>inc_common_statistic((void *)&my_variable, amount);</li></ul>

##5. fill buffer to print or other use
<ul><li>fill_common_statistic((void *)&asd->finish, buf, buf_size);</li></ul>

##6. Modify Makefile
###Add include head file
<ul><li>EXTRA_CFLAGS += -I$(PWD)/stat_lib </li></ul>
###Add to include EXPORT_SYMBOL
<ul><li>KBUILD_EXTRA_SYMBOLS += $(PWD)/stat_lib/Module.symvers </li></ul>
###add to complile statistic module
<ul><li>make -c stat_lib </li></ul>
###add to install statistic module
<ul><li>make -c stat_lib install</li></ul>

##Note:
<p>  You just need to add this module to your code path, compile and install this module fisrtly.</p>
  
