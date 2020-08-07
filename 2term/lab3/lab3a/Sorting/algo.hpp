#pragma once

#include <iostream>
#include <algorithm>
#include <random>
#include <ctime>
#include <climits>
#include <functional>
#include <future>
#include <map>

typedef unsigned int uint;
typedef int(*pfunction)(int* array, const int& left, const int& right);		// partition function

namespace ng {

	class algo {
	private:
		/* -------------- merge sort -------------- */
		template <typename T>
		static void merge(T* array, const int& left, const int& middle, const int& right);

		template <typename T, typename ComparableFunction>
		static void merge(T* array, const int& left, const int& middle, const int& right, ComparableFunction comparableFunction);

		/* -------------- counting sort for radix sort -------------- */

		template <typename T>
		static T getMax(T* array, const int& n);

		template <typename T, typename Getter>
		static T getMax(T* array, const int& n, Getter getter);

		template <typename T>
		static void rcountingSort(T* array, const int& n, const int& exp);			// radix counting sort

		template <typename T, typename Getter>
		static void rcountingSort(T* array, const int& n, const int& exp, Getter getter);

	public:
		/* -------------- median of three */
		template <typename T>
		static int mof3(T* array, const int& left, const int& middle, const int& right);

		template <typename T, typename ComparableFunction>
		static int mof3(T* array, const int& left, const int& middle, const int& right, ComparableFunction comparableFunction);

		/* -------------- reverse -------------- */
		template <typename T>
		static void reverse(T* array, const int& n);

		/* -------------- shuffle -------------- */
		template <typename T>
		static void shuffle(T* array, const int& n);

		template <typename T>
		static void almostShuffle(T* array, const int& n);

		/* -------------- bubble sort -------------- */
		template <typename T>
		static void bubbleSort(T* array, const int& n);

		/* -------------- selection sort -------------- */
		template <typename T>
		static void selectionSort(T* array, const int& n);

		/* -------------- insertion sort -------------- */
		template <typename T>
		static void insertionSort(T* array, const int& left, const int& right);

		/* -------------- shellSort -------------- */
		template <typename T>
		static void shellSort(T* array, const int& n);

		/* -------------- counting sort -------------- */
		template <typename T>
		static void countingSort(T* array, const int& n);

		template <typename FType, typename T, typename Getter>											// FType = field type
		static void countingSort(T* array, const int& n, Getter getter);

		/* -------------- radix sort -------------- */
		template <typename T>
		static void radixSort(T* array, const int& n);

		template <typename T, typename Getter>
		static void radixSort(T* array, const int& n, Getter getter);

		/* -------------- quicksort -------------- */
		template <typename T>
		static int llpartition(T* array, const int& left, const int& right);			// Lomuto left partition

		template <typename T>
		static int lrpartition(T* array, const int& left, const int& right);			// Lomuto right partition

		template <typename T>
		static int hlpartition(T* array, const int& left, const int& right);			// Hoare left partition

		template <typename T>
		static int hrpartition(T* array, const int& left, const int& right);			// Hoare right partition

		template <typename T>
		static int lmof3partition(T* array, const int& left, const int& right);			// Lomuto median of 3 partition

		template <typename T>
		static int hmof3partition(T* array, const int& left, const int& right);			// Hoare median of 3 partition

		template <typename RandomIterator>
		static RandomIterator partition(RandomIterator left, RandomIterator right);

		template <typename T, typename ComparableFunction>
		static int partition(T* array, const int& left, const int& right, ComparableFunction comparableFunction);

		template <typename T>
		static void quicksort(T* array, const int& left, const int& right, pfunction partition = hmof3partition);

		template <typename T, typename ComparableFunction>
		static void quicksort(T* array, const int& left, const int& right, ComparableFunction comparableFunction);

		template <typename RandomIterator, int cutoff = 4096>
		void static pquicksort(RandomIterator left, RandomIterator right);				// parallel quicksort

