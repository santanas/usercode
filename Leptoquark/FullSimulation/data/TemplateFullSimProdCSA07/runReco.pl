#!/usr/bin/perl

#--------------------------------------------------------------
# History     01/Oct/2007  First shot
# -------
#             
#--------------------------------------------------------------
# Emanuele Di Marco  <emanuele@cern.ch>
#--------------------------------------------------------------

print "Starting...\n";

use Time::Local;
use Getopt::Std;
getopts('d:j:i:c:b:q:s:S:C:g:w:l:z:nrh');

if(!$opt_c) {help();}
if(!$opt_j) {help();}
if($opt_j){$jobs=$opt_j;}
$templateCfg = $opt_c;
if($opt_b){$baseName = $opt_b;}
else{$baseName = "analysis";}
if($opt_q){$queue = $opt_q;}
else{$queue="1nh";}
if($opt_s){$scratch=$opt_s;}
else{$scratch="./"}
if($opt_S){$hltInputDir=$opt_S;}
else {help();}
if($opt_g){$group=$opt_g;}
else{$group=1;}
if($opt_w){$workdir=$opt_w;}
else{$workdir="/afs/cern.ch/user/e/emanuele/work/HtoWWAnalysis/src/";}
if($opt_z){$wait=$opt_z;}
else{$wait=9999999;}
if($opt_C){$castorDir=$opt_C;}
if($opt_i){$initialseed=$opt_i;}
else{
    print "Warning: initial seed not set, using 1000 as default!\n";
    $initialseed=1000;
}

my $logdir = "$scratch/Log";
my $confdir = "$scratch/Cfg";
my $scriptdir = "$scratch/Csh";

# -- Create directories if not yet existent
if (-d "$logdir") {
    # do nothing
} else {
    system("/bin/mkdir $logdir"); 
    system("chmod 755 $logdir");
    if (-d "$logdir") {
	print " -> created $logdir\n";
    } else {
	die "run: cannot create $logdir\n";
    }
}
if (-d "$confdir") {
    # do nothing
} else {
    system("/bin/mkdir $confdir"); 
    system("chmod 755 $confdir");
    if (-d "$confdir") {
	print " -> created $confdir\n";
    } else {
	die "run: cannot create $confdir\n";
    }
}
if (-d "$scriptdir") {
    # do nothing
} else {
    system("/bin/mkdir $scriptdir"); 
    system("chmod 755 $scriptdir");
    if (-d "$scriptdir") {
	print " -> created $scriptdir\n";
    } else {
	die "run: cannot create $scriptdir\n";
    }
}

$hltInputDir = "rfio:"."$hltInputDir";
$logdir = "$logdir";
$confdir = "$confdir";
$scriptdir = "$scriptdir";
$castorDir = "$castorDir";

# get some useful environment variables
my $user = $ENV{USER};
my $hostname = $ENV{HOST};
my $pwd = $ENV{PWD};

$i+=0;

