--TEST--
StringBuilder basics
--SKIPIF--
<?php
if (!extension_loaded('stringbuilder')) die('skip stringbuilder extension not available');
?>
--FILE--
<?php 
$builder = new StringBuilder();
$builder->append('hello');
$builder->append(' world');
var_dump($builder->toString());
$builder->chop(6);
$builder->append(' universe');
var_dump(
	$builder->toString(),
	$builder->length(),
	$builder->capacity()
);
?>
===DONE===
--EXPECTF--
string(11) "hello world"
string(14) "hello universe"
int(14)
int(%d)
===DONE===
