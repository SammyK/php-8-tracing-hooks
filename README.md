# Tracing hooks for PHP 8

**TL;DR: Make tracers and profilers first-class citizens by adding low-overhead hooks to the ZendEngine in PHP 8.**

## Insufficient hooks for tracers

As code telemetry and introspection becomes increasingly more important in complex distributed environments, demand for a low-overhead tracing hook into the ZendEngine is growing.

Unfortunately, PHP has no built-for-tracing hooks like [many](https://github.com/golang/go/blob/master/src/runtime/trace.go) other languages. *@TODO Add more language examples.* This leaves tracers and profilers having to hook into the ZendEngine with insufficient hooks with relatively high overhead. The hooks also have many side effects and edge cases that can lead to a negative impact on user's code. And with the JIT coming in PHP 8, most of the existing hooks will stop working for tracers altogether.

For a basic tracer to record function and method call execution times, the following hooks are currently available:

### VM Execute Hook

Extensions can override the `zend_execute_ex` and `zend_execute_internal` function pointers to wrap function calls. This has the following drawbacks for userland functions (namely `zend_execute_ex`):

- Adds extra overhead just for overriding the function *(@TODO how much?)*
- Forces the VM to add to the native stack which makes it easy to cause a stack overflow from userland
- Won't work with the JIT *(@TODO can we confirm and add why?)*

There's also the issue of the "noisy neighbor". If a neighboring extension overrides either of those function pointers and does not call the original function, the chain will be broken.

### User Opcode Handers

Extensions can add custom opcode handlers (`zend_set_user_opcode_handler()`) for function calls (`ZEND_DO_FCALL`, `ZEND_DO_FCALL_BY_NAME`, `ZEND_DO_ICALL` and `ZEND_DO_UCALL`). This has the following drawbacks:

- Adds extra overhead *(@TODO how much?)*
- The handler is run only at the start of the function call. Since tracers need to know when the function ends, this requires forwarding along the original call with something like `zend_call_function()` and causes a lot of extra work to be done to handle the VM state.
- Won't work with the JIT *(@TODO can we confirm and add why?)*

### AST injection

It is possible to inject pre & post hooks before function and method calls via `zend_ast_process` as illustrated by [a POC by @beberlei](https://github.com/beberlei/php-ast-tracer-poc) and a [a POC by @morrisonlevi & @SammyK](https://github.com/morrisonlevi/ddastrace). This has the following drawbacks:

- Adds overhead *(@TODO how much?)* since it requires an internal function call before and after every traced function/method call
- Doesn't work for internal functions

### Zend Extension

If an extension hooks into the engine as a Zend Extension, it will have access to the `fcall_begin_handler` and `fcall_end_handler` hooks. This has the following drawbacks:

- Adds overhead *(@TODO how much?)*
- Is overkill for wanting basic function/method call hooks

### Other hacks

- Injecting an object into the local symbol table and relying on `__destruct()` to know when the call is over
- *@TODO any other crazy ideas?*

## Proposed Hook

The hooks should:

- [ ] Work for userland and internal functions & methods
- [ ] Work with JIT
- [ ] Be enabled on an individual function/method level, or enabled for all functions/methods
- [ ] Be designed for production; have low overhead when used *(@TODO add target percentage?)* and zero overhead when not used
- [ ] Not require the extension to forward the hook along to other extensions

*@TODO API design*
