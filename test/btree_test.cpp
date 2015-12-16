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

    //for (int i = 200; i < 300; i++) {
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

//    for (int i = 50; i < 100; i++) {
//        if(i==59){
//            std::cout<<"here"<<std::endl;
//        }
//        if(i==85){
//            std::cout<<"here"<<std::endl;
//        }
//		insert_status = t.insert(i, i - 1);
//		if (!insert_status) {
//			std::stringstream ss;
//			ss << " insert " << i << " error";
//			BOOST_FAIL(ss.str());
//		}
//		BOOST_CHECK_EQUAL(t.find(i), i - 1);
//        inserted.insert(i);
//	}

//    for (int i = 30; i < 40; i++) {
//        insert_status = t.insert(i,i-1);
//        if (!insert_status) {
//            std::stringstream ss;
//            ss << " insert " << i << " error";
//            BOOST_FAIL(ss.str());
//        }
//		BOOST_CHECK_EQUAL(t.find(i), i - 1);
//        inserted.insert(i);
//    }

//    for (int i = 300; i < 400; i++) {
//		insert_status = t.insert(i, i - 1);
//        if (!insert_status) {
//            std::stringstream ss;
//            ss << " insert " << i << " error";
//            BOOST_FAIL(ss.str());
//        }
//		BOOST_CHECK_EQUAL(t.find(i), i - 1);
//        inserted.insert(i);
//    }

    for(auto i: inserted){
		auto res=t.find(i);
		if (res != (i - 1)) {
			std::stringstream ss;
			ss << " read " << i << " error";
            BOOST_FAIL(ss.str());
		}
    }

	/*auto start_node_w=t.find_node(-11);
	auto stop_node_w = t.find_node(300);
	auto start_node = start_node_w.lock();
	auto stop_node = stop_node_w.lock();
	while (start_node != stop_node) {
		for (size_t i = 0; i < start_node->vals.size(); i++) {
			std::cout << " " << start_node->vals[i];
		}
		start_node = start_node->next.lock();
	}*/
	
	trees::BTree<int, int> t2(10);
	clock_t read_t0 = clock();
	for (int i = 0; i < 100000; i++) {
		t2.insert(i,i);
	}
	clock_t read_t1 = clock();
	std::cout<<"=> :" << " time: " << ((float)read_t1 - read_t0) / CLOCKS_PER_SEC<<std::endl;
}
