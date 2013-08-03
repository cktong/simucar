<?php
header('Content-disposition: attachment; filename=testzip.txt');
header('Content-type: application/txt');
readfile('testzip.txt');
?>