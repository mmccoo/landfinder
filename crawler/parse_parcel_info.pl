#!/usr/bin/perl

use warnings;
use strict;

use Data::Dump;
use WWW::Mechanize;
use XML::LibXML;

sub get_property_data {
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

    my %property_info;
    
    my $doc = $dom->documentElement();
    foreach my $node ($doc->findnodes("//a[\@id='ctl00_cphParcelSearch_lnkAccount']")) {
	$property_info{account} = $node->textContent();
    }
    $property_info{parcel} = get_text_value($doc, "input", name=>'ctl00$cphParcelSearch$txtParcel');
    $property_info{owner}  = get_text_value($doc, "input", name=>'ctl00$cphParcelSearch$txtOwner');
    $property_info{address} = 
	get_text_value($doc, "input", name=>'ctl00$cphParcelSearch$txtAddress1') . " " .
	get_text_value($doc, "input", name=>'ctl00$cphParcelSearch$txtAddress2');
    $property_info{situs} = get_text_value($doc, "input", id=>'ctl00_cphParcelSearch_txtSitus');
    $property_info{legaldesc} = get_text_content($doc, "textarea", id=>'ctl00_cphParcelSearch_txtLegalDescr');
   
    foreach my $node ($doc->findnodes("//table[\@id='ctl00_cphParcelSearch_tblPhotographs']//img")) {
	printf("here %s\n", $node->getAttribute("src"));
	push(@{$property_info{photos}}, $node->getAttribute("src"));
    }

    my @assessment;
    foreach my $node ($doc->findnodes("//table[\@id='ctl00_cphParcelSearch_tblAssessmentInformation']/tr[2]/td")) {
	push(@assessment, $node->textContent());
    }
    $property_info{assessment} = \@assessment;

    return \%property_info;
}

my @properties;
foreach my $file (@ARGV) {
    push(@properties, get_property_data($file));
}

print(Data::Dump::pp(\@properties));
print("done\n");


sub get_text_value {
    my $doc = shift;
    my $tag = shift;
    my $attr = shift;
    my $val = shift;
    foreach my $node ($doc->findnodes(sprintf("//%s[\@%s='%s']", $tag, $attr, $val))) {
	return $node->getAttribute("value");
    }
}
sub get_text_content {
    my $doc = shift;
    my $tag = shift;
    my $attr = shift;
    my $val = shift;
    foreach my $node ($doc->findnodes(sprintf("//%s[\@%s='%s']", $tag, $attr, $val))) {
	return $node->textContent();
    }
}
