#!/usr/bin/env php
<?php

function myrandom() {
    static $rnd_state = 1;
    $rnd_state = ($rnd_state * 1103515245 + 12345) & 0x7FFFFFFF;
    return $rnd_state;
}

$items = array();
$it = ord('a');
while ($it <= ord('z')) {
    $items []= chr($it);
    $it++;
}

function item() {
    global $items;
    return $items[myrandom() % count($items)];
}

function width() {
    return myrandom() % 9 + 1;
}

function generate($depth) {
    if ($depth == 0) {
        return item();
    } else {
        $result = array();
        $width = width();
        for ($i = 0; $i < $width; $i++) {
            $it = item();
            $result[$it] = generate($depth - 1);
        }
        return $result;
    }
}

function tcount($tree) {
    global $items;
    $result = array();
    foreach ($items as $item) {
        $result[$item] = 0;
    }
    count_accum($tree, $result);
    return $result;
}

function count_accum($tree, &$accum) {
    if (!is_array($tree)) {
        $accum[$tree]++;
    } else {
        foreach ($tree as $k => $v) {
            $accum[$k]++;
            count_accum($v, $accum);
        }
    }
}

$depth = $argv[1];

$tree = generate($depth);

if ($depth < 4) {
    print_r($tree);
}

$counts = tcount($tree);
print_r($counts);
