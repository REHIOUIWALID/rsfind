#!/bin/bash

dossier="./libmagic"
name="magic.c"
chaine="Copyright"

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


    # TEST rsfind dossier -name CHAINE
find $dossier -name $name > test.txt
./rsfind $dossier --name $name > rsfind.txt

nb=$(diff test.txt rsfind.txt | wc -l)
if [ $nb -eq 0 ]
then
    echo "  rsfind dossier -name CHAINE ok"
else
    echo "  rsfind dossier -name CHAINE not ok"
fi
# depuis rep Courant
find . -name $name > test.txt
./rsfind --name $name > rsfind.txt

nb=$(diff test.txt rsfind.txt | wc -l)
if [ $nb -eq 0 ]
then
    echo "  rsfind dossier -name CHAINE ok"
else
    echo "  rsfind dossier -name CHAINE not ok"
fi

    # TEST rsfind dossier -t CHAINE 
grep -r -l $dossier -e $chaine > test.txt
./rsfind $dossier -t $chaine > rsfind.txt

nb=$(diff test.txt rsfind.txt | wc -l)
if [ $nb -eq 0 ]
then
    echo "  rsfind dossier -t CHAINE ok"
else
    echo "  rsfind dossier -t CHAINE not ok"
fi

    # TEST rsfind dossier -i 
# find $dossier -type f -exec file \; | grep image | cut -d : -f 1
# Probleme pour lancer le find ci-dessus !


    # TEST rsfind dossier -l
find $dossier -exec ls -l {} \; > test.txt
./rsfind $dossier -l > rsfind.txt
if [ $nb -eq 0 ]
then
    echo "  rsfind dossier -l ok"
else
    echo "  rsfind dossier -l not ok"
fi


rm test.txt
rm rsfind.txt
make clean > nul
rm nul




