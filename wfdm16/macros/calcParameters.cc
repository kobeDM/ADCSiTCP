#include "inc/shinclude.h"

void calcParameters( const String& inputFilePath,const String& outputFilePath) {
    SetShStyle( );

    const int chN = 8;
    const int clkN = 4096;

    // load raw data

    TFile rFile( inputFilePath.c_str( ) );
    if( !rFile.IsOpen( ) ) {
        ShUtil::Cerr( "Error: Rootfile connot open !!" );
        std::abort( );
    }
    TTree* rTree = dynamic_cast< TTree* >( rFile.Get( "tree" ) );
    if( rTree == nullptr ) {
        ShUtil::Cerr( "Error: Raw tree cannot open !!" );
        std::abort( );
    }
    
    // make new tree

    TFile pFile( outputFilePath.c_str(), "recreate" );
    if( !pFile.IsOpen( ) ) {
        ShUtil::Cerr( "Error: Rootfile connot open !!" );
        std::abort( );
    }
    TTree *pTree = new TTree( "p_tree", "p_tree" );
    if( pTree == nullptr ) {
        ShUtil::Cerr( "Error: Parameter tree cannot open !!" );
        std::abort( );
    }

    int fadcVar[chN][clkN] = {};
    for ( int ch = 0; ch < chN; ch++ )
        rTree->SetBranchAddress( Form("fadc%d", ch), fadcVar[ch] );

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

    const int totEvt = rTree->GetEntries( );
    const int clk_offset = 2;
    const int clk_base_len = 200;
    const double fadc_min_thr = -2000.;
    const double fadc_max_thr = 2000.;

    std::cout << "Total events: " << totEvt << std::endl;

    for( int evt = 0; evt < totEvt; ++evt ) {
        ShUtil::PrintProgressBar( evt, totEvt );
        rTree->GetEntry( evt );

        for( int ch = 0; ch < chN; ++ch ) {
            if (ch != 0) continue;

            double pedN = 0.;
            double ped_fadc = 0.;
            double max_fadc = -4096.;
            double min_fadc = 4096.;
            double sum_fadc = 0.;

            for( int clk = clk_offset; clk < clk_base_len + clk_offset; ++clk ) {
                if ( fadcVar[ch][clk] < fadc_min_thr || fadcVar[ch][clk] > fadc_max_thr ) continue;
                    ped_fadc += fadcVar[ch][clk];
                    pedN++;
            }
            ped_fadc = ped_fadc / pedN;

            for( int clk = clk_offset; clk < clkN; ++clk ) {
                if ( fadcVar[ch][clk] < fadc_min_thr || fadcVar[ch][clk] > fadc_max_thr ) continue;
                sum_fadc += fadcVar[ch][clk];
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

    return;
}