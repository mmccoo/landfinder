#!/usr/bin/perl

use warnings;
use strict;

use WWW::Mechanize;
use Data::Dump;

$| = 1;

my $mech = WWW::Mechanize->new();

#http://www.cowlitzinfo.net/ArcGIS/rest/services/LiveMaps/AssessorMap/MapServer/export?imageSR=102100&f=image&bbox=-13680000%2C5756000%2C-13570000%2C5842000&format=png8&size=1215%2C568&transparent=true&bboxSR=102100&dpi=96

# large map bounds
my ($xl, $yl, $xh, $yh) = (-13680000, 5756000, -13570000, 5843000);

my $largeurl = sprintf('http://www.cowlitzinfo.net/ArcGIS/rest/services/LiveMaps/AssessorMap/MapServer/export?imageSR=102100&f=image&bbox=%d%%2C%d%%2C%d%%2C%d&format=png8&size=2000%%2C2000&transparent=true&bboxSR=102100&dpi=96', $xl, $yl, $xh, $yh);

$mech->get($largeurl);


print $mech->content();

