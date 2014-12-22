#!/usr/bin/perl

#http://www.cowlitzinfo.net/ArcGIS/rest/services/LiveMaps/AssessorMap/MapServer/export?imageSR=102100&f=image&bboxSR=102100&bbox=-13680500%2C5803500%2C-13680000%2C5804000&format=png8&size=1200%2C1200&transparent=true&dpi=96

#http://www.cowlitzinfo.net/ArcGIS/rest/services/LiveMaps/AssessorMap/MapServer/export?imageSR=102100&f=image&bboxSR=102100&bbox=-13680000%2C5803500%2C-13679500%2C5804000&format=png8&size=1200%2C1200&transparent=true&dpi=96

# 5803750
#-13679750 5803750
# GD::Simple

#http://www.cowlitzinfo.net/ArcGIS/rest/services/LiveMaps/AssessorMap/MapServer/0/query?returnGeometry=true&spatialRel=esriSpatialRelIntersects&f=json&outFields=%2A&inSR=102100&maxAllowableOffset=0&geometryType=esriGeometryPoint&outSR=102100&geometry=%7B%22x%22%3A%2D13680413%2E824136274%2C%22y%22%3A5803715%2E350713715%7D

# #  	10th
# 	11th
# 	12th
# 	13th
# 	14th
# 	16th
# 	17th
# 	18th
# 	19th
# 	1st
# 	20th
# 	21st
# 	22nd
# 	23rd
# 	24th
# 	2nd
# 	3rd
# 	4th
# 	5th
# 	6th
# 	7th
# 	8th
# 	9th
# 	Aaron
# 	Abbey
# 	Abbotsford
# 	Acacia
# 	Academy
# 	Adams
# 	Adler
# 	Alameda
# 	Aldaglen
# 	Alder
# 	Aldercrest
# 	Allen
# 	Alma
# 	Aloha
# 	Alpenridge
# 	Alroy
# 	Alturas
# 	Apache
# 	Apple
# 	Arcadia
# 	Argus
# 	Ash
# 	Astro
# 	Autumn Hills
# 	Ayers
# 	Azalea
# 	Badger
# 	Baker
# 	Balboa
# 	Ball Park
# 	Banyon
# 	Barnes
# 	Barr
# 	Bates
# 	Baywood
# 	Bear Creek
# 	Beasley
# 	Beauvais
# 	Behshel Heights
# 	Bella Vista
# 	Bellevue
# 	Bentley
# 	Bloyd
# 	Blue Jay
# 	Bluff
# 	Boardwalk
# 	Bobolink
# 	Bodine
# 	Bonneville
# 	Boone
# 	Bootleg Hill
# 	Boston
# 	Bowmont
# 	Brighton
# 	Broken Mountain
# 	Brookside
# 	Brussat
# 	Burcham
# 	Canaan
# 	Canvasback
# 	Cardinal
# 	Carolyn
# 	Carriage
# 	Carroll
# 	Catlin
# 	Cedar
# 	Cedar Falls
# 	Cedarglen
# 	Chatto
# 	Cheadwal
# 	Cherokee
# 	Cherry
# 	Chestnut
# 	Chickadee
# 	Church
# 	Cimarron
# 	Clark
# 	Claudine
# 	Clearwater
# 	Cliff Wood
# 	Clinton
# 	Clydesdale
# 	Coach House
# 	Coal Mine
# 	Cole
# 	Collins
# 	Colorado
# 	Columbia
# 	Corduroy
# 	Cornwell
# 	Coweeman
# 	Coweeman Park
# 	Cowlitz
# 	Cowlitz Gardens
# 	Cowlitz Villa
# 	Coyote Canyon
# 	Crawford
# 	Crescent
# 	Crestwood
# 	Cris
# 	Croy
# 	Cunningham
# 	Daffodil
# 	Dayton
# 	Decatur
# 	Deer Park
# 	Deer Trail
# 	Derek
# 	Desert Inn
# 	Dike
# 	Dillon
# 	Dirk
# 	Divide
# 	Division
# 	Dodds
# 	Donation
# 	Douglas
# 	Dunbar
# 	Duncan
# 	Duncan Spur
# 	Dungan
# 	Dusty
# 	Early Bird
# 	East
# 	Easton
# 	Eddie
# 	Edinburgh
# 	Elizabeth
# 	Elm
# 	Emerald
# 	Emma
# 	English
# 	Essex
# 	Eveland
# 	Evergreen
# 	Evergreen Spur
# 	Fishers
# 	Fishpond
# 	Flatt
# 	Florence
# 	Fouch
# 	Fox
# 	Front
# 	Fulbright
# 	Galloway
# 	Garden
# 	Garneys
# 	Gatewood
# 	General Delivery
# 	Gentry
# 	Glen More
# 	Goble Creek
# 	Godfrey
# 	Golden Eagle
# 	Grace
# 	Grade
# 	Graham
# 	Grant
# 	Greeley
# 	Greenwood
# 	Grimm
# 	Guier
# 	Hageman
# 	Hale Barber
# 	Hamilton Heights
# 	Hampshire
# 	Hancock
# 	Hardy
# 	Harris
# 	Harris Street
# 	Harrison
# 	Haussler
# 	Hawthorne
# 	Hazel
# 	Hi Loch
# 	Hiddenridge
# 	Highland
# 	Highland Park
# 	Hilltop
# 	Hillview
# 	Holcomb
# 	Holcomb Loop
# 	Holcomb Spur
# 	Holly
# 	Home
# 	Home Town
# 	Hope
# 	Horseshoe Bend
# 	Hoskins
# 	Hummingbird
# 	Indigo
# 	Ineer
# 	Isaacson
# 	Island View
# 	Ivy
# 	Jasmine
# 	Jefferson
# 	Jensen
# 	John
# 	Johnson
# 	Jones
# 	Joseph
# 	Joy
# 	Kari Jo
# 	Keen
# 	Kelso
# 	Kennington Heights
# 	Kiltie
# 	King
# 	Kingfisher
# 	Kingsbury
# 	Kinnear
# 	Kismet
# 	Kitchen
# 	Kokanee
# 	Kona
# 	Kool
# 	Krystle
# 	Lark
# 	Lasalle
# 	Laurel
# 	Leif
# 	Lenora
# 	Lewis
# 	Lexington
# 	Libby
# 	Lincoln
# 	Line
# 	Long
# 	Lord
# 	Louise
# 	Lowrane
# 	Luxury
# 	Madera
# 	Mahaffey
# 	Main
# 	Makela
# 	Manteca
# 	Maple
# 	Maple Hill
# 	Maranatha
# 	Maryhill
# 	Masada
# 	Maxwell
# 	Mayfield Garden
# 	Mcdonald
# 	Mcgeary
# 	Mckall
# 	Mcmenomy
# 	Meadow
# 	Meadowlark
# 	Melrose
# 	Mendota
# 	Merced
# 	Meridian
# 	Michels
# 	Mill
# 	Miller
# 	Milwaukee
# 	Mimosa
# 	Minor
# 	Mission
# 	Misty Mountain
# 	Mockingbird
# 	Modesto
# 	Molly
# 	Monterey
# 	Mount Pleasant
# 	Mt Brynion
# 	Nella
# 	Nelson
# 	Nesbit Loop
# 	Nicholson
# 	Niechelle
# 	Nighthatch
# 	Nightingale
# 	Nob
# 	Noble Ridge
# 	Nordstrom
# 	North
# 	Norwood
# 	Oak
# 	Ohio
# 	Old Pacific
# 	Olive
# 	Olsen
# 	Osprey
# 	Ostrander
# 	Overlook
# 	Owl Creek
# 	P G Sweet
# 	Pacific
# 	Pacific Avenue
# 	Painter Roth
# 	Palm
# 	Pare
# 	Parker
# 	Parrott
# 	Pauli
# 	Paxton
# 	Peaceful
# 	Peacock
# 	Pelican
# 	Peninsula
# 	Penny
# 	Peppermill
# 	Pin Creek
# 	Pine
# 	Pineway
# 	Pleasant Hill
# 	Po Box
# 	Ponderosa
# 	Prairie
# 	Pries
# 	Primrose
# 	Quartz
# 	Radcliffe
# 	Rae
# 	Rainbow
# 	Rainbow Heights
# 	Ranch
# 	Randa
# 	Raven Ridge
# 	Ravena
# 	Ravenwood
# 	Red Row
# 	Redpath
# 	Regal
# 	Remington
# 	Risley
# 	Rivendell
# 	River
# 	Riverside
# 	Robert
# 	Rocky Point
# 	Roley
# 	Rollingwood
# 	Rons
# 	Rose Garden
# 	Rose Valley
# 	Rosewood
# 	Ross
# 	Royal
# 	Ruby
# 	Sakari
# 	Sales Barn
# 	Sanctuary
# 	Saratoga
# 	Schoolhouse
# 	Shady
# 	Shadywood
# 	Shanghai
# 	Shawnee
# 	Shoreview
# 	Sierra
# 	Sitka
# 	Solomon
# 	Sparks
# 	Sparrow
# 	Speer
# 	Spruce Creek
# 	Stardust
# 	Starlight
# 	Stock
# 	Sunburst
# 	Sunnyside
# 	Sunrise
# 	Surfview
# 	Swanson
# 	Swiftwater
# 	Talley
# 	Tam Oshanter
# 	Tanager
# 	Tara
# 	Tatonka
# 	Teresa
# 	Terrace
# 	Three Rivers
# 	Tilla
# 	Topeka
# 	Tranquil
# 	Trapper
# 	Travis
# 	Trenton
# 	Tweed
# 	Two Trees
# 	Tybren Heights
# 	Union
# 	University
# 	Valley View
# 	Ventura
# 	Vermillion
# 	Veys
# 	Villa
# 	Vine
# 	Virginia
# 	Vision
# 	Vista
# 	Wagner
# 	Walker
# 	Walnut
# 	Walnut Acres
# 	Wapiti
# 	Warbler
# 	Washburn
# 	Washington
# 	Waxwing
# 	Western
# 	Westminster
# 	Westside
# 	Wilderness
# 	Wildlife
# 	Williams
# 	Williams Finney
# 	Willow
# 	Willowbrook
# 	Wilshire
# 	Winchester
# 	Witherbee
# 	Wood
# 	Woodpecker
# 	Wyatt
# 	Yellow Brick
# 	Yew
# 	Young
# 	Zillig

