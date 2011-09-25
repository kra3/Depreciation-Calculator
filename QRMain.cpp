/*
 * QRMain.cpp
 * Created: 18/05/2010
 * Last updated: 21/05/2010
 * Author: Arun.K.R<the1.arun@gmail.com>
 */

//SYSTEM HEADER FIELS//
#include <fstream.h>
#include <string.h>
#include <dos.h>

//USER CREATED INCLUDES//
#include "asset.cpp"

//DEFINE GLOBAL DATA//
#define ESC   0x1b              /* Define the escape key   Hex=1B;ASCII=27    */

enum bool{mock=-1,false,true};
typedef struct depiTable{ //convinient structure to seatilalize depriciation schedule file.
    int assetCode;
    double *depiTbl;
}dTbl;

fstream fileHndl;
dTbl summaryTbl;

//GLOBAL FUNCTION DECLARATION//
void createAssetsFile();
void doDepriciation();
void appendToAssetsFile();
void searializeAssets(char*);
void drawDepiTable(dTbl*, int);
void beep();
bool yesNoCheck(char*);

//PROGRAM ENTRY POINT//
int main(){
    bool getMeOut = false;

    do{
        int choice;

        clrscr();
        cout<<"\n\n\n\t MENU \n\n";
        cout<<"\t~~~~~~\n\n";
        cout<<"  [1] Create an assets file.\n\n";
        cout<<"  [2] Calculate depreciation and display depreciation tables.\n\n";
        cout<<"  [3] Append a new assets.\n\n";
        cout<<"  [4] Exit from the Application.\n\n";
        cout<<"  Select a menu item [1/2/3/4] : [   ]\b\b\b";
        choice = getche() - '0';
        cout<<"\b"<<choice;
        delay(10);

        switch(choice){
            case 1:
                clrscr();
                createAssetsFile();
                getMeOut = false;
                break;
            case 2:
                clrscr();
                doDepriciation();
                getMeOut = false;
                break;
            case 3:
                clrscr();
                appendToAssetsFile();
                getMeOut = false;
                break;
            case 4:
                getMeOut = true;
                break;
            default:
                // Wrong choice. Please try again...";
                getMeOut = false;
                beep();
                break;
        }
    }while(!getMeOut);

    return 0;
}

//GLOBAL FUNCTION DEFINITIONS

/*
   * Creates asset file and add entries to the file.
   * returns : nothing
   */
void createAssetsFile(){
    clrscr();
    char *assetFileName;

    // a) create asset file with user specified name

    do{
        cout<<"\n[Warning: If file with same name exists, it'll be deleted]\n\n\t Enter name for asset file: ";
        beep();
        gets(assetFileName);
    }while(!strlen(assetFileName)); //name need atleast one character

    strcat(assetFileName, ".qrd");
    fileHndl.open(assetFileName, ios::out|ios::in|ios::trunc|ios::binary);
    if(!fileHndl){
        cout<<"\n\nError in creating file...\n Exiting....\n";
        delay(2000);
        exit(-1);
    }
    fileHndl.close();
    cout<<"\n\n\t File succesfully created !\n\t Please Wait.....";
    delay(300);

    //b. Prompt the user to enter the asset code and the rest of the data (as in choice 1) and write them to the assets file.

    searializeAssets(assetFileName);
}

  /*
   * Creates depriciation schedule file. Calculates depriciation table & write them out to file.
   * Also prints a summary depriciation table.
   * returns : nothing
   */
