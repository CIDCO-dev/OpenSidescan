#/usr/bin/perl

use warnings;
use strict;

sub readHits(){
	my ($file,$hitsRef)=@_;

	open(FILE, '<', "$file.hits") or die("No hits file for $file\n");

	while(<FILE>){
		my $line=$_;
		chomp($line);
		if($line =~ /(\d+) (\d+) (\d+)/g){
			push(@$hitsRef,[$1,$2,$3]);
		}
	}

	close(FILE);
}

sub isHit{
	my ($hitsRef,$channel,$x,$y,$width,$height)=@_;

	for my $h (@$hitsRef){
		if(
			($channel==@$h[0]) &&
			((@$h[1] >= $x) && (@$h[1] <= ($x + $width )) ) &&
			((@$h[2] >= $y) && (@$h[2] <= ($y + $height)) )
		){
			return 1;
		}
	}

	return undef;	
}

die("ga.pl directory\n") unless scalar(@ARGV)==1;

my $directory = $ARGV[0];

print "[+] Looking into $directory\n";

my @genome = (100,50,10,5,60);
#my @genome = (200,50,20,5,200);

my $truePositive= 0;
my $precisionCount =0;
my $recallCount = 0;
my $recalled=0;

for my $file (split(/\n/,`ls $directory/*.xtf`)){
	print "[+] Testing $file\n";
	my @hits;
	&readHits($file,\@hits);

	# print STDERR scalar(@hits) . " hits\n";

	# print "@genome\n";


	my $results = `./build/bin/sidescan-detect $file $genome[0] $genome[1] $genome[2] $genome[3] $genome[4] 15000 2> /dev/null`;

	my @detections = map {my $l=$_;chomp($l);$l} split(/\n/,$results);

	# Compute precision
	for my $line (@detections){
		if($line =~ /(\d+) (\d+) (\d+) (\d+) (\d+)/g){
			if(isHit(\@hits,$1,$2,$3,$4,$5)){
				# print "Hit\n";
				$truePositive++;
			}
			else{
				print "Bad detection: ($line)\n";
			}

			$precisionCount++
		}
	}


	#compute recall

	for my $h (@hits){

		for my $d (@detections){
			
			if($d =~ /(\d+) (\d+) (\d+) (\d+) (\d+)/){
				my ($channel,$x,$y,$width,$height)=($1,$2,$3,$4,$5);


				if(
		                        ($channel==@$h[0]) &&
        		                ((@$h[1] >= $x) && (@$h[1] <= ($x + $width )) ) &&
                        		((@$h[2] >= $y) && (@$h[2] <= ($y + $height)) )
				){
					# print "Recalled\n";
					$recalled++;	
					last;
				}	
			}
		}

		$recallCount++;
	}
}

print "Precision: " . (($truePositive+0.0)/($precisionCount+0.0))*100 ."%\n";
print "Recall: " . ( ($recalled + 0.0)/($recallCount+0.0)) * 100  ."%\n";