$cfgIndex+=0;
# loop on datasets
for($i=0; $i<$jobs; $i+=$group) {
    $cfgIndex++;
    $seed = $initialseed+$cfgIndex;
    # open the template cfg file
    open (TPLCFGFILE,"$templateCfg");
    @tplcfgfile=<TPLCFGFILE>;

    # write the cfg file
    $cfgfile = $confdir."/".$baseName."\-RECO\-$cfgIndex".".cfg";
    open (CFGFILE,">$cfgfile");
    # some standard stuff
    my $now = localtime time;
    print CFGFILE "##\n## This file was generated automatically on $now\n";
    print CFGFILE "## by user $user on host $hostname from $pwd\n";
    print CFGFILE "##\n\n";
    $j+=0;
    for($j=0; $j<($#tplcfgfile+1); $j++) {
	if($tplcfgfile[$j] =~ /\s+untracked\svstring\sfileNames\s\=\s\{\'file\:hlt\.root\'\}/) {
	    $dataset=$datasets[$i+$group-1];
	    chop $dataset;
	    $inputRootFile = $hltInputDir."/".$baseName."\_HLT_$seed".".root";
	    print CFGFILE "untracked vstring fileNames = {'file:$inputRootFile'}\n";
	    print "Writing cfg file $cfgfile... \n";
	}
	elsif($tplcfgfile[$j] =~ /\s+untracked\sstring\sfileName\s\=\s\'reco\.root\'/) {
	    $nameRoot=$baseName."\_RECO_$seed".".root";
	    print CFGFILE "untracked string fileName = 'file:$nameRoot'\n"
	} 
	else {
	    $line=$tplcfgfile[$j];
	    print CFGFILE "$line";
	}
    }
}

# now loop on created cfgfiles, and submit the jobs
if(!$opt_n) {
    @LIST=qx(/bin/ls -1 "$confdir" | /bin/grep "$baseName");
    
    my $subJobs = 0;
    my $totalJobs = $#LIST;
    my $totSubJobs = 0;
    foreach $cfgfile(@LIST) {
	# -- Cut off trailing extension and possible directories in front
	($barefile = $cfgfile) =~ s/\.cfg//g;
	$rest = substr($barefile, 0, rindex($barefile, '/')+1); 
	$barefile =~ s/$rest//;
	chop $barefile;
	$scriptfile = $scriptdir."/".$barefile.".csh";
	$CMSSWlogfile = $barefile.".CMSSWlog";
	open(SCRIPTFILE,">$scriptfile");
	print SCRIPTFILE "#!/usr/local/bin/tcsh\n\n";
	print SCRIPTFILE "pwd\n";
	print SCRIPTFILE "ls\n";
	print SCRIPTFILE "setenv CMS_PATH \$VO_CMS_SW_DIR\n";
	print SCRIPTFILE "source \$VO_CMS_SW_DIR/cmsset_default.csh\n";
	print SCRIPTFILE "setenv workdir \"$workdir\"\n";
	print SCRIPTFILE "cd \$workdir\n\n";
	print SCRIPTFILE "eval \`scramv1 runtime \-csh\`\n";
	print SCRIPTFILE "cd \-\n";
	chomp ($cfgfile);
	print SCRIPTFILE "cmsRun $confdir\/$cfgfile >\& $CMSSWlogfile\n";
	print SCRIPTFILE "pwd\n";
	print SCRIPTFILE "ls\n";
	print SCRIPTFILE "/usr/bin/rfcp \*\.root $castorDir \n";
	print SCRIPTFILE "/bin/cp \*\.CMSSWlog $logdir \n";
	system("/bin/chmod 777 $scriptfile");
	$logfile = $logdir."/".$barefile.".JOBlog";


	$totSubJobs++;
	$subJobs++;
	if($opt_r) {
	    system("bsub -J $barefile -oo $logfile -q $queue < $scriptfile");
	    print("bsub -J $barefile -oo $logfile -q $queue \< $scriptfile \n");
	}
	else {
            system("bsub -J $barefile -oo $logfile -q $queue < $scriptfile");
	    print("bsub -J $barefile -oo $logfile -q $queue \< $scriptfile \n");
	}
    }
}


sub help(){
    print "Usage: run.pl -c <templateCfg> -S <dirWhereHLT> -w <workdir> -j <njobs> [-i <initialseed>] [-b <basename>] [-s <scratch>] [-C <castorDir>] [-z <maxJobs>] [-n] [-r]\n";
    print "Exemplum: run.pl -c RelVal_HLTFromRaw.cfg -w higgsProd160/src/productionCfgs -j 100 -i 1000 -b qqH160 -s ~/scratch0 -z 20 -r \n";
    print "Options:\n";
    print "-c cfgFile:       choose the template cfg file\n";
    print "-S dirWhereHLT: choose the directory where the HLT Root files are located\n";
    print "-w /afs/.../src:  choose the workdir (the dir where you do eval...)\n";
    print "-j <njobs>:       choose the number of jobs to run\n";
    print "-i <initialseed>: the seed used for the first jobs\n";
    print "-b <basename>:    choose the basename for the cfg files and output ROOT files (default is \"analysis\")\n";
    print "-s <scratch>:     choose the area where to put the output ROOT files (default is current dir)\n";
    print "-C /castor/...:   choose the CASTOR dir where put the output rootfiles. If not given use <scratch>/output\n";
    print "-n:               if given, create the cfg files, but not the scriptfiles and do not submit the jobs\n";
    print "-r:               if given, randomize the names of the jobs\n";
    die "enjoy, wasting CPU is the spice of life...\n";
}
