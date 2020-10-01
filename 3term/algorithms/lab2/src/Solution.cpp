#include "Solution.hpp"

#include <random>

#include "Random.hpp"
#include "Timer.hpp"

namespace ng {

    // constructor / destructor
    Solution::Solution() : time(0.0) {
        this->Init();
        this->Shuffle();
    }

    void Solution::Solve(bool print) {
        if (print)
            this->Print("init");

        {
            Timer timer(this->time);
            this->Sort(0, static_cast<int>(this->bolts.size() - 1));
        }

        if (print)
            this->Print("result");

        this->PrintTime();
    }

    // private methods
    void Solution::Init() {
        int n;

        std::cin >> n;
        this->bolts.resize(n);
        this->nuts.resize(n);

        for (int i = 0; i < this->bolts.size(); ++i) {
            this->bolts[i] = i;
            this->nuts[i] = i;
        }
    }

    void Solution::Shuffle() {
        for (int i = 0; i < this->bolts.size(); ++i) {
            std::swap(this->bolts[i], this->bolts[ng::random::irand(0, i)]);
            std::swap(this->nuts[i], this->nuts[ng::random::irand(0, i)]);
        }
    }

    int Solution::Partition(int left, int right) {
        int pivot = (left + right) / 2;
        int current = left - 1;
        int shadow = -1;

        for (int i = left; i <= right; ++i) {
            if (this->nuts[i] < this->bolts[pivot])
                std::swap(this->nuts[++current], this->nuts[i]);

            if (this->nuts[i] == this->bolts[pivot])
                shadow = i;
        }

        std::swap(this->nuts[current + 1], this->nuts[shadow]);
        std::swap(this->bolts[current + 1], this->bolts[pivot]);

        pivot = current + 1;
        current = left - 1;

        for (int i = left; i <= right; ++i)
            if (this->bolts[i] < this->nuts[pivot])
                std::swap(this->bolts[++current], this->bolts[i]);

        return current + 1;
    }

    void Solution::Sort(int left, int right) {
        if (left >= right)
            return;

        int pivot = this->Partition(left, right);

        Sort(left, pivot - 1);
        Sort(pivot + 1, right);
    }

    void Solution::Print(const char* title) const {
        if (title)
            std::cout << title << ":" << std::endl;

        std::cout << "bolts: [ ";
        for (const auto& bolt : this->bolts)
            std::cout << bolt << " ";
        std::cout << "]" << std::endl;

        std::cout << "nuts: [ ";
        for (const auto& nut : this->nuts)
            std::cout << nut << " ";
        std::cout << "]" <<  std::endl;
    }

    void Solution::PrintTime() const {
        std::cout << "time = " << this->time << " sec" << std::endl;
    }

}