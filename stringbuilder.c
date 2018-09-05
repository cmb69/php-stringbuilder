/*
   +----------------------------------------------------------------------+
   | PHP Version 7                                                        |
   +----------------------------------------------------------------------+
   | Copyright (c) 2018 The PHP Group                                     |
   +----------------------------------------------------------------------+
   | This source file is subject to version 3.01 of the PHP license,      |
   | that is bundled with this package in the file LICENSE, and is        |
   | available through the world-wide-web at the following url:           |
   | http://www.php.net/license/3_01.txt                                  |
   | If you did not receive a copy of the PHP license and are unable to   |
   | obtain it through the world-wide-web, please send a note to          |
   | license@php.net so we can mail you a copy immediately.               |
   +----------------------------------------------------------------------+
   | Authors: Christoph M. Becker <cmb@php.net>                           |
   +----------------------------------------------------------------------+
*/

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "php.h"
#include "ext/standard/info.h"
#include "Zend/zend_smart_str.h"
#include "php_stringbuilder.h"

static zend_class_entry *string_builder_ce;
static zend_object_handlers string_builder_object_handlers;

typedef struct _string_builder_object {
	smart_str str;
	zend_object std;
} string_builder_object;

static inline string_builder_object *string_builder_from_obj(zend_object *obj) {
	return (string_builder_object*)((char*)(obj) - XtOffsetOf(string_builder_object, std));
}

#define SB_OBJ(zv) string_builder_from_obj(Z_OBJ_P((zv)))

/* {{{ proto StringBuilder::__construct([string $str])
   Initializes a StringBuilder instance. */
PHP_METHOD(StringBuilder, __construct)
{
	zval *object = getThis();
	string_builder_object *sb_obj = SB_OBJ(object);
	zend_string *str = NULL;

	ZEND_PARSE_PARAMETERS_START(0, 1)
		Z_PARAM_OPTIONAL
		Z_PARAM_STR(str)
	ZEND_PARSE_PARAMETERS_END();

	if (str) {
		smart_str_append(&sb_obj->str, str);
	} else {
		smart_str_appends(&sb_obj->str, "");
	}
}
/* }}} */

/* {{{ proto void StringBuilder::append(string $str)
   Appends the specified string to this character sequence. */
/* TODO: overload */
PHP_METHOD(StringBuilder, append)
{
	zval *object = getThis();
	string_builder_object *sb_obj = SB_OBJ(object);
	zend_string *str;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_STR(str)
	ZEND_PARSE_PARAMETERS_END();

	smart_str_append(&sb_obj->str, str);
}
/* }}} */

/* {{{ proto int StringBuilder::capacity()
   Returns the current capacity. */
PHP_METHOD(StringBuilder, capacity)
{
	zval *object = getThis();
	string_builder_object *sb_obj = SB_OBJ(object);

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}

	RETURN_LONG(sb_obj->str.a);
}
/* }}} */

/* {{{ proto void StringBuilder::chop(int $len)
   Shortens the string by len bytes. */
PHP_METHOD(StringBuilder, chop)
{
	zval *object = getThis();
	string_builder_object *sb_obj = SB_OBJ(object);
	zend_long len;

	ZEND_PARSE_PARAMETERS_START(1, 1)
		Z_PARAM_LONG(len)
	ZEND_PARSE_PARAMETERS_END();

	if (len <= 0 || (size_t) len > smart_str_get_len(&sb_obj->str)) {
		php_error_docref(NULL, E_WARNING, "failed to chop");
		RETURN_FALSE;
	}

	ZSTR_LEN(sb_obj->str.s) -= len;
}
/* }}} */

/* {{{ proto int StringBuilder::length()
   Returns the length (character count). */
PHP_METHOD(StringBuilder, length)
{
	zval *object = getThis();
	string_builder_object *sb_obj = SB_OBJ(object);

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}

	RETURN_LONG(smart_str_get_len(&sb_obj->str));
}
/* }}} */

/* {{{ proto string StringBuilder::toString()
   Returns a string representing the data in this sequence. */
PHP_METHOD(StringBuilder, toString)
{
	zval *object = getThis();
	string_builder_object *sb_obj = SB_OBJ(object);

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}

	/* TODO: do we need to zend_string_dup() here? */
	RETURN_STR(zend_string_copy(sb_obj->str.s));
}
/* }}} */

