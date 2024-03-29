#include "inc/shinclude.h"

void drawOneEvtWF( const String& input, const int& evtNum, const String& prefix = "evt" )
{
    SetShStyle( );
    
    TFile file( input.c_str( ) );
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
    if( evtNum >= totEvt ) {
        ShUtil::Cerr( "the event number is over range." );
        std::abort( );
    }

    pTree->GetEntry( evtNum );

    std::vector< TGraph* > grFADCArr;
    grFADCArr.reserve( 8 );

    for( int ch = 0; ch < 8; ++ch ) {
        String histName = Form( "hist_%d", ch );
        int clkArr[2048] = {};
        for( int clk = 0; clk < 2048; ++clk ) {
            fadcVar[ch][clk] += ch*500;
            clkArr[clk] = clk;
        }
        TGraph* pGraph = new TGraph( 2048, clkArr, fadcVar[ch]);
        grFADCArr.push_back( pGraph );
    }

    TCanvas cvs( "cvs", "cvs", 800, 600 );

    // bool isFirst = true;
    TMultiGraph mg;
    int idx = 0;
    for( auto pGraph : grFADCArr ) {
        if( pGraph == nullptr ) return;
        // if( idx > 3 ) break;

        pGraph->SetMarkerSize( 1 );
        pGraph->SetMarkerColor( idx+1 );
        pGraph->SetLineColor( idx+1 );
        pGraph->SetMarkerStyle( 2 );
        mg.Add( pGraph );
        ++idx;
    }

    mg.Draw("AP");
    mg.GetXaxis()->SetTitle( "clock [40 MHz]" );
    mg.GetYaxis()->SetTitle( "ADC count (relative)" );
    mg.GetYaxis()->SetRangeUser( 0, 4800 );
    mg.Draw("AP");
    
    cvs.SaveAs( Form( "%s_%d.png", prefix.c_str( ), evtNum ) );
    
    return;
}
