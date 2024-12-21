
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

void new_event(void);

static int __init hello_init(void) {
	struct event_list *element;

	if (num_of_greet == 0 || (num_of_greet >= 5 && num_of_greet <= 10)) {
		printk(KERN_WARNING "Number is 0 or from 5 to 10\n");
	}
	if (num_of_greet > 10) {
		BUG_ON(1);
	}
	int i;
	for (i = 0; i < num_of_greet; i++) {
		printk(KERN_EMERG "Hello, World!\n");
		if (i == 7) {
			element = NULL;
		}
		else {
			element = kmalloc(sizeof(struct event_list), GFP_KERNEL);
		}
		BUG_ON(!element);
		element->event_time = ktime_get();
		list_add_tail(&element->list, &event_list_head);
	}
	return 0;
}

static void __exit hello_exit(void) {
	struct event_list *md, *tmp;
	list_for_each_entry_safe(md, tmp, &event_list_head, list) {
	printk(KERN_EMERG "Time: %lld\n", md->event_time);
	list_del(&md->list);
	kfree(md);
	}
}

void new_event(void) {
	struct event_list *element = kmalloc(sizeof(struct event_list), GFP_KERNEL);
	if (!element) {
	printk(KERN_ERR "Failed to allocate memory for new event\n");
	return;
	}
	element->event_time = ktime_get();
	list_add_tail(&element->list, &event_list_head);
}

module_init(hello_init);
module_exit(hello_exit);
