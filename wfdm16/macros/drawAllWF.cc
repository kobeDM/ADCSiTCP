#include "inc/shinclude.h"

void drawAllWF( const std::string inputFilePath, const std::string outputFilePath ) {
    SetShStyle( );

    const int chN = 8;
    const int clkN = 4096;

    // load raw data

    TFile rFile( inputFilePath.c_str( ) );
    if ( !rFile.IsOpen( ) ) {
        std::cout << "Error: Rootfile connot open !!" << std::endl;
        return;
    }
    TTree* rTree = dynamic_cast< TTree* >( rFile.Get( "tree" ) );
    if ( rTree == nullptr ) {
        std::cout << "Error: Raw tree cannot open !!" << std::endl;
        return;
    }

    int evtID;
    int fadcVar[chN][clkN] = {};
    for (int i = 0; i < chN; i++)
        rTree->SetBranchAddress( Form("fadc%d", i), fadcVar[i]);

    std::vector< TH2D* > histArr;
    histArr.reserve( chN );
    for ( int ch = 0; ch < chN; ch++ ) {
        histArr.push_back( new TH2D( Form( "hist%d", ch ), Form( "hist%d", ch ), clkN, 0, clkN, 4096., -2048., 2048. ) );
    }

    // fill histograms

    const int totEvt = rTree->GetEntries( );
    const int clk_offset = 2;
    const double wf_x_min = 480., wf_x_max = 520.;
    const double wf_y_min = -500., wf_y_max = 500.;
    
    for ( int evt = 0; evt < totEvt; ++evt ) {
        ShUtil::PrintProgressBar( evt, totEvt );
        rTree->GetEntry( evt );
        for ( int ch = 0; ch < chN; ch++ ) {
            for ( int clk = clk_offset; clk < clkN; clk++ ) {
                histArr[ch]->Fill( clk, fadcVar[ch][clk] );
            }
        }
    }

    // plot histograms

    TCanvas *cvwf = new TCanvas( "cvwf", "cvwf", 1200, 600 );

    const Int_t NRGBs = 5; const Int_t NCont = 255;
    Double_t stops[NRGBs] = { 0.00, 0.34, 0.61, 0.84, 1.00 };
    Double_t red[NRGBs]   = { 0.00, 0.00, 0.87, 1.00, 1.00 };
    Double_t green[NRGBs] = { 0.00, 0.81, 1.00, 0.20, 0.00 };
    Double_t blue[NRGBs]  = { 0.80, 1.00, 0.12, 0.00, 0.00 };
    TColor::CreateGradientColorTable( NRGBs, stops, red, green, blue, NCont );
    gStyle->SetNumberContours( NCont );

    cvwf->Divide( 4, 2 );

    for ( int ch = 0; ch < chN; ch++ ) {
        cvwf->cd( ch + 1 );
        // cvsh.cd( ch+1 )->SetLogz();
        gPad->SetRightMargin( 0.2 );
        TH2D* pHist = histArr.at( ch );
        if( pHist == nullptr ) continue;

        pHist->GetXaxis()->SetTitle( "clock [40 MHz]" );
        pHist->GetYaxis()->SetTitle( "ADC count" );
        pHist->GetXaxis()->SetRangeUser( wf_x_min, wf_x_max );
        pHist->GetYaxis()->SetRangeUser( wf_y_min, wf_y_max );
        pHist->GetZaxis()->SetTitle( "Entries" );
        pHist->GetZaxis()->SetRangeUser(0, totEvt);
        pHist->Draw( "colz" );

        ShTUtil::CreateDrawText( 0.55, 0.85, Form( "channel %d", ch ) );
    }

    cvwf->SaveAs( outputFilePath.c_str( ) );

    return;
}