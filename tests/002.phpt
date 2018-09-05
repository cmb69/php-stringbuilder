--TEST--
StringBuilder construction with argument
--SKIPIF--
<?php
if (!extension_loaded('stringbuilder')) die('skip stringbuilder extension not available');
?>
--FILE--
<?php
$builder = new StringBuilder('a string');
var_dump($builder->toString());
?>
===DONE===
--EXPECT--
string(8) "a string"
===DONE===
