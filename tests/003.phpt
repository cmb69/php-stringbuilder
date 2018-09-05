--TEST--
Convert empty StringBuffer to string
--SKIPIF--
<?php
if (!extension_loaded('stringbuilder')) die('skip stringbuilder extension not available');
?>
--FILE--
<?php
$builder = new StringBuilder;
var_dump($builder->toString());
?>
===DONE===
--EXPECT--
string(0) ""
===DONE===
