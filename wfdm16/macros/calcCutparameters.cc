#include "inc/shinclude.h"

void calcCutParameters( const std::string rawFilePath, const std::string paramFilePath, const std::string outputFilePath ) {
    SetShStyle( );

    std::cout << "*** cutParameters start ***" << std::endl;

    const int chN = 8;
    const int clkN = 4096;

    // load raw data

    TFile rFile( rawFilePath.c_str( ) );
    if ( !rFile.IsOpen( ) ) {
        std::cout << "Error: Raw rootfile connot open !!" << std::endl;
        return;
    }
    TTree* rTree = dynamic_cast< TTree* >( rFile.Get( "tree" ) );
    if ( rTree == nullptr ) {
        std::cout << "Error: Raw tree cannot open !!" << std::endl;
        return;
    }
    
    // load parameters data

    TFile pFile( paramFilePath.c_str( ) );
    if ( !pFile.IsOpen( ) ) {
        std::cout << "Error: Parameter rootfile connot open !!" << std::endl;
        return;
    }
    TTree* pTree = dynamic_cast< TTree* >( pFile.Get( "p_tree" ) );
    if ( pTree == nullptr ) {
        std::cout << "Error: Parameter tree cannot open !!" << std::endl;
        return;
    }

    // make new tree

    TFile cFile( outputFilePath.c_str(), "recreate" );
    if ( !cFile.IsOpen( ) ) {
        std::cout << "Error: Rootfile connot open !!" << std::endl;
        return;
    }
    TTree *cTree = new TTree( "c_tree", "c_tree" );
    if ( cTree == nullptr ) {
        std::cout << "Error: Cut tree cannot open !!" << std::endl;
        return;
    }

    const int totEvt = rTree->GetEntries( );
    std::cout << "Total events: " << totEvt << std::endl;

    int evtID;
    int fadcVar[chN][clkN] = {};
    double ped[chN] = {};
    int pMin[chN] = {}, pMax[chN] = {};
    int pInt[chN] = {};

    std::cout << "Loading branches..." << std::endl;

    for ( int ch = 0; ch < chN; ++ch ) {
        rTree->SetBranchAddress( Form("fadc%d", ch), fadcVar[ch] );
        pTree->SetBranchAddress( Form("ped%d", ch), &ped[ch] );
        pTree->SetBranchAddress( Form("pMin%d", ch), &pMin[ch] );
        pTree->SetBranchAddress( Form("pMax%d", ch), &pMax[ch] );
        pTree->SetBranchAddress( Form("pInt%d", ch), &pInt[ch] );
    }

    std::cout << "Loading branches done." << std::endl;

    std::cout << "Setting cutTree branches..." << std::endl;

    double fadcVarCut[chN][clkN] = {};
    double pMinCut[chN] = {}, pMaxCut[chN] = {};
    double pIntCut[chN] = {};
    
    cTree->Branch( "evtID", &evtID, "evtID/I" );
    for ( int ch = 0; ch < chN; ch++) {
        cTree->Branch( Form( "fadcCut%d", ch ), fadcVarCut[ch], Form( "fadcCut%d[%d]/D", ch, clkN ) );
        cTree->Branch( Form( "pedCut%d", ch ), &ped[ch], Form( "pedCut%d/D", ch ) );
        cTree->Branch( Form( "pMaxCut%d", ch ), &pMaxCut[ch], Form( "pMaxCut%d/D", ch ) );
        cTree->Branch( Form( "pMinCut%d", ch ), &pMinCut[ch], Form( "pMinCut%d/D", ch ) );
        cTree->Branch( Form( "pIntCut%d", ch ), &pIntCut[ch], Form( "pIntCut%d/D", ch ) );
    }

    std::cout << "Setting cutTree branches done." << std::endl;

    std::cout << "Start filling data ..." << std::endl;

    for( int evt = 0; evt < totEvt; ++evt ) {
        ShUtil::PrintProgressBar( evt, totEvt );
        rTree->GetEntry( evt );
        pTree->GetEntry( evt );

        for( int ch = 0; ch < chN; ++ch ) {
            for ( int clk = 0; clk < clkN; ++clk ) {
                fadcVarCut[ch][clk] = fadcVar[ch][clk] - ped[ch];
            }
            pMinCut[ch] = pMin[ch] - ped[ch];
            pMaxCut[ch] = pMax[ch] - ped[ch];
            pIntCut[ch] = pInt[ch] - ped[ch] * clkN;
        }
        evtID = evt;
        cTree->Fill( );
    }
    std::cout << std::endl;

    std::cout << "Filling data done." << std::endl;

    std::cout << "Writing data ..." << std::endl;

    cTree->Write( );

    std::cout << "Writing data done." << std::endl;

    std::cout << "*** cutParameters end ***" << std::endl;

    return;
}