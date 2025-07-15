#include <iostream>
#include <queue>
#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <atomic>
#include <chrono>

std::mutex outputMutex;  // For clean output

// Task with priority (1 = highest)
struct Task {
    int priority;
    std::function<void()> job;

    Task(int p, std::function<void()> j) : priority(p), job(j) {}

    // Lower number = higher priority
    bool operator<(const Task& other) const {
        return priority > other.priority;
    }
};

class TaskScheduler {
private:
    std::priority_queue<Task> taskQueue;
    std::mutex mtx;
    std::condition_variable cv;
    std::vector<std::thread> workers;
    std::atomic<bool> stop{false};

public:
    TaskScheduler(int numThreads) {
        for (int i = 0; i < numThreads; ++i) {
            workers.emplace_back([this, i]() {
                while (true) {
                    Task task(0, [] {});
                    {
                        std::unique_lock<std::mutex> lock(mtx);
                        cv.wait(lock, [this]() {
                            return !taskQueue.empty() || stop;
                        });

                        if (stop && taskQueue.empty()) return;

                        task = taskQueue.top();
                        taskQueue.pop();
                    }

                    {
                        std::lock_guard<std::mutex> lock(outputMutex);
                        std::cout << "[Thread " << i + 1 << "] Running Task with Priority " << task.priority << "\n";
                    }

                    task.job();

                    {
                        std::lock_guard<std::mutex> lock(outputMutex);
                        std::cout << "[Thread " << i + 1 << "] Finished Task " << task.priority << "\n";
                    }
                }
            });
        }
    }

    void addTask(int priority, const std::function<void()>& job) {
        {
            std::lock_guard<std::mutex> lock(mtx);
            taskQueue.emplace(priority, job);
        }
        cv.notify_one();
    }

    void shutdown() {
        {
            std::lock_guard<std::mutex> lock(mtx);
            stop = true;
        }
        cv.notify_all();

        for (auto& t : workers)
            if (t.joinable())
                t.join();
    }

    ~TaskScheduler() {
        shutdown();
    }
};

int main() {
    {
        std::lock_guard<std::mutex> lock(outputMutex);
        std::cout << "=== Simple Task Scheduler ===\n";
    }

    TaskScheduler scheduler(3);

    // Add tasks (priority 1 = highest)
    scheduler.addTask(1, [] {
        std::lock_guard<std::mutex> lock(outputMutex);
        std::cout << "   >> [Task 1] Urgent task complete\n";
    });

    scheduler.addTask(2, [] {
        std::lock_guard<std::mutex> lock(outputMutex);
        std::cout << "   >> [Task 2] Normal task complete\n";
    });

    scheduler.addTask(3, [] {
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        std::lock_guard<std::mutex> lock(outputMutex);
        std::cout << "   >> [Task 3] Background task complete\n";
    });

    std::this_thread::sleep_for(std::chrono::seconds(2));
    scheduler.shutdown();

    {
        std::lock_guard<std::mutex> lock(outputMutex);
        std::cout << "=== All Tasks Completed ===\n";
    }

    return 0;
}
