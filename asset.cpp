/*
 * Asset.hpp
 * Created: 18/05/2010
 * Last updated: 19/05/2010
 * Author: Arun.K.R<the1.arun@gmail.com>
 */
 
#ifndef ASSET_CPP
#define ASSET_CPP

#include <iostream.h>
#include <process.h>
#include <stdio.h>
#include <ctype.h>
#include <conio.h>

class Assets{
//Data Members
private: 	
	int code; //asset code
	char name[30]; //asset name
	char type[1]; //asset type (either M/F for Mobile/FIxed)
	int quantity; //quantity
	int usefulLifeYears; //Useful Life years of the asset
	double assetValue; //Value of the asset
	double salvageValue; //Value of asset after a period

//Member Functions
private:
	int sumUptoN(int); //To calculate sum of the disgits of useful Life Years.
	double* straightLine(); //to calculate the depreciation of fixed assets.
	double* sumOfYearsDigits(); //to calculate the depreciation of mobile assets.
public:
	double* createDepriciationTable(); //to create depriciation table based upon asset type.
	void getAssets();	// read data into the data members
	void showAssets(); //print data into screen
	
	int getCode(){ //returns code
		return code;
	}
	
	//returns usefulLifeYears. It's useful since, it's the same as depriciation table's size.
	// In c++ finding size of dynamic array is not possible when, array is actually a pointer.
	int getUsefulLifeYears(){ 
		return usefulLifeYears;
	}
};
 
  
 // Function Definitions
 
 /*
   * Calculates depriciation in straight-ine method
   * returns : Depriciation array of double values
   */
 double* Assets::straightLine(){
	double depriciationPerYear = assetValue/usefulLifeYears;
	double *depriciation = new double[usefulLifeYears];

	for(int i = 0; i < usefulLifeYears; ++i){
		depriciation[i] = depriciationPerYear;
		/* using calculated value over and over has a speed advantage over
		 * doing calculation each time it loops.
		 */
	}
	return depriciation;
}

/*
   * Calculates depriciation in sum-of-years-digit method
   * returns : Depriciation array of double values
   */
double* Assets::sumOfYearsDigits(){
	double tmpYr = usefulLifeYears;
	double sumYr = sumUptoN(tmpYr);
	double *depriciation = new double[usefulLifeYears];

	for(int i = 0; i < usefulLifeYears; ++i){
		depriciation[i] = assetValue * (tmpYr-- / sumYr);
	}
	return depriciation;
}

/*
   * Calculates sum upto N
   * ie., sum upto 3 is 1+2+3. ie., 6
   * returns : Sum upto N
   */
int Assets::sumUptoN(int N){
	int result = 0;
	for(int i = 0; i <= N; --N){
		result += N;
	}
	return result;
}
 
 /*
   * Call appropriate depriciation method based on asset type.
   * straight-ine method for Fixed assets.
   * sum-of-years-digit method for Mobile assets.
   * returns : Depriciation array of double values
   */
 double* Assets::createDepriciationTable(){
	double *depriciationTbl;
	
	if(type[0] == 'F' || type[0] == 'f'){
		depriciationTbl = straightLine();
	}else if(type[0] == 'M' || type[0] == 'm'){
		depriciationTbl = sumOfYearsDigits();
	}else{
		//something wrong happened somewhere
		exit(-1);
	}
	
	return depriciationTbl;
 }
 
 /*
   * Read data into class
   * returns : nothing
   */
void Assets::getAssets(){
	clrscr();
	
	cout<<"\n\n"<<"   Asset Code                 : ";
	cin>>code;
	cout<<"   Asset name                 : ";
	gets(name); 	
	cout<<"   Type -  [M]obile / [F]ixed : ";
	gets(type);	
	cout<<"   Quantity                   : ";
	cin>>quantity;	
	cout<<"   Useful life (years)        : ";
	cin>>usefulLifeYears;	
	cout<<"   Asset value (QR)           : ";
	cin>>assetValue;	
	cout<<"   Salvage value (QR)         : ";
	cin>>salvageValue;
	
	//Each entry is not checked for correctness since it's tiresome
	//instead at main class input is shown to user for verification
	//if found error get input once again
}
 
 /*
   * Print data into screen. It's for testing validity of data in the developing stage & no use there after.
   * returns : nothing
   */
 void Assets::showAssets(){
	cout<<"\n\n"<<"   Asset Code                 : "<<code;
	cout<<"\n"<<"   Asset name                 : "<<name;
	cout<<"\n"<<"   Type -  [M]obile / [F]ixed : "<<(char)toupper(type[0]);
	cout<<"\n"<<"   Quantity                   : "<<quantity;
	cout<<"\n"<<"   Useful life (years)        : "<<usefulLifeYears;
	cout<<"\n"<<"   Asset value (QR)           : "<<assetValue;
	cout<<"\n"<<"   Salvage value (QR)         : "<<salvageValue;
 }
 
 
 #endif