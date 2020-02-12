#include <iostream>

/*
#include <cxxabi.h>
template<typename T>
void printType()
{
	int status;
	char *result = __cxxabiv1::__cxa_demangle(typeid(T).name(), nullptr, nullptr, &status);
	std::cout << std::string(result) << std::endl;
	if constexpr(is_failure_v<T>)
		std::cout << T::message() << std::endl;
	free(result);
}
 */

bool blocks_checked();
bool expressions_checked();
bool failure_cases_checked();

int main()
{
	bool success = true;
	// call external functions to make sure the test files were compiled
	success &= blocks_checked();
	success &= failure_cases_checked();
	success &= expressions_checked();

	if (success)
	{
		std::cout << "Static compile time tests were compiled successfully. Nothing to test at runtime." << std::endl;
		return EXIT_SUCCESS;
	}
	else
	{
		std::cerr << "A static compile time test reported failure at runtime. This should never happen." << std::endl;
		return EXIT_FAILURE;
	}
}
