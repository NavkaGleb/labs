#include "DemoMode.hpp"

// private methods

void DemoMode::swap(const int& i, const int& j) {

	sf::FloatRect temp = this->array[i].getGlobalBounds();

	this->array[i].setSize(this->array[j].getGlobalBounds());
	this->array[j].setSize(temp);

	this->array[i].updateYPosition(this->wsize.y);
	this->array[j].updateYPosition(this->wsize.y);

}

void DemoMode::shuffle(const int& delayTime) {

	int rposition;

	for (int i = 0; i < this->n && !this->scancel; i++) {

		rposition = this->irand(this->mt) % (i + 1);
		this->swap(i, rposition);

		for (int k = 0; k <= i; k++)
			this->array[k].setFillColor((k == rposition) ? this->colors["orange"] : this->colors["idle"]);

		std::this_thread::sleep_for(std::chrono::milliseconds(delayTime));

	}

}

void DemoMode::almostShuffle(const int& delayTime) {

	int power = 0;
	int rposition;

	while (std::pow(2, power++) < this->n) {}

	for (int i = 0; i < this->n && !this->scancel; i += power) {

		rposition = this->irand(this->mt) % (i + 1);
		this->swap(i, rposition);

		for (int k = 0; k <= i; k++)
			this->array[k].setFillColor((k == rposition) ? this->colors["orange"] : this->colors["idle"]);

		std::this_thread::sleep_for(std::chrono::milliseconds(delayTime));

	}

}

void DemoMode::reverse(const int& delayTime) {

	for (int i = 0; i < this->n / 2 && !this->scancel; i++) {

		this->swap(i, this->n - i - 1);

		this->array[i].setFillColor(this->colors["orange"]);
		this->array[this->n - i - 1].setFillColor(this->colors["orange"]);

		if (i != 0) {

			this->array[i - 1].setFillColor(this->colors["idle"]);
			this->array[this->n - i].setFillColor(this->colors["idle"]);

		}

		std::this_thread::sleep_for(std::chrono::milliseconds(delayTime));

	}

}

void DemoMode::reset(const int& left, const int& right) {

	for (int i = left; i <= right; i++)
		this->array[i].setFillColor(this->colors["idle"]);

}

void DemoMode::bubbleSort(const int& delayTime) {

	bool swapped;

	for (int i = 0; i < this->n - 1 && !this->scancel; i++) {

		swapped = false;

		for (int j = 0; j < this->n - i - 1 && !this->scancel; j++) {

			if (this->array[j] > this->array[j + 1]) {

				this->swap(j, j + 1);
				swapped = true;

			}

			for (int k = 0; k < this->n; k++)
				this->array[k].setFillColor((k < this->n - i) ? this->colors["back"] : this->colors["idle"]);
			this->array[j + 1].setFillColor(this->colors["orange"]);

			std::this_thread::sleep_for(std::chrono::milliseconds(delayTime));

		}

		if (!swapped)
			break;

	}

}

void DemoMode::selectionSort(const int& delayTime) {

	int mindex = 0;

	for (int i = 0; i < this->n - 1 && !this->scancel; i++) {

		mindex = i;

		for (int j = i; j < this->n && !this->scancel; j++)
			if (this->array[j] < this->array[mindex])
				mindex = j;

		this->swap(i, mindex);

		for (int k = 0; k < this->n; k++)
			this->array[k].setFillColor((k <= i) ? this->colors["back"] : this->colors["idle"]);

		this->array[i].setFillColor(this->colors["orange"]);
		this->array[mindex].setFillColor(this->colors["orange"]);

		std::this_thread::sleep_for(std::chrono::milliseconds(delayTime));

	}

}

void DemoMode::insertionSort(const int& left, const int& right, const int& delayTime) {

	for (int i = left + 1; i <= right && !this->scancel; i++) {

		for (int j = i; j > left && this->array[j] < this->array[j - 1] && !this->scancel; j--) {

			this->swap(j, j - 1);

			for (int k = left; k <= right; k++)
				this->array[k].setFillColor((k <= i) ? this->colors["back"] : this->colors["idle"]);

			this->array[i].setFillColor(this->colors["orange"]);
			this->array[j - 1].setFillColor(this->colors["orange"]);

			std::this_thread::sleep_for(std::chrono::milliseconds(delayTime));

		}

	}

}

