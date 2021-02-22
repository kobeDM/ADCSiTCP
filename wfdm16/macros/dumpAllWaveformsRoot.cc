#include "inc/shinclude.h"

void dumpAllWaveformsRoot( const String& inputFile, const String& outputFile )
{
    SetShStyle( );

    std::vector< TH2F* > histArr;
    histArr.reserve( 8 );
    for( int i = 0; i < 8; ++i ) {
        String histName = Form( "hist_%d", i );
        histArr.push_back( new TH2F( histName.c_str( ), histName.c_str( ), 2048, 0.0, 2048.0, 4096, 0.0, 4096.0 ) );
    }

    TFile file( inputFile.c_str( ) );
    TTree* pTree = dynamic_cast< TTree* >( file.Get( "tree" ) );
    if( pTree == nullptr ) return;

    int   fadcVar[8][2048] = {};
    pTree->SetBranchAddress( "fadc0", fadcVar[0] );
    pTree->SetBranchAddress( "fadc1", fadcVar[1] );
    pTree->SetBranchAddress( "fadc2", fadcVar[2] );
    pTree->SetBranchAddress( "fadc3", fadcVar[3] );
    pTree->SetBranchAddress( "fadc4", fadcVar[4] );
    pTree->SetBranchAddress( "fadc5", fadcVar[5] );
    pTree->SetBranchAddress( "fadc6", fadcVar[6] );
    pTree->SetBranchAddress( "fadc7", fadcVar[7] );

    int totEvt = pTree->GetEntries( );
    for( int evt = 0; evt < totEvt; ++evt ) {
        ShUtil::PrintProgressBar( evt, totEvt );
        pTree->GetEntry( evt );
        
        for( int ch = 0; ch < 8; ++ch ) {
            TH2F* pHist = histArr.at( ch );
            if( pHist == nullptr ) continue;
            for( int clk = 0; clk < 2048; ++clk ) {
                pHist->Fill( clk, fadcVar[ch][clk] );
            }
        }
    }

    TCanvas cvs( "cvs", "cvs", 1200, 600 );

    const Int_t NRGBs = 5; const Int_t NCont = 255;
    Double_t stops[NRGBs] = { 0.00, 0.34, 0.61, 0.84, 1.00 };
    Double_t red[NRGBs]   = { 0.00, 0.00, 0.87, 1.00, 1.00 };
    Double_t green[NRGBs] = { 0.00, 0.81, 1.00, 0.20, 0.00 };
    Double_t blue[NRGBs]  = { 0.80, 1.00, 0.12, 0.00, 0.00 };
    TColor::CreateGradientColorTable( NRGBs, stops, red, green, blue, NCont );
    gStyle->SetNumberContours( NCont );

    cvs.Divide( 4, 2 );

    for( int ch = 0; ch < 8; ++ch ) {
        cvs.cd( ch+1 );
        gPad->SetRightMargin( 0.2 );
        TH2F* pHist = histArr.at( ch );
        if( pHist == nullptr ) continue;

        pHist->GetXaxis()->SetTitle( "clock [40 MHz]" );
        pHist->GetYaxis()->SetTitle( "ADC count" );
        // pHist->GetYaxis()->SetRangeUser( 200.0, 4000.0 );
        pHist->GetZaxis()->SetTitle( "Entries" );
        pHist->Draw( "colz" );
    }

    cvs.SaveAs( Form( "%s.png", outputFile.c_str( ) ) );
    // cvs.SaveAs( Form( "%s.pdf", outputFile.c_str( ) ) );
    // cvs.SaveAs( Form( "%s.eps", outputFile.c_str( ) ) );

    return;
}
