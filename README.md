landfinder
==========

reading this is really helpful. basically, UTM is in units of meter
http://en.wikipedia.org/wiki/Universal_Transverse_Mercator_coordinate_system

utmconv.pl is sample code for converting utm to lat/long
extract_stats is intended to hold code for extracting stats about plots. what percent aspect, slope...

xtract_stats/notes for more tips

cowlitz county makes their GIS data available here:
http://www.cowlitzinfo.net/applications/cowlitzgisdownloads/(S(esd0tb45ulked135nfecic55))/downloads.aspx
This data doesn't include information about the owners. That can extracted by crawling the website.


columbia county is here:
http://www.co.columbia.or.us/departments/assessors-office/assessors-gis-data

klickitat county has an interactive map here:
http://imap.klickitatcounty.org/
it's clickable, so it could be crawled. PErhaps there's a link somewhere to enable direct download.



cowlitz.pl was an attempt to gather all of the parcels in the county via the street search capability on the website

The website is here:
http://www.cowlitzinfo.net/applications/cowlitzassessorparcelsearch/(S(vn2an345hyg5wwyt52nrqw55))/searchcp.aspx?Public=Y

Cowlitz/parcels.db is a sqlite3 db that contains a merge of the assessment data and the boundaries in one table.

genparcelpaths.pl generates a kml for google earth based on a simple sqlite select. Parcel number, account number, and coordinages. PN/AN in cowlitz is a unique identified for a piece of property.

from https://www.mapbox.com/tilemill/docs/guides/terrain-data/

Note: If there is not a 1:1 relation between your vertical and horizontal units, you will want to use the -s (scale) option to indicate the difference. Since Google Mercator X & Y units are meters, and NED elevation is also stored in meters this wasn’t necessary for our example. If your elevation data is stored in feet, you might do this instead (since there are approximately 3.28 feet in 1 meter):


aspect: It is reported as degrees (or radians) from the established zero marker (usually North) and is also called the azimuth angle