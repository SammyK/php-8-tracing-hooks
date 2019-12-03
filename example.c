#include "zend_instrument.h"

void example(void) {
	zend_instrument noop = {
		.begin = NULL,
		.end = NULL,
		.exception = NULL,
		.ext = NULL,
	};

	// instrument all function and method calls
	zend_instrument_register(
		(zend_instrument_target) {
			.type = ZEND_INSTRUMENT_ALL_CALLS,
		},
		noop);

	// instrument mysqli_connect
	zend_instrument_register(
		(zend_instrument_target) {
			.type = ZEND_INSTRUMENT_FCALL,
			.funcname = "mysqli_connect",
		},
		noop);

	// instrument SplFixedArray::toArray
	zend_instrument_register(
		(zend_instrument_target) {
			.type = ZEND_INSTRUMENT_MCALL,
			.classname = "SplFixedArray",
			.methodname = "toArray",
		},
		noop);

	// instrument abstract method
	zend_instrument_register(
		(zend_instrument_target) {
			.type = ZEND_INSTRUMENT_MCALL,
			.classname = "FilterIterator",
			.methodname = "accept",
			.instrument_inheritors = 1,
		},
		noop);
}
