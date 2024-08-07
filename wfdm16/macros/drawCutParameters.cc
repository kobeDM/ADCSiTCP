#include "inc/shinclude.h"

void drawCutParameters( const std::string inputFilePath, const std::string outputDirPath ) {
    SetShStyle( );

    const int chN = 8;
    const int clkN = 4096;

    const int maxCutHistBin = 100;
    const int minCutHistBin = 100;
    const int intCutHistBin = 100;
    const double maxCutHistMin = -500., maxCutHistMax = 500.;
    const double minCutHistMin = -500., minCutHistMax = 500.;
    const double intCutHistMin = -20000., intCutHistMax = 20000.;

    std::vector < TH1D* > maxCutArr;
    std::vector < TH1D* > minCutArr;
    std::vector < TH1D* > intCutArr;
    maxCutArr.reserve( chN );
    minCutArr.reserve( chN );
    intCutArr.reserve( chN );
    for ( int ch = 0; ch < chN; ch++ ) {
        maxCutArr.push_back( new TH1D( Form( "max%d", ch ), Form( "max%d", ch ), maxCutHistBin, maxCutHistMin, maxCutHistMax ) );
        minCutArr.push_back( new TH1D( Form( "min%d", ch ), Form( "min%d", ch ), minCutHistBin, minCutHistMin, minCutHistMax ) );
        intCutArr.push_back( new TH1D( Form( "int%d", ch ), Form( "int%d", ch ), intCutHistBin, intCutHistMin, intCutHistMax ) );
    }

    // load cut parameters

    TFile cFile( inputFilePath.c_str( ) );  
    if ( !cFile.IsOpen( ) ) {
        std::cout << "Error: Rootfile connot open !!" << std::endl;
        return;
    }
    TTree* cTree = dynamic_cast< TTree* >( cFile.Get( "c_tree" ) );
    if ( cTree == nullptr ) {
        std::cout << "Error: Cut tree cannot open !!" << std::endl;
        return;
    }

    int evtID;
    double pMax[chN] = {};
    double pMin[chN] = {};
    double pInt[chN] = {};

    cTree->SetBranchAddress( "evtID", &evtID );
    for ( int ch = 0; ch < chN; ch++) {
        cTree->SetBranchAddress( Form( "pMax%d", ch ), &pMax[ch] );
        cTree->SetBranchAddress( Form( "pMin%d", ch ), &pMin[ch] );
        cTree->SetBranchAddress( Form( "pInt%d", ch ), &pInt[ch] );
    }

    // fill histograms

    const int totEvt = cTree->GetEntries( );
    for ( int evt = 0; evt < totEvt; ++evt ) {
        cTree->GetEntry( evt );
        for ( int ch = 0; ch < chN; ++ch ) {
            maxCutArr[ch]->Fill( pMax[ch] );
            minCutArr[ch]->Fill( pMin[ch] );
            intCutArr[ch]->Fill( pInt[ch] );
        }
    }

    // plot histograms

    TCanvas *maxccv = new TCanvas( "maxccv", "maxccv", 1200, 600 );
    TCanvas *minccv = new TCanvas( "minccv", "minccv", 1200, 600 );
    TCanvas *intccv = new TCanvas( "intccv", "intccv", 1200, 600 );
    maxccv->Divide( 4, 2 );
    minccv->Divide( 4, 2 );
    intccv->Divide( 4, 2 );

    for ( int ch = 0; ch < chN; ch++ ) {
        maxccv->cd( ch + 1 );
        maxccv->cd( ch + 1 )->SetGrid( );
        maxCutArr[ch]->SetLineColor( 1 );
        maxCutArr[ch]->SetLineWidth( 2 );
        maxCutArr[ch]->SetXTitle( "Max (cut) [ADC]" );
        maxCutArr[ch]->SetYTitle( "Counts" );
        maxCutArr[ch]->Draw( );
        ShTUtil::CreateDrawText( 0.55, 0.85, Form( "channel %d", ch ) );

        minccv->cd( ch + 1 );
        minccv->cd( ch + 1 )->SetGrid( );
        minCutArr[ch]->SetLineColor( 1 );
        minCutArr[ch]->SetLineWidth( 2 );
        minCutArr[ch]->SetXTitle( "Min (cut) [ADC]" );
        minCutArr[ch]->SetYTitle( "Counts" );
        minCutArr[ch]->Draw( );
        ShTUtil::CreateDrawText( 0.55, 0.85, Form( "channel %d", ch ) );

        intccv->cd( ch + 1 );
        intccv->cd( ch + 1 )->SetGrid( );
        intCutArr[ch]->SetLineColor( 1 );
        intCutArr[ch]->SetLineWidth( 2 );
        intCutArr[ch]->SetXTitle( "Integral (cut) [ADC]" );
        intCutArr[ch]->SetYTitle( "Counts" );
        intCutArr[ch]->Draw( );
        ShTUtil::CreateDrawText( 0.55, 0.85, Form( "channel %d", ch ) );
    }

    const std::string maxCutHistPath = outputDirPath + "/maxCutHist.pdf";
    const std::string minCutHistPath = outputDirPath + "/minCutHist.pdf";
    const std::string intCutHistPath = outputDirPath + "/intCutHist.pdf";
    maxccv->SaveAs( maxCutHistPath.c_str( ) );
    minccv->SaveAs( minCutHistPath.c_str( ) );
    intccv->SaveAs( intCutHistPath.c_str( ) );

    return;
}