
#ifndef TESTMANAGER_H
#define TESTMANAGER_H

#include <iostream>
#include <vector>
#include <functional>
#include <chrono>
#include <iomanip>

class Test {
public:
    using TestFunc = std::function<bool()>;  // Tipo para funções de teste

    struct TestCase {
        std::string name;
        TestFunc func;
        bool expectedResult;
    };

    // Adiciona um teste à lista
    void pushTest(const std::string& name, TestFunc func, bool expectedResult) {
        testCases.push_back({name, func, expectedResult});
    }

    // Executa todos os testes
    void runTests() {
        int passed = 0;
        int failed = 0;

        for (const auto& testCase : testCases) {
            auto start = std::chrono::high_resolution_clock::now();
            bool result = testCase.func();
            auto end = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

            bool testPassed = (result == testCase.expectedResult);
            if (testPassed) {
                passed++;
            } else {
                failed++;
            }

            // Imprime o resultado do teste
            std::cout << "Test: " << testCase.name
                      << ", Expected: " << (testCase.expectedResult ? "true" : "false")
                      << ", Result: " << (result ? "true" : "false")
                      << ", Time: " << duration << " ms"
                      << ", " << (testPassed ? "Passed" : "Failed") << std::endl;
        }

        // Imprime as estatísticas finais
        std::cout << "\nTest Summary:\n";
        std::cout << "Passed: " << passed << "\n";
        std::cout << "Failed: " << failed << "\n";
    }

private:
    std::vector<TestCase> testCases;  // Lista de todos os testes
};
#endif
