#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE Main
#include <boost/test/unit_test.hpp>

#include <btree.h>

#include <sstream>

BOOST_AUTO_TEST_CASE(BtreeTest) {
	const size_t n = 3;
	trees::BTree t(n);
	BOOST_CHECK_EQUAL(t.find(10), 0);

    for (int i = 0; i < int(2 * n); ++i) {
		bool insert_status = t.insert(i);
		BOOST_CHECK(insert_status);
		BOOST_CHECK_EQUAL(t.find(i), i);
	}
	/*t.print();*/
	bool insert_status = t.insert(111);
	BOOST_CHECK(insert_status);
    //t.print();
	BOOST_CHECK_EQUAL(t.find(111), 111);
	//t.print();
	for (int i = 200; i < 300; i++) {
//        t.print();
        if (i == 203) {
        //	t.print();
        }
		insert_status = t.insert(i);
		//t.print();
		if (!insert_status) {
			std::stringstream ss;
			ss << " insert " << i << " error";
			BOOST_FAIL(ss.str());
		}
        if (i == 203) {
    //		t.print();
        }
		BOOST_CHECK_EQUAL(t.find(i), i);
	}


	for (int i = 50; i < 100; i++) {
//        t.print();
        if (i == 89) {
            //t.print();
        }
		insert_status = t.insert(i);
        //t.print();
		if (!insert_status) {
			std::stringstream ss;
			ss << " insert " << i << " error";
			BOOST_FAIL(ss.str());
		}
        if (i == 89) {
            //t.print();
        }
		BOOST_CHECK_EQUAL(t.find(i), i);
	}

    for (int i = 30; i < 40; i++) {
        insert_status = t.insert(i);
        if (!insert_status) {
            std::stringstream ss;
            ss << " insert " << i << " error";
            BOOST_FAIL(ss.str());
        }
        BOOST_CHECK_EQUAL(t.find(i), i);
    }

    for (int i = 300; i < 400; i++) {
        insert_status = t.insert(i);
        if (!insert_status) {
            std::stringstream ss;
            ss << " insert " << i << " error";
            BOOST_FAIL(ss.str());
        }
        BOOST_CHECK_EQUAL(t.find(i), i);
    }
}
