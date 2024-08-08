#include <pthread.h>

#include <csignal>

#include <condition_variable>
#include <thread>

#include <boost/optional.hpp>
#include <boost/optional/optional_io.hpp>

#define BOOST_TEST_MODULE test module test_signal_manager
#include <boost/test/included/unit_test.hpp>

#include <viam/sdk/module/signal_manager.hpp>

struct signal_waiter {
    std::condition_variable cv;
    std::mutex m;
    std::thread t;

    bool started_waiting = false;
    int result = -1;

    void run(viam::sdk::SignalManager mgr) {
        t = std::thread{[&] {
            std::unique_lock<std::mutex> lk(m);
            started_waiting = true;
            cv.notify_one();
            lk.unlock();

            result = mgr.wait();
        }};
    }

    void block_on_setup() {
        std::unique_lock<std::mutex> lk(m);
        cv.wait(lk, [this] { return started_waiting; });
    }

    int kill_thread(int sig) {
        return pthread_kill(t.native_handle(), sig);
    }
};

namespace viam {
namespace sdktests {
namespace signal_manager {

BOOST_AUTO_TEST_SUITE(test_signal_manager)

BOOST_AUTO_TEST_CASE(test_default_manager) {
    for (int sig : {SIGINT, SIGTERM}) {
        signal_waiter waiter;

        waiter.run(sdk::SignalManager());
        waiter.block_on_setup();

        BOOST_CHECK_EQUAL(waiter.kill_thread(sig), 0);
        waiter.t.join();

        BOOST_CHECK_EQUAL(waiter.result, 0);
    }
}

BOOST_AUTO_TEST_CASE(test_install_nonfatal) {
    int usr1_count = 0;

    signal_waiter waiter;

    sdk::SignalManager::signal_map sig_map;
    sig_map.sigusr1_handler = [&usr1_count, &waiter](int) {
        std::unique_lock<std::mutex> lk(waiter.m);
        waiter.started_waiting = false;
        waiter.cv.notify_one();

        return ++usr1_count;
    };

    waiter.run(sdk::SignalManager(std::move(sig_map)));

    waiter.block_on_setup();

    auto count_usr1 = [&](int iteration) {
        BOOST_CHECK_EQUAL(waiter.kill_thread(SIGUSR1), 0);

        // make sure that the handler actually gets to run before we spam more signals
        {
            std::unique_lock<std::mutex> lk(waiter.m);
            waiter.cv.wait(lk, [&waiter] { return !waiter.started_waiting; });
        }
        BOOST_CHECK_EQUAL(usr1_count, iteration);
        waiter.started_waiting = true;
    };

    for (int iteration : {1, 2}) {
        count_usr1(iteration);
    }

    BOOST_CHECK_EQUAL(pthread_kill(waiter.t.native_handle(), SIGTERM), 0);
    waiter.t.join();
    BOOST_CHECK_EQUAL(waiter.result, 0);
}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace signal_manager
}  // namespace sdktests
}  // namespace viam
