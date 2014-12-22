

use warnings;
use strict;

#select PARCNO, ACCOUNTNO, Coordinates from Parcels limit 5;

my $templ = "path.tmpl";

open(my $fp, "<", $templ) || die("can't open $templ\n");

my $main = join("", <$fp>);

my $path = <<EOL;
		<Placemark>
			<name>__PATHNAME__</name>
			<styleUrl>#msn_ylw-pushpin</styleUrl>
			<LineString>
				<tessellate>1</tessellate>
				<coordinates>
				  __COORDINATES__
				</coordinates>
			</LineString>
		</Placemark>
EOL
    

    
my @paths;
while (my $line = <ARGV>) {
    chomp $line;
    my ($pn, $an, $coord) = split(/\|/, $line);
    
    my $p = $path;
    $p =~ s/__PATHNAME__/$pn:$an/gs;
    $p =~ s/__COORDINATES__/$coord/gs;
    push(@paths, $p);
}

my $paths = join("\n", @paths);

$main =~ s/__PLACEMARKS__/$paths/;

print $main;
