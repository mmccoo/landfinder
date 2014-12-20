#!/usr/intel/pkgs/perl/5.14.1/bin/perl
use UsrIntel::R1;

use strict;
use warnings;

use FindBin;
BEGIN {
    my $path = $FindBin::Bin;
    unshift @INC, "$path/install/";
} ## end BEGIN

use Data::Dump;
use Carp;
use Time::HiRes qw( sleep  gettimeofday tv_interval);

# print Data::Dump::pp(\%INC);

#open STDOUT, '>', $FindBin::Script . ".out" or die "Can't redirect STDOUT: $!";
#open STDERR, ">&STDOUT"     or die "Can't dup STDOUT: $!";


#my $starttime = [gettimeofday];
#my $elapsed = tv_interval($starttime);


$| = 1;



use Geo::Coordinates::UTM;

my $ellipsoid = 30;
my $zone = '10T';


#Lower Left  (  490385.000, 5135485.000) 
#Upper Right (  500005.000, 5149405.000) 

my $easting =   490385.000;
my $northing = 5135485.0;
my ($latitude1,$longitude1)=utm_to_latlon($ellipsoid,$zone,$easting,$northing);

printf("lat %s long %s\n", $latitude1, $longitude1);

$easting  = 500005.000;
$northing = 5149405.0;
my ($latitude2,$longitude2)=utm_to_latlon($ellipsoid,$zone,$easting,$northing);

printf("lat %s long %s\n", $latitude2, $longitude2);


printf("%s,%s,0\n", $longitude1, $latitude1);
printf("%s,%s,0\n", $longitude2, $latitude1);
printf("%s,%s,0\n", $longitude2, $latitude2);
printf("%s,%s,0\n", $longitude1, $latitude2);
printf("%s,%s,0\n", $longitude1, $latitude1);
