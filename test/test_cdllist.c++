#include "gtest/gtest.h"

extern "C" {
    #include "cdllist.h"
	#include "talloc.h"
}

cdllist* create_cdllist_node() {
    return (cdllist*) talloc(sizeof(cdllist));
}

cdllist* init_cdllist_head() {
    cdllist* tmp = (cdllist*) talloc(sizeof(cdllist));
    tmp->next = tmp->prev = tmp;
    return tmp;
}


TEST(cdllist, list_bad_insert) {
	cdllist* test_list = NULL;
    cdllist* a = NULL;
    
    EXPECT_EQ(insert_head(a, test_list), 1);
    EXPECT_EQ(insert_tail(a, test_list), 1);

    test_list = init_cdllist_head();
    EXPECT_EQ(insert_head(a, test_list), 1);
    EXPECT_EQ(insert_tail(a, test_list), 1);
    
    EXPECT_EQ(is_empty(test_list), 1);
    EXPECT_EQ(size(test_list), 0);
    
    tfree();
}


TEST(cdllist, list_bad_removes) {
	cdllist* test_list = NULL;
    cdllist* a = NULL;

    EXPECT_EQ(remove_from_head(test_list), nullptr);
    EXPECT_EQ(remove_from_list(a), 1);

    a = create_cdllist_node();
    EXPECT_EQ(remove_from_list(a), 1);
}

TEST(cdllist, list_insertion) {
    cdllist* test_list = init_cdllist_head();
    test_list->next = test_list;
    test_list->prev = test_list;
    EXPECT_EQ(size(test_list), 0);

    cdllist* a = create_cdllist_node();
    EXPECT_EQ(insert_head(a, test_list), 0);
    EXPECT_EQ(size(test_list), 1);
    EXPECT_EQ(test_list->next, a);
    EXPECT_EQ(test_list->prev, a);
    EXPECT_EQ(a->next, test_list);
    EXPECT_EQ(a->prev, test_list);

    cdllist* b = create_cdllist_node();
    EXPECT_EQ(insert_head(b, test_list), 0);
    EXPECT_EQ(size(test_list), 2);
    EXPECT_EQ(test_list->next, b);
    EXPECT_EQ(test_list->prev, a);
    EXPECT_EQ(b->next, a);
    EXPECT_EQ(b->prev, test_list);
    EXPECT_EQ(a->next, test_list);
    EXPECT_EQ(a->prev, b);

    cdllist *c = create_cdllist_node();
    EXPECT_EQ(insert_tail(c, test_list), 0);
    EXPECT_EQ(size(test_list), 3);
    EXPECT_EQ(test_list->next, b);
    EXPECT_EQ(test_list->prev, c);
    EXPECT_EQ(b->next, a);
    EXPECT_EQ(b->prev, test_list);
    EXPECT_EQ(a->next, c);
    EXPECT_EQ(a->prev, b);
    EXPECT_EQ(c->next, test_list);
    EXPECT_EQ(c->prev, a);
    
    tfree();
}


TEST(cdllist, remove) {
    cdllist* test_list = init_cdllist_head();
    cdllist* a = create_cdllist_node();

    insert_head(a, test_list);

    /* remove elem a to result in empty list */
    EXPECT_EQ(size(test_list), 1);
    EXPECT_EQ(remove_from_list(a), 0);
    EXPECT_EQ(size(test_list), 0);
    EXPECT_EQ(test_list->next, test_list);
    EXPECT_EQ(test_list->prev, test_list);
    
    /* remove elem a from head */
    cdllist* b = create_cdllist_node();
    insert_head(b, test_list);
    insert_head(a, test_list);
    EXPECT_EQ(size(test_list), 2);
    EXPECT_EQ(remove_from_list(a), 0);
    EXPECT_EQ(size(test_list), 1);
    EXPECT_EQ(test_list->next, b);
    EXPECT_EQ(test_list->prev, b);
    EXPECT_EQ(b->prev, test_list);
    EXPECT_EQ(b->next, test_list);

    /* remove elem a from tail */
    insert_tail(a, test_list);
    EXPECT_EQ(remove_from_list(a), 0);
    EXPECT_EQ(size(test_list), 1);

    /* remove elem a from middle */
    cdllist* c = create_cdllist_node();
    insert_tail(a, test_list);
    insert_tail(c, test_list);
    EXPECT_EQ(size(test_list), 3);
    EXPECT_EQ(remove_from_list(a), 0);
    EXPECT_EQ(size(test_list), 2);

    /* tests remove from head */ 
    EXPECT_EQ(remove_from_head(test_list), b);
    EXPECT_EQ(size(test_list), 1);
    EXPECT_EQ(test_list->next, c);
    EXPECT_EQ(test_list->prev, c);
    EXPECT_EQ(c->prev, test_list);
    EXPECT_EQ(c->next, test_list);

    tfree();
}
