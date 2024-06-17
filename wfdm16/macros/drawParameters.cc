#include "inc/shinclude.h"

void drawParameters( const std::string inputFilePath, const std::string outputDirPath ) {
    SetShStyle( );

    const int chN = 8;
    const int clkN = 4096;

    const int pedHistBin = 100;
    const int maxHistBin = 100;
    const int minHistBin = 100;
    const int intHistBin = 100;
    const double pedHistMin = -100., pedHistMax = 100.;
    const double maxHistMin = -500., maxHistMax = 500.;
    const double minHistMin = -500., minHistMax = 500.;
    const double intHistMin = -20000., intHistMax = 20000.;

    std::vector < TH1D* > pedArr;
    std::vector < TH1D* > maxArr;
    std::vector < TH1D* > minArr;
    std::vector < TH1D* > intArr;
    pedArr.reserve( chN );
    maxArr.reserve( chN );
    minArr.reserve( chN );
    intArr.reserve( chN );
    for ( int ch = 0; ch < chN; ch++ ) {
        pedArr.push_back( new TH1D( Form( "ped%d", ch ), Form( "ped%d", ch ), pedHistBin, pedHistMin, pedHistMax ) );
        maxArr.push_back( new TH1D( Form( "max%d", ch ), Form( "max%d", ch ), maxHistBin, maxHistMin, maxHistMax ) );
        minArr.push_back( new TH1D( Form( "min%d", ch ), Form( "min%d", ch ), minHistBin, minHistMin, minHistMax ) );
        intArr.push_back( new TH1D( Form( "int%d", ch ), Form( "int%d", ch ), intHistBin, intHistMin, intHistMax ) );
    }

    // load parameters

    TFile pFile( inputFilePath.c_str( ) );
    if ( !pFile.IsOpen( ) ) {
        std::cout << "Error: Rootfile connot open !!" << std::endl;
        return;
    }
    TTree* pTree = dynamic_cast< TTree* >( pFile.Get( "p_tree" ) );
    if ( pTree == nullptr ) {
        std::cout << "Error: Parameter tree cannot open !!" << std::endl;
        return;
    }

    int evtID;
    double ped[chN] = {};
    int pMin[chN] = {};
    int pMax[chN] = {};
    int pInt[chN] = {};

    pTree->Branch( "evtID/I", &evtID, "evtID/I" );
    for ( int ch = 0; ch < chN; ch++) {
        pTree->SetBranchAddress( Form( "ped%d", ch ), &ped[ch] );
        pTree->SetBranchAddress( Form( "pMax%d", ch ), &pMax[ch] );
        pTree->SetBranchAddress( Form( "pMin%d", ch ), &pMin[ch] );
        pTree->SetBranchAddress( Form( "pInt%d", ch ), &pInt[ch] );
    }

    // fill histograms

    const int totEvt = pTree->GetEntries( );
    for ( int evt = 0; evt < totEvt; ++evt ) {
        ShUtil::PrintProgressBar( evt, totEvt );
        pTree->GetEntry( evt );
        for ( int ch = 0; ch < chN; ch++ ) {
            pedArr[ch]->Fill( ped[ch] );
            maxArr[ch]->Fill( pMax[ch] );
            minArr[ch]->Fill( pMin[ch] );
            intArr[ch]->Fill( pInt[ch] );
        }
    }

    // plot histograms

    TCanvas *pedcv = new TCanvas( "pedcv", "pedcv", 1200, 600 );
    TCanvas *maxcv = new TCanvas( "maxcv", "maxcv", 1200, 600 );
    TCanvas *mincv = new TCanvas( "mincv", "mincv", 1200, 600 );
    TCanvas *intcv = new TCanvas( "intcv", "intcv", 1200, 600 );
    pedcv->Divide( 4, 2 );
    maxcv->Divide( 4, 2 );
    mincv->Divide( 4, 2 );
    intcv->Divide( 4, 2 );

    for ( int ch = 0; ch < chN; ch++ ) {
        pedcv->cd( ch + 1 );
        pedcv->cd( ch + 1 )->SetGrid( );
        pedArr[ch]->SetLineColor( 1 );
        pedArr[ch]->SetLineWidth( 2 );
        pedArr[ch]->SetXTitle( "Pedestal [ADC]" );
        pedArr[ch]->SetYTitle( "Counts" );
        pedArr[ch]->Draw( );
        ShTUtil::CreateDrawText( 0.55, 0.85, Form( "channel %d", ch ) );

        maxcv->cd( ch + 1 );
        maxcv->cd( ch + 1 )->SetGrid( );
        maxArr[ch]->SetLineColor( 1 );
        maxArr[ch]->SetLineWidth( 2 );
        maxArr[ch]->SetXTitle( "Max [ADC]" );
        maxArr[ch]->SetYTitle( "Counts" );
        maxArr[ch]->Draw( );
        ShTUtil::CreateDrawText( 0.55, 0.85, Form( "channel %d", ch ) );

        mincv->cd( ch + 1 );
        mincv->cd( ch + 1 )->SetGrid( );
        minArr[ch]->SetLineColor( 1 );
        minArr[ch]->SetLineWidth( 2 );
        minArr[ch]->SetXTitle( "Min [ADC]" );
        minArr[ch]->SetYTitle( "Counts" );
        minArr[ch]->Draw( );
        ShTUtil::CreateDrawText( 0.55, 0.85, Form( "channel %d", ch ) );

        intcv->cd( ch + 1 );
        intcv->cd( ch + 1 )->SetGrid( );
        intArr[ch]->SetLineColor( 1 );
        intArr[ch]->SetLineWidth( 2 );
        intArr[ch]->SetXTitle( "Integral [ADC]" );
        intArr[ch]->SetYTitle( "Counts" );
        intArr[ch]->Draw( );
        ShTUtil::CreateDrawText( 0.55, 0.85, Form( "channel %d", ch ) );
    }

    const std::string pedPlotPath = outputDirPath + "/pedPlot.png";
    const std::string maxPlotPath = outputDirPath + "/maxPlot.png";
    const std::string minPlotPath = outputDirPath + "/minPlot.png";
    const std::string intPlotPath = outputDirPath + "/intPlot.png";
    pedcv->SaveAs( pedPlotPath.c_str( ) );
    maxcv->SaveAs( maxPlotPath.c_str( ) );
    mincv->SaveAs( minPlotPath.c_str( ) );
    intcv->SaveAs( intPlotPath.c_str( ) );

    return;
}