void doDepriciation(){
    clrscr();
    ifstream inFile;
    ofstream outFile;
    char *assetFileName, *depischedFileName;
    Assets asset;
    dTbl dt;
    summaryTbl.assetCode = 0;
    int maxUsefulLifeYrs = 0;
    double *tmpDepiTbl;

    //a. Prompt the user to enter the name of the asset depreciation schedule file and create it.

    do{
        cout<<"\n[Warning: If file with same name exists, it'll be deleted]\n\n\t Enter name for depriciation schedule file: ";
        beep();
        gets(depischedFileName);
    }while(!strlen(depischedFileName)); //name need atleast one character

    strcat(depischedFileName, ".qrd");
    fileHndl.open(depischedFileName, ios::out|ios::in|ios::trunc|ios::binary);
    if(!fileHndl){
        cout<<"\n\nError in creating file...\n Exiting....\n";
        delay(2000);
        exit(-1);
    }
    fileHndl.close();
    cout<<"\n\n\t File succesfully created !\n\t Please Wait.....";
    delay(300);

    //b. Prompt the user to enter the name of the assets file.

    do{
        cout<<"\n\n\t Enter the  name of asset file: ";
        beep();
        gets(assetFileName);
    }while(!strlen(assetFileName)); //name need atleast one character

    strcat(assetFileName, ".qrd");
    fileHndl.open(assetFileName, ios::out|ios::in|ios::app|ios::binary|ios::nocreate);
    if(!fileHndl){
        cout<<"\n\nError in opening file... or No file with that name...\n";
        delay(2000);
        return;
    }
    fileHndl.close();
    cout<<"\n\n\t File succesfully opened !\n\t Please Wait.....";
    delay(300);
    clrscr();

    //c. Read the assets file.

    outFile.open(depischedFileName, ios::app|ios::binary);
    inFile.open(assetFileName, ios::binary);
    while( inFile.read((char*)&asset, sizeof(asset)) ){
        dt.depiTbl = asset.createDepriciationTable(); //c. For each asset calculate the depreciation value.
        dt.assetCode = asset.getCode();
        int yrs = asset.getUsefulLifeYears();

        //d. store the depreciation tables to the depreciation schedule file.
        outFile.write((char*)&dt, sizeof(dt));

        //e. Display the depreciation tables on the screen.
        cout<<"\n\n Asset code: "<<dt.assetCode;
        cout<<"\n Depriciation Table : \n";
        drawDepiTable(&dt, yrs);

        if(maxUsefulLifeYrs < yrs){
            maxUsefulLifeYrs = yrs; // for the purpose of summary depriciation table
        }
    }
    inFile.close();
    outFile.close();

    //f. Calculate and display a summary depreciation table.

    tmpDepiTbl = new double[maxUsefulLifeYrs];
    for(int i =0; i < maxUsefulLifeYrs; ++i){
        tmpDepiTbl[i] = 0;
    }

    inFile.open(assetFileName, ios::binary);
    while( inFile.read((char*)&asset, sizeof(asset)) ){
        double *tmpTbl = asset.createDepriciationTable();
        int tblSz = asset.getUsefulLifeYears();

        for(int i = 0; i < tblSz; ++i){
            tmpDepiTbl[i] += tmpTbl[i];
        }
    }
    inFile.close();

    summaryTbl.depiTbl = tmpDepiTbl;
    clrscr();
    cout<<"\n\t SUMMARY DEPRICIATION TABLE \n\n";
    drawDepiTable(&summaryTbl, maxUsefulLifeYrs);
}

/*
   * Open asset file and append further entries to it.
   * returns : nothing
   */
void appendToAssetsFile(){
    clrscr();
    char *assetFileName;

    // a. Prompt the user to enter the name of the assets file.

    do{
        cout<<"\n\n\t Enter the  name of asset file: ";
        beep();
        gets(assetFileName);
    }while(!strlen(assetFileName)); //name need atleast one character

    //b. Open the assets file in append mode so that not to destroy the file.

    strcat(assetFileName, ".qrd");
    fileHndl.open(assetFileName, ios::out|ios::in|ios::app|ios::binary|ios::nocreate);
    if(!fileHndl){
        cout<<"\n\nError in creating file... or No file with that name...\n";
        delay(2000);
        return;
    }
    fileHndl.close();
    cout<<"\n\n\t File succesfully opened !\n\t Please Wait.....";
    delay(300);

    //c. Prompt the user to enter the asset code and the rest of the data and write them to the assets file.

    searializeAssets(assetFileName);

}

/*
   * Write asset entries to the file.
   * fileName   : The 'asset' file's name. To which data have to be searialized.
   * returns    : nothing.
   */
void searializeAssets(char *fileName){
    clrscr();
    char choice;
    ofstream outFile;
    Assets asset;

    asset.getAssets();
    clrscr();
    asset.showAssets();

    do{
        cout<<"\n\n Does the values are correct? [Y/N] : [   ]\b\b\b";
        choice=getche();
        cout<<"\b"<<(char)toupper(choice);
        delay(10);
    }while(yesNoCheck(&choice) == mock);

    if(choice == 'Y' || choice == 'y'){ //Write it to file
        outFile.open(fileName, ios::app|ios::binary);
        outFile.write((char*)&asset, sizeof(asset));
        outFile.close();
    }

    do{
        cout<<"\n\nAdd another entry to asset's file? [Y/N] : [   ]\b\b\b";
        choice=getche();
        cout<<"\b"<<(char)toupper(choice);
        delay(10);
    }while(yesNoCheck(&choice) == mock);

    if(choice == 'N' || choice == 'n'){
        clrscr();
        return;
    }else{ // chose to add another entry.
        searializeAssets(fileName); //recursion of course.
    }

}

/*
   * Prints depriciation tables.
   * depi       : structure containing depriciation table array and curresponding asset's code.
   * tblSz      : Size of depriciation table array.
   * returns    : nothing
   */
void drawDepiTable(dTbl *depi, int tblSz){
    cout<<"\n"<<"Year\t|Depriciation\n";
    for(int i = 0; i < tblSz; ++i){
        cout<<" "<<i+1<<"\t| "<<depi->depiTbl[i]<<"\n";
    }
    cout<<"\n"<<"Press any key to proceed... ";
    getch();
}

/*
   * Beeps at 2000Hz for 150 milli seconds.
   * returns : nothing.
   */
void beep(){
    sound(2000);
    delay(150);
    nosound();
}

/*
   * Check wether given choice is Yes/No
   * ans            : character string that has to be checked.
   * returns    : true for Yes, false for No/ESC, mock otherwise.
   */
bool yesNoCheck(char *ans){
    if(*ans=='y'||*ans=='Y')
    return(true);
    else if(*ans=='n'||*ans=='N'||*ans==ESC)
    return(false);
    else
    return(mock);
}
