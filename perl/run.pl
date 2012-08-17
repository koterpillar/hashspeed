#!/usr/bin/env perl

use POSIX;
use Data::Dumper;

my @items = ('a'..'z');
sub item {
    return $items[floor(rand() * @items)];
}

sub width {
    return floor(rand() * 10);
}

sub generate {
    my ($depth) = @_;

    if ($depth == 0) {
        return item();
    } else {
        my $result = {};
        my $width = width();
        for (my $i = 0; $i < $width; $i++) {
            $result->{item()} = generate($depth - 1);
        }
        return $result;
    }
}

my $tree = generate($ARGV[0]);


