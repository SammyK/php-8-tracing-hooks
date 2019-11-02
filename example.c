#include "zend_instrument.h"

void example(void) {
	zend_instrument noop = {
		.begin = NULL,
		.end = NULL,
		.exception = NULL,
		.ext = NULL,
	};

	// instrument SplFixedArray::toArray
	zend_instrument_register(
		(zend_instrument_target) {
			.type = ZEND_INSTRUMENT_CALL,
			.classname = "SplFixedArray",
			.funcname = "toArray",
		},
		noop);

	// instrument require/include/eval
	zend_instrument_register(
		(zend_instrument_target) {
			.type = ZEND_INSTRUMENT_OPCODE,
			.opcode = ZEND_INCLUDE_OR_EVAL
		},
		noop);

	// instrument PDOStatement::execute
	zend_instrument_register(
		(zend_instrument_target) {
			.type = ZEND_INSTRUMENT_CALL,
			.classname = "SplFixedArray",
			.funcname = "toArray",
		},
		(zend_instrument) {
			.begin = NULL,
			.end = NULL,
			.exception = NULL,
			.ext = NULL,
		});
}
