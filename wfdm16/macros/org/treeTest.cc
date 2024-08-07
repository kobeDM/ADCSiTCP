#include "inc/shinclude.h"

void treeTest() {
    TFile *rFile = new TFile( inputFilePath.c_str( ), outputFilePath.c_str( ) );
    if (!rFile->IsOpen()) {
        std::cout << "Error: Rootfile connot open !!" << std::endl;
        return;
    }
    TTree *rTree = (TTree *)rFile->Get("tree");

    TFile *rFile = new TFile( inputFilePath.c_str( ), outputFilePath.c_str( ) );
    if (!rFile->IsOpen()) {
        std::cout << "Error: Rootfile connot open !!" << std::endl;
        return;
    }
    TTree *rTree = (TTree *)rFile->Get("tree");

}