void DemoMode::shellSort(const int& delayTime) {

	int h = 1;

	while (h < this->n / 3)
		h = 3 * h + 1;

	while (h >= 1 && !this->scancel) {

		for (int i = h; i < this->n && !this->scancel; i++) {

			for (int j = i; j >= h && this->array[j] < this->array[j - h] && !this->scancel; j -= h) {

				this->swap(j, j - h);

				for (int k = 0; k < this->n; k++)
					this->array[k].setFillColor((k == j || k == j - h) ? this->colors["orange"] : this->colors["idle"]);

				std::this_thread::sleep_for(std::chrono::milliseconds(delayTime));

			}

		}

		h = h / 3;

	}

}

int DemoMode::partition(const int& left, const int& right, const int& delayTime) {

	this->swap(right, ng::algo::mof3(this->array, left, (left + right) / 2, right));

	int i = left - 1;
	int j = right;

	while (!this->scancel) {

		while (this->array[++i] < this->array[right])
			if (i == right)
				break;

		while (this->array[right] < this->array[--j])
			if (j == left)
				break;

		if (j <= i)
			break;

		this->swap(i, j);

		for (int k = left; k <= right; k++)
			this->array[k].setFillColor((k == i || k == j) ? this->colors["orange"] : this->colors["back"]);

		std::this_thread::sleep_for(std::chrono::milliseconds(delayTime));

	}

	for (int k = left; k <= right; k++)
		this->array[k].setFillColor((k == i || k == j) ? this->colors["orange"] : this->colors["back"]);

	this->swap(right, i);
	std::this_thread::sleep_for(std::chrono::milliseconds(delayTime));

	return i;

}

void DemoMode::quicksort(const int& left, const int& right, const int& cutoff,
	const int& qdelayTime, const int& idelayTime, const bool& mthread) {

	if (this->scancel)
		return;

	if (right <= left + cutoff) {

		this->insertionSort(left, right, idelayTime);
		this->reset(left, right);
		return;

	}

	int plocation = this->partition(left, right, qdelayTime);

	for (int i = left; i <= right; i++)
		this->array[i].setFillColor((i == plocation) ? this->colors["red"] : this->colors["back"]);

	if (mthread && right - left >= 20) {

		auto lpart = std::async(std::launch::async, [&]() {
			this->quicksort(left, plocation - 1, cutoff, qdelayTime, idelayTime, mthread);
			return this->reset(left, plocation);
		});
		this->quicksort(plocation + 1, right, cutoff, qdelayTime, idelayTime, mthread);
		this->reset(plocation, right);
		lpart.wait();

	} else {

		this->quicksort(left, plocation - 1, cutoff, qdelayTime, idelayTime, mthread);
		this->reset(left, plocation);
		this->quicksort(plocation + 1, right, cutoff, qdelayTime, idelayTime, mthread);

	}


}

void DemoMode::merge(const int& left, const int& middle, const int& right, const int& delayTime) {

	int n_ = right - left + 1;
	auto* carray = new ng::RectangleShape[n_];

	int i = left;
	int j = middle + 1;

	for (int k = 0; k < n_ && !this->scancel; k++) {

		if ((this->array[i] < this->array[j] && i <= middle) || j > right) {

			carray[k] = this->array[i++];
			carray[k].setXPosition(static_cast<float>(left + k) * this->width);

		} else if ((this->array[i] > this->array[j] && j <= right) || i > middle) {

			carray[k] = this->array[j++];
			carray[k].setXPosition(static_cast<float>(left + k) * this->width);

		}

		for (int t = left; t <= right; t++)
			this->array[t].setFillColor(this->colors["back"]);

		this->array[(i > middle) ? middle : i].setFillColor(this->colors["orange"]);
		this->array[(j > right) ? right : j].setFillColor(this->colors["orange"]);
		this->array[left + k].setFillColor(this->colors["red"]);

		std::this_thread::sleep_for(std::chrono::milliseconds(delayTime));

	}

	this->array[right].setFillColor(this->colors["back"]);
	this->array[middle].setFillColor(this->colors["back"]);

	for (int k = 0; k < n_ && !this->scancel; k++) {

		this->array[left + k] = carray[k];
		this->array[left + k].setFillColor(this->colors["orange"]);
		this->array[(k != 0) ? left + k - 1 : left + k].setFillColor(this->colors["idle"]);

		std::this_thread::sleep_for(std::chrono::milliseconds(3));

	}

	delete [] carray;

}

