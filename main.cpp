#include <iostream>
#include "include/doctest_proxy.h"
#include "GLFW/glfw3.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "implot.h"

int basic_test() {
    return 4;
}

int main(int argc, char **argv) {
	doctest::Context context;

	// !!! THIS IS JUST AN EXAMPLE SHOWING HOW DEFAULTS/OVERRIDES ARE SET !!!

	// defaults
	context.addFilter("test-case-exclude", "*math*"); // exclude test cases with "math" in their name
	context.setOption("abort-after", 5);              // stop test execution after 5 failed assertions
	context.setOption("order-by", "name");            // sort the test cases by their name

	context.applyCommandLine(argc, argv);

	// overrides
	context.setOption("no-breaks", true);             // don't break in the debugger when assertions fail

	int res = context.run(); // run

	if(context.shouldExit()) // important - query flags (and --exit) rely on the user doing this
		return res;          // propagate the result of the tests

	int client_stuff_return_code = 0;
	// your program - if the testing framework is integrated in your production code
	std::cout << "Hello, World!" << std::endl;
	client_stuff_return_code = 0;

	return res + client_stuff_return_code; // the result from doctest is propagated here as well
}
TEST_CASE("example doctest") {
    CHECK(basic_test() == 4);
}