/* {{{ arginfo
 */
ZEND_BEGIN_ARG_INFO_EX(arginfo_stringbuilder_construct, 0, 0, 0)
	ZEND_ARG_INFO(0, str)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_stringbuilder_append, 0)
	ZEND_ARG_INFO(0, str)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_stringbuilder_capacity, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_stringbuilder_chop, 0)
	ZEND_ARG_INFO(0, len)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_stringbuilder_length, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_stringbuilder_toString, 0)
ZEND_END_ARG_INFO()
/* }}} */

/* {{{ string_builder_class_methods */
static const zend_function_entry string_builder_class_methods[] = {
	PHP_ME(StringBuilder,	__construct,	arginfo_stringbuilder_construct,	ZEND_ACC_PUBLIC)
	PHP_ME(StringBuilder,	append,			arginfo_stringbuilder_append,		ZEND_ACC_PUBLIC)
	PHP_ME(StringBuilder,	capacity,		arginfo_stringbuilder_capacity,		ZEND_ACC_PUBLIC)
	PHP_ME(StringBuilder,	chop,			arginfo_stringbuilder_chop,			ZEND_ACC_PUBLIC)
	PHP_ME(StringBuilder,	length,			arginfo_stringbuilder_length,		ZEND_ACC_PUBLIC)
	PHP_ME(StringBuilder,	toString,		arginfo_stringbuilder_toString,		ZEND_ACC_PUBLIC)
	PHP_FE_END
};
/* }}} */

static zend_object *string_builder_create_object(zend_class_entry *ce) /* {{{ */
{
	string_builder_object *intern = emalloc(sizeof(string_builder_object) + zend_object_properties_size(ce));

	zend_object_std_init(&intern->std, ce);
	object_properties_init(&intern->std, ce);

	intern->str.s = NULL;
	intern->str.a = 0;
	intern->std.handlers = &string_builder_object_handlers;

	return &intern->std;
}
/* }}} */

static void string_builder_object_free_storage(zend_object *object) /* {{{ */
{
	string_builder_object *intern = string_builder_from_obj(object);

	smart_str_free(&intern->str);
	zend_object_std_dtor(&intern->std);
}
/* }}} */

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(stringbuilder)
{
	zend_class_entry tmp_ce;
	INIT_CLASS_ENTRY(tmp_ce, "StringBuilder", string_builder_class_methods);
	string_builder_ce = zend_register_internal_class(&tmp_ce);
	string_builder_ce->create_object = string_builder_create_object;
	memcpy(&string_builder_object_handlers, &std_object_handlers, sizeof(zend_object_handlers));
	string_builder_object_handlers.offset = XtOffsetOf(string_builder_object, std);
	string_builder_object_handlers.free_obj = string_builder_object_free_storage;

	return SUCCESS;
}
/* }}} */

/* {{{ PHP_RINIT_FUNCTION
 */
PHP_RINIT_FUNCTION(stringbuilder)
{
#if defined(ZTS) && defined(COMPILE_DL_STRINGBUILDER)
	ZEND_TSRMLS_CACHE_UPDATE();
#endif

	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(stringbuilder)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "stringbuilder support", "enabled");
	php_info_print_table_end();
}
/* }}} */

/* {{{ stringbuilder_functions[]
 */
static const zend_function_entry stringbuilder_functions[] = {
	PHP_FE_END
};
/* }}} */

/* {{{ stringbuilder_module_entry
 */
zend_module_entry stringbuilder_module_entry = {
	STANDARD_MODULE_HEADER,
	"stringbuilder",				/* Extension name */
	stringbuilder_functions,		/* zend_function_entry */
	PHP_MINIT(stringbuilder),		/* PHP_MINIT - Module initialization */
	NULL,							/* PHP_MSHUTDOWN - Module shutdown */
	PHP_RINIT(stringbuilder),		/* PHP_RINIT - Request initialization */
	NULL,							/* PHP_RSHUTDOWN - Request shutdown */
	PHP_MINFO(stringbuilder),		/* PHP_MINFO - Module info */
	PHP_STRINGBUILDER_VERSION,		/* Version */
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_STRINGBUILDER
# ifdef ZTS
ZEND_TSRMLS_CACHE_DEFINE()
# endif
ZEND_GET_MODULE(stringbuilder)
#endif