# mech-dump 'http://www.cowlitzinfo.net/applications/cowlitzassessorparcelsearch/%28S%28azg3hab4pnwn54qmxbozb345%29%29/searchcp.aspx?Public=Y'

use warnings;
use strict;

use WWW::Mechanize;
use Data::Dump;

$| = 1;

my $mech = WWW::Mechanize->new();

my $url = 'http://www.cowlitzinfo.net/applications/cowlitzassessorparcelsearch/%28S%28azg3hab4pnwn54qmxbozb345%29%29/searchcp.aspx?Public=Y';
$mech->post($url);

get_street_info('Aaron');

sub get_street_info {
    my $street = shift; 

    $mech->set_fields('ctl00$cphParcelSearch$txtStreetName' => $street);

    my @submits = $mech->find_all_submits(name_regex => qr/btnSearch/);
    foreach my $submit (@submits) {
	printf("submitting %s\n", $submit->{name});
	$mech->submit_form(button=>$submit->name);
	last;
    }

    my $filename = "cowlitzpages/" . $street . "_page1";
    open(my $fp, ">", $filename);
    print($fp $mech->content());
    close($fp);

    my @pages = ( $mech->content() =~ m/vSearchResults','(Page\$[0-9]+)'/g);
    print("pages ", join(", ", @pages), "\n");

    my %seenpages = ('Page$1' => 1);
    while (my $page = shift @pages) {
	next if ($seenpages{$page});
	$seenpages{$page} = 1;
	printf("getting page %s %s\n", $street, $page);
	$mech->set_fields('__EVENTARGUMENT'=> $page, 
			  '__EVENTTARGET'=>'ctl00$cphParcelSearch$gvSearchResults' 
	    );
	my $ret = $mech->submit();

	my ($pagenum) = ($page =~ m/Page\$([0-9]+)/);
	printf("saving page %s\n", $pagenum);

	my $filename = "cowlitzpages/" . $street . "_page" . $pagenum;
	open(my $fp, ">", $filename);
	print($fp $mech->content());
	close($fp);

	my @morepages = ( $mech->content() =~ m/vSearchResults','(Page\$[0-9]+)'/g);

	print("more pages ", join(", ", @morepages), "\n");
	push(@pages, @morepages);
    }
}
exit;

print("links\n");
foreach my $link ($mech->links()) {
    print Data::Dump::pp($link);
}

#print Data::Dump::pp($mech->response());
my $content = $mech->content();
print $content, "\n";

my @pages = ($content =~ m/vSearchResults','(Page\$[0-9]+)'/g);
print("pages ", join(", ", @pages), "\n");

my @accounts = ($content =~ m/>(R[0-9]+)<\/fon.*333333">(WD[0-9]+)<\/fon/g);
print(join(", ", @accounts), "\n");



while (@accounts) { 
    my $account = shift @accounts;
    my $parcel  = shift @accounts;

    printf("###### account %s parcel %s\n", $account, $parcel);
    my $sub_url = 'http://www.cowlitzinfo.net/applications/cowlitzassessorparcelsearch/%28S%28hm5twz55jizw0j45fxhdcwum%29%29/ParcelDataCP.aspx?Account=' . $account . '&Public=Y&Parcel=' . $parcel;
    $mech->get( $sub_url );


    print($mech->content());
    last;
}

exit;
$mech->dump_forms();

$mech->follow_link( n => 1 );
$mech->follow_link( text_regex => qr/download this/i );
$mech->follow_link( url => 'http://host.com/index.html' );
 

 
$mech->submit_form(
    form_name => 'search',
    fields    => { query  => 'pot of gold', },
    button    => 'Search Now'
);

my @submits = $mech->find_all_submits();
foreach my $submit (@submits) { printf("submit %s\n", $submit->{name}); }