void DemoMode::mergeSort(const int& left, const int& right, const int& cutoff,
	const int& mdelayTime, const int& idelayTime, const bool& mthread) {

	if (this->scancel)
		return;

	if (right <= left + cutoff) {

		this->insertionSort(left, right, idelayTime);
		this->reset(left, right);
		return;

	}

	int middle = (left + right) / 2;

	if (mthread && right - left >= 20) {

		auto lpart = std::async(std::launch::async, [&]() { return this->mergeSort(left, middle, cutoff, mdelayTime, idelayTime, mthread); });
		this->mergeSort(middle + 1, right, cutoff, mdelayTime, idelayTime, mthread);
		lpart.wait();

	} else {

		this->mergeSort(left, middle, cutoff, mdelayTime, idelayTime, mthread);
		this->mergeSort(middle + 1, right, cutoff, mdelayTime, idelayTime, mthread);

	}

	this->merge(left, middle, right, mdelayTime);

}

void DemoMode::bumergeSort(const int& delayTime) {

	for (int size = 1; size < this->n && !this->scancel; size += size)
		for (int left = 0; left < this->n - size && !this->scancel; left += size + size)
			this->merge(left, left + size - 1, std::min(left + size + size - 1, this->n - 1), delayTime);

}

void DemoMode::rcountingSort(const int& exp, const int& delayTime) {

	int count[10] = { 0 };
	auto* result = new ng::RectangleShape[this->n];

	for (int i = 0; i < this->n && !this->scancel; i++)
		count[(static_cast<int>(this->array[i].getGlobalBounds().height * 1000) / exp) % 10]++;

	for (int i = 1; i < 10 && !this->scancel; i++)
		count[i] += count[i - 1];

	for (int i = this->n - 1; i >= 0 && !this->scancel; i--)
		result[--count[(static_cast<int>(this->array[i].getGlobalBounds().height * 1000) / exp) % 10]] = this->array[i];

	for (int i = 0; i < this->n && !this->scancel; i++) {

		this->array[i].setSize(result[i].getGlobalBounds());
		this->array[i].updateYPosition(this->wsize.y);

		for (int k = 0; k < this->n; k++)
			this->array[k].setFillColor((i == k) ? this->colors["orange"] : this->colors["idle"]);

		std::this_thread::sleep_for(std::chrono::milliseconds(delayTime));

	}

	delete [] result;

}

void DemoMode::radixSort(const int& delayTime) {

	ng::RectangleShape melement = this->array[0];

	for (int i = 0; i < this->n; i++)
		if (this->array[i] > melement)
			melement = this->array[i];

	for (int exp = 1; static_cast<int>(melement.getGlobalBounds().height * 1000) / exp > 0 && !this->scancel; exp *= 10)
		this->rcountingSort(exp, delayTime);

}

void DemoMode::initFonts() {

	if (!this->font.loadFromFile("../Fonts/Fraud.ttf"))
		throw std::invalid_argument("failed to load the font | DemoMode");

}

