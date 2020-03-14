#!/bin/bash

dossier="./libmagic"
name="HEAD"
chaine="commit"

touch test.txt
touch rsfind.txt
make rsfind > nul
rm nul

    # TEST rsfind DOSSIER = find DOSSIER
find $dossier > test.txt
./rsfind $dossier > rsfind.txt


nb=$(diff test.txt rsfind.txt | wc -l)
if [ $nb -eq 0 ]
then
    echo "  rsfind dossier ok"
else
    echo "  rsfind dossier not ok"
fi