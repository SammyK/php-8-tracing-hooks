#ifndef ZEND_INSTRUMENT_H
#define ZEND_INSTRUMENT_H

#include <Zend/zend.h>
#include <Zend/zend_vm_opcodes.h>

typedef void (*zend_instrument_fn)(zend_execute_data *, zval *, void *ext);

struct zend_instrument {
	zend_instrument_fn begin;
	zend_instrument_fn end;
	zend_instrument_fn exception;
	void *ext;
};
typedef struct zend_instrument zend_instrument;

struct zend_instrument_target {
	enum { ZEND_INSTRUMENT_OPCODE, ZEND_INSTRUMENT_CALL } type;
	union {
		zend_uchar opcode;
		struct {
			/* 1. Use only funcname to trace a function
			 * 2. Use both for a method
			 * 3. Use neither for all function calls
			 */
			const char *classname;
			const char *funcname;
		};
	};
};
typedef struct zend_instrument_target zend_instrument_target;

_Bool zend_instrument_register(zend_instrument_target, zend_instrument);
_Bool zend_instrument_unregister(zend_instrument_target, zend_instrument);

#endif
