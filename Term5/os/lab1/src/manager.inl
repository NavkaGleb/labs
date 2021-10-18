#include <chrono>

#include <sys/wait.h>
#include <sys/poll.h>

namespace os_lab1 {

template <typename T>
Manager<T>::Manager(const ComputationFunction<T>& f, const ComputationFunction<T>& g)
  : is_running_(true) {

  f_.instance = f;
  g_.instance = g;

  f_.pipe.Open();
  g_.pipe.Open();
}

template <typename T>
Manager<T>::~Manager() {
  f_.pipe.Close();
  g_.pipe.Close();
}

template <typename T>
void Manager<T>::Run() {
  while (true) {
    T x;

    std::cout << "Enter x: ";
    std::cin >> x;

    keyboard_.SwitchToNonblockingInput();

    for (std::size_t i = 0; i < kSoftFailAttemptCount; ++i) {
      if (!is_running_) {
        return;
      }

      std::cout << "\nAttempt: " << i << std::endl;

      if (f_.result.status == ComputationFunctionStatus::kUnknown) {
        f_.process_id = this_process::SpawnChild([&] {
          f_.pipe.WriteSingle(f_.instance(x));
          return 0;
        });
      }

      if (g_.result.status == ComputationFunctionStatus::kUnknown) {
        g_.process_id = this_process::SpawnChild([&] {
          g_.pipe.WriteSingle(g_.instance(x));
          return 0;
        });
      }

      poll_ = NonblockingReadPoll({
        .pipes                = { f_.pipe, g_.pipe },
        .pipe_listen_callback = GetPollPipeListenCallback(),
        .listen_callback      = GetPollListenCallback()
      });

      poll_.Listen();
    }

    std::cout << "Maximum attempts for 'soft fail' has been reached" << std::endl;

    switch (Confirm("Please confirm performing of next computation (30s) [y/n]", 30)) {
      case UserResponse::kYes: {
        continue;
      }
      case UserResponse::kNo: {
        return;
      }
      case UserResponse::kTimeout: {
        std::cout << "Next computation is not confirmed within 30s, closing..." << std::endl;
        return;
      }
    }

    keyboard_.SwitchToBlockingInput();
  }
}

template <typename T>
NonblockingReadPoll::PipeListenCallback Manager<T>::GetPollPipeListenCallback() {
  return [&](const UnnamedPipe& pipe) {
    if (pipe == f_.pipe) {
      f_.pipe.ReadSingle(f_.result);
    }

    if (pipe == g_.pipe) {
      g_.pipe.ReadSingle(g_.result);
    }

    if (f_.result.status != ComputationFunctionStatus::kUnknown && g_.result.status != ComputationFunctionStatus::kUnknown) {
      if (f_.result.status == ComputationFunctionStatus::kHardFail || g_.result.status == ComputationFunctionStatus::kHardFail) {
        std::cout << "F: " << f_.result.status << std::endl;
        std::cout << "G: " << g_.result.status << std::endl;

        poll_.StopListen();
        StopRunning();

        return;
      }

      if (f_.result.status == ComputationFunctionStatus::kValue && g_.result.status == ComputationFunctionStatus::kValue) {
        std::cout << "F:      " << f_.result.status << " -> " << *f_.result.value << std::endl;
        std::cout << "G:      " << g_.result.status << " -> " << *g_.result.value << std::endl;
        std::cout << "Result: " << *f_.result.value + *g_.result.value << std::endl;

        poll_.StopListen();
        StopRunning();

        return;
      }

      if (f_.result.status == ComputationFunctionStatus::kSoftFail && g_.result.status == ComputationFunctionStatus::kSoftFail) {
        std::cout << "F: " << f_.result.status << std::endl;
        std::cout << "G: " << g_.result.status << std::endl;

        f_.result.status = ComputationFunctionStatus::kUnknown;
        g_.result.status = ComputationFunctionStatus::kUnknown;

        poll_.StopListen();

        return;
      }

      if (f_.result.status == ComputationFunctionStatus::kSoftFail) {
        std::cout << "F: " << f_.result.status << std::endl;

        f_.result.status = ComputationFunctionStatus::kUnknown;
      }

      if (g_.result.status == ComputationFunctionStatus::kSoftFail) {
        std::cout << "G: " << g_.result.status << std::endl;

        g_.result.status = ComputationFunctionStatus::kUnknown;
      }

      poll_.StopListen();
    }
  };
}

template <typename T>
NonblockingReadPoll::ListenCallback Manager<T>::GetPollListenCallback() {
  return [&] {
    if (keyboard_.IsPressed() && keyboard_.GetLastCharacter() == 27) {
      switch (Confirm("Please confirm that computation should be stopped (5s) [y/n]", 5)) {
        case UserResponse::kYes: {
          if (f_.result.status == ComputationFunctionStatus::kHardFail || g_.result.status == ComputationFunctionStatus::kHardFail) {
            std::cout << "Overridden by system" << std::endl;

            std::cout << "F: " << f_.result.status << std::endl;
            std::cout << "G: " << g_.result.status << std::endl;

            poll_.StopListen();
            StopRunning();

            break;
          }

          if (f_.result.status == ComputationFunctionStatus::kValue && g_.result.status == ComputationFunctionStatus::kValue) {
            std::cout << "F:      " << f_.result.status << " -> " << *f_.result.value << std::endl;
            std::cout << "G:      " << g_.result.status << " -> " << *g_.result.value << std::endl;
            std::cout << "Result: " << *f_.result.value + *g_.result.value << std::endl;

            poll_.StopListen();
            StopRunning();

            break;
          }

          std::cout << "Cancellation..." << std::endl;

          if (f_.result.status == ComputationFunctionStatus::kUnknown) {
            std::cout << "F don't finish" << std::endl;
            process::Kill(f_.process_id, 1);
          }

          if (g_.result.status == ComputationFunctionStatus::kUnknown) {
            std::cout << "G don't finish. " << std::endl;
            process::Kill(g_.process_id, 1);
          }

          std::cout << "F: " << f_.result.status << std::endl;
          std::cout << "G: " << g_.result.status << std::endl;

          poll_.StopListen();
          StopRunning();

          break;
        }

        case UserResponse::kNo: {
          std::cout << "Action was not confirmed" << std::endl;
          break;
        }

        case UserResponse::kTimeout: {
          std::cout << "Action is not confirmed within 5s. Proceeding..." << std::endl;
          break;
        }
      }
    }
  };
}

template <typename T>
typename Manager<T>::UserResponse Manager<T>::Confirm(const std::string& message, std::uint32_t seconds) {
  std::cout << message << std::endl;

  auto start = std::chrono::steady_clock::now();

  while (std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - start).count() <= seconds) {
    if (keyboard_.IsPressed()) {
      auto response = keyboard_.GetLastCharacter();

      if (response == 'y') {
        return UserResponse::kYes;
      }

      if (response == 'n') {
        return UserResponse::kNo;
      }
    }
  }

  return UserResponse::kTimeout;
}

template <typename T>
void Manager<T>::StopRunning() {
  is_running_ = false;
}

} // namespace os_lab1