void DemoMode::initButtons() {

	int bamount = 16;																						// button amount
	float bwidth = 330.f;																					// button width
	float yOffset = 10.f;																					// button y offset
	float bheight = (static_cast<float>(this->wsize.y) - yOffset) / static_cast<float>(bamount) - yOffset;	// button height
	unsigned bcharacterSize = 24;																			// button character size

	this->menu->addButton("shuffle", bheight * 0 + yOffset * 1, bwidth, bheight, "shuffle - 0", bcharacterSize);
	this->menu->addButton("almost shuffle", bheight * 1 + yOffset * 2, bwidth, bheight, "almost shuffle - 1", bcharacterSize);
	this->menu->addButton("reverse", bheight * 2 + yOffset * 3, bwidth, bheight, "reverse - 2", bcharacterSize);
	this->menu->addButton("bubble sort", bheight * 3 + yOffset * 4, bwidth, bheight, "bubble sort - f1", bcharacterSize);
	this->menu->addButton("selection sort", bheight * 4 + yOffset * 5, bwidth, bheight, "selection sort - f2", bcharacterSize);
	this->menu->addButton("insertion sort", bheight * 5 + yOffset * 6, bwidth, bheight, "insertion sort - f3", bcharacterSize);
	this->menu->addButton("shell sort", bheight * 6 + yOffset * 7, bwidth, bheight, "shell sort - f4", bcharacterSize);
	this->menu->addButton("quicksort", bheight * 7 + yOffset * 8, bwidth, bheight, "quicksort - f5", bcharacterSize);
	this->menu->addButton("combined quicksort", bheight * 8 + yOffset * 9, bwidth, bheight, "combined quicksort - f6", bcharacterSize);
	this->menu->addButton("parallel quicksort", bheight * 9 + yOffset * 10, bwidth, bheight, "parallel quicksort - f7", bcharacterSize);
	this->menu->addButton("merge sort", bheight * 10 + yOffset * 11, bwidth, bheight, "merge sort - f8", bcharacterSize);
	this->menu->addButton("combined merge sort", bheight * 11 + yOffset * 12, bwidth, bheight, "combined merge sort - f9", bcharacterSize);
	this->menu->addButton("parallel merge sort", bheight * 12 + yOffset * 13, bwidth, bheight, "parallel merge sort - f10", bcharacterSize);
	this->menu->addButton("bottom-up merge sort", bheight * 13 + yOffset * 14, bwidth, bheight, "bottom-up merge sort - f11", bcharacterSize);
	this->menu->addButton("radix sort", bheight * 14 + yOffset * 15, bwidth, bheight, "radix sort - f12", bcharacterSize);

	this->menu->addButton("quit", bheight * 15 + yOffset * 16, bwidth, bheight, "quit", bcharacterSize);

}

void DemoMode::initColors() {

	this->colors["idle"] = sf::Color(255, 255, 255, 255);
	this->colors["back"] = sf::Color(218, 230, 210, 230);
	this->colors["orange"] = sf::Color(255, 146, 20, 255);
	this->colors["red"] = sf::Color(255, 82, 82, 200);
	this->colors["light green"] = sf::Color(166, 255, 181, 230);

}

void DemoMode::initArray() {

	for (int i = 0; i < this->n; i++) {

		this->array[i].setSize(
			this->width,
			(static_cast<float>(i + 1) / static_cast<float>(this->n)) * (static_cast<float>(this->wsize.y) - 10.f)
		);

		this->array[i].setPosition(
			static_cast<float>(i) * (this->width),
			static_cast<float>(this->wsize.y) - this->array[i].getGlobalBounds().height
		);

	}

}

bool DemoMode::getKtime() {

	if (this->ktime >= this->mktime) {

		this->ktime = 0.f;
		return true;

	}

	return false;

}

void DemoMode::updateDt() {

	this->dt = this->dtClock.restart().asSeconds();

}

void DemoMode::updateKtime() {

	if (this->ktime < this->mktime)
		this->ktime += 40.f * this->dt;

}

void DemoMode::updateMousePosition() {

	this->mousePosWindow = sf::Mouse::getPosition(*this->window);

}

