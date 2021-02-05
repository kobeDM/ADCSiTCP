#include "inc/shinclude.h"

void dumpAllWaveforms( const String& inputFile, const String& outputFile )
{
    SetShStyle( );

    TCanvas cvs( "cvs", "cvs", 800, 600 );
    gPad->SetRightMargin( 0.2 );

    const Int_t NRGBs = 5; const Int_t NCont = 255;
    Double_t stops[NRGBs] = { 0.00, 0.34, 0.61, 0.84, 1.00 };
    Double_t red[NRGBs]   = { 0.00, 0.00, 0.87, 1.00, 1.00 };
    Double_t green[NRGBs] = { 0.00, 0.81, 1.00, 0.20, 0.00 };
    Double_t blue[NRGBs]  = { 0.80, 1.00, 0.12, 0.00, 0.00 };
    TColor::CreateGradientColorTable( NRGBs, stops, red, green, blue, NCont );
    gStyle->SetNumberContours( NCont );
    
    TH2F hist( "hist", "hist", 2048, 0.0, 2048.0, 4096, 0.0, 4096.0 );

    std::ifstream ifs;
    ifs.open( inputFile );
    if( ifs.is_open( ) == false ) return;
    
    int trig = 0;
    int adc = 0;
    int clk = 0;
    while( !ifs.eof( ) ) {
        std::string line = "";
        std::getline( ifs, line );
        if( line.length( ) <= 0 || strncmp( line.c_str( ), "#", 1 ) == 0 ) continue;

        std::stringstream ss( line );
        ss >> trig >> adc;

        hist.Fill( clk, adc );
        clk++;
        if( clk >= 2048 ) clk = 0;
    }

    hist.GetXaxis()->SetTitle("Clock [5 MHz]");
    hist.GetYaxis()->SetTitle("ADC count");

    hist.Draw("colz");
    cvs.SaveAs( Form( "%s.png", outputFile.c_str( ) ) );
    
    return;
}
