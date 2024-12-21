

#include <linux/init.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/types.h>
#include <linux/list.h>
#include <linux/ktime.h>
#include <linux/slab.h>

MODULE_AUTHOR("Romanuk Kostya");
MODULE_DESCRIPTION("Lab 4");
MODULE_LICENSE("Dual BSD/GPL");

static unsigned int num_of_greet = 1; // Default meaning
module_param(num_of_greet, uint, S_IRUGO);
MODULE_PARM_DESC(num_of_greet, "Number of times Hello, World! string will be printed");

struct event_list {
	struct list_head list;
	ktime_t event_time;
};

static LIST_HEAD(event_list_head);


static int __init hello_init(void) {
	int i;
	struct event_list *element;

	if (num_of_greet == 0 || (num_of_greet >= 5 && num_of_greet <= 10)) {
		pr_warn("Number is 0 or from 5 to 10\n");
	}
	if (num_of_greet > 10) {
		BUG_ON(1);
	}
	
	for (i = 0; i < num_of_greet; i++) {
		if (i == 7) {
			element = NULL;
		} else {
			element = kmalloc(sizeof(*element), GFP_KERNEL);
		}
		BUG_ON(!element);

		element->event_time = ktime_get();
		list_add_tail(&element->list, &event_list_head);
			pr_info("Hello, World!\n");
	}
	return 0;
}

static void __exit hello_exit(void) {
	struct event_list *element, *tmp;
	list_for_each_entry_safe(element, tmp, &event_list_head, list) {
	pr_info("Time: %lld\n", element->event_time);
	list_del(&element->list);
	kfree(element);
	}
}

module_init(hello_init);
module_exit(hello_exit);
