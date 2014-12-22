#!/usr/bin/perl

use warnings;
use strict;

use Data::Dump;
use WWW::Mechanize;
use XML::LibXML;

sub get_property_accounts {
    my $filename = shift;

    my $parser = XML::LibXML->new();

    my @retval;
    print("parsing $filename\n");
    my $dom = XML::LibXML->load_html(
	location => $filename,
	# parser options ...
	load_ext_dtd => 0,
	validation => 0,
	recover=> 2,
	);

    foreach my $node ($dom->documentElement()->findnodes('//tr[@bgcolor="#E3EAEB"]')) {
	my @fields;
	foreach my $td ($node->findnodes("td")) {
	    my $val =  $td->textContent;
	    $val =~ s/[^[:ascii:]]//;
	    push(@fields, $val);
	}
    
	printf("new node %s\n", join(":", @fields));
	push(@retval, \@fields);
    }
    return @retval;
}

my @properties;
foreach my $file (@ARGV) {
    push(@properties, get_property_accounts($file));
}

my $mech = WWW::Mechanize->new();
foreach my $property (@properties) {
    my $parcel = $property->[9];
    my $account = $property->[1];
    my $filename = sprintf("cowlitzparcel/P_%s_A_%s", $parcel, $account);
    next if (-e $filename);
    my $url = sprintf('http://www.cowlitzinfo.net/applications/cowlitzassessorparcelsearch/(S(01i5cb453zhmjq45rak11l55))/ParcelDataCP.aspx?Account=%s&Public=Y&Parcel=%s', $account, $parcel);
    $mech->get($url);

    open(my $fp, ">", $filename);
    print($fp $mech->content());
    close($fp);
    exit;
}


print(Data::Dump::pp(\@properties));
print("done\n");
