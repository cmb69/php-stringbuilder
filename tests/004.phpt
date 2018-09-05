--TEST--
Chop margins
--SKIPIF--
<?php
if (!extension_loaded('stringbuilder')) die('stringbuilder extension not available');
?>
--FILE--
<?php
$builder = new StringBuilder();
$builder->chop(0);
var_dump($builder->toString());

$builder = new StringBuilder('chop');
$builder->chop(4);
var_dump($builder->toString());

$builder = new StringBuilder('chop');
$builder->chop(5);
var_dump($builder->toString());
?>
===DONE===
--EXPECTF--
Warning: StringBuilder::chop(): failed to chop in %s on line %d
string(0) ""
string(0) ""

Warning: StringBuilder::chop(): failed to chop in %s on line %d
string(4) "chop"
===DONE===
