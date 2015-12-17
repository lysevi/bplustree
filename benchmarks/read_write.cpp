#include <btree.h>
#include <ctime>
#include <sstream>
#include <map>
struct BigData
{
	BigData(size_t i) { memset(this, (int)i, sizeof(BigData)); }
	BigData() = default;
	uint64_t id;
	uint64_t time;
	uint64_t flag;
	uint64_t value;
};

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
	std::cout << "insertion: " << insertion_count << std::endl;
	for (size_t N = 3; N < max_N; N++) {
		std::cout << "N=" << N;
		trees::BTree<size_t, BigData> t2(N);
		std::map<size_t, BigData> mt2{};
		//write
		{

			clock_t read_t0 = clock();
			for (size_t i = 0; i < insertion_count; i++) {
				t2.insert(i, BigData(i));
			}
			clock_t read_t1 = clock();


			clock_t mt2read_t0 = clock();
			for (size_t i = 0; i < insertion_count; i++) {
				mt2[i] = BigData(i);
			}
			clock_t mt2read_t1 = clock();

			std::cout << " w:" << " time: " << ((float)read_t1 - read_t0) / CLOCKS_PER_SEC
				<< " map time:" << ((float)mt2read_t1 - mt2read_t0) / CLOCKS_PER_SEC << std::endl;
		}
		//read
		{
			clock_t read_t0 = clock();
			for (size_t i = 0; i < insertion_count; i++) {
				t2.find(i);
			}
			clock_t read_t1 = clock();

			
			clock_t mt2read_t0 = clock();
			for (size_t i = 0; i < insertion_count; i++) {
				auto v = mt2[i];
			}
			clock_t mt2read_t1 = clock();

			std::cout << " r:" << " time: " << ((float)read_t1 - read_t0) / CLOCKS_PER_SEC
				<< " map time:" << ((float)mt2read_t1 - mt2read_t0) / CLOCKS_PER_SEC << std::endl;
		}
	}
}