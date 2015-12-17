#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE Main
#include <boost/test/unit_test.hpp>

#include <btree.h>
#include <ctime>
#include <sstream>
#include <iostream>
#include <set>

BOOST_AUTO_TEST_CASE(BtreeTest) {
    std::set<int> inserted;
	const size_t n = 3;
	trees::BTree<int,int> t(n);
    BOOST_CHECK_EQUAL(t.find(10), 0);

    bool insert_status =false;

    for (int i = 0; i < int(2 * n); ++i) {
        insert_status = t.insert(i,i-1);
		BOOST_CHECK(insert_status);
		BOOST_CHECK_EQUAL(t.find(i), i-1);
        inserted.insert(i);
	}

    insert_status = t.insert(111,110);
	BOOST_CHECK(insert_status);
	BOOST_CHECK_EQUAL(t.find(111), 110);

    for (int i = 200; i < 300; i++) {
        insert_status = t.insert(i, i - 1);
        if (!insert_status) {
            std::stringstream ss;
            ss << " insert " << i << " error";
            BOOST_FAIL(ss.str());
        }
        BOOST_CHECK_EQUAL(t.find(i), i - 1);
        inserted.insert(i);
    }

    for (int i = 50; i < 100; i++) {
		insert_status = t.insert(i, i - 1);
		if (!insert_status) {
			std::stringstream ss;
			ss << " insert " << i << " error";
			BOOST_FAIL(ss.str());
		}
		BOOST_CHECK_EQUAL(t.find(i), i - 1);
        inserted.insert(i);
	}

    for (int i = 30; i < 40; i++) {
        insert_status = t.insert(i,i-1);
        if (!insert_status) {
            std::stringstream ss;
            ss << " insert " << i << " error";
            BOOST_FAIL(ss.str());
        }
		BOOST_CHECK_EQUAL(t.find(i), i - 1);
        inserted.insert(i);
    }

    for (int i = 300; i < 400; i++) {
		insert_status = t.insert(i, i - 1);
        if (!insert_status) {
            std::stringstream ss;
            ss << " insert " << i << " error";
            BOOST_FAIL(ss.str());
        }
		BOOST_CHECK_EQUAL(t.find(i), i - 1);
        inserted.insert(i);
    }

    for(auto i: inserted){
		auto res=t.find(i);
		if (res != (i - 1)) {
			std::stringstream ss;
			ss << " read " << i << " error";
            BOOST_FAIL(ss.str());
		}
    }
}

BOOST_AUTO_TEST_CASE(QueryRange) {
	size_t insertion_count = 100;
	trees::BTree<size_t, size_t> t(3);

	for (size_t i = 0; i < insertion_count; i++) {
		t.insert(i, i);
	}

	auto start_node_w = t.find_node(0);
	auto stop_node_w = t.find_node(insertion_count * 2);
	auto start_node = start_node_w.lock();
	auto stop_node = stop_node_w.lock();

	size_t prev_key{};
	bool first = true;
	while (start_node != stop_node) {
		for (size_t i = 0; i < start_node->vals.size(); i++) {
			auto current_key = start_node->vals[i].first;
			if (first) {
				prev_key = current_key;
				first = false;
			} else {
				BOOST_CHECK_EQUAL(current_key - 1, prev_key);
				prev_key = current_key;
			}
		}
		start_node = start_node->next.lock();
	}
}

BOOST_AUTO_TEST_CASE(ArrayFuncs) {
    const size_t test_count=2;
    int t1[test_count][5]=
    {
        {1,3,4,5,0},
        {1,3,4,5,0}
    };
    int t2[test_count][5]=
    {
        {1,2,3,4,5},
        {1,2,3,4,5}
    };
    for(size_t test=0;test<test_count;test++){
        trees::insert_to_array(t1[test], 5 ,4,2);
        for(size_t i=0;i<5;i++){
            if(t1[test][i]!=t2[test][i]){
                std::stringstream ss{};
                ss<<" test fail: #"<<test;
                BOOST_FAIL(ss.str());
            }
        }
    }
}

//BOOST_AUTO_TEST_CASE(OneKeyManyValues) {
//	size_t insertion_count = 100;
//	trees::BTree<size_t, size_t> t(3);
//	clock_t read_t0 = clock();
//	for (size_t i = 0; i < insertion_count; i++) {
//		t.insert(10, i);
//	}
//
//	auto start_node_w = t.find_node(0);
//	auto stop_node_w = t.find_node(insertion_count * 2);
//	auto start_node = start_node_w.lock();
//	auto stop_node = stop_node_w.lock();
//	while (start_node != stop_node) {
//		for (size_t i = 0; i < start_node->vals.size(); i++) {
//			insertion_count--;
//		}
//		start_node = start_node->next.lock();
//	}
//	BOOST_CHECK_EQUAL(insertion_count, 0);
//}