void DemoMode::updateInput() {

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num0) && this->getKtime() && !this->sthread && this->sorted)
		this->sthread = new std::future<void>(std::async(std::launch::async, &DemoMode::shuffle, this, 5));

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num1) && this->getKtime() && !this->sthread && this->sorted)
		this->sthread = new std::future<void>(std::async(std::launch::async, &DemoMode::almostShuffle, this, 50));

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num2) && this->getKtime() && !this->sthread && this->sorted)
		this->sthread = new std::future<void>(std::async(std::launch::async, &DemoMode::reverse, this, 10));

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::F1) && this->getKtime() && !this->sthread && !this->sorted)
		this->sthread = new std::future<void>(std::async(std::launch::async, &DemoMode::bubbleSort, this, 1));

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::F2) && this->getKtime() && !this->sthread && !this->sorted)
		this->sthread = new std::future<void>(std::async(std::launch::async, &DemoMode::selectionSort, this, 40));

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::F3) && this->getKtime() && !this->sthread && !this->sorted)
		this->sthread = new std::future<void>(std::async(std::launch::async, &DemoMode::insertionSort, this, 0, this->n - 1, 1));

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::F4) && this->getKtime() && !this->sthread && !this->sorted)
		this->sthread = new std::future<void>(std::async(std::launch::async, &DemoMode::shellSort, this, 20));

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::F5) && this->getKtime() && !this->sthread && !this->sorted)
		this->sthread = new std::future<void>(std::async(std::launch::async, &DemoMode::quicksort, this, 0, this->n - 1, 0, 40, 0, false));

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::F6) && this->getKtime() && !this->sthread && !this->sorted)
		this->sthread = new std::future<void>(std::async(std::launch::async, &DemoMode::quicksort, this, 0, this->n - 1, 10, 40, 20, false));

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::F7) && this->getKtime() && !this->sthread && !this->sorted)
		this->sthread = new std::future<void>(std::async(std::launch::async, &DemoMode::quicksort, this, 0, this->n - 1, 10, 40, 20, true));

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::F8) && this->getKtime() && !this->sthread && !this->sorted)
		this->sthread = new std::future<void>(std::async(std::launch::async, &DemoMode::mergeSort, this, 0, this->n - 1, 0, 15, 0, false));

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::F9) && this->getKtime() && !this->sthread && !this->sorted)
		this->sthread = new std::future<void>(std::async(std::launch::async, &DemoMode::mergeSort, this, 0, this->n - 1, 10, 15, 20, false));

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::F10) && this->getKtime() && !this->sthread && !this->sorted)
		this->sthread = new std::future<void>(std::async(std::launch::async, &DemoMode::mergeSort, this, 0, this->n - 1, 0, 25, 0, true));

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::F11) && this->getKtime() && !this->sthread && !this->sorted)
		this->sthread = new std::future<void>(std::async(std::launch::async, &DemoMode::bumergeSort, this, 30));

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::F12) && this->getKtime() && !this->sthread && !this->sorted)
		this->sthread = new std::future<void>(std::async(std::launch::async, &DemoMode::radixSort, this, 10));

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape) && this->getKtime())
		this->paused = !this->paused;

}

void DemoMode::updateButtons() {

	if (this->menu->isButtonPressed("shuffle") && this->getKtime() && !this->sthread && this->sorted)
		this->sthread = new std::future<void>(std::async(std::launch::async, &DemoMode::shuffle, this, 5));

	if (this->menu->isButtonPressed("almost shuffle") && this->getKtime() && !this->sthread && this->sorted)
		this->sthread = new std::future<void>(std::async(std::launch::async, &DemoMode::almostShuffle, this, 50));

	if (this->menu->isButtonPressed("reverse") && this->getKtime() && !this->sthread && this->sorted)
		this->sthread = new std::future<void>(std::async(std::launch::async, &DemoMode::reverse, this, 10));

	if (this->menu->isButtonPressed("bubble sort") && this->getKtime() && !this->sthread && !this->sorted)
		this->sthread = new std::future<void>(std::async(std::launch::async, &DemoMode::bubbleSort, this, 1));

	if (this->menu->isButtonPressed("selection sort") && this->getKtime() && !this->sthread && !this->sorted)
		this->sthread = new std::future<void>(std::async(std::launch::async, &DemoMode::selectionSort, this, 40));

	if (this->menu->isButtonPressed("insertion sort") && this->getKtime() && !this->sthread && !this->sorted)
		this->sthread = new std::future<void>(std::async(std::launch::async, &DemoMode::insertionSort, this, 0, this->n - 1, 1));

	if (this->menu->isButtonPressed("shell sort") && this->getKtime() && !this->sthread && !this->sorted)
		this->sthread = new std::future<void>(std::async(std::launch::async, &DemoMode::shellSort, this, 20));

	if (this->menu->isButtonPressed("quicksort") && this->getKtime() && !this->sthread && !this->sorted)
		this->sthread = new std::future<void>(std::async(std::launch::async, &DemoMode::quicksort, this, 0, this->n - 1, 0, 40, 0, false));

	if (this->menu->isButtonPressed("combined quicksort") && this->getKtime() && !this->sthread && !this->sorted)
		this->sthread = new std::future<void>(std::async(std::launch::async, &DemoMode::quicksort, this, 0, this->n - 1, 10, 40, 20, false));

	if (this->menu->isButtonPressed("parallel quicksort") && this->getKtime() && !this->sthread && !this->sorted)
		this->sthread = new std::future<void>(std::async(std::launch::async, &DemoMode::quicksort, this, 0, this->n - 1, 0, 40, 0, true));

	if (this->menu->isButtonPressed("merge sort") && this->getKtime() && !this->sthread && !this->sorted)
		this->sthread = new std::future<void>(std::async(std::launch::async, &DemoMode::mergeSort, this, 0, this->n - 1, 0, 15, 0, false));

	if (this->menu->isButtonPressed("combined merge sort") && this->getKtime() && !this->sthread && !this->sorted)
		this->sthread = new std::future<void>(std::async(std::launch::async, &DemoMode::mergeSort, this, 0, this->n - 1, 10, 15, 20, false));

	if (this->menu->isButtonPressed("parallel merge sort") && this->getKtime() && !this->sthread && !this->sorted)
		this->sthread = new std::future<void>(std::async(std::launch::async, &DemoMode::mergeSort, this, 0, this->n - 1, 0, 25, 0, true));

	if (this->menu->isButtonPressed("bottom-up merge sort") && this->getKtime() && !this->sthread && !this->sorted)
		this->sthread = new std::future<void>(std::async(std::launch::async, &DemoMode::bumergeSort, this, 20));

	if (this->menu->isButtonPressed("radix sort") && this->getKtime() && !this->sthread && !this->sorted)
		this->sthread = new std::future<void>(std::async(std::launch::async, &DemoMode::radixSort, this, 10));

	if (this->menu->isButtonPressed("quit") && this->getKtime()) {

		this->scancel = true;
		this->quit = true;

	}

}

