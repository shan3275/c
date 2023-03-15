#ifndef LIST_H
#define LIST_H

struct list_node
{
    struct list_node *head;
    struct list_node *next;                          /**< point to next node. */
    struct list_node *prev;                          /**< point to prev node. */
};
typedef struct list_node list_t;                  /**< Type for lists. */


/**
 * @brief initialize a list
 *
 * @param l list to be initialized
 */
static __inline void list_init(list_t *l)
{
    l->head = l->next = l->prev = l;
}

/**
 * @brief insert a node after a list
 *
 * @param l list to insert it
 * @param n new node to be inserted
 */
static __inline void list_insert_after(list_t *l, list_t *n)
{
    l->next->prev = n;
    n->next = l->next;

    l->next = n;
    n->prev = l;

    n->head = l->head;
}

/**
 * @brief insert a node before a list
 *
 * @param n new node to be inserted
 * @param l list to insert it
 */
static __inline void list_insert_before(list_t *l, list_t *n)
{
    l->prev->next = n;
    n->prev = l->prev;

    l->prev = n;
    n->next = l;

    n->head = l->head;
}

/**
 * @brief remove node from list.
 * @param n the node to remove from the list.
 */
static __inline void list_remove(list_t *n)
{
    n->next->prev = n->prev;
    n->prev->next = n->next;

    n->head = n->next = n->prev = n;
}

/**
 * @brief tests whether a list is empty
 * @param l the list to test.
 */
static __inline int list_isempty(const list_t *l)
{
    return (l->next == l);
}

static __inline int list_contains_node(list_t *l, list_t *n)
{
    return (n->head == l->head);
}

static __inline int list_ishead(list_t *n)
{
    return (n->head == n);
}
/**
 * @brief get the struct for this entry
 * @param node the entry point
 * @param type the type of structure
 * @param member the name of list in structure
 */
#define list_entry(node, type, member) \
    ((type *)((char *)(node) - (unsigned long)(&((type *)0)->member)))

/**
 * list_for_each_entry  -   iterate over list of given type
 * @pos:    the type * to use as a loop cursor.
 * @head:   the head for your list.
 * @member: the name of the list_struct within the struct.
 */
#define list_for_each_entry(pos, head, member)              \
    for (pos = list_entry((head)->next, typeof(*pos), member);  \
         &pos->member != (head);    \
         pos = list_entry(pos->member.next, typeof(*pos), member))

/**
 * list_first_entry - get the first element from a list
 * @ptr:    the list head to take the element from.
 * @type:   the type of the struct this is embedded in.
 * @member: the name of the list_struct within the struct.
 *
 * Note, that list is expected to be not empty.
 */
#define list_first_entry(ptr, type, member) \
    list_entry((ptr)->next, type, member)

#endif
