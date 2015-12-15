#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE Main
#include <boost/test/unit_test.hpp>

#include <btree.h>

#include <sstream>
#include <set>

BOOST_AUTO_TEST_CASE(BtreeTest) {
    std::set<int> inserted;
	const size_t n = 3;
	trees::BTree t(n);
    BOOST_CHECK_EQUAL(t.find(10), 0);

    bool insert_status =false;

    for (int i = 0; i < int(2 * n); ++i) {
        insert_status = t.insert(i);
		BOOST_CHECK(insert_status);
		BOOST_CHECK_EQUAL(t.find(i), i);
        inserted.insert(i);
	}
    insert_status = t.insert(111);
	BOOST_CHECK(insert_status);
	BOOST_CHECK_EQUAL(t.find(111), 111);

    for (int i = 200; i < 300; i++) {
		insert_status = t.insert(i);
		if (!insert_status) {
			std::stringstream ss;
			ss << " insert " << i << " error";
			BOOST_FAIL(ss.str());
		}
        BOOST_CHECK_EQUAL(t.find(i), i);
        inserted.insert(i);
	}


	for (int i = 50; i < 100; i++) {
		insert_status = t.insert(i);
		if (!insert_status) {
			std::stringstream ss;
			ss << " insert " << i << " error";
			BOOST_FAIL(ss.str());
		}
		BOOST_CHECK_EQUAL(t.find(i), i);
        inserted.insert(i);
	}

    for (int i = 30; i < 40; i++) {
        insert_status = t.insert(i);
        if (!insert_status) {
            std::stringstream ss;
            ss << " insert " << i << " error";
            BOOST_FAIL(ss.str());
        }
        BOOST_CHECK_EQUAL(t.find(i), i);
        inserted.insert(i);
    }

    for (int i = 300; i < 400; i++) {
        insert_status = t.insert(i);
        if (!insert_status) {
            std::stringstream ss;
            ss << " insert " << i << " error";
            BOOST_FAIL(ss.str());
        }
        BOOST_CHECK_EQUAL(t.find(i), i);
        inserted.insert(i);
    }

    for(auto i: inserted){
		auto res=t.find(i);
		if (res != i) {
			std::stringstream ss;
			ss << " read " << i << " error";
			BOOST_FAIL(ss.str());
		}
    }
}