		template <typename T>
		static void cquicksort(T* array, const int& left, const int& right, const int& cutoff, pfunction = algo::llpartition);

		/* -------------- merge sort -------------- */
		template <typename T>
		static void mergeSort(T* array, const int& left, const int& right);

		template <typename T, typename ComparableFunction>
		static void mergeSort(T* array, const int& left, const int& right, ComparableFunction comparableFunction);

		template <typename T>
		static void cmergeSort(T* array, const int& left, const int& right, const int& cutoff);

		template <typename T, int cutoff = 4096>
		static void pmergeSort(T* array, const int& left, const int& right);		// parallel mergeSort

		template <typename T>
		static void buMergeSort(T* array, const int& n);

	};

	// private functions
	template <typename T>
	void algo::merge(T* array, const int& left, const int& middle, const int& right) {

		int n = right - left + 1;
		auto* carray = new T[n];

		int i = left;
		int j = middle + 1;

		for (int k = 0; k < n; k++) {

			if (j > right || (array[i] < array[j] && i <= middle))
				carray[k] = array[i++];
			else if (i > middle || (array[i] > array[j] && j <= right))
				carray[k] = array[j++];

		}

		for (int k = 0; k < n; k++)
			array[left + k] = carray[k];

		delete [] carray;

	}

	template <typename T, typename ComparableFunction>
	void algo::merge(T* array, const int& left, const int& middle, const int& right, ComparableFunction comparableFunction) {

		int n = right - left + 1;
		auto* carray = new T[n];

		int i = left;
		int j = middle + 1;

		for (int k = 0; k < n; k++) {

			if ((j > right || (comparableFunction(array[i], array[j]) && i <= middle)))
				carray[k] = array[i++];
			else if (i > middle || (!comparableFunction(array[i], array[j]) && j <= right))
				carray[k] = array[j++];

		}

		for (int k = 0; k < n; k++)
			array[left + k] = carray[k];

		delete [] carray;

	}

	template <typename T>
	T algo::getMax(T* array, const int& n) {

		T melement = array[0];

		for (int i = 1; i < n; i++)
			if (array[i] > melement)
				melement = array[i];

		return melement;

	}

	template <typename T, typename Getter>
	T algo::getMax(T* array, const int& n, Getter getter) {

		T melement = array[0];

		for (int i = 1; i < n; i++)
			if (getter(array[i]) > getter(melement))
				melement = array[i];

		return melement;

	}

	template <typename T>
	void algo::rcountingSort(T* array, const int& n, const int& exp) {

		int count[10] = { 0 };
		auto* result = new T[n];

		for (int i = 0; i < n; i++)
			count[(array[i] / exp) % 10]++;

		for (int i = 1; i < 10; i++)
			count[i] += count[i - 1];

		for (int i = n - 1; i >= 0; i--)
			result[--count[(array[i] / exp) % 10]] = array[i];

		for (int i = 0; i < n; i++)
			array[i] = result[i];

		delete [] result;

	}

	template <typename T, typename Getter>
	void algo::rcountingSort(T* array, const int& n, const int& exp, Getter getter) {

		int count[10] = { 0 };
		auto* result = new T[n];

		for (int i = 0; i < n; i++)
			count[(getter(array[i]) / exp) % 10]++;

		for (int i = 1; i < 10; i++)
			count[i] += count [i - 1];

		for (int i = n - 1; i >= 0; i--)
			result[--count[(getter(array[i]) / exp) % 10]] = array[i];

		for (int i = 0; i < n; i++)
			array[i] = result[i];

		delete [] result;

	}

	// public functions
	template <typename T>
	int algo::mof3(T* array, const int& left, const int& middle, const int& right) {

		if (array[left] > array[middle]) {

			if (array[middle] > array[right])
				return middle;

			if (array[left] > array[right])
				return right;

			return left;

		} else {

			if (array[left] > array[right])
				return left;

			if (array[middle] > array[right])
				return right;

			return middle;

		}

	}

