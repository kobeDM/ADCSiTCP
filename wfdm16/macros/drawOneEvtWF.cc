#include "inc/shinclude.h"

void drawOneEvtWF( const String& inputFilePath, const int& evtNum, const String& prefix = "evt" )
{
    SetShStyle( );

    const int chN = 8;
    const int clkN = 4096;

    std::vector< TGraph* > grFADCArr;
    grFADCArr.reserve( chN );
    for( int ch = 0; ch < chN; ch++ ) {
        grFADCArr.push_back( new TGraph( ) );
    }
    
    TFile rFile( inputFilePath.c_str( ) );
    if( !rFile.IsOpen( ) ) {
        ShUtil::Cerr( "Error: Rootfile connot open !!" );
        std::abort( );
    }
    TTree* pTree = dynamic_cast< TTree* >( rFile.Get( "tree" ) );
    if( pTree == nullptr ) {
        ShUtil::Cerr( "Error: Parameter tree cannot open !!" );
        std::abort( );
    }
    // pTree->SetDirectory(nullptr);

    int fadcVar[chN][clkN] = {};
    for (int i = 0; i < chN; i++)
        pTree->SetBranchAddress( Form("fadc%d", i), fadcVar[i] );

    int totEvt = pTree->GetEntries( );
    if( evtNum >= totEvt ) {
        ShUtil::Cerr( "Error: The event number is over range." );
        std::abort( );
    }

    pTree->GetEntry( evtNum );
    // for ( int clk = 0; clk < clkN; clk++ ) {
    //     if ( clk > 2048 ) break;
    //     std::cout << clk << "\t" << fadcVar[0][clk] << std::endl;
    // }

    const int clk_offset = 2;
    const int fadc_offset = 500;

    for( int ch = 0; ch < chN; ++ch ) {
        int clkArr[clkN] = {};
        for( int clk = 0; clk < clkN; ++clk ) {
            // std::cout << "ch: " << ch << std::endl;
            // if ( clk < 2048 )
                // std::cout << clk << "\t" << fadcVar[0][clk] << std::endl;
            fadcVar[ch][clk] += ( ch + 1 ) * fadc_offset;
            clkArr[clk] = clk;
        }
        grFADCArr[ch] = new TGraph( clkN, clkArr, fadcVar[ch] );
    }

    TCanvas cvs( "cvs", "cvs", 800, 600 );

    TMultiGraph mg;
    int idx = 0;
    for( auto pGraph : grFADCArr ) {
        if( pGraph == nullptr ) return;
        pGraph->SetMarkerSize( 1 );
        pGraph->SetMarkerColor( idx+1 );
        pGraph->SetLineColor( idx+1 );
        pGraph->SetMarkerStyle( 2 );
        mg.Add( pGraph );
        ++idx;
    }

    mg.GetXaxis()->SetTitle( "clock [40 MHz]" );
    mg.GetYaxis()->SetTitle( "ADC count (relative)" );
    mg.GetXaxis()->SetRangeUser( 480, 520 );
    // mg.GetXaxis()->SetRangeUser( 0, 1000 );
    mg.GetYaxis()->SetRangeUser( 0, 4800 );
    // mg.GetYaxis()->SetRangeUser( 480, 520 );
    mg.Draw("AP");
    

    cvs.SaveAs( Form( "%s_%d.png", prefix.c_str( ), evtNum ) );

    return;
}
