g++ -o makeTree makeTree.C `root-config --cflags --glibs`
./makeTree > log.txt
g++ -o analyzeTree analyzeTree.C `root-config --cflags --glibs` -I.
./analyzeTree
root -l -b plotHisto.C 

