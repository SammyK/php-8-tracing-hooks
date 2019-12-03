#ifndef ZEND_INSTRUMENT_H
#define ZEND_INSTRUMENT_H

#include <Zend/zend.h>

typedef void (*zend_instrument_fn)(zend_execute_data *, zval *frame_data, void *instrument_data);

struct zend_instrument {
	zend_instrument_fn begin;
	zend_instrument_fn end;
	zend_instrument_fn exception;

	// this is for the INSTRUMENT lifetime, not FRAME lifetime
	void *instrument_data;
};
typedef struct zend_instrument zend_instrument;

struct zend_instrument_target {
	enum {
		ZEND_INSTRUMENT_ALL_CALLS,
		ZEND_INSTRUMENT_FCALL,
		ZEND_INSTRUMENT_MCALL,
	} type;
	int flags;
	union {
		const char *funcname;
		struct {
			const char *methodname;
			const char *classname;
			_Bool instrument_inheritors;
		};
	};
};
typedef struct zend_instrument_target zend_instrument_target;

_Bool zend_instrument_register(zend_instrument_target, zend_instrument);
_Bool zend_instrument_unregister(zend_instrument_target, zend_instrument);

#endif