	template <typename T, typename ComparableFunction>
	int algo::mof3(T* array, const int& left, const int& middle, const int& right, ComparableFunction comparableFunction) {

		if (comparableFunction(array[left], array[middle])) {

			if (comparableFunction(array[middle], array[right]))
				return middle;

			if (comparableFunction(array[left], array[right]))
				return right;

			return left;

		} else {

			if (comparableFunction(array[left], array[right]))
				return left;

			if (comparableFunction(array[middle], array[right]))
				return right;

			return middle;

		}

	}

	template <typename T>
	void algo::reverse(T* array, const int& n) {

		for (int i = 0; i < n / 2; i++)
			std::swap(array[i], array[n - i - 1]);

	}

	template <typename T>
	void algo::shuffle(T* array, const int& n) {

		static std::mt19937 mt(std::clock());
		static std::uniform_int_distribution<int> irand(0, INT_MAX);

		for (int i = 0; i < n; i++)
			std::swap(array[i], array[irand(mt) % (i + 1)]);

	}

	template <typename T>
	void algo::almostShuffle(T* array, const int& n) {

		static std::mt19937 mt(std::clock());
		static std::uniform_int_distribution<int> irand(0, INT_MAX);

		int power = 0;

		while (std::pow(2, power++) < n) {}

		for (int i = 0; i < n; i += power)
			std::swap(array[i], array[irand(mt) % (i + 1)]);

	}

	template <typename T>
	void algo::bubbleSort(T* array, const int& n) {

		/* optimized bubbleSort */

		bool swapped;

		for (int i = 0; i < n - 1; i++) {

			swapped = false;

			for (int j = 0; j < n - i - 1; j++) {

				if (array[j] > array[j + 1]) {

					std::swap(array[j], array[j + 1]);
					swapped = true;

				}

			}

			if (!swapped)
				break;

		}

	}

	template <typename T>
	void algo::selectionSort(T* array, const int& n) {

		/* basic selectionSort */

		int mindex = 0;																// min index

		for (int i = 0; i < n - 1; i++) {

			mindex = i;

			for (int j = i; j < n; j++)
				if (array[j] < array[mindex])
					mindex = j;

			std::swap(array[i], array[mindex]);

		}

	}

	template <typename T>
	void algo::insertionSort(T* array, const int& left, const int& right) {

		/* basic insertionSort from left to right */

		for (int i = left; i <= right; i++)
			for (int j = i; j > left && array[j] < array[j - 1]; j--)
				std::swap(array[j], array[j - 1]);

	}

	template <typename T>
	void algo::shellSort(T* array, const int& n) {

		/* basic shellSort */

		int h = 1;

		while (h < n / 3)
			h = 3 * h + 1;

		while (h >= 1) {

			for (int i = h; i < n; i++)
				for (int j = i; j >= h && array[j] < array[j - h]; j -= h)
					std::swap(array[j], array[j - h]);

			h = h / 3;

		}

	}

	template <typename T>
	void algo::countingSort(T* array, const int& n) {

		int j = 0;																				// for iteration array
		T melement = array[0];																	// max element

		for (int i = 1; i < n; i++)
			if (array[i] > melement)
				melement = array[i];

		auto* count = new T[melement + 1]();

		for (int i = 0; i < n; i++)
			count[array[i]]++;

		for (int i = 0; i < melement + 1; i++)
			while (count[i] != 0)
				array[j++] = i, count[i]--;

		delete [] count;

	}

	template <typename FType, typename T, typename Getter>
	void algo::countingSort(T* array, const int& n, Getter getter) {

		int j = 0;																				// for iteration array
		std::map<FType, std::vector<T>> count;

		for (int i = 0; i < n; i++)
			count[getter(array[i])].emplace_back(array[i]);

		for (auto& p : count)
			for (int i = 0; i < p.second.size(); i++)
				array[j++] = p.second[i];

	}

