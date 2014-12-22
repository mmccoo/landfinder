#!/usr/bin/perl

use strict;
use warnings;

use GD::Simple;

my $file = "/home/mmccoo/3.png";
my $image1 = GD::Image->newFromPng($file);
$file = "/home/mmccoo/4.png";
my $image2 = GD::Image->newFromPng($file);


my $result = new GD::Image(3600,1800);
$result->copyMerge($image1, 0,   0, 0,0, 1800, 1800, 100);
$result->copyMerge($image2, 1800,0, 0,0, 1800, 1800, 100);

print($result->png);
