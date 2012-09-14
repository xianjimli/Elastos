//==========================================================================
// Copyright (c) 2000-2009,  Elastos, Inc.  All Rights Reserved.
//==========================================================================

#include <stdio.h>
#include <Animals.h>
#include <NewPets.h>
using namespace Elastos;

ECode PrintLyrics(IAnimal* pAnimal, Boolean lastVerse)
{
    String owner;
    pAnimal->GetOwner(&owner);

    String pet;
    pAnimal->GetKind(&pet);

    String sound;
    pAnimal->Says(&sound);

    String soundx2 = sound + "-" + sound;

    String lyrics;
    lyrics = owner + " had a farm, ee-igh, ee-igh, oh!\n";
    lyrics += "And on the farm he had a ";
    lyrics += pet + ", ee-igh, ee-igh, oh!\n";
    lyrics += "With a ";
    lyrics += soundx2 + " here and a ";
    lyrics += soundx2 + " there.\n";
    lyrics += "Here a ";
    lyrics += sound + ", there a ";
    lyrics += sound + ", everywhere a ";
    lyrics += soundx2 + ".\n";

    printf("%s\n", (const char*)lyrics);

    if (lastVerse) {
        printf("%s", (const char*)owner);
        printf(" had a farm, ee-igh-igh, ee-igh-igh, ohh!!\n");
    }
    return NOERROR;
}

int main (int argc, char *argv[])
{
    ECode ec;
    IAnimal* pAnimal = NULL;
    printf("\n");

    ec = CPig::New(&pAnimal);
    if (FAILED(ec)) {
        printf("The pig isn't here... ^_^");
        return ec;
    }

    PrintLyrics(pAnimal, FALSE);
    pAnimal->Release();

    ec = CCat::New(&pAnimal);
    if (FAILED(ec)) {
        printf("The cat isn't here... ^_^");
        return ec;
    }
    PrintLyrics(pAnimal, TRUE);
    pAnimal->Release();

    return NOERROR;
}

