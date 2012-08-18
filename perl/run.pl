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

sub count {
    my ($tree) = @_;

    my $result = {
        map { ($_ => 0) }
        @items
    };
    count_accum($tree, $result);
    return $result;
}

sub count_accum {
    my ($tree, $accum) = @_;

    if (!ref($tree)) {
        $accum->{$tree}++;
    } else {
        while (my ($k, $v) = each %{$tree}) {
            $accum->{$k}++;
            count_accum($v, $accum);
        }
    }
}

my $tree = generate($ARGV[0]);
my $counts = count($tree);
print Dumper($counts);
