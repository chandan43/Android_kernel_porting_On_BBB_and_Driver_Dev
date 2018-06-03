#include <linux/module.h>
#include <linux/moduleparam.h>

static int base_var[5];
static int irq;
static char *name=0;

module_param_array(base_var,int,NULL,0);
MODULE_PARM_DESC(base_var,"base_var, Intiger Var: min 1 and max 5");

module_param(irq,int,0644);
MODULE_PARM_DESC(irq,"Interger");

module_param(name,charp,0644);
MODULE_PARM_DESC(name,"String : Device driver name");


int init_module(void){
	int i;
	pr_info("IRQ No. %d and Device name is %s\n",irq,name);
	for(i=0;i<5;i++){
		pr_info("Base var [%d]  %d\n",i+1,base_var[i]);
	}
	return 0;
}

void cleanup_module(void){
	pr_info("IRQ No. %d and Device name is %s\n",irq,name);
	pr_info("Good Bye\n");
}

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Cjha@cavium.com");
MODULE_DESCRIPTION("Cmd line arg for module");
