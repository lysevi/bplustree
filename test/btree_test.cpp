#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE Main
#include <boost/test/unit_test.hpp>

#include <btree.h>

BOOST_AUTO_TEST_CASE(BtreeTest) {
	trees::BTree t(3);
	BOOST_CHECK_EQUAL(t.find(10), 0);

	bool insert_status=t.insert(1);
	BOOST_CHECK(insert_status);
	BOOST_CHECK_EQUAL(t.find(1), 1);
}