bool DemoMode::updateSthread() {

	if (this->sthread) {

		if (this->sthread->wait_for(std::chrono::milliseconds(0)) == std::future_status::ready) {

			this->sorted = !this->sorted;
			delete this->sthread;
			this->sthread = nullptr;
			this->reset(0, this->n - 1);

		}

	}

	return false;

}

void DemoMode::update() {

	while (this->window->pollEvent(this->event)) {

		if (this->event.type == sf::Event::Closed) {

			this->scancel = true;
			this->window->close();

		}

	}

	if (this->window->hasFocus()) {

		if (this->quit)
			this->window->close();

		this->updateKtime();
		this->updateMousePosition();
		this->updateInput();
		this->updateSthread();

		if (this->paused) {

			this->menu->update(this->mousePosWindow);
			this->updateButtons();

		}

	}

}

void DemoMode::render() {

	this->window->clear();

	for (int i = 0; i < this->n; i++)
		this->window->draw(this->array[i]);

	if (this->paused)
		this->menu->render(*this->window);

	this->window->display();

}

// constructor / destructor

DemoMode::DemoMode(unsigned width, unsigned height, int n) : event(sf::Event()), scancel(false), mt(std::clock()) {

	// init window
	this->videoMode = sf::VideoMode(width, height);
	this->window = new sf::RenderWindow(this->videoMode, "sorting");
	this->wsize.x = width;
	this->wsize.y = height;
	this->paused = false;
	this->quit = false;

	// init fonts
	this->initFonts();

	// init menu
	this->menu = new Menu(this->videoMode, 0.2, 1.f, this->font);
	this->initButtons();

	// init dt and ktime
	this->dt = 0.f;
	this->ktime = 10.f;
	this->mktime = 10.f;

	// init thread
	this->sthread = nullptr;
	this->sorted = true;

	// init column width
	this->width = static_cast<float>(this->wsize.x) / static_cast<float>(n);

	// init colors
	this->initColors();

	// init array
	this->n = n;
	this->array = new ng::RectangleShape[n];
	this->initArray();

	// init random
	this->irand = std::uniform_int_distribution<int>(0, INT_MAX);

}

DemoMode::~DemoMode() {

	// delete sorting thread
	if (this->sthread)
		this->sthread->get();
	delete this->sthread;

	// delete render window
	delete this->window;

	// delete menu
	delete this->menu;

	// delete array
	delete [] this->array;

}

// public methods

void DemoMode::run() {

	std::cout << "press ESC to open the menu" << std::endl;

	while (this->window->isOpen()) {

		this->updateDt();
		this->update();
		this->render();

	}

}