	template <typename T>
	void algo::radixSort(T* array, const int& n) {

		T melement = algo::getMax(array, n);

		for (int exp = 1; melement / exp > 0; exp *= 10)
			algo::rcountingSort(array, n, exp);

	}

	template <typename T, typename Getter>
	void algo::radixSort(T* array, const int& n, Getter getter) {

		T melement = algo::getMax(array, n, getter);

		for (int exp = 1; getter(melement) / exp > 0; exp *= 10)
			algo::rcountingSort(array, n, exp, getter);

	}

	template <typename T>
	int algo::llpartition(T* array, const int& left, const int& right) {

		int lwall = left;															// left wall

		for (int i = left + 1; i <= right; i++)
			if (array[i] < array[left])
				std::swap(array[i], array[++lwall]);

		std::swap(array[lwall], array[left]);
		return lwall;

	}

	template <typename T>
	int algo::lrpartition(T* array, const int& left, const int& right) {

		int lwall = left;															// left wall

		for (int i = left; i < right; i++)
			if (array[i] < array[right])
				std::swap(array[i], array[lwall++]);

		std::swap(array[lwall], array[right]);

		return lwall;

	}

	template <typename T>
	int algo::hlpartition(T* array, const int& left, const int& right) {

		int i = left;
		int j = right + 1;

		while (true) {

			while (array[++i] < array[left])
				if (i == right)
					break;

			while (array[left] < array[--j])
				if (j == left)
					break;

			if (j <= i)
				break;

			std::swap(array[i], array[j]);

		}

		std::swap(array[left], array[j]);

		return j;

	}

	template <typename T>
	int algo::hrpartition(T* array, const int& left, const int& right) {

		int i = left - 1;
		int j = right;

		while (true) {

			while (array[++i] < array[right])
				if (i == right)
					break;

			while (array[right] < array[--j])
				if (j == left)
					break;

			if (j <= i)
				break;

			std::swap(array[i], array[j]);

		}

		std::swap(array[right], array[i]);

		return i;

	}

	template <typename T>
	int algo::lmof3partition(T* array, const int& left, const int& right) {

		std::swap(array[left], array[algo::mof3(array, left, (left + right) / 2, right)]);
		return algo::llpartition(array, left, right);

	}

	template <typename T>
	int algo::hmof3partition(T* array, const int& left, const int& right) {

		std::swap(array[left], array[algo::mof3(array, left, (left + right) / 2, right)]);
		return algo::hlpartition(array, left, right);

	}

	template <typename RandomIterator>
	RandomIterator algo::partition(RandomIterator left, RandomIterator right) {

		RandomIterator i = left;
		RandomIterator j = right + 1;

		while (true) {

			while (*(++i) < *left)
				if (i == right) break;

			while (*left < *(--j))
				if (j == left) break;

			if (j <= i) break;

			std::swap(*i, *j);

		}

		std::swap(*left, *j);

		return j;

	}

	template <typename T, typename ComparableFunction>
	int algo::partition(T* array, const int& left, const int& right, ComparableFunction comparableFunction) {

		std::swap(array[left], array[algo::mof3(array, left, (left + right) / 2, right, comparableFunction)]);

		int i = left;
		int j = right + 1;

		while (true) {

			while (comparableFunction(array[++i], array[left]))
				if (i == right)
					break;

			while (comparableFunction(array[left], array[--j]))
				if (j == left)
					break;

			if (j <= i)
				break;

			std::swap(array[i], array[j]);

		}

		std::swap(array[left], array[j]);

		return j;

	}

	template <typename T>
	void algo::quicksort(T* array, const int& left, const int& right, pfunction partition) {

		/* basic quicksort */

		if (right <= left)
			return;

		int plocation = partition(array, left, right);							// pivot location

		algo::quicksort(array, left, plocation - 1, partition);
		algo::quicksort(array, plocation + 1, right, partition);

	}

	template <typename T, typename ComparableFunction>
	void algo::quicksort(T* array, const int& left, const int& right, ComparableFunction comparableFunction) {

		/* basic quicksort using comparable function */

		if (right <= left)
			return;

		int plocation = partition(array, left, right, comparableFunction);

		quicksort(array, left, plocation - 1, comparableFunction);
		quicksort(array, plocation + 1, right, comparableFunction);

	}

	template <typename RandomIterator, int cutoff>
	void algo::pquicksort(RandomIterator left, RandomIterator right) {

		if (right <= left)
			return;

		RandomIterator plocation = algo::partition(left, right);

		if (right - left > cutoff) {

			auto lpart = std::async(std::launch::async, [&]() { return algo::pquicksort(left, plocation - 1); });
			algo::pquicksort(plocation + 1, right);
			lpart.wait();

		} else {

			algo::pquicksort(left, plocation - 1);
			algo::pquicksort(plocation + 1, right);

		}

	}

	template <typename T>
	void algo::cquicksort(T* array, const int& left, const int& right, const int& cutoff, pfunction partition) {

		/* combined quicksort with insertionSort */

		if (right <= left + cutoff - 1) {

			if (right == left)
				return;

			algo::insertionSort(array, left, right);
			return;

		}

		int pivot = partition(array, left, right);

		algo::cquicksort(array, left, pivot - 1, cutoff, partition);
		algo::cquicksort(array, pivot + 1, right, cutoff, partition);

	}

	template <typename T>
	void algo::mergeSort(T* array, const int& left, const int& right) {

		/* basic mergeSort */

		if (right <= left)
			return;

		int middle = (left + right) / 2;

		algo::mergeSort(array, left, middle);
		algo::mergeSort(array, middle + 1, right);

		if (array[middle] < array[middle + 1])
			return;

		algo::merge(array, left, middle, right);

	}

	template <typename T, typename ComparableFunction>
	void algo::mergeSort(T* array, const int& left, const int& right, ComparableFunction comparableFunction) {

		/* basic mergeSort using comparable function  */

		if (right <= left)
			return;

		int middle = (left + right) / 2;

		algo::mergeSort(array, left, middle, comparableFunction);
		algo::mergeSort(array, middle + 1, right, comparableFunction);

		if (comparableFunction(array[middle], array[middle + 1]))
			return;

		algo::merge(array, left, middle, right, comparableFunction);

	}

	template <typename T>
	void algo::cmergeSort(T* array, const int& left, const int& right, const int& cutoff) {

		/* combined mergeSort with insertionSort */

		if (right <= left + cutoff - 1) {

			algo::insertionSort(array, left, right);
			return;

		}

		int middle = (left + right) / 2;

		algo::cmergeSort(array, left, middle, cutoff);
		algo::cmergeSort(array, middle + 1, right, cutoff);

		if (array[middle] < array[middle + 1])
			return;

		algo::merge(array, left, middle, right);

	}

	template <typename T, int cutoff>
	void algo::pmergeSort(T* array, const int& left, const int& right) {

		if (right <= left)
			return;

		int middle = (left + right) / 2;

		if (right - left > cutoff) {

			auto lpart = std::async(std::launch::async, [&]() { return algo::pmergeSort<T, cutoff>(array, left, middle); });
			algo::pmergeSort(array, middle + 1, right);
			lpart.wait();

		} else {

			algo::pmergeSort<T, cutoff>(array, left, middle);
			algo::pmergeSort<T, cutoff>(array, middle + 1, right);

		}

		algo::merge(array, left, middle, right);

	}

	template <typename T>
	void algo::buMergeSort(T* array, const int& n) {

		/* bottom up mergeSort */

		for (int size = 1; size < n; size += size)
			for (int left = 0; left < n - size; left += size + size)
				algo::merge(array, left, left + size - 1, std::min(left + size + size - 1, n - 1));

	}

}