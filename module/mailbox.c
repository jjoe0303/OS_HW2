#include "mailbox.h"

MODULE_LICENSE("Dual BSD/GPL");

static void get_process_name(char *ouput_name);
static ssize_t mailbox_read(struct kobject *kobj,
                            struct kobj_attribute *attr, char *buf);
static ssize_t mailbox_write(struct kobject *kobj,
                             struct kobj_attribute *attr, const char *buf, size_t count);

static struct kobject *hw2_kobject;
static struct kobj_attribute mailbox_attribute
    = __ATTR(mailbox, 0660, mailbox_read, mailbox_write);

static int num_entry_max = 2;

static int num_entry = 0;

module_param(num_entry_max, int, S_IRUGO);

static void get_process_name(char *ouput_name)
{
	memcpy(ouput_name, current->comm, sizeof(current->comm));
}

static ssize_t mailbox_read(struct kobject *kobj,
                            struct kobj_attribute *attr, char *buf)
{
	if(list_empty(&(MailboxHead.head))) {
		return ERR_EMPTY;
	}
	//struct mailbox_entry_t *getmail = (struct mailbox_entry_t *)kmalloc(sizeof(struct mailbox_entry_t),GFP_KERNEL);
	//getmail=MailboxHead.head.next;
	//char *message = list_entry(MailboxHead.head.next,)

	struct mailbox_entry_t *get;
//	spin_lock(&get->lock);
	get = container_of(MailboxHead.head.next,struct mailbox_entry_t,entry);
	sprintf(buf,"%s",get->message);
	num_entry=num_entry-1;
	printk("%d:Receive:%s\n",num_entry,buf);
	list_del(MailboxHead.head.next);
//	spin_unlock(&get->lock);
	return strlen(buf);
}

static ssize_t mailbox_write(struct kobject *kobj,
                             struct kobj_attribute *attr, const char *buf, size_t count)
{
	struct mailbox_entry_t *newmail;
	if(num_entry==num_entry_max) {
		printk("FULL!! loss message:%s\n",buf);
		return ERR_FULL;
	}
	newmail = (struct mailbox_entry_t *)kmalloc(sizeof(struct mailbox_entry_t),
	          GFP_KERNEL);
	sprintf((*newmail).message,"%s",buf);
	list_add(&((*newmail).entry),&(MailboxHead.head));
	num_entry=num_entry+1;
	printk("%d:message:%s\n",num_entry,(*newmail).message);
	return strlen(buf);
}

static int __init mailbox_init(void)
{
	printk("Insert\n");
	INIT_LIST_HEAD(&(MailboxHead.head));
	num_entry=0;
	hw2_kobject = kobject_create_and_add("hw2", kernel_kobj);
	sysfs_create_file(hw2_kobject, &mailbox_attribute.attr);
	return 0;
}

static void __exit mailbox_exit(void)
{
	printk("Remove\n");
	kobject_put(hw2_kobject);
}

module_init(mailbox_init);
module_exit(mailbox_exit);
