#include "modes/InteractiveMode.h"

int main() {

	const std::string mpath = "../files/monsters.txt";
	const std::string mbpath = "../files/monsters.bin";
	const std::string lpath = "../files/locations.txt";

	DataBase database(mpath, mbpath, lpath);

	char response = 'y';
	short mode;

	// main loop
	while (response == 'y') {

		DataBase::print(database.getModes());
		std::cin >> mode;

		if (mode == 0) {

			InteractiveMode interactiveMode(database);
			interactiveMode.run();

		} else if (mode == 1) {

			DemoMode demoMode(database, 3000);
			demoMode.run();

		} else if (mode == 2) {

			Benchmark benchmark(database, 10);
			benchmark.run();

		}

		// response input
		std::cout << '\n' << "do you want to select another mode? press 'y' or 'q'. 'y' - yes, 'q' - quit" << '\n';
		std::cin >> response;

		// check response
		while (response != 'y' && response != 'q') {

			std::cout << "please, press 'y' or 'q'!" << '\n';
			std::cin >> response;

		}

	}

	return 0;

}