#!/usr/bin/perl

use strict;

### die "Usage $0  <list of LQ masses in GeV separated by spaces>\n" unless ($#ARGV gt -1); 

my @masses = (250, 400, 650, 1000);
print "Running on LQ masses: @masses   - Please modify the array \@masses to add/remove some.\n";
my @triggers = ('HLT1ElectronRelaxed\(33\)', 'HLT1EMHighEt\(40\)', 'HLT1EMVeryHighEt\(41\)');
my @ors = ("$triggers[1],$triggers[2]","$triggers[1],$triggers[2],$triggers[3]");
my $outfile = "triggerEffic.dat";
    
my @summary;
my @effic;
my @tmp;

foreach my $mass (@masses) {
    my $file="log"; $file=$file.$mass."GeV.txt";    
    print "Opening file $file\n";
    open FILE, "<$file" or die $!;
    @tmp=grep( /$triggers[0]/ && ! /$triggers[1]/ && ! /$triggers[2]/ , <FILE>);
    push(@effic, @tmp);
    close FILE;
    open FILE, "<$file" or die $!;
    @tmp=grep( /$triggers[1]/ && ! /$triggers[2]/ && ! /$triggers[0]/ , <FILE>);
    push(@effic, @tmp);
    close FILE;
    open FILE, "<$file" or die $!;
    @tmp=grep( /$triggers[2]/ && ! /$triggers[0]/ && ! /$triggers[1]/ , <FILE>);
    push(@effic, @tmp);
    close FILE;
    open FILE, "<$file" or die $!;
    @tmp=grep( /$ors[0]/ && ! /$ors[1]/ , <FILE>);
    push(@effic, @tmp);
    close FILE;
    open FILE, "<$file" or die $!;
    @tmp=grep( /$ors[1]/ , <FILE>);
    push(@effic, @tmp);
    close FILE;
}

#print @effic;

my @eff;
my @err;
foreach my $line (@effic){
    my @temp = split(/ +/, $line);  
    push(@eff, @temp[1]);
    push(@err, @temp[2]);
}

#print @eff;

print "Writing to output file $outfile\n";
open OUTFILE, ">$outfile" or die "unable to open $outfile $!";
printf OUTFILE "Mass   HLT1ElectronRelaxed   HLT1EMHighEt      HLT1EMVeryHighEt    HighORVeryHigh    HighORVeryHighORRel\n";

my $nn=0;
foreach my $mass (@masses) {
    printf OUTFILE ("%4i   %1.4f  %1.4f    %1.4f  %1.4f    %1.4f  %1.4f    %1.4f  %1.4f    %1.4f  %1.4f \n", $mass, @eff[$nn],@err[$nn],  @eff[$nn+1],@err[$nn+1],  @eff[$nn+2],@err[$nn+2],  @eff[$nn+3],@err[$nn+3]  ,  @eff[$nn+4],@err[$nn+4] );  
    $nn=$nn+5;
} 

