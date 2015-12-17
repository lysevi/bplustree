#include <btree.h>
#include <ctime>
#include <sstream>

int main(int argc, char*argv[]) {
	std::cout << "usage: " << argv[0] << " [insertion_count] [max N]" << std::endl;
	size_t insertion_count = 100000;
	size_t max_N = 10;
	if (argc > 1) {
		std::istringstream(argv[1]) >> insertion_count;
	}

	if (argc > 2) {
		std::istringstream(argv[2]) >> max_N;
	}
	std::cout << "insertion: " << insertion_count<<std::endl;
	for (size_t N = 3; N < max_N; N++) {
		std::cout << "N=" << N;

		trees::BTree<size_t, size_t> t2(N);
		clock_t read_t0 = clock();
		for (size_t i = 0; i < insertion_count; i++) {
			t2.insert(i, i);
		}
		clock_t read_t1 = clock();
		std::cout << " :" << " time: " << ((float)read_t1 - read_t0) / CLOCKS_PER_SEC << std::endl;
	}
	
	/*trees::BTree<size_t, size_t> t(3);
	clock_t read_t0 = clock();
	for (size_t i = 0; i < insertion_count; i++) {
		t.insert(10, i);
	}

	auto start_node_w = t.find_node(0);
	auto stop_node_w = t.find_node(insertion_count*2);
	auto start_node = start_node_w.lock();
	auto stop_node = stop_node_w.lock();
	while (start_node != stop_node) {
		for (size_t i = 0; i < start_node->vals.size(); i++) {
			std::cout << " " << start_node->vals[i].first;
		}
		start_node = start_node->next.lock();
	}
	std::cout << std::endl;*/

}