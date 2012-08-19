#!/usr/bin/env perl

use POSIX;
use Data::Dumper;

my $rnd_state = 1;

sub myrandom {
    $rnd_state = ($rnd_state * 1103515245 + 12345) & 0x7FFFFFFF;
    return $rnd_state;
}

my @items = ('a'..'z');
sub item {
    return $items[myrandom() % @items];
}

sub width {
    return myrandom() % 9 + 1;
}

sub generate {
    my ($depth) = @_;

    if ($depth == 0) {
        return item();
    } else {
        my $result = {};
        my $width = width();
        for (my $i = 0; $i < $width; $i++) {
            my $it = item();
            $result->{$it} = generate($depth - 1);
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

my $depth = $ARGV[0];

my $tree = generate($depth);

if ($depth < 4) {
    print Dumper($tree);
}

my $counts = count($tree);
print Dumper($counts);
