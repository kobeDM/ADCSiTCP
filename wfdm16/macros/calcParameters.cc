#include "inc/shinclude.h"

void calcParameters( const String& inputFilePath,const String& outputFilePath) {
    SetShStyle( );

    const int chN = 8;
    const int clkN = 4096;

    // load raw data
    
    TFile *rFile = new TFile( inputFilePath.c_str( ), "recreate" );
    if ( !rFile ) {
        std::cout << "Error: Rootfile connot open !!" << std::endl;
        return;
    }
    TTree *rTree = (TTree *)rFile->Get("tree");
    if ( !rTree ) {
        std::cout << "Error: Raw tree cannot open !!" << std::endl;
        return;
    }

    int fadcVar[chN][clkN] = {};
    for (int i = 0; i < chN; i++) 
        rTree->SetBranchAddress( Form("fadc%d", i), fadcVar[i]);
    
    // make new tree

    TFile *pFile = new TFile( outputFilePath.c_str(), "recreate" );
    TTree *pTree = new TTree( "p_tree", "p_tree" );

    int evtID;
    double ped[chN] = {};
    int pMin[chN] = {}, pMax[chN] = {};
    int pInt[chN] = {};

    pTree->Branch( "evtID/I", &evtID, "evtID/I" );

    for ( int ch = 0; ch < chN; ch++) {
        pTree->Branch( Form( "ped%d", ch ), &ped[ch], "ped/D" );
        pTree->Branch( Form( "pMax%d", ch ), &pMax[ch], "pMax/I" );
        pTree->Branch( Form( "pMin%d", ch ), &pMin[ch], "pMin/I" );
        pTree->Branch( Form( "pInt%d", ch ), &pInt[ch], "pInt/I" );
    }

    const int totEvt = pTree->GetEntries( );
    const int clk_offset = 2;
    const int clk_base_len = 200;

    for( int evt = 0; evt < totEvt; ++evt ) {
        ShUtil::PrintProgressBar( evt, totEvt );
        pTree->GetEntry( evt );

        for( int ch = 0; ch < chN; ++ch ) {
            if (ch != 0) continue;

            double ped_fadc = 0.;
            double max_fadc = -4096.;
            double min_fadc = 4096.;
            double sum_fadc = 0.;

            for( int clk = clk_offset; clk < clk_base_len; ++clk )
                ped_fadc += fadcVar[ch][clk];
            ped_fadc = ped_fadc / ( clk_base_len - clk_offset );

            for( int clk = clk_offset; clk < chN; ++clk ) {
                sum_fadc += fadcVar[ch][clk];
                fadcVar[ch][clk];
                if( max_fadc < fadcVar[ch][clk] )
                    max_fadc = fadcVar[ch][clk];
                if( min_fadc > fadcVar[ch][clk] )
                    min_fadc = fadcVar[ch][clk];
            }

            ped[ch] = ped_fadc;
            pMin[ch] = min_fadc;
            pMax[ch] = max_fadc;
            pInt[ch] = sum_fadc;
        }
        evtID = evt;
        pTree->Fill( );
    }

    pTree->Write( );
    pFile->Close( );
}