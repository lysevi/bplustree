#include <btree.h>
#include <ctime>
#include <sstream>

int main(int argc, char*argv[]) {
	std::cout << "usage: " << argv[0] << " [insertion_count] [max N]"<<std::endl;
	size_t insertion_count = 1000000;
	size_t max_N = 10;
	if (argc > 1) {
		std::istringstream(argv[1]) >> insertion_count;
	}

	if (argc > 2) {
		std::istringstream(argv[2]) >> max_N;
	}

	for (size_t N = 3; N < max_N; N++) {
		std::cout << "N=" << N;

		trees::BTree<int, int> t2(N);
		clock_t read_t0 = clock();
		for (size_t i = 0; i < insertion_count; i++) {
			t2.insert(i, i);
		}
		clock_t read_t1 = clock();
		std::cout << " :" << " time: " << ((float)read_t1 - read_t0) / CLOCKS_PER_SEC << std::endl;
	}
}