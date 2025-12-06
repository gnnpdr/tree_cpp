//нужно проверить базовые функции - создание узла, создвние дерева, заполнение дерева (и вывод) без балансировки
#pragma once

#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

class TestException : public std::runtime_error
{
public:
    TestException(const std::string& test_name, const std::string& message)
        : std::runtime_error("Test '" + test_name + "' failed: " + message) {}
};

#define THROW_TEST_FAIL(message) throw TestException(__func__, message)

#define ASSERT(condition, message) if (!(condition)) THROW_TEST_FAIL(message)

class ITester
{
public:
    virtual ~ITester() = default;
    virtual void run_all_tests() = 0;
    virtual void print_results() const = 0;
    
protected:
    struct TestResult
    {
        std::string name;
        bool passed;
        std::string error_message;
    };
    
    std::vector<TestResult> test_results_;
    
    template<typename TestFunc>
    void run_test(const std::string& test_name, TestFunc test_func)
    {
        try
        {
            test_func();
            test_results_.push_back({test_name, true, ""});
            std::cout << "good " << test_name << std::endl;
        }
        catch (const TestException& e)
        {
            test_results_.push_back({test_name, false, e.what()});
            std::cout << "problem " << test_name << ": " << e.what() << std::endl;
        }
        catch (const std::exception& e)
        {
            test_results_.push_back({test_name, false, std::string("Unexpected: ") + e.what()});
            std::cout << "problem " << test_name << ": UNEXPECTED: " << e.what() << std::endl;
        }
    }
};