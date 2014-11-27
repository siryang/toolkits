use strict;
use warnings;

print "RIGHT" if ($ARGV[0] and $ARGV[0] eq "abc");
exit 0;

while(<>)
{
	chomp;
	my $url = "http://translate.google.com/translate_tts?tl=en&q=$_";
	`wget --user-agent=FireFox -O "$_.mp3" 2>&1 >NUL "$url"`;
	die if $? != 0;
	print "\"$_.mp3\"\n";
}

